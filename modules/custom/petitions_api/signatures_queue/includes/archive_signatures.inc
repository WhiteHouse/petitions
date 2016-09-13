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
 *
 * @see signatures_queue_invoke_workflow()
 */
function _signatures_queue_archive_signatures($job_id, $server_name, $worker_name, array $options = array()) {
  // Define watchdog_suffix for logging.
  $watchdog_suffix = _signatures_queue_watchdog_suffix('archive_signatures', $job_id, $server_name, $worker_name);

  // Archive and delete expired pending signature validations.
  _signatures_queue_archive_pending_signature_validations($watchdog_suffix);

  // Archive and delete processed signature validations.
  _signatures_queue_archive_processed_signature_validations($watchdog_suffix);

  // Archive and delete orphaned validations.
  _signatures_queue_archive_orphaned_validations($watchdog_suffix);

  return SIGNATURES_QUEUE_STATUS_OK;
}

/**
 * Archive and delete pending signature validations not validated in time.
 *
 * @param string $watchdog_suffix
 *   A string of job details as created by _signatures_queue_watchdog_suffix().
 *
 * @throws Exception
 */
function _signatures_queue_archive_pending_signature_validations($watchdog_suffix) {
  // Determine if archiving is enabled.
  $archiving_is_enabled = variable_get('signatures_queue_archive_invalid_signatures', 1);
  if ($archiving_is_enabled) {
    $secret_validation_keys_to_archive = _signatures_queue_get_signature_validation_keys_to_archive();
    if (!empty($secret_validation_keys_to_archive)) {
      $signatures_processed_query = db_select('signatures_pending_validation_queue', 's')
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
          'timestamp_preprocessed_signature',
        ))
        ->condition('secret_validation_key', $secret_validation_keys_to_archive, 'IN')
        ->execute();

      // Set the active database to the signatures_archive db.
      signatures_queue_set_db('signatures_archive');
      $archived_count = 0;

      while ($signature = $signatures_processed_query->fetchAssoc()) {
        try {
          db_merge('signatures_not_validated_archive')
            ->key(array('sid' => $signature['sid']))
            ->fields($signature)
            ->execute();
          $archived_count++;
        }
        catch (PDOException $e) {
          logger_event('exceptions.signatures_queue.4b4729c');
          watchdog('signatures_queue', 'Failed to archive item to signatures_not_validated_archive due to a database error: item: !item, exception: !exception. @suffix', array(
            '!item' => petition_format_for_watchdog($signature),
            '!exception' => petition_format_for_watchdog($e),
            '@suffix' => $watchdog_suffix,
          ), WATCHDOG_CRITICAL);
          // Remove the key from archived key list if it couldn't be archived.
          if (($key = array_search($signature['secret_validation_key'], $secret_validation_keys_to_archive)) !== FALSE) {
            unset($secret_validation_keys_to_archive[$key]);
          }
        }
      }

      $count_signatures_not_validated_archive = db_query('select count(sid) from {signatures_not_validated_archive}')->fetchField();
      _signatures_queue_data_store_size_event('signatures_not_validated_archive', $count_signatures_not_validated_archive);

      // Log the number of successfully archived signatures.
      if ($archived_count) {
        watchdog('signatures_queue', '@number records archived in signatures_not_validated_archive. Records moved from signatures_pending_validation_queue to signatures_not_validated_archive represent pending signatures that were not validated before their corresponding petition(s) stopped accepting signatures. @suffix', array(
          '@number' => $archived_count,
          '@suffix' => $watchdog_suffix,
        ), WATCHDOG_INFO);
        logger_event('signatures_queue.data_store.signatures_not_validated_archive.item_added', 'count', $archived_count);
      }
      // Set the active database back to default.
      db_set_active();

      // Delete the records from the primary database.
      db_delete('signatures_pending_validation_queue')
        ->condition('secret_validation_key', $secret_validation_keys_to_archive, 'IN')
        ->execute();
    }
  }
}

/**
 * Archive signature validations that have already been processed.
 *
 * @param string $watchdog_suffix
 *   A string of job details as created by _signatures_queue_watchdog_suffix().
 *
 * @throws Exception
 */
