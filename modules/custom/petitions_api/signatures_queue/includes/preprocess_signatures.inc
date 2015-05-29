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
 * @return int
 *   Returns one of the following status codes indicating the workflow outcome:
 *     - SIGNATURES_QUEUE_STATUS_OK
 *     - SIGNATURES_QUEUE_STATUS_BAD_REQUEST
 *     - SIGNATURES_QUEUE_STATUS_FORBIDDEN
 *     - SIGNATURES_QUEUE_STATUS_NOT_FOUND
 *     - SIGNATURES_QUEUE_STATUS_SERVER_ERROR
 *
 * @see signatures_queue_invoke_workflow()
 */
function _signatures_queue_preprocess_signatures($job_id, $server_name, $worker_name, array $options = array()) {
  $watchdog_suffix = _signatures_queue_watchdog_suffix('preprocess_signatures', $job_id, $server_name, $worker_name);
  $debug = variable_get('signatures_queue_debug', FALSE);

  // Preprocess signatures pending validation.
  _signatures_queue_preprocess_signatures_preprocess_signatures($watchdog_suffix, $debug);

  // Preprocess validations.
  _signatures_queue_preprocess_signatures_preprocess_validations($watchdog_suffix, $debug);

  return SIGNATURES_QUEUE_STATUS_OK;
}

/**
 * Preprocesses signatures pending validation.
 *
 * This claims items from signatures_pending_validation_queue, inserts them into
 * the signatures_pending_validations table, and deletes them from
 * signatures_pending_validation_queue.
 *
 * @param string $watchdog_suffix
 *   A string of job details as created by _signatures_queue_watchdog_suffix().
 * @param bool $debug
 *   Enable debugging for verbose logging.
 */
