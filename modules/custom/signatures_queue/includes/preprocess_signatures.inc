<?php
/**
 * @file
 * The "preprocess signatures" step.
 *
 * Retrieve a batch of records from signatures_pending_validation_queue, store
 *   in mysql signatures_pending_validation table, delete from SQS.
 *
 * Retrieve a batch of records from signatures_validated_queue, store in mysql
 *   signatures_validated table, delete from SQS.
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
 *   (optional) The name of the server processing the workflow. Defaults to
 *   SIGNATURES_QUEUE_SERVER_NAME_DEFAULT.
 * @param string $worker_name
 *   (optional) An arbitrary identifier for the invoking process (e.g., cron
 *   job). Defaults to SIGNATURES_QUEUE_WORKER_NAME_DEFAULT.
 * @param array $options
 *   (optional) An array of arbitrary options data as required by particular
 *   workflows. Defaults to an empty array.
 *
 * @return bool
 *   Returns TRUE if the workflow executes successfully or FALSE if not.
 *
 * @see signatures_queue_invoke_workflow()
 */
function signatures_queue_preprocess_signatures($job_id, $server_name = SIGNATURES_QUEUE_SERVER_NAME_DEFAULT, $worker_name = SIGNATURES_QUEUE_WORKER_NAME_DEFAULT, array $options = array()) {

  // Processing is the same for the two queues so we loop through them.
  $queues = array(
    'signatures_pending_validation_queue' => 'signatures_pending_validation',
    'signatures_validated_queue' => 'signatures_validated',
  );
  foreach ($queues as $queue => $table) {
    // Varables we use on every log message.
    $log_common = array(
      '%worker' => $worker_name,
      '%server' => $server_name,
      '%queue' => $queue,
    );
    $log_suffix = 'Job: preprocess_signatures. Job ID: [id]. Worker: %worker. Server: %server.';

    // Preprocess a batch of signatures in $queue queue.
    $result = signatures_queue_preprocess_signatures_batch($queue, $table);

    if ($result['saved']) {
      watchdog('signatures_queue',
        '%num records retrieved and deleted from %queue, and stored in mysql tables. ' . $log_suffix,
        array_merge($log_common, array('%num' => $result['saved']))
      );
    }
    else {
      watchdog('signatures_queue',
        'Error. %queued records are in %queue. 0 records were retrieved. ' . $log_suffix,
        array_merge($log_common, array('%queued' => $result['queued'])),
        ($result['queued'] ? WATCHDOG_ERROR : WATCHDOG_WARNING)
      );
    }

    if ($result['failed']) {
      watchdog('signatures_queue',
        'Error. %num records left in %queue after failed attempt to store them in mysql. These will be preprocessed again when lock expires. ' . $log_suffix,
        array_merge($log_common, array('%num' => $result['failed'])),
        WATCHDOG_ERROR
      );
    }
  }

  return TRUE;
}

/**
 * Get batch size.
 *
 * Get admin-defined number of pending and validated records to be processed at
 *   a time (which can be adjusted during heavy load).
 */
function signatures_queue_get_queue_batch_size() {
  // Get the size from the configured variable.
  $batch_size = variable_get('signatures_queue_preprocess_batch_size', 100);

  // If there are other load handling adjustments, they can be added here.
  $load = sys_getloadavg();
  // 8 is chosen for a 6 to 8 core CPU server. Load is typically < 1.
  if ($load[0] > 8) {
    // Scale down the batch with higher load.
    // Example: if $batch_size is 200 and load is 10, $batch_size will be 20.
    $batch_size = $batch_size / $load[0];
    // Log the $batch_size scaling? watchdog(...);
  }

  return (int) $batch_size;
}

/**
 * Retrieve a batch of pending signatures and store them in mysql.
 *
 * Retrieve a batch of records from queue_name,
 *   store in mysql $table_name table, delete from SQS.
 *   (If we fail to insert into mysql, re-insert into SQS)
 *
 * @param string $queue_name
 *   The name of the queue to process.
 *   Expecting either signatures_pending_validation_queue or
 *   signatures_validated_queue.
 *
 * @param string $table_name
 *   The name of the table for storing records.
 *   Expecting either signatures_pending_validation or signatures_validated.
 *
 * @return array
 *   Array info about records moved into mysql and items remaining in queue
 *   - retrieved, count of items retrieved from queue to be moved to mysql
 *   - saved, count of records successfully inserted into mysql table
 *   - failed, count of items not successfully moved from a queue into mysql
 *   - queued, total number of items currently in the queue
 */
function signatures_queue_preprocess_signatures_batch($queue_name, $table_name) {
  $return = array('saved', 'failed', 'queued', 'retrieved');
  $return = array_combine($return, array_fill(0, count($return), 0));

  $queue = SignaturesQueue::get($queue_name);
  $queue->createQueue();
  $return['queued'] = $queue->numberOfItems();

  $max = signatures_queue_get_queue_batch_size();
  $i = 0;

  // The single '=' is intentional as claimItem returns an object or false.
  while (($i < $max) && ($item = $queue->claimItem())) {

    // Set the active database to the signatures_processing db.
    signatures_queue_set_db();

    // Save the item in signatures_processing db.
    // The $item->data array is conviniently the same as the field mapping we
    // need for the db_insert() call.
    if (array_key_exists('signup', $item->data)) {
      // Cast signup to int for database type compatibility.
      $item->data['signup'] = (int) $item->data['signup'];
    }
    try {
      $id = db_insert($table_name)
        ->fields($item->data)
        ->execute();
      $insert_success = TRUE;
      $return['saved']++;
    }
    catch (Exception $e) {
      // Set the active database back to default, in case db logging is enabled.
      db_set_active();
      watchdog('signatures_queue',
        'Error while attempting db insert: %error',
        array('%error' => $e->errorInfo[2]),
        WATCHDOG_ERROR);
      $insert_success = FALSE;
    }

    // Set the active database back to default, in case queue backend is using
    // Drupal's database.
    db_set_active();

    if ($insert_success) {
      // Delete from queue, after resetting active db.
      $queue->deleteItem($item);
    }
    else {
      $return['failed']++;
    }

    $i++;
  }
  $return['retrieved'] = $i;

  return $return;
}
