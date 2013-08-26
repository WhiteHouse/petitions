<?php

/**
 * @file
 * The "preprocess signatures" workflow.
 *
 * Retrieve a batch of records from signatures_pending_validation_queue, store
 * in signatures_pending_validation table, delete from queue.
 *
 * Retrieve a batch of records from signatures_validated_queue, store in mysql
 * signatures_validated table, delete from queue.
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
function _signatures_queue_preprocess_signatures($job_id, $server_name = SIGNATURES_QUEUE_SERVER_NAME_DEFAULT, $worker_name = SIGNATURES_QUEUE_WORKER_NAME_DEFAULT, array $options = array()) {
  $watchdog_suffix = _signatures_queue_watchdog_suffix('preprocess_signatures', $job_id, $server_name, $worker_name);

  // Processing is the same for the two queues so we loop through them.
  $queues = array(
    'signatures_pending_validation_queue' => 'signatures_pending_validation',
    'signatures_validated_queue' => 'signatures_validated',
  );
  foreach ($queues as $queue => $table) {
    // Preprocess a batch of signatures in $queue queue.
    $result = signatures_queue_preprocess_signatures_batch($queue, $table);

    if ($result['saved']) {
      watchdog('signatures_queue', '@number records were retrieved and deleted from @queue and stored in database tables. @suffix', array(
        '@number' => $result['saved'],
        '@queue' => $queue,
        '@suffix' => $watchdog_suffix,
      ), WATCHDOG_INFO);
    }
    else {
      if ($result['queued']) {
        watchdog('signatures_queue', '@queue contains @number record(s), but none were retrieved. @suffix', array(
          '@queue' => $queue,
          '@number' => number_format($number_of_items),
          '@suffix' => $watchdog_suffix,
        ), WATCHDOG_ERROR);
      }
      else {
        watchdog('signatures_queue', '@queue is empty. No signatures will be preprocessed. @suffix', array(
          '@queue' => $queue,
          '@suffix' => $watchdog_suffix,
        ), WATCHDOG_INFO);
      }
    }

    if ($result['failed']) {
      watchdog('signatures_queue', '@number record(s) were left in @queue after failure to move them into the database. (Will try again.) @suffix', array(
        '@number' => $result['failed'],
        '@queue' => $queue,
        '@suffix' => $watchdog_suffix,
      ), WATCHDOG_ERROR);
    }
  }

  return TRUE;
}

/**
 * Retrieve a batch of pending signatures and store them in mysql.
 *
 * Retrieve a batch of records from queue_name,
 *   store in mysql $table_name table, delete from queue.
 *   (If we fail to insert into mysql, re-insert into queue)
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

  $max = signatures_queue_get_queue_batch_size('preprocess_signatures');
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
      // Preprocess data to be inserted.
      $record = (array) $item->data;
      $id = db_insert($table_name)
        ->fields($record)
        ->execute();
      $insert_success = TRUE;
      $return['saved']++;
    }
    catch (Exception $e) {
      // Set the active database back to default, in case db logging is enabled.
      db_set_active();
      watchdog('signatures_queue', 'Error while attempting db insert: %error', array(
        '%error' => $e->errorInfo[2],
      ), WATCHDOG_ERROR);
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