function _signatures_queue_preprocess_signatures_preprocess_signatures($watchdog_suffix, $debug = FALSE) {
  // Claim a batch of signatures from signatures_pending_validation_queue.
  $batch = _signatures_queue_preprocess_signatures_claim_batch('signatures_pending_validation_queue', $watchdog_suffix);

  // Debug. Print batch to log.
  if ($debug) {
    watchdog('signatures_queue', 'DEBUG ENABLED: Batch of signatures to be preprocessed: !batch. @suffix', array(
      '!batch' => petitionslog_format_for_watchdog($batch),
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_DEBUG);
  }

  // Return early if the batch is empty.
  if (empty($batch)) {
    petitionslog_event('signatures_queue.workflow.preprocess_signatures.caught_up');
    return FALSE;
  }

  // Dedupe the batch and insert into the signatures_pending_validation table.
  $inserted = _signatures_queue_preprocess_signatures_safe_insert_batch_into_table($batch, 'signatures_pending_validation', $watchdog_suffix);

  if ($inserted) {
    // Delete the batch from signatures_pending_validation_queue.
    _signatures_queue_preprocess_signatures_delete_signatures_batch_from_queue($batch, 'signatures_pending_validation_queue');
  }
}

/**
 * Preprocesses validations.
 *
 * This claims items from validations_queue, inserts them into
 * the validations table, and deletes them from validations_queue.
 *
 * @param string $watchdog_suffix
 *   A string of job details as created by _signatures_queue_watchdog_suffix().
 * @param bool $debug
 *   Enable debugging for verbose logging.
 */
function _signatures_queue_preprocess_signatures_preprocess_validations($watchdog_suffix, $debug = FALSE) {

  // Claim a batch of signatures from validations_queue.
  $batch = _signatures_queue_preprocess_signatures_claim_batch('validations_queue', $watchdog_suffix);

  // Debug. Print batch to log.
  if ($debug) {
    watchdog('signatures_queue', 'DEBUG ENABLED: Batch of validations to be preprocessed: !batch. @suffix', array(
      '!batch' => petitionslog_format_for_watchdog($batch),
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_DEBUG);
  }

  // Return early if the batch is empty.
  if (empty($batch)) {
    // @todo The name of this log event implies the existence of a discrete
    //   "Preprocess Validations" workflow that does not currently exist as
    //   such. This was done because the current "Preprocess Signatures"
    //   workflow is actually preprocessing two things, not one, and cannot be
    //   said to be caught up when only one of the source data stores is empty.
    //   (And there is no guarantee that both will EVER be empty at the same
    //   time.) This suggests an opportunity to refine the workflow design by
    //   separating this workflow into two. Ticket #PT-1438 has been created to
    //   do so.
    petitionslog_event('signatures_queue.workflow.preprocess_validations.caught_up');
    return FALSE;
  }

  // Dedupe the batch and insert into the validations table.
  $inserted = _signatures_queue_preprocess_signatures_safe_insert_batch_into_table($batch, 'validations', $watchdog_suffix);

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
  $number_of_items = $queue->numberOfItems();
  if (!$number_of_items) {
    watchdog('signatures_queue', '@queue is empty. No items will be preprocessed. @suffix', array(
      '@queue' => $queue_name,
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_INFO);
    _signatures_queue_data_store_size_event($queue_name, $number_of_items);
    return array();
  }
  else {
    watchdog('signatures_queue', '@number items retrieved from @queue queue for preprocessing. @suffix', array(
      '@number' => $number_of_items,
      '@queue' => $queue_name,
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_INFO);
  }

  // Build the batch.
  $submitted_signatures = array();

  // Some queue backends have the ability to claim multiple items in one
  // request.
  //
  // @todo It is undesirable for us to maintain parallel branches of logic based
  //   on a difference between classes. We should instead patch core to add this
  //   method to the queue interface. See https://drupal.org/node/2163061.
  if (method_exists($queue, 'claimItemMultiple')) {
    // Maximum number of messages per AWS SQS request is 10, so default to 10.
    $chunk_size = variable_get('signatures_queue_preprocess_claim_chunk_size', 10);
    $num_requests = ceil($batch_size / $chunk_size);
    for ($i = 0; $i < $num_requests && $queue->numberOfItems() > 0; $i++) {
      $items = $queue->claimItemMultiple(NULL, $chunk_size);
      if ($items) {
        // @todo Check for empty objects. Is this still a problem in the context
        //   of a multiple item claim?
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
      if ($item) {
        $submitted_signatures[] = $item;
      }
    }
  }

  $number_of_items = $queue->numberOfItems();
  _signatures_queue_data_store_size_event($queue_name, $number_of_items);

  // Create batch keyed by unique secret validation key.
  $batch = array();
  foreach ($submitted_signatures as $item) {
    if (!empty($item->data['secret_validation_key'])) {
      if ($queue_name == 'signatures_pending_validation_queue') {
        // Log time elapsed since passing through previous workflow.
        $elapsed_since_initiating_signature_validation = signatures_queue_get_microtime_elapsed_since($item->data['timestamp_initiated_signature_validation']);
        petitionslog_event('signatures_queue.data_flow.time_elapsed.initiated_signature_validation__to__preprocessed_signature', 'time', $elapsed_since_initiating_signature_validation);

        // Add timestamp.
        $item->data['timestamp_preprocessed_signature'] = time();
      }
      elseif ($queue_name == 'validations_queue') {
        // Log time elapsed since passing through previous workflow.
        $elapsed_since_receiving_signature_validation = signatures_queue_get_microtime_elapsed_since($item->data['timestamp_received_signature_validation']);
        petitionslog_event('signatures_queue.data_flow.time_elapsed.received_signature_validation__to__preprocessed_validation', 'time', $elapsed_since_receiving_signature_validation);

        // Add timestamp.
        $item->data['timestamp_preprocessed_validation'] = time();
      }

      $key = $item->data['secret_validation_key'];
      $batch[$key] = $item;
    }
  }

  return $batch;
}

/**
 * Safely inserts a batch of items into a given database table.
 *
 * Prevents duplicate key integrity constraint violations by deduping the batch
 * with the destination table first.
 *
 * @param array $batch
 *   A batch of items as returned by
 *   _signatures_queue_preprocess_signatures_claim_batch().
 * @param string $table_name
 *   The name of the table to insert the batch into.
 * @param string $watchdog_suffix
 *   A string of job details as created by _signatures_queue_watchdog_suffix().
 *
 * @return bool
 *   Returns TRUE if items were successfully inserted, or FALSE if not.
 */
function _signatures_queue_preprocess_signatures_safe_insert_batch_into_table($batch, $table_name, $watchdog_suffix) {

  // Dedupe signatures by cross-referencing chunks of the new submissions
  // against data in the database.
  $new_signatures = _signatures_queue_preprocess_signatures_dedupe_signatures($table_name, $batch);

  $sanitized_signatures = _signatures_queue_preprocess_signatures_sanitize_batch($table_name, $new_signatures);

  // Build insert query.
  $fields = _signatures_queue_preprocess_signatures_insert_batch_fields($table_name);
  signatures_queue_set_db();
  $query = db_insert($table_name);
  $query->fields(array_values($fields));

  // Add values.
  $items_added = 0;
  foreach ($sanitized_signatures as $item) {
    // Restrict values to those that correspond to database fields.
    $values = array_intersect_key((array) $item->data, $fields);

    // Add the resulting values to the query.
    $query->values($values);
    $items_added++;
  }

  // Execute the query.
  try {
    $query->execute();
    petitionslog_event("signatures_queue.data_store.{$table_name}.item_added", 'count', $items_added);
  }
  catch (Exception $e) {
    petitionslog_event('exceptions.signatures_queue.6e13628');
    db_set_active();
    watchdog('signatures_queue', 'Error inserting signatures into @table. @suffix. Exception: !e', array(
      '@table' => $table_name,
      '@suffix' => $watchdog_suffix,
      '!e' => petitionslog_format_for_watchdog($e),
    ), WATCHDOG_ERROR);

    return FALSE;
  }

  $count = db_select($table_name)->countQuery()->execute()->fetchField();
  _signatures_queue_data_store_size_event($table_name, $count);

  db_set_active();

  return TRUE;
}

/**
 * Sanitizes a batch of items for safe database insertion.
 *
 * Prevents database errors from type, length, and character encoding issues.
 *
 * @param string $table_name
 *   The name of the table the batch will be inserted into.
 * @param array $batch
 *   A batch of items as returned by
 *   _signatures_queue_preprocess_signatures_claim_batch().
 *
 * @throws Exception
 *   Throws an exception in case of an invalid table name argument.
 *
 * @return array
 *   Returns the sanitized batch of items.
 */
function _signatures_queue_preprocess_signatures_sanitize_batch($table_name, $batch) {
  // Get the schema for the table the batch will be inserted into.
  module_load_install('signatures_queue');
  if ($table_name == 'signatures_pending_validation') {
    $schema = _signatures_queue_get_signatures_schema();
  }
  elseif ($table_name = 'validations') {
    $schema = _signatures_queue_get_validations_schema();
  }
  else {
    throw new Exception(t('Invalid table name.'));
  }

  // Loop through batch items.
  foreach ($batch as $item) {
    // Loop through item values.
    foreach ($item->data as $key => $value) {
      if (isset($schema['fields'][$key]['type'])) {

        // Sanitize VARCHAR fields.
        if ($schema['fields'][$key]['type'] == 'varchar') {
          // Prevent PDO exceptions from unsupported characters like this guy:
          // http://www.fileformat.info/info/unicode/char/1F600/index.htm. (Not
          // kidding.)
          $plain_value = utf8_encode($value);

          // Truncate value to the maximum length of its target database column.
          $trimmed_value = drupal_substr($plain_value, 0, $schema['fields'][$key]['length']);

          $item->data[$key] = $trimmed_value;
        }

        // Sanitize INT fields.
        elseif ($schema['fields'][$key]['type'] == 'int') {
          // Make sure INTs come through as actual INTs!
          $item->data[$key] = (int) $item->data[$key];
        }
      }
    }
  }

  return $batch;
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
    unset($fields['timestamp_preprocessed_validation']);
    unset($fields['timestamp_processed_signature']);
  }
  elseif ($table_name == 'validations') {
    // Use values from table schema to determine fields for db_insert().
    $schema = _signatures_queue_get_validations_schema();
    $fields = drupal_map_assoc(array_keys($schema['fields']));
    // Unset auto-inc and empty fields.
    unset($fields['vid']);
    unset($fields['timestamp_processed_signature']);
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
 *   The name of the table into which signatures will be inserted.
 * @param array $submitted_signatures
 *   An associative array of submitted signatures, keyed by
 *   secret_validation_key.
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

  // @todo It is undesirable for us to maintain parallel branches of logic based
  //   on a difference between classes. We should instead patch core to add this
  //   method to the queue interface. See https://drupal.org/node/2163061.
  if (method_exists($queue, 'deleteItemMultiple')) {
    // Maximum number of messages per AWS SQS request is 10, so default to 10.
    $chunk_size = variable_get('signatures_queue_preprocess_delete_chunk_size', 10);
    $chunks = array_chunk($batch, $chunk_size, TRUE);
    foreach ($chunks as $chunk) {
      $count_items = count($chunk);
      $queue->deleteItemMultiple($chunk);
      petitionslog_event("signatures_queue.data_store.{$queue_name}.item_removed", 'count', $count_items);
    }
  }
  else {
    // Delete items.
    foreach ($batch as $item) {
      $queue->deleteItem($item);
      petitionslog_event("signatures_queue.data_store.{$queue_name}.item_removed");
    }
  }
  $count_queue = $queue->numberOfItems();
  _signatures_queue_data_store_size_event($queue_name, $count_queue);
}
