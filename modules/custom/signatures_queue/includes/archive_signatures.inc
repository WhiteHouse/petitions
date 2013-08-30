<?php

/**
 * @file
 * The "archive signatures" workflow.
 *
 * Archives pending signatures once they are no longer valid.  If archiving is
 * enabled, it will move rows from the signatures_pending_validation table to
 * the signatures_not_validated table.
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
function _signatures_queue_archive_signatures($job_id, $server_name, $worker_name) {
  // Define watchdog_suffix for logging.
  $watchdog_suffix = _signatures_queue_watchdog_suffix('archive_signatures', $job_id, $server_name, $worker_name);

  // Find out the oldest timestamp after which ALL queues have been emptied.
  $queues_last_emptied = _signatures_queue_queues_last_emptied();

  // Determine if archiving is enabled.
  $archiving_is_enabled = variable_get('signatures_queue_archive_invalid_signatures', 1);

  // Archive pending signatures not validated in time to be counted toward a petition.
  if ($archiving_is_enabled) {
    _signatures_queue_archive_invalid_signatures($queues_last_emptied, $watchdog_suffix);
  }

  // Delete pending signatures not validated in time to be counted toward a petition.
  _signatures_queue_delete_invalid_signatures($queues_last_emptied, $watchdog_suffix);

  // Archive orphaned validations.
  if ($archiving_is_enabled) {
    _signatures_queue_archive_orphaned_validations($queues_last_emptied, $watchdog_suffix);
  }

  // Delete orphaned validations.
  _signatures_queue_delete_orphaned_validations($queues_last_emptied, $watchdog_suffix);

  return TRUE;
}

/**
 * Archive pending signatures that were not validated before petition closed.
 *
 * @param int $queues_last_emptied
 *   The oldest date that all queues were last emptied.
 *
 * @param string $watchdog_suffix
 *   Defined suffix for watchdog statements.
 */
function _signatures_queue_archive_invalid_signatures($queues_last_emptied, $watchdog_suffix) {
  // Set the active database to the signatures_processing db.
  signatures_queue_set_db();

  // Select signatures from the pending validation table whose
  // timestamp_validation_close is older than (before) the last time queues
  // were emptied. These will be moved from the pending validation table to the
  // not validated table.
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
    ->condition('timestamp_validation_close', $queues_last_emptied, '<');

  // Count the results to determine how many rows will be moved.
  $results = $query->execute()->fetchAll();
  $archived_count = count($results);

  // Insert the invalid signatures into the signatures_not_validated table.
  db_insert('signatures_not_validated')
    ->from($query)
    ->execute();

  // Log the number of successfully archived signatures.
  watchdog('signatures_queue', '@number records archived in signatures_not_validated. These signature submissions were not validated before their corresponding petitions closed. @suffix', array(
    '@number' => $archived_count,
    '@suffix' => $watchdog_suffix,
  ), WATCHDOG_INFO);

  // Set the active database back to default.
  db_set_active();
}

/**
 * Delete pending signatures that were not validated before petition closed.
 *
 * @param int $queues_last_emptied
 *   The oldest date that all queues were last emptied.
 *
 * @param string $watchdog_suffix
 *   Defined suffix for watchdog statements.
 */
function _signatures_queue_delete_invalid_signatures($queues_last_emptied, $watchdog_suffix) {
  // Set the active database to the signatures_processing db.
  signatures_queue_set_db();

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
 * Archives orphaned validations.
 *
 * Orphaned validations are validation records that have no corresponding
 * pending signatures. We only archive these after the petition is closed
 * because it's possible for legitimate pairs of signatures and validations to
 * come through with validations arriving in mysql before the corresponding
 * pending validation arrives. If signatures_pending_validation_queue an
 * validations_queue are processed at different rates or if they are not stored
 * in FIFO--first in first out-- queues, legit validations can actually arrive
 * first. As a result, this is the only time we're 100% sure we're not
 * erroneously determining a good validation to be an orphan.
 *
 * @param int $queues_last_emptied
 *   The oldest date that all queues were last emptied.
 *
 * @param string $watchdog_suffix
 *   Defined suffix for watchdog statements.
 */
function _signatures_queue_archive_orphaned_validations($queues_last_emptied, $watchdog_suffix) {
  // Set the active database to the signatures_processing db.
  signatures_queue_set_db();

  // Query to select records from validations that do not have matching
  // secret_validation_key records in signatures_pending_validation.
  $query = db_select('validations', 'a');
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

  // Insert the orphaned validations into validations_orphaned table.
  db_insert('validations_orphaned')
    ->from($query)
    ->execute();

  // Log the number of successfully archived orphaned signatures.
  watchdog('signatures_queue', '@number records archived in validations_orphaned. These signature submissions did not have matching secret validation keys. @suffix', array(
      '@number' => $orphaned_count,
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_INFO);

  // Set the active database back to default.
  db_set_active();
}

/**
 * Deletes orphaned validations.
 *
 * (See _signatures_queue_archive_orphaned_validations() for clarification on
 * what an "orphaned validation" is.)
 *
 * @param int $queues_last_emptied
 *   The oldest date that all queues were last emptied.
 *
 * @param string $watchdog_suffix
 *   Defined suffix for watchdog statements.
 */
function _signatures_queue_delete_orphaned_validations($queues_last_emptied, $watchdog_suffix) {
  // Set the active database to the signatures_processing db.
  signatures_queue_set_db();

  // Delete signatures that are no longer valid from the pending table.
  if ($columns) {
    $deleted_signatures = db_delete('validations')
      ->condition('secret_validation_key', array($columns), 'IN')
      ->execute();
  }

  // Log the number of signatures deleted from validations.
  watchdog('signatures_queue', '@number records retrieved and deleted from validations. These signature submissions did not have matching secret validation keys. @suffix', array(
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
 *   - validations_queue, 4 days ago
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
    'validations_queue' => $two_weeks_ago,
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
