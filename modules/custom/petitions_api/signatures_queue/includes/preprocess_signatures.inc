<?php

/**
 * @file
 * The "preprocess signatures" workflow.
 *
 * Retrieve a batch of records from signatures_pending_validation_queue, store
 * in signatures_pending_validation table, delete from queue.
 *
 * Retrieve a batch of records from validations_queue, store in mysql
 * validations table, delete from queue.
 */

/**
 * Preprocesses signatures.
 *
 * This function should not be invoked directly. Use
 * signatures_queue_invoke_workflow() instead.
 *
 * @param string $job_id
 *   A unique identifier for the job, as created by
 *   _signatures_queue_create_job_id().
 * @param string $server_name
 *   (optional) An arbitrary identifier for the server processing the workflow.
 * @param string $worker_name
 *   (optional) An arbitrary identifier for the invoking process (e.g., cron
 *   job).
 * @param array $options
 *   (optional) An array of options. Defaults to an empty array. It is currently
 *   unused by this workflow.
 *
 * @return bool
 *   Returns TRUE if the workflow executes successfully or FALSE if not.
 *
 * @see signatures_queue_invoke_workflow()
 */
function _signatures_queue_preprocess_signatures($job_id, $server_name, $worker_name, array $options = array()) {
  $watchdog_suffix = _signatures_queue_watchdog_suffix('preprocess_signatures', $job_id, $server_name, $worker_name);

  // Preprocess signatures pending validation.
  _signatures_queue_preprocess_signatures_preprocess_signatures($watchdog_suffix);

  // Preprocess validations.
  _signatures_queue_preprocess_signatures_preprocess_validations($watchdog_suffix);
}

/**
 * Preprocesses signatures pending validation.
 *
 * This claims items from signatures_pending_validation_queue, inserts them into
 * the signatures_pending_validations table, and deletes them from the queue.
 *
 * @param string $watchdog_suffix
 *   A string of job details as created by _signatures_queue_watchdog_suffix().
 */
function _signatures_queue_preprocess_signatures_preprocess_signatures($watchdog_suffix) {
  // Claim a batch of signatures from signatures_pending_validation_queue.
  $batch = _signatures_queue_preprocess_signatures_claim_batch('signatures_pending_validation_queue', $watchdog_suffix);

  // Return early if the batch is empty.
  if (empty($batch)) {
    return FALSE;
  }

  // Insert the batch into the signatures_pending_validation table.
  $inserted = _signatures_queue_preprocess_signatures_insert_batch_into_table($batch, 'signatures_pending_validation', $watchdog_suffix);

  if ($inserted) {
    // Delete the batch from signatures_pending_validation_queue.
    _signatures_queue_preprocess_signatures_delete_signatures_batch_from_queue($batch, 'signatures_pending_validation_queue');
  }
}

/**
 * Preprocesses validations.
 *
 * This claims items from validations_queue, inserts them into
 * the validations_queue, and deletes them from the queue.
 *
 * @param string $watchdog_suffix
 *   A string of job details as created by _signatures_queue_watchdog_suffix().
 */
function _signatures_queue_preprocess_signatures_preprocess_validations($watchdog_suffix) {

  // Claim a batch of signatures from validations_queue.
  $batch = _signatures_queue_preprocess_signatures_claim_batch('validations_queue', $watchdog_suffix);

  // Return early if the batch is empty.
  if (empty($batch)) {
    return FALSE;
  }

  // Merge the batch into the validations table.
  $inserted = _signatures_queue_preprocess_signatures_insert_batch_into_table($batch, 'validations', $watchdog_suffix);

  if ($inserted) {
    // Delete the batch from validations_queue.
    _signatures_queue_preprocess_signatures_delete_signatures_batch_from_queue($batch, 'validations_queue');
  }
}

