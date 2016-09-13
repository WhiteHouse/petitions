<?php

/**
 * @file
 * The "process signatures" workflow.
 *
 * Signature processing includes these steps:
 *  - Match records from validations to signatures_pending_validation tables.
 *  - If signature (email address) does not have a corresponding user, create
 *    one.
 *  - If a signature record with this email does not exist in
 *    petition_signatures create one, and get the signature id.
 */

/**
 * Process signatures.
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
function _signatures_queue_process_signatures($job_id, $server_name, $worker_name, array $options) {

  signatures_queue_debug_logger('Beginning to process signatures.');

  $watchdog_suffix = _signatures_queue_watchdog_suffix('process_signatures', $job_id, $server_name, $worker_name);
  $limit = signatures_queue_get_queue_batch_size('process_signatures');
  $debug = variable_get('signatures_queue_debug', FALSE);

  $queue_name = 'signature_validation_processor_queue';
  $queue = SignaturesQueue::get($queue_name);
  $queue->createQueue();

  $items_in_queue = (int) $queue->numberOfItems();

  logger_event('signatures_queue.data_store.signatures_pending_validation.batch', 'gauge', $items_in_queue);

  // If there are fewer items in the queue than the batch size the workflow has
  // "caught up".
  if ($items_in_queue < $limit) {
    logger_event('signatures_queue.workflow.process_signatures.caught_up');
  }

  $count = 0;
  $count_skipped = 0;

  // Process the batch.
  $number_to_process = min($limit, $items_in_queue);
  for ($i = 1; $i <= $number_to_process; $i++) {
    if ($item = $queue->claimItem()) {
      timer_start('claim_duration');

      // Set variable for item ID so it exists after the item is deleted.
      $queue_item_id = $item->item_id;
      if (empty($queue_item_id)) {
        $queue_item_id = substr(md5(microtime()), 0, 10);
      }
      signatures_queue_debug_logger('Claimed item from queue. ID: @id.', array('@id' => $queue_item_id));

      // Make sure timestamps are valid and petition IDs match.
      if (!_signatures_queue_process_signatures_assert_legitimate($item, $debug) || !_signatures_queue_process_signatures_valid_petition($item->petition_id, $debug)) {
        // Skip processing illegitimate item.
        $item->signature_id = 'invalid';
        _signatures_queue_process_signatures_log_times($item);
        // Keep track of how many invalid matches are skipped and considered
        // "processed".
        $count_skipped++;
        $queue->deleteItem($item);
        $timer = timer_stop('claim_duration');
        $time = (int) $timer['time'];
        logger_event('signatures_queue.data_flow.time_elapsed.signature_item_processing_duration', 'time', $time);
        // Delete the timer variable to prevent accumulation.
        unset($GLOBALS['timers']['claim_duration']['start']);

        signatures_queue_debug_logger('Released item as invalid. ID: @id.', array('@id' => $queue_item_id));

        continue;
      }

      signatures_queue_debug_logger('Beginning user creation. ID: @id.', array('@id' => $queue_item_id));

      // Create user if a user with this email does not exist yet.
      $user = _signatures_queue_process_signatures_create_user($item);

      signatures_queue_debug_logger('Ending user creation, beginning getting signature ID. ID: @id.', array('@id' => $queue_item_id));

      // Get signature_id, if this user has already signed.
      $item->signature_id = _signatures_queue_process_signatures_get_signature_id($item->petition_id, $user->uid);

      // Determine if this is a new signature.
      $new_signature = (!$item->signature_id) ? TRUE : FALSE;
      if ($new_signature) {

        signatures_queue_debug_logger('Ending getting signature ID, beginning saving signature. ID: @id.', array('@id' => $queue_item_id));

        // If this is a new signature, create new signature record.
        $signature_id = _signatures_queue_process_signatures_save_signature($item, $user, $debug, $watchdog_suffix);

        signatures_queue_debug_logger('Ending saving signature. ID: @id.', array('@id' => $queue_item_id));

        if (empty($signature_id)) {
          // Log failure.
          logger_event('exceptions.signatures_queue.f79c9a5');
          watchdog('signatures_queue', 'New signature could not be created. secret_validation_key: @secret_validation_key, vid: @vid, petition_id: @petition_id, timestamp_received_signature_validation: @timestamp_received_signature_validation. @suffix', array(
            '@secret_validation_key' => $item->secret_validation_key,
            '@vid' => $item->vid,
            '@petition_id' => $item->petition_id,
            '@timestamp_received_signature_validation' => $item->timestamp_received_signature_validation,
            '@suffix' => $watchdog_suffix,
          ), WATCHDOG_CRITICAL);
          // Do NOT move items to processed table or remove from preprocess table.
          $queue->releaseItem($item);
          $timer = timer_stop('claim_duration');
          $time = (int) $timer['time'];
          logger_event('signatures_queue.data_flow.time_elapsed.signature_item_processing_duration', 'time', $time);
          // Delete the timer variable to prevent accumulation.
          unset($GLOBALS['timers']['claim_duration']['start']);

          signatures_queue_debug_logger('Signature could not be saved, re-queueing. ID: @id.', array('@id' => $queue_item_id));

          continue;
        }
      }
      else {

        signatures_queue_debug_logger('Ending getting signature ID, not saving signature as it is a duplicate. ID: @id.', array('@id' => $queue_item_id));

        $watchdog_suffix = _signatures_queue_watchdog_suffix('process_signatures', $job_id, $server_name, $worker_name);
        watchdog('signatures_queue', 'This user already signed this petition. No new signature record will be created or counted. uid: @uid, petition_id: @petition_id, secret_validation_key: @secret_validation_key, vid: @vid,  timestamp_received_signature_validation: @timestamp_received_signature_validation. @suffix', array(
          '@uid' => $user->uid,
          '@petition_id' => $item->petition_id,
          '@secret_validation_key' => $item->secret_validation_key,
          '@vid' => $item->vid,
          '@timestamp_received_signature_validation' => $item->timestamp_received_signature_validation,
          '@suffix' => $watchdog_suffix,
        ), WATCHDOG_NOTICE);

        // Debug. Log signature item processed.
        if ($debug) {
          watchdog('signatures_queue', 'DEBUG ENABLED: Signature item processed: !item. @suffix', array(
            '!item' => petition_format_for_watchdog($item),
            '@suffix' => $watchdog_suffix,
          ), WATCHDOG_DEBUG);
        }
      }

      signatures_queue_debug_logger('Beginning queue item clean up tasks. ID: @id.', array('@id' => $queue_item_id));

      _signatures_queue_process_signatures_log_times($item);

      $count++;
      $queue->deleteItem($item);
      $timer = timer_stop('claim_duration');
      $time = (int) $timer['time'];
      logger_event('signatures_queue.data_flow.time_elapsed.signature_item_processing_duration', 'time', $time);
      // Delete the timer variable to prevent accumulation.
      unset($GLOBALS['timers']['claim_duration']['start']);

      signatures_queue_debug_logger('Ending queue item clean up tasks. ID: @id', array('@id' => $queue_item_id));
    }
    else {
      break;
    }
  }

  signatures_queue_debug_logger('Beginning final logging.');

  _signatures_queue_data_store_size_event('signatures_pending_validation', $queue->numberOfItems());
  _signatures_queue_data_store_size_event('signatures_processed', $count);

  // Log successes.
  $watchdog_suffix = _signatures_queue_watchdog_suffix('process_signatures', $job_id, $server_name, $worker_name);
  watchdog('signatures_queue', '@number valid signatures have been recorded. (Records from validations table were matched to signatures_pending_validation table, processed, and counted.) @suffix', array(
    '@number' => $count,
    '@suffix' => $watchdog_suffix,
  ), WATCHDOG_INFO);

  // Log failures.
  if ($count_skipped) {
    $watchdog_suffix = _signatures_queue_watchdog_suffix('process_signatures', $job_id, $server_name, $worker_name);
    watchdog('signatures_queue', 'Warning: @number records from the validations table were matched to corresponding records in signatures_pending_validation, but were NOT counted because the records are not valid. This may indicate that someone has tampered with a validation link (changing the petition_id or validation_close_timestamp) or logic in the codes base has changed somewhere.  @suffix', array(
      '@number' => $count_skipped,
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_WARNING);
  }

  signatures_queue_debug_logger('Ending final logging, processing signatures complete.');

  return SIGNATURES_QUEUE_STATUS_OK;
}

/**
 * Sanity checks to assert that the signature is legitimate.
 *
 * Confirm validated time is before petition close time.
 *
 * If petition IDs in the two records from validations and
 * signatures_pending_validation do NOT match, this is suspicious behavior
 * indicating someone potentially gaming the system. Alert and notify people.
 *
 * @param string $item
 *   The item from the database query.
 * @param bool $debug
 *   Switch to enable debugging with verbose logging.
 *
 * @return bool
 *   Returns TRUE if item is legitimate.
 */