function _signatures_queue_archive_processed_signature_validations($watchdog_suffix) {
  // Determine if archiving is enabled.
  $archiving_is_enabled = variable_get('signatures_queue_archive_invalid_signatures', 1);
  if ($archiving_is_enabled) {
    $secret_validation_keys_to_archive = _signatures_queue_get_signature_validation_keys_to_archive(TRUE);
    if (!empty($secret_validation_keys_to_archive)) {

      $signatures_processed_query = db_select('signatures_pending_validation_queue', 's')
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
          'timestamp_preprocessed_signature',
        ))
        ->condition('secret_validation_key', $secret_validation_keys_to_archive, 'IN')
        ->execute();

      // Set the active database to the signatures_archive db.
      signatures_queue_set_db('signatures_archive');
      $archived_count = 0;

      while ($signature = $signatures_processed_query->fetchAssoc()) {
        try {
          db_merge('signatures_processed_archive')
            ->key(array('sid' => $signature['sid']))
            ->fields($signature)
            ->execute();
          $archived_count++;
        }
        catch (PDOException $e) {
          logger_event('exceptions.signatures_queue.9491ced');
          watchdog('signatures_queue', 'Failed to archive an item to signatures_processed_archive due to a database error: item: !item, exception: !exception. @suffix', array(
            '!item' => petition_format_for_watchdog($signature),
            '!exception' => petition_format_for_watchdog($e),
            '@suffix' => $watchdog_suffix,
          ), WATCHDOG_CRITICAL);
          // Remove the key from archived key list if it couldn't be archived.
          if (($key = array_search($signature['secret_validation_key'], $secret_validation_keys_to_archive)) !== FALSE) {
            unset($secret_validation_keys_to_archive[$key]);
          }
        }
      }

      $count_signatures_processed_archive = db_query('select count(sid) from {signatures_processed_archive}')->fetchField();
      _signatures_queue_data_store_size_event('signatures_processed_archive', $count_signatures_processed_archive);

      // Log the number of successfully archived signatures.
      watchdog('signatures_queue', '@number processed records from signatures_pending_validation_queue table archived in signatures_processed_archive. These records represent either signatures correctly validated and counted before their corresponding petitions closed. @suffix', array(
        '@number' => $archived_count,
        '@suffix' => $watchdog_suffix,
      ), WATCHDOG_INFO);
      logger_event('signatures_queue.data_store.signatures_processed_archive.item_added', 'count', $archived_count);

      // Set the active database back to default.
      db_set_active();

      // Retrieve the matching validations records.
      $archived_count = 0;
      $validations_query = db_select('pending_validations_queue', 'v')
        ->fields('v')
        ->condition('secret_validation_key', $secret_validation_keys_to_archive, 'IN')
        ->execute();

      // Set the active database to the signatures_archive db.
      signatures_queue_set_db('signatures_archive');

      // Archive the validations records.
      while ($validation = $validations_query->fetchAssoc()) {
        try {
          db_merge('validations_processed_archive')
            ->key(array('vid' => $validation['vid']))
            ->fields($validation)
            ->execute();
          $archived_count++;
        }
        catch (PDOException $e) {
          logger_event('exceptions.signatures_queue.807e94e');
          watchdog('signatures_queue', 'Failed to archive item to validations_processed_archive due to a database error: item: !item, exception: !exception. @suffix', array(
            '!item' => petition_format_for_watchdog($validation),
            '!exception' => petition_format_for_watchdog($e),
            '@suffix' => $watchdog_suffix,
          ), WATCHDOG_CRITICAL);
          // Remove the key from archived key list if it couldn't be archived.
          if (($key = array_search($signature['secret_validation_key'], $secret_validation_keys_to_archive)) !== FALSE) {
            unset($secret_validation_keys_to_archive[$key]);
          }
        }
      }
      $count = db_query('select count(vid) from {validations_processed_archive}')->fetchField();
      _signatures_queue_data_store_size_event('validations_processed_archive', $count);

      // Log the number of successfully archived validations.
      watchdog('signatures_queue', '@number records archived in validations_processed_archive. These records represent either validations correctly matched to pending signatures and counted before their corresponding petitions closed, or illegitimate submissions that were deliberately never counted. @suffix', array(
        '@number' => $archived_count,
        '@suffix' => $watchdog_suffix,
      ), WATCHDOG_INFO);
      logger_event('signatures_queue.data_store.validations_processed_archive.item_added', 'count', $archived_count);
      // Set the active database back to default.
      db_set_active();

      // Delete the records from the primary database.
      db_delete('signatures_pending_validation_queue')
        ->condition('secret_validation_key', $secret_validation_keys_to_archive, 'IN')
        ->execute();

      db_delete('pending_validations_queue')
        ->condition('secret_validation_key', $secret_validation_keys_to_archive, 'IN')
        ->execute();
    }
  }
}

