<?php

/**
 * @file
 * The "archive signatures" workflow.
 *
 * Retrieve and delete batches of signatures from signatures_submitted_queue,
 * send a validation email, then store signature info in
 * signatures_pending_validation_queue.
 */

/**
 * Initiates archive of signatures.
 *
 * This function should not be invoked directly. Use
 * signatures_queue_invoke_workflow() instead.
 *
 * @return bool
 *   Returns TRUE if the workflow executes successfully or FALSE if not.
 *
 * @see signatures_queue_invoke_workflow()
 */
function signatures_queue_archive_signatures() {
  // Define a variable for signatures_queue_archive_unvalidated_signatures.
  $archive_signatures = variable_get('signatures_queue_archive_unvalidated_signatures', 1);

  // Set the active database to the signatures_processing db.
  signatures_queue_set_db();

  if ($archive_signatures == 1) {
    // Select signatures from the pending validation table whose
    // timestamp_validation_close is older then (before) the last time queues
    // were emptied.
    //
    // These will be moved from the pending validation table to the not valid
    // table.
    $query = db_select('signatures_pending_validation', 's');
    $query
      ->fields('s', array(
        'id',
        'secret_validation_key',
        'signature_source_api_key',
        'timestamp_petition_close',
        'timestamp_validation_close',
        'petition_id',
        'first_name',
        'last_name',
        'zip',
        'email',
        'signup',
        'timestamp_validation_email_sent',
        'timestamp_submitted'))
      ->condition('timestamp_validation_close', $time, '<');

    db_insert('signatures_not_valid')
      ->from($query)
      ->execute();
  }
  // Delete signatures that are no longer valid from the pending table.
  db_delete('signatures_pending_validation')
    ->condition('timestamp_validation_close', $time, '<')
    ->execute();

  // Set the active database back to default, in case queue backend is using
  // Drupal's database.
  db_set_active();

  return TRUE;
}

/**
 * Archives orphaned signatures.
 *
 * @param int $queues_last_emptied
 *   The oldest date that all queues were last emptied.
 *
 * @param string $watchdog_suffix
 *   Defined suffix for watchdog statements.
 *
 * @return bool
 *   TRUE, success. FALSE, failure.
 */
function _signatures_queue_archive_orphan_signatures($queues_last_emptied, $watchdog_suffix) {
  // Set the active database to the signatures_processing db.
  signatures_queue_set_db();

  // Query to select records from signatures_validated that do not have matching
  // secret_validation_key records in signatures_pending_validation.
  $query = db_select('signatures_validated', 'a');
  $query->leftJoin('signatures_pending_validation', 'b', 'a.secret_validation_key = b.secret_validation_key');
  $query
    ->fields('a', array(
      'id',
      'secret_validation_key',
      'timestamp_validated',
      'timestamp_validation_close',
      'client_ip',
      'petition_id'))
    ->isNull('b.secret_validation_key')
    ->condition('a.timestamp_validation_close', $queues_last_emptied, '<');

  $columns = $query->execute()->fetchCol(1);

  // Count the results to determine how many rows will be moved.
  $results = $query->execute()->fetchAll();
  $orphaned_count = count($results);

  // Insert the orphaned signatures into signatures_orphans table.
  db_insert('signatures_orphans')
    ->from($query)
    ->execute();

  // Log the number of successfully archived orphaned signatures.
  watchdog('signatures_queue', '@number records archived in signatures_orphans. These signature submissions did not have matching secret validation keys. @suffix', array(
      '@number' => $orphaned_count,
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_INFO);

  // Delete signatures that are no longer valid from the pending table.
  if ($columns) {
    $deleted_signatures = db_delete('signatures_validated')
      ->condition('secret_validation_key', array($columns), 'IN')
      ->execute();
  }

  // Log the number of signatures deleted from signatures_validated.
  watchdog('signatures_queue', '@number records retrieved and deleted from signatures_validated. These signature submissions did not have matching secret validation keys. @suffix', array(
      '@number' => number_format($deleted_signatures),
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_INFO);

  // Set the active database back to default.
  db_set_active();
}

/**
 * Find out the oldest timestamp after which ALL queues have been emptied.
 *
 * For example, assume these are the days when the following queues were emptied
 * last:
 *   - signatures_submitted_queue, yesterday
 *   - signatures_validated_queue, 4 days ago
 *   - signautres_pending_validation_queue, 2 days ago
 *
 * In this case, the timestamp we want to find and return would be the unix
 * timestamp for 4 days before now, that is, time().
 *
 * @return int
 *   The oldest date that all queues were last emptied.
 */
function _signatures_queue_queues_last_emptied() {
  $queues_last_emptied = time();
  $two_weeks_ago = time() - 14 * 24 * 60 * 60;

  // By default, assume queues are emptied at least every two weeks.
  $queues = array(
    'signatures_submitted_queue' => $two_weeks_ago,
    'signatures_validated_queue' => $two_weeks_ago,
    'signautres_pending_validation_queue' => $two_weeks_ago,
  );

  // Invoke a hook to let implementers of queue backends determine when queue
  // was last emptied.
  /*
  // TODO Placeholder. Implement under PT-960.
  foreach ($queues as $queue_name => $value) {
  $result = module_invoke(‘signatures_queue_last_emptied’, $queue_name);
  // TODO Use this value in place of $two_weeks_ago, wherever available.
  }
  // */

  // Get the oldest (lowest) timemestamp. Because we don't want to accidentally
  // archive data that may not be done being processed.
  $queues_last_emptied = min($queues);

  return $queues_last_emptied;
}
