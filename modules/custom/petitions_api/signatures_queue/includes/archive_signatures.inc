<?php

/**
 * @file
 * The "archive signatures" workflow.
 *
 * Archives pending signatures once they are no longer valid.  If archiving is
 * enabled, it will move rows from the signatures_pending_validation table to
 * the signatures_not_validated_archive table.
 */

/**
 * Initiates archive of signatures.
 *
 * This function should not be invoked directly. Use
 * signatures_queue_invoke_workflow() instead.
 *
 * @param string $job_id
 *   A unique identifier for the job, as created by
 *   _signatures_queue_create_job_id().
 * @param string $server_name
 *   An arbitrary identifier for the server processing the workflow.
 * @param string $worker_name
 *   An arbitrary identifier for the invoking process (e.g., cron job).
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
function _signatures_queue_archive_signatures($job_id, $server_name, $worker_name, array $options = array()) {
  // Define watchdog_suffix for logging.
  $watchdog_suffix = _signatures_queue_watchdog_suffix('archive_signatures', $job_id, $server_name, $worker_name);

  // Find out the oldest timestamp after which ALL queues have been emptied.
  $queues_last_emptied = _signatures_queue_queues_last_emptied();

  // Determine if archiving is enabled.
  $archiving_is_enabled = variable_get('signatures_queue_archive_invalid_signatures', 1);

  // Archive pending signatures not validated in time to be counted toward a
  // petition.
  if ($archiving_is_enabled) {
    _signatures_queue_archive_invalid_signatures($queues_last_emptied, $watchdog_suffix);
  }

  // Delete pending signatures not validated in time to be counted toward a
  // petition.
  _signatures_queue_delete_invalid_signatures($queues_last_emptied, $watchdog_suffix);

  if ($archiving_is_enabled) {
    // Archive orphaned validations.
    $ids = _signatures_queue_archive_orphaned_validations($queues_last_emptied, $watchdog_suffix);
    // Delete orphaned validations which were archived.
    _signatures_queue_delete_orphaned_validations($queues_last_emptied, $watchdog_suffix, $ids);
  }
  else {
    // Delete orphaned validations.
    _signatures_queue_delete_orphaned_validations($queues_last_emptied, $watchdog_suffix);
  }

  // Archive processed signatures after a petition closes.
  if ($archiving_is_enabled) {
    _signatures_queue_archive_processed_signatures($queues_last_emptied, $watchdog_suffix);
  }

  // Delete processed signatures from processing table after petition closes.
  _signatures_queue_delete_processed_signatures($queues_last_emptied, $watchdog_suffix);

  // Archive processed validations after a petition closes.
  if ($archiving_is_enabled) {
    _signatures_queue_archive_processed_validations($queues_last_emptied, $watchdog_suffix);
  }

  // Delete processed validations from processing table after petition closes.
  _signatures_queue_delete_processed_validations($queues_last_emptied, $watchdog_suffix);

  return SIGNATURES_QUEUE_STATUS_OK;
}

/**
 * Archive pending signatures that were not validated before petition closed.
 *
 * @param int $queues_last_emptied
 *   The oldest date that all queues were last emptied.
 * @param string $watchdog_suffix
 *   A string of job details as created by _signatures_queue_watchdog_suffix().
 */