/**
 * Archive and delete validations with no pending signature validation.
 *
 * @param string $watchdog_suffix
 *   A string of job details as created by _signatures_queue_watchdog_suffix().
 *
 * @throws Exception
 */
function _signatures_queue_archive_orphaned_validations($watchdog_suffix) {
  // Determine if archiving is enabled.
  $archiving_is_enabled = variable_get('signatures_queue_archive_invalid_signatures', 1);
  if ($archiving_is_enabled) {

    $query = db_select('pending_validations_queue', 'a');
    $query->leftJoin('signatures_pending_validation_queue', 'b', 'a.secret_validation_key = b.secret_validation_key');
    $query
      ->fields('a', array('vid'))
      ->isNull('b.secret_validation_key')
      ->condition('a.timestamp_validation_close', time(), '<');
    $limit = (int) variable_get('signatures_queue_archive_batch_size', 5000);
    $query->range(0, $limit);
    $validations_to_archive = $query->execute()->fetchCol();
    if (!empty($validations_to_archive)) {

      $orphaned_count = 0;
      $validations_query = db_select('pending_validations_queue', 'v')
        ->fields('v')
        ->condition('vid', $validations_to_archive, 'IN')
        ->execute();

      // Set the active database to the signatures_archive db.
      signatures_queue_set_db('signatures_archive');
      while ($validation = $validations_query->fetchAssoc()) {
        try {
          db_merge('validations_orphaned_archive')
            ->key(array('vid' => $validation['vid']))
            ->fields($validation)
            ->execute();
          $orphaned_count++;
        }
        catch (PDOException $e) {
          logger_event('exceptions.signatures_queue.fa29d17');
          watchdog('signatures_queue', 'Failed to archive item to validations_orphaned_archive due to a database error: item: !item, exception: !exception. @suffix', array(
            '!item' => petition_format_for_watchdog($validation),
            '!exception' => petition_format_for_watchdog($e),
            '@suffix' => $watchdog_suffix,
          ), WATCHDOG_CRITICAL);
          // Remove the key from archived key list if it couldn't be archived.
          if (($key = array_search($validation['vid'], $validations_to_archive)) !== FALSE) {
            unset($validations_to_archive[$key]);
          }
        }
      }

      $count_total = db_query('select count(vid) from {validations_orphaned_archive}')->fetchField();
      _signatures_queue_data_store_size_event('validations_orphaned_archive', $count_total);

      // Set the active database back to default.
      db_set_active();
      // Delete the records from the primary database.
      db_delete('pending_validations_queue')
        ->condition('vid', $validations_to_archive, 'IN')
        ->execute();

      // Log the number of successfully archived orphaned signatures.
      if ($orphaned_count) {
        watchdog('signatures_queue', '@number records archived in validations_orphaned_archive. Orphaned validations are records from the validations table that did not have a corresponding record in the signatures_pending_validation_queue table. These are likely the result of someone tampering with signature validation URLs. @suffix', array(
          '@number' => $orphaned_count,
          '@suffix' => $watchdog_suffix,
        ), WATCHDOG_ALERT);
        logger_event("signatures_queue.data_store.validations_orphaned_archive.item_added", 'count', $orphaned_count);
      }

    }
  }
}

/**
 * Fetch the secret_validation_keys that should be archived.
 *
 * @param bool|FALSE $processed
 *   Flag to determined whether we need processed validations.
 *
 * @return array
 *   Array of secret_validation_keys which match the conditions.
 */
function _signatures_queue_get_signature_validation_keys_to_archive($processed = FALSE) {

  $query = db_select('signatures_pending_validation_queue', 's')
    ->fields('s', array('secret_validation_key'))
    ->condition('s.processed', $processed, '=');
  $limit = (int) variable_get('signatures_queue_archive_batch_size', 5000);
  $query->range(0, $limit);

  if (!$processed) {
    // Select only signatures without validations.
    $query->leftJoin('pending_validations_queue', 'v', 'v.secret_validation_key = s.secret_validation_key');
    $query->isNull('v.secret_validation_key');

    // Determine the date before which unprocessed records should be archived.
    $minimum_lifetime = '-' . variable_get('signatures_queue_minimum_signature_lifetime', '2 weeks');
    $archive_date = strtotime($minimum_lifetime);
    $query->condition('s.timestamp_received_new_signature', $archive_date, '<');
  }

  return $query->execute()->fetchCol();
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