function _signatures_queue_process_signatures_assert_legitimate($item, $debug = FALSE) {
  // Innocent until proven guilty.
  $legitimate = TRUE;

  // Check timestamps.
  if ($item->timestamp_received_signature_validation > $item->timestamp_validation_close) {
    // This suggests suspicious activity, notify admin.
    $params = array_merge($item, array(
      'subject' => t('Timestamp discrepancy'),
      'notice' => t("There's something funny about this signature validation. Signatures cannot be validated (see timestamp_received_signature_validation) after a petition is closed (see timestamp_validation_close, this is when we stop allowing people to sign this particular petition). Either the logic in our code has changed, or someone is tampering with validation URLs to trick us into counting validations after a petition is closed."),
    ));
    _signatures_queue_notify_admin($params);
    $legitimate = FALSE;
  }

  // Check petition IDs.
  if ($item->petition_id != $item->validated_petition_id) {
    // This suggests suspicious activity, notify admin.
    $params = array_merge($item, array(
      'subject' => t('Petition ID discrepancy'),
      'notice' => t("There's something funny about this signature validation. When the signature was first submitted, it was submitted with one petition ID. When the signature was validated, the URL included a different petition ID. Either the logic in our code has changed, or someone is tampering with validation URLs (for example, to trick people into signing one petition when they think they signed a different one)."),
    ));
    _signatures_queue_notify_admin($params);
    $legitimate = FALSE;
  }

  // Debug?
  if ($debug && !$legitimate) {
    watchdog('signatures_queue', 'DEBUG ENABLED: Signature flagged illegitimate: info = !info', array(
      '!info' => petition_format_for_watchdog($params),
    ), WATCHDOG_DEBUG);
  }

  return $legitimate;
}

