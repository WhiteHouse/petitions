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
 * @param string $watchdog_suffix
 *   A string of job details as created by _signatures_queue_watchdog_suffix().
 */
function _signatures_queue_preprocess_signatures_preprocess_signatures($watchdog_suffix) {
  // Claim a batch of signatures from signatures_pending_validation_queue.
  $batch = _signatures_queue_preprocess_signatures_claim_batch('signatures_pending_validation_queue', $watchdog_suffix);

  // Return early if the batch is empty.
  if (!$batch) {
    return;
  }

  // Insert the batch into the signatures_pending_validation table.
  _signatures_queue_preprocess_signatures_insert_batch_into_table($batch, 'signature_validations', $watchdog_suffix);

  // Delete the batch from signatures_pending_validation_queue.
  _signatures_queue_preprocess_signatures_delete_signatures_batch_from_queue($batch, 'signatures_pending_validation_queue');
}

/**
 * Preprocesses validations.
 *
 * @param string $watchdog_suffix
 *   A string of job details as created by _signatures_queue_watchdog_suffix().
 */
function _signatures_queue_preprocess_signatures_preprocess_validations($watchdog_suffix) {
  // Claim a batch of signatures from validations_queue.
  $batch = _signatures_queue_preprocess_signatures_claim_batch('validations_queue');

  // Merge the batch into the validations table.
  _signatures_queue_preprocess_signatures_insert_batch_into_table($batch, 'validations', $watchdog_suffix);

  // Delete the batch from validations_queue.
  _signatures_queue_preprocess_signatures_delete_signatures_batch_from_queue($batch, 'validations_queue');
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
  $batch = array();
  $items_skipped = 0;
  for ($i = 0; ($i < $batch_size && $queue->numberOfItems() > 0); $i++) {
    // Claim item.
    $item = $queue->claimItem();

    // Some queue backends (e.g., Amazon SQS) can return empty objects. Skip
    // them.
    if (!$item) {
      $items_skipped++;
      continue;
    }

    $batch[] = $item;
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
 * @throws Exception
 *   Throws an exception if an invalid table name is supplied.
 */
function _signatures_queue_preprocess_signatures_insert_batch_into_table($batch, $table_name, $watchdog_suffix) {
  // Prepare the query and get the destination schema.
  module_load_install('signatures_queue');
  if ($table_name == 'signature_validations') {
    $query = db_insert('signature_validations');
    $schema = _signatures_queue_get_signatures_schema();
  }
  elseif ($table_name == 'validations_queue') {
    $query = db_merge('validations_queue');
    $schema = _signatures_queue_get_validations_schema();
  }
  else {
    throw new Exception(t('Invalid table name.'));
  }

  // Add values.
  foreach ($batch as $item) {
    // Typecast the "signup" value for database type compatibility.
    if (array_key_exists('signup', $item->data)) {
      $item->data['signup'] = (int) $item->data['signup'];
    }

    // Restrict values to those that correspond to database fields.
    $fields = array_keys($schema['fields']);
    $values = array_intersect_key((array) $item->data, array_flip($fields));

    // Add the resulting values to the query.
    $query->values($values);
  }

  // Execute the query.
  signatures_queue_set_db();
  try {
    $query->execute();
    db_set_active();
  }
  catch (Exception $e) {
    db_set_active();
    watchdog('signatures_queue', "Error while attempting db insert. @suffix.\n%error", array(
      '%error' => $e->getMessage(),
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_ERROR);
  }
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

  // Delete items.
  foreach ($batch as $item) {
    $queue->deleteItem($item);
  }
}