function _signatures_queue_archive_invalid_signatures($queues_last_emptied, $watchdog_suffix) {
  // Set the active database to the signatures_processing db.
  signatures_queue_set_db();

  // Select signatures from the pending validation table whose
  // timestamp_validation_close is older than (before) the last time queues
  // were emptied. These will be moved from the pending validation table to the
  // not validated table.
  $query = db_select('signatures_pending_validation', 's')
    ->fields('s', array(
      'sid',
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
      'timestamp_initiated_signature_validation',
      'timestamp_received_new_signature'))
    ->condition('timestamp_validation_close', $queues_last_emptied, '<')
    ->execute();

  // Variable to count the results to determine how many rows will be moved.
  $archived_count = 0;

  // Set the active database to the signatures_archive db.
  signatures_queue_set_db('signatures_archive');

  // Insert the invalid signatures into the signatures_not_validated_archive
  // table.
  while ($row = $query->fetchAssoc()) {
    try {
      db_insert('signatures_not_validated_archive')
        ->fields($row)
        ->execute();
      $archived_count++;
    }
    catch (PDOException $e) {
      petitionslog_event('exceptions.signatures_queue.4b4729c');
      watchdog('signatures_queue', 'Failed to archive item to signatures_not_validated_archive due to a database error: item: !item, exception: !exception. @suffix', array(
        '!item' => petitionslog_format_for_watchdog($row),
        '!exception' => petitionslog_format_for_watchdog($e),
        '@suffix' => $watchdog_suffix,
      ), WATCHDOG_CRITICAL);
    }
  }
  $count_signatures_not_validated_archive = db_select('signatures_not_validated_archive')->countQuery()->execute()->fetchField();
  _signatures_queue_data_store_size_event('signatures_not_validated_archive', $count_signatures_not_validated_archive);

  // Log the number of successfully archived signatures.
  if ($archived_count) {
    watchdog('signatures_queue', '@number records archived in signatures_not_validated_archive. Records moved from signatures_pending_validation to signatures_not_validated_archive represent pending signatures that were not validated before their corresponding petition(s) stopped accepting signatures. @suffix', array(
      '@number' => $archived_count,
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_INFO);
    petitionslog_event('signatures_queue.data_store.signatures_not_validated_archive.item_added', 'count', $archived_count);

    // Set the active database back to default.
    db_set_active();
  }
}

/**
 * Delete pending signatures that were not validated before petition closed.
 *
 * @param int $queues_last_emptied
 *   The oldest date that all queues were last emptied.
 * @param string $watchdog_suffix
 *   A string of job details as created by _signatures_queue_watchdog_suffix().
 */
function _signatures_queue_delete_invalid_signatures($queues_last_emptied, $watchdog_suffix) {
  // Set the active database to the signatures_processing db.
  signatures_queue_set_db();

  // Delete signatures that are no longer valid from the pending table.
  $deleted_signatures = db_delete('signatures_pending_validation')
    ->condition('timestamp_validation_close', $queues_last_emptied, '<')
    ->execute();

  // Log the number of signatures deleted from signatures_pending_validation.
  watchdog('signatures_queue', '@number records retrieved and deleted from signatures_pending_validation. These signature submissions were not validated before their corresponding petitions closed. @suffix', array(
      '@number' => number_format($deleted_signatures),
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_INFO);

  // Set the active database back to default.
  db_set_active();
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
 * @param string $watchdog_suffix
 *   A string of job details as created by _signatures_queue_watchdog_suffix().
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
      'vid',
      'secret_validation_key',
      'timestamp_received_signature_validation',
      'timestamp_validation_close',
      'client_ip',
      'petition_id'))
    ->isNull('b.secret_validation_key')
    ->condition('a.timestamp_validation_close', $queues_last_emptied, '<');

  $results = $query->execute();
  $columns = array();

  // Variable to count the results to determine how many rows will be moved.
  $orphaned_count = 0;

  // Set the active database to the signatures_archive db.
  signatures_queue_set_db('signatures_archive');

  // Insert the orphaned validations into validations_orphaned_archive table.
  while ($row = $results->fetchAssoc()) {
    $columns[] = $row['secret_validation_key'];
    try {
      db_insert('validations_orphaned_archive')
        ->fields($row)
        ->execute();
      $orphaned_count++;
    }
    catch (PDOException $e) {
      petitionslog_event('exceptions.signatures_queue.fa29d17');
      watchdog('signatures_queue', 'Failed to archive item to validations_orphaned_archive due to a database error: item: !item, exception: !exception. @suffix', array(
        '!item' => petitionslog_format_for_watchdog($row),
        '!exception' => petitionslog_format_for_watchdog($e),
        '@suffix' => $watchdog_suffix,
      ), WATCHDOG_CRITICAL);
    }
  }

  // Log the number of successfully archived orphaned signatures.
  if ($orphaned_count) {
    watchdog('signatures_queue', '@number records archived in validations_orphaned_archive. Orphaned validations are records from the validations table that did not have a corresponding record in the signatures_pending_validation table. These are likely the result of someone tampering with signature validation URLs. @suffix', array(
        '@number' => $orphaned_count,
        '@suffix' => $watchdog_suffix,
      ), WATCHDOG_ALERT);
    petitionslog_event("signatures_queue.data_store.validations_orphaned_archive.item_added", 'count', $orphaned_count);
  }
  $count_total = db_select('validations_orphaned_archive')->countQuery()->execute()->fetchField();
  _signatures_queue_data_store_size_event('validations_orphaned_archive', $count_total);

  // Set the active database back to default.
  db_set_active();

  return $columns;
}

/**
 * Deletes orphaned validations.
 *
 * (See _signatures_queue_archive_orphaned_validations() for clarification on
 * what an "orphaned validation" is.)
 *
 * @param int $queues_last_emptied
 *   The oldest date that all queues were last emptied.
 * @param string $watchdog_suffix
 *   A string of job details as created by _signatures_queue_watchdog_suffix().
 */