/**
 * Determine whether a given petition ID is valid.
 *
 * If petition IDs in the two records from validations and
 * signatures_pending_validation do NOT match, this is suspicious behavior
 * indicating someone potentially gaming the system. Alert and notify people.
 *
 * @param string $petition_id
 *   The petition ID.
 * @param bool $debug
 *   Switch to enable debugging.
 *
 * @return bool
 *   Returns TRUE if petition is valid, FALSE if not.
 */
function _signatures_queue_process_signatures_valid_petition($petition_id, $debug = FALSE) {
  // Make sure petition exists.
  $nid = petition_get_nid($petition_id);
  $petition_found = db_query("SELECT n.nid FROM {node} n WHERE n.type = 'petition' AND n.nid = :nid", array(':nid' => $nid))->fetchObject();

  if (!$petition_found) {
    if ($debug) {
      watchdog('signatures_queue', 'Invalid petition: id = @id', array(
        '@id' => $petition_id,
      ), WATCHDOG_DEBUG);
    }
    return FALSE;
  }

  // If not found to be invalid, it's valid.
  return TRUE;
}

/**
 * If user with this email does not exist, create one.
 *
 * @param string $item
 *   The item from the database query.
 *
 * @return object
 *   Returns a Drupal user object.
 */
function _signatures_queue_process_signatures_create_user($item) {
  if ($uid = petition_user_exists_by_mail($item->email)) {
    $user = user_load($uid);
    return $user;
  }

  // Create users in the same format as wh_core_user_register_validate().
  $user = (object) array(
    'is_new' => TRUE,
    'mail' => trim($item->email),
    'name' => _signatures_queue_process_signatures_get_unique_username($item->email),
    'status' => 1,
  );
  $new_user = user_save($user);

  // Confirm that Profile2 is enabled.
  if (module_exists('profile2')) {
    // Invoke profile2_get_types() to retrieve an array of all profiles.
    foreach (profile2_get_types() as $type_name => $profile_type) {
      // Confirm that the profile is available on user registration forms.
      if (empty($profile_type->data->registration)) {
        continue;
      }
      // Save profile data.
      $profile = profile2_create(array('type' => $type_name, 'user' => $new_user));
      $profile->field_first_name[LANGUAGE_NONE][0]['value'] = $item->first_name;
      $profile->field_last_name[LANGUAGE_NONE][0]['value'] = $item->last_name;
      $profile->field_zip[LANGUAGE_NONE][0]['value'] = $item->zip;
      // Invoke petitions_signatures_display_location() for format location.
      if (module_exists('petitions_signatures_display')) {
        $location = FALSE;
        if (!empty($item->zip) && strlen($item->zip) == 5 && is_numeric($item->zip)) {
          $location = petitions_signatures_display_location($item->zip);
        }

        if (is_array($location)) {
          $loc = reset($location);
          $profile->field_city[LANGUAGE_NONE][0]['value'] = $loc['city'];
          $profile->field_state[LANGUAGE_NONE][0]['value'] = $loc['state'];
          ($loc['country'] == 'us') AND $loc['country'] = 'United States';
          $profile->field_country[LANGUAGE_NONE][0]['value'] = $loc['country'];
        }
        else {
          $profile->field_city[LANGUAGE_NONE][0]['value'] = $item->city;
          $profile->field_state[LANGUAGE_NONE][0]['value'] = $item->state;
          $profile->field_country[LANGUAGE_NONE][0]['value'] = $item->country;
        }
      }
      profile2_save($profile);
    }
  }

  return $new_user;
}