/**
 * Claims a batch of items from a given queue.
 *
 * @param string $queue_name
 *   The name of the queue to claim from.
 * @param string $watchdog_suffix
 *   A string of job details as created by _signatures_queue_watchdog_suffix().
 *
 * @return array
 *   An indexed array of queue items.
 */
function _signatures_queue_preprocess_signatures_claim_batch($queue_name, $watchdog_suffix) {
  // Get the batch size.
  $batch_size = signatures_queue_get_queue_batch_size('preprocess_signatures');

  // Get the queue.
  $queue = SignaturesQueue::get($queue_name);
  $queue->createQueue();

  // Log and return early if the queue is empty.
  if (!$queue->numberOfItems()) {
    watchdog('signatures_queue', '@queue is empty. No items will be preprocessed. @suffix', array(
      '@queue' => $queue_name,
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_INFO);
    return array();
  }

  // Build the batch.
  $submitted_signatures = array();
  $items_skipped = 0;

  // AWS SQS has the ability to claim multiple items in one request.
  if (method_exists($queue, 'claimItems')) {
    // Maximum number of messages per SQS request is 10. Must batch in 10s.
    $num_requests = ceil($batch_size / 10);
    for ($i = 0; $i < $num_requests && $queue->numberOfItems() > 0; $i++) {
      if ($items = $queue->claimItems(NULL, 10)) {
        // @todo check for empty objects. Is this still a problem?
        $submitted_signatures = array_merge($submitted_signatures, $items);
      }
    }
  }
  else {
    for ($i = 0; ($i < $batch_size && $queue->numberOfItems() > 0); $i++) {
      // Claim item.
      $item = $queue->claimItem();

      // Some queue backends (e.g., Amazon SQS) can return empty objects. Skip
      // them.
      if (!$item) {
        $items_skipped++;
        continue;
      }

      $submitted_signatures[] = $item;
    }
  }

  // Create batch keyed by unique secret validation key.
  $batch = array();
  foreach ($submitted_signatures as $item) {
    if (!empty($item->data['secret_validation_key'])) {
      $key = $item->data['secret_validation_key'];
      $batch[$key] = $item;
    }
  }

  // Log anomalies.
  if ($items_skipped) {
    watchdog('signatures_queue', 'Skipped !num records because they were empty. @suffix', array(
      '!num' => $items_skipped,
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_ERROR);
  }

  return $batch;
}

/**
 * Inserts a batch of items into a given database table.
 *
 * @param array $batch
 *   The batch of items as returned by
 *   _signatures_queue_preprocess_signatures_claim_batch().
 * @param string $table_name
 *   The name of the table to insert the batch into.
 * @param string $watchdog_suffix
 *   A string of job details as created by _signatures_queue_watchdog_suffix().
 *
 * @return bool
 *   Returns TRUE if items were successfully inserted.
 */
function _signatures_queue_preprocess_signatures_insert_batch_into_table($batch, $table_name, $watchdog_suffix) {

  // Dedupe signatures by cross-referencing chunks of the new submissions
  // against data in the database.
  $new_signatures = _signatures_queue_preprocess_signatures_dedupe_signatures($table_name, $batch);

  // Build insert query.
  $fields = _signatures_queue_preprocess_signatures_insert_batch_fields($table_name);
  signatures_queue_set_db();
  $query = db_insert($table_name);
  $query->fields(array_values($fields));

  // Add values.
  foreach ($new_signatures as $item) {
    // Typecast the "signup" value for database type compatibility.
    if (array_key_exists('signup', $item->data)) {
      $item->data['signup'] = (int) $item->data['signup'];
    }

    // Restrict values to those that correspond to database fields.
    $values = array_intersect_key((array) $item->data, $fields);

    // Add the resulting values to the query.
    $query->values($values);
  }

  // Execute the query.
  try {
    signatures_queue_set_db();
    $query->execute();
    db_set_active();
  }
  catch (Exception $e) {
    db_set_active();
    watchdog('signatures_queue', "Error while attempting db insert. @suffix.\n%error", array(
      '%error' => $e->getMessage(),
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_ERROR);

    return FALSE;
  }

  return TRUE;
}

/**
 * Returns an array of fields to be used for inserting batch into local db.
 *
 * @param string $table_name
 *   The name of the table that the batch will be inserted into.
 *
 * @return array
 *   An associative array of fields, keyed by field name.
 *
 * @throws Exception
 *   Throws an exception if an invalid table name is supplied.
 */
function _signatures_queue_preprocess_signatures_insert_batch_fields($table_name) {
  // Prepare the query and get the destination schema.
  module_load_install('signatures_queue');
  if ($table_name == 'signatures_pending_validation') {
    // Use values from table schema to determine fields for db_insert().
    $schema = _signatures_queue_get_signatures_schema();
    $fields = drupal_map_assoc(array_keys($schema['fields']));
    // Unset auto-inc and empty fields.
    unset($fields['sid']);
    unset($fields['timestamp_processed']);
  }
  elseif ($table_name == 'validations') {
    // Use values from table schema to determine fields for db_insert().
    $schema = _signatures_queue_get_validations_schema();
    $fields = drupal_map_assoc(array_keys($schema['fields']));
    // Unset auto-inc and empty fields.
    unset($fields['vid']);
    unset($fields['timestamp_processed']);
    unset($fields['timestamp_validation_close']);
  }
  else {
    throw new Exception(t('Invalid table name.'));
  }

  return $fields;
}

/**
 * Removes extant signatures from an array of new signature submissions.
 *
 * @param string $table_name
 *   The name of the table that signatures into which signatures will be
 *   inserted.
 *
 * @param array $submitted_signatures
 *   An associative array of submitted signatures, keyed by
 *   secret_validation_key.
 *
 * @param int $chunk_size
 *   The size of chunks to be deduped. This will be used to spilt the SELECT
 *   query into multiple, smaller queries.
 *
 * @return array
 *   An array in the same format as $chunk, with existing signatures removed.
 */
function _signatures_queue_preprocess_signatures_dedupe_signatures($table_name, $submitted_signatures, $chunk_size = NULL) {

  if (!$chunk_size) {
    $chunk_size = variable_get('signatures_queue_preprocess_dedupe_chunk_size', 50);
  }

  signatures_queue_set_db();
  $new_signatures = array();
  $chunks = array_chunk($submitted_signatures, $chunk_size, TRUE);
  foreach ($chunks as $chunk) {
    // Check to see if any of the vids already exist in our local db.
    $query = db_select($table_name, 't');
    $query->fields('t', array('secret_validation_key'));
    $query->condition('secret_validation_key', array_keys($submitted_signatures), 'IN');
    $existing_signatures = $query->execute()->fetchAllKeyed(0, 0);
    // Add only signatures that are not in both arrays.
    $new_signatures += array_diff_key($chunk, $existing_signatures);
  }
  db_set_active();

  return $new_signatures;
}
/**
 * Deletes a batch of items from a given queue.
 *
 * @param array $batch
 *   The batch of items as returned by
 *   _signatures_queue_preprocess_signatures_claim_batch().
 * @param string $queue_name
 *   The name of the queue to delete the batch from.
 */
function _signatures_queue_preprocess_signatures_delete_signatures_batch_from_queue($batch, $queue_name) {
  // Get the queue.
  $queue = SignaturesQueue::get($queue_name);
  $queue->createQueue();

  if (method_exists($queue, 'deleteItems')) {
    // Maximum number of messages per SQS request is 10.
    $max_messages_per_request = 10;
    $chunks = array_chunk($batch, $max_messages_per_request, TRUE);
    foreach ($chunks as $chunk) {
      $queue->deleteItems($chunk);
    }
  }
  else {
    // Delete items.
    foreach ($batch as $item) {
      $queue->deleteItem($item);
    }
  }
}