function _signatures_queue_delete_orphaned_validations($queues_last_emptied, $watchdog_suffix, $columns = FALSE) {
  // Set the active database to the signatures_processing db.
  signatures_queue_set_db();

  if (!$columns) {
    // Query to select records from validations that do not have matching
    // secret_validation_key records in signatures_pending_validation.
    $query = db_select('validations', 'a');
    $query->leftJoin('signatures_pending_validation', 'b', 'a.secret_validation_key = b.secret_validation_key');
    $query
      ->fields('a', array(
        'vid',
        'secret_validation_key',
        'timestamp_received_signature_validation',
        'timestamp_validation_close',
        'client_ip',
        'petition_id'))
      ->isNull('b.secret_validation_key')
      ->condition('a.timestamp_validation_close', $queues_last_emptied, '<');
    $columns = $query->execute()->fetchCol(1);
  }

  //Initialize variable.
  $deleted_signatures = 0;

  // Delete signatures that are no longer valid from the pending table.
  if (count($columns)) {
    $deleted_signatures = db_delete('validations')
      ->condition('secret_validation_key', $columns, 'IN')
      ->execute();
  }

  // Log the number of signatures deleted from validations.
  if ($deleted_signatures) {
    watchdog('signatures_queue', '@number orphaned validation records retrieved and deleted from validations table. @suffix', array(
        '@number' => empty($deleted_signatures) ? 0 : number_format($deleted_signatures),
        '@suffix' => $watchdog_suffix,
      ), WATCHDOG_INFO);
  }

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
  $last_emptied_timestamps = signatures_queue_check_empty();

  // When using the AWS SQS, $queue->numberOfItems() returns only an approximate
  // number, and could return a false 'empty' response. To add an extra layer of
  // protection, we're also adding a minimum lifetime to signatures. I.E.
  // they cannot be archived until they are at least x days old.
  $minimum_lifetime = strtotime(variable_get('signatures_queue_minimum_signature_lifetime', '2 weeks')) - time();
  $last_emptied_timestamps['signatures_queue_minimum_signature_lifetime'] = time() - $minimum_lifetime;

  $queues_last_emptied = min($last_emptied_timestamps);

  return $queues_last_emptied;
}

/**
 * Archive processed signatures that were validated before petition closed.
 *
 * @param int $queues_last_emptied
 *   The oldest date that all queues were last emptied.
 * @param string $watchdog_suffix
 *   A string of job details as created by _signatures_queue_watchdog_suffix().
 */
function _signatures_queue_archive_processed_signatures($queues_last_emptied, $watchdog_suffix) {
  // Set the active database to the signatures_processing db.
  signatures_queue_set_db();

  // Select signatures from the processed signatures table whose
  // timestamp_validation_close is older than (before) the last time queues
  // were emptied. These will be moved from the processed signatures table to
  // the processed signatures archive table.
  $query = db_select('signatures_processed', 's');
  $query
    ->fields('s', array(
      'sid',
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
      'timestamp_initiated_signature_validation',
      'timestamp_received_new_signature',
      'timestamp_processed_signature'))
    ->condition('timestamp_validation_close', $queues_last_emptied, '<');

  // Variable to count the results to determine how many rows will be moved.
  $results = $query->execute();
  $archived_count = 0;

  // Set the active database to the signatures_archive db.
  signatures_queue_set_db('signatures_archive');

  // Insert the processed signatures into the signatures_processed_archive
  // table.
  while ($row = $results->fetchAssoc()) {
    try {
      db_insert('signatures_processed_archive')
        ->fields($row)
        ->execute();
      $archived_count++;
    }
    catch (PDOException $e) {
      petitionslog_event('exceptions.signatures_queue.9491ced');
      watchdog('signatures_queue', 'Failed to archive an item to signatures_processed_archive due to a database error: item: !item, exception: !exception. @suffix', array(
        '!item' => petitionslog_format_for_watchdog($row),
        '!exception' => petitionslog_format_for_watchdog($e),
        '@suffix' => $watchdog_suffix,
      ), WATCHDOG_CRITICAL);
    }
  }

  $count_signatures_processed_archive = db_select('signatures_processed_archive')->countQuery()->execute()->fetchField();
  _signatures_queue_data_store_size_event('signatures_processed_archive', $count_signatures_processed_archive);

  // Log the number of successfully archived signatures.
  watchdog('signatures_queue', '@number records from signatures_processed table archived in signatures_processed_archive. These records represent either signatures correctly validated and counted before their corresponding petitions closed, or illegitimate submissions that were deliberately never. @suffix', array(
    '@number' => $archived_count,
    '@suffix' => $watchdog_suffix,
  ), WATCHDOG_INFO);
  petitionslog_event('signatures_queue.data_store.signatures_processed_archive.item_added', 'count', $archived_count);

  // Set the active database back to default.
  db_set_active();
}