/**
 * Add record to signature_validations table.
 *
 * Add signature ID and API key to signature_validations table. (Note: Possible
 * many-to-one relationship with signature records in petition_signatures and
 * validations in signatures_validations.)
 *
 * @param string $item
 *   The item from the database query.
 * @param string $watchdog_suffix
 *   A string of job details as created by _signatures_queue_watchdog_suffix().
 *
 * @return bool
 *   Returns TRUE on success.
 */
function _signatures_queue_process_signatures_add_to_signatures_validations($item, $watchdog_suffix) {
  // Save to database.
  try {
    db_insert('signature_validations')
      ->fields(array(
        'secret_validation_key' => $item->secret_validation_key,
        'signature_id' => $item->signature_id,
        'petition_id' => $item->petition_id,
        'signature_source_api_key' => $item->signature_source_api_key,
        'email' => $item->email,
        'timestamp_processed_signature' => time(),
      ))
      ->execute();
  }
  catch (PDOException $exception) {
    // @todo Abuse detection.
    logger_event('exceptions.signatures_queue.a49d9ef');
    watchdog('signatures_queue', 'An item could not be added due to a database error: item: !item, exception: !exception. @suffix', array(
      '!item' => petition_format_for_watchdog($item),
      '!exception' => petition_format_for_watchdog($exception),
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_CRITICAL);
  }

  return TRUE;
}

/**
 * Log time elapsed since passing through previous workflows.
 *
 * @param array $item
 *   The queue item.
 */
function _signatures_queue_process_signatures_log_times($item) {
  $elapsed_since_receiving_signature_validation = signatures_queue_get_microtime_elapsed_since($item->timestamp_received_signature_validation);
  logger_event('signatures_queue.data_flow.time_elapsed.received_signature_validation__to__processed_signature', 'time', $elapsed_since_receiving_signature_validation);
  $elapsed_since_preprocessing_signatures = signatures_queue_get_microtime_elapsed_since($item->timestamp_preprocessed_signature);
  logger_event('signatures_queue.data_flow.time_elapsed.preprocessed_signature__to__processed_signature', 'time', $elapsed_since_preprocessing_signatures);
  $elapsed_since_preprocessing_validation = signatures_queue_get_microtime_elapsed_since($item->timestamp_preprocessed_validation);
  logger_event('signatures_queue.data_flow.time_elapsed.preprocessed_validation__to__processed_signature', 'time', $elapsed_since_preprocessing_validation);
}

/**
 * Get a unique username to correspond with a given email address.
 *
 * @param string $email
 *   The email address.
 *
 * @return string
 *   A unique username.
 */
function _signatures_queue_process_signatures_get_unique_username($email) {
  // uniqid() gets a prefixed unique identifier based on the current time in
  // microseconds, so it should never create a duplicate, even in a
  // multithreaded processing scenario.
  $prefix = '';
  $more_entropy = TRUE;
  $unique_id = uniqid($prefix, $more_entropy);
  $unique_id_length = strlen($unique_id);

  // The user "name" column in the database cannot exceed 60 characters, so the
  // "safe email" value is truncated accordingly.
  // @see user_schema()
  $sanitized_email = preg_replace("/[^\x80-\xF7 [:alnum:]@]/", '_', trim($email));
  $max_username_length = 60;
  $max_email_length = $max_username_length - $unique_id_length;
  $safe_email = substr($sanitized_email, 0, $max_email_length);

  return "{$safe_email}{$unique_id}";
}

/**
 * Gets a signature ID based on a petition ID and user ID.
 *
 * @param string|int $petition_id
 *   A petition ID.
 * @param int $uid
 *   A user ID.
 *
 * @return string|int|false
 *   The ID of the signature if found or FALSE if not. IDs may be
 *   strings (legacy MongoDB IDs) or integers (entity IDs).
 */
function _signatures_queue_process_signatures_get_signature_id($petition_id, $uid) {
  $signatures = SignaturesSelectQueryFactory::create()
    ->setPetitionId($petition_id)
    ->setUid($uid)
    ->execute()
    ->getResult();
  return (!empty($signatures[0]['id'])) ? $signatures[0]['id'] : FALSE;
}

/**
 * Create a new signature record.
 *
 * @param object $item
 *   Signature data that has worked its way down the pipeline through the
 *   signature queue.
 * @param object $user
 *   Drupal user, owner of this signature.
 * @param bool $debug
 *   Enable debugging.
 * @param string $watchdog_suffix
 *   Suffix for watchdog.
 *
 * @return string|false
 *   The ID of the signature created, or FALSE if none could be created.
 */
function _signatures_queue_process_signatures_save_signature($item, $user, $debug, $watchdog_suffix) {
  $petition = PetitionsController::loadObject($item->petition_id);

  // No such petition could be found.
  if (!$petition) {
    return FALSE;
  }

  $signature = new SignatureItem();
  // Set the legacy petition ID first else we end up with no petition ID at all.
  // We have to set it in the case when mongo reads are disabled but mongo
  // writes are still enabled.
  $signature
    ->setLegacyPetitionId($petition->getLegacyId())
    ->setPetitionId($petition->getEntityId())
    ->setFirstName($item->first_name)
    ->setLastName($item->last_name)
    ->setCreated($item->timestamp_received_new_signature)
    ->setUid($user->uid)
    ->setUser($user)
    ->setZip($item->zip)
    ->setIpAddress($item->client_ip);
  $signature = SignaturesController::save($signature);

  // Store metrics for fraud reporting.
  petition_update_email_ip_metrics($item->petition_id, $item->email, $item->client_ip);
  petition_set_metadata_value($item->petition_id, 'petition_signatures_updated', 1);

  // Add signature ID and API key to signatures_validations table.
  _signatures_queue_process_signatures_add_to_signatures_validations($item, $watchdog_suffix);

  // Subscribe to list if requested.
  if ((bool) $item->signup) {
    $signatory = array(
      'first_name' => $item->first_name,
      'last_name' => $item->last_name,
      'email' => $item->email,
      'zip' => $item->zip,
      'petition_id' => $item->petition_id,
    );
    petition_increment_metadata($item->petition_id, PetitionItem::PETITION_SIGNATURE_COUNT_EMAIL_SIGNUPS);
    module_invoke_all('petition_signatory_opt_in', $signatory);
  }

  if ($debug) {
    if ($signature->getId() !== NULL) {
      watchdog('signatures_queue', 'DEBUG ENABLED: Signature has been saved. Signature entity: !signature', array(
        '!signature' => petition_format_for_watchdog($signature->toEntity()),
      ), WATCHDOG_DEBUG);
    }
  }

  return $signature->getId();

}