/**
 * Delete processed signatures that were validated before petition closed.
 *
 * @param int $queues_last_emptied
 *   The oldest date that all queues were last emptied.
 * @param string $watchdog_suffix
 *   A string of job details as created by _signatures_queue_watchdog_suffix().
 */
function _signatures_queue_delete_processed_signatures($queues_last_emptied, $watchdog_suffix) {
  // Set the active database to the signatures_processing db.
  signatures_queue_set_db();

  // Delete signatures that are no longer valid from the pending table.
  $deleted_signatures = (int) db_delete('signatures_processed')
    ->condition('timestamp_validation_close', $queues_last_emptied, '<')
    ->execute();
  $count_signatures_processed = db_select('signatures_processed')->countQuery()->execute()->fetchField();
  _signatures_queue_data_store_size_event('signatures_processed', $count_signatures_processed);

  // Log the number of signatures deleted from signatures_processed.
  watchdog('signatures_queue', '@number records retrieved and deleted from signatures_processed. @suffix', array(
      '@number' => number_format($deleted_signatures),
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_INFO);
  petitionslog_event('signatures_queue.data_store.signatures_processed.item_removed', 'count', $deleted_signatures);

  // Set the active database back to default.
  db_set_active();
}

/**
 * Archive processed validations that were checked before petition closed.
 *
 * @param int $queues_last_emptied
 *   The oldest date that all queues were last emptied.
 * @param string $watchdog_suffix
 *   A string of job details as created by _signatures_queue_watchdog_suffix().
 */
function _signatures_queue_archive_processed_validations($queues_last_emptied, $watchdog_suffix) {
  // Set the active database to the signatures_processing db.
  signatures_queue_set_db();

  // Select validations from the processed validation table whose
  // timestamp_validation_close is older than (before) the last time queues
  // were emptied. These will be moved from the processed validations table to
  // the processed validations archive table.
  $query = db_select('validations_processed', 'v')
    ->fields('v')
    ->condition('timestamp_validation_close', $queues_last_emptied, '<');
  $results = $query->execute();

  // Variable to count the results to determine how many rows will be moved.
  $archived_count = 0;

  // Set the active database to the signatures_archive db.
  signatures_queue_set_db('signatures_archive');

  // Insert the processed validations into the validations_processed_archive
  // table.
  while ($row = $results->fetchAssoc()) {
    try {
      db_insert('validations_processed_archive')
        ->fields($row)
        ->execute();
      $archived_count++;
    }
    catch (PDOException $e) {
      petitionslog_event('exceptions.signatures_queue.807e94e');
      watchdog('signatures_queue', 'Failed to archive item to validations_processed_archive due to a database error: item: !item, exception: !exception. @suffix', array(
        '!item' => petitionslog_format_for_watchdog($row),
        '!exception' => petitionslog_format_for_watchdog($e),
        '@suffix' => $watchdog_suffix,
      ), WATCHDOG_CRITICAL);
    }
  }

  $count = db_select('validations_processed_archive')->countQuery()->execute()->fetchField();
  _signatures_queue_data_store_size_event('validations_processed_archive', $count);

  // Log the number of successfully archived validations.
  watchdog('signatures_queue', '@number records archived in validations_processed_archive. These records represent either validations correctly matched to pending signatures and counted before their corresponding petitions closed, or illegitimate submissions that were deliberately never counted. @suffix', array(
    '@number' => $archived_count,
    '@suffix' => $watchdog_suffix,
  ), WATCHDOG_INFO);
  petitionslog_event('signatures_queue.data_store.validations_processed_archive.item_added', 'count', $archived_count);

  // Set the active database back to default.
  db_set_active();
}

/**
 * Delete processed validations that were checked before petition closed.
 *
 * @param int $queues_last_emptied
 *   The oldest date that all queues were last emptied.
 * @param string $watchdog_suffix
 *   A string of job details as created by _signatures_queue_watchdog_suffix().
 */
function _signatures_queue_delete_processed_validations($queues_last_emptied, $watchdog_suffix) {
  // Set the active database to the signatures_processing db.
  signatures_queue_set_db();

  // Delete signatures that are no longer valid from the pending table.
  $deleted_validations = (int) db_delete('validations_processed')
    ->condition('timestamp_validation_close', $queues_last_emptied, '<')
    ->execute();

  $count = db_select('validations_processed')->countQuery()->execute()->fetchField();
  _signatures_queue_data_store_size_event('validations_processed', $count);

  // Log the number of validations deleted from validations_processed.
  watchdog('signatures_queue', '@number records retrieved and deleted from validations_processed. These validations were correctly matched before their corresponding petitions closed. @suffix', array(
      '@number' => number_format($deleted_validations),
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_INFO);
  petitionslog_event('signatures_queue.data_store.validations_processed.item_removed', 'count', $deleted_validations);

  // Set the active database back to default.
  db_set_active();
}
