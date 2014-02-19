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
 * @return bool
 *   Returns TRUE if the workflow executes successfully or FALSE if not.
 *
 * @see signatures_queue_invoke_workflow()
 */
function _signatures_queue_process_signatures($job_id, $server_name, $worker_name, $options) {
  $limit = signatures_queue_get_queue_batch_size('process_signatures');

  // Set the active database to the signatures_processing db.
  signatures_queue_set_db();

  // Match records from validations to signatures_pending_validation.
  $query = db_select('signatures_pending_validation', 'p');
  $query->join('validations', 'v', 'p.secret_validation_key=v.secret_validation_key');
  $query->fields('p')->range(0, $limit);
  // Add fields from the validations table.
  $query->addField('v', 'vid');
  $query->addField('v', 'client_ip');
  $query->addField('v', 'petition_id', 'validated_petition_id');
  $query->addField('v', 'timestamp_validated');

  $result = $query->execute();

  // Set the active database back to default, in case queue backend wasn't using
  // Drupal's database.
  db_set_active();

  $count = 0;
  $count_illegitimate = 0;

  // Process the batch.
  while ($item = $result->fetchAssoc()) {
    // Make sure timestamps are valid and petition IDs match.
    if (!_signatures_queue_process_signatures_assert_legitimate($item)) {
      // Skip processing illegitimate item.
      $item['signature_id'] = 'illegitimate';
      _signatures_queue_process_signatures_move_to_processed($item);
      // Keep track of how many invalid matches are skipped and considered
      // "processed".
      $count_illegitimate++;
      continue;
    }

    // Create user if a user with this email does not exist yet.
    $user = _signatures_queue_process_signatures_create_user($item);

    // Confirm wh_petitions module is enabled before calling it's functions.
    // Only save signatures into mongodb if that shunt is not enabled, see
    // wh_petitions_shunt() for further details.
    if (module_exists('wh_petitions') && !shunt_is_enabled('wh_petitions_signature_create')) {
      // Get signature_id, if this user has already signed.
      $item['signature_id'] = wh_petitions_get_signature_id($item['petition_id'], $user->uid);

      // If this is a new signature...
      if (!$item['signature_id']) {
        // Create new record in petition_signatures.
        $item['signature_id'] = _signatures_queue_process_signatures_create_record($item, $user);
        // New signature was created successfully. Increment signature count.
        wh_petitions_increment_count($item['petition_id']);
      }
      else {
        // Log failure.
        $watchdog_suffix = _signatures_queue_watchdog_suffix('process_signatures', $job_id, $server_name, $worker_name);
        watchdog('signatures_queue', 'New signature could not be created. secret_validation_key: @secret_validation_key, vid: @vid, petition_id: @petition_id, timestamp_validated: @timestamp_validated. @suffix', array(
          '@secret_validation_key' => $item['secret_validation_key'],
          '@vid' => $item['vid'],
          '@petition_id' => $item['petition_id'],
          '@timestamp_validated' => $item['timestamp_validated'],
          '@suffix' => $watchdog_suffix,
        ), WATCHDOG_CRITICAL);
        // Do NOT move items to processed table or remove from preprocess table.
        continue;
      }
    }

    // Also save signature into MySQL if the shunt is not enabled. Note that
    // the interim mongo2mysql function needs an existing signature_id, so we
    // check for that also.
    // @todo: Once MongoDB has been disabled, refactor the $item data structure
    // to create and populate a signature object, then call $signature->save.
    // This will replace the interim mongo2mysql wrapper below.
    if (!shunt_is_enabled('signature_mail_mysql_save') && !empty($item['signature_id'])) {
      signature_mail_mongo2mysql_save($item);
    }

    // Add signature ID and API key to signatures_validations table.
    _signatures_queue_process_signatures_add_to_signatures_validations($item);

    // Move item to processed tables and delete from pre-process tables.
    _signatures_queue_process_signatures_move_to_processed($item);

    // Subscribe to list if requested.
    if ((bool) $item['signup']) {
      $signatory = array(
        'first_name' => $item['first_name'],
        'last_name' => $item['last_name'],
        'email' => $item['email'],
        'zip' => $item['zip'],
        'petition_id' => $item['petition_id'],
      );
      module_invoke_all('petition_signatory_opt_in', $signatory);
    }

    $count++;
  }

  // Log successes.
  $watchdog_suffix = _signatures_queue_watchdog_suffix('process_signatures', $job_id, $server_name, $worker_name);
  watchdog('signatures_queue', '@number valid signatures have been counted. (Records from validations table were matched to signatures_pending_validation table, processed, and counted.) @suffix', array(
    '@number' => $count,
    '@suffix' => $watchdog_suffix,
  ), WATCHDOG_INFO);

  // Log failures.
  if ($count_illegitimate) {
    $watchdog_suffix = _signatures_queue_watchdog_suffix('process_signatures', $job_id, $server_name, $worker_name);
    watchdog('signatures_queue', 'Warning: @number records from the validations table were matched to corresponding records in signatures_pending_validation, but were NOT counted because the records are not valid. This may indicate that someone has tampered with a validation link (changing the petition_id or validation_close_timestamp) or logic in the codes base has changed somewhere.  @suffix', array(
      '@number' => $count_illegitimate,
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_WARNING);
  }

  return TRUE;
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
 *
 * @return bool
 *   Returns TRUE if item is legitimate.
 */
function _signatures_queue_process_signatures_assert_legitimate($item) {
  // Innocent until proven guilty.
  $legitimate = TRUE;

  // Check timestamps.
  if ($item['timestamp_validated'] > $item['timestamp_validation_close']) {
    // This suggests suspicious activity, notify admin.
    $params = array_merge($item, array(
      'subject' => t('Timestamp discrepancy'),
      'notice' => t("There's something funny about this signature validation. Signatures cannot be validated (see timestamp_validated) after a petition is closed (see timestamp_validation_close, this is when we stop allowing people to sign this particular petition). Either the logic in our code has changed, or someone is tampering with validation URLs to trick us into counting validations after a petition is closed."),
    ));
    _signatures_queue_notify_admin($params);
    $legitimate = FALSE;
  }

  // Check petition IDs.
  if ($item['petition_id'] != $item['validated_petition_id']) {
    // This suggests suspicious activity, notify admin.
    $params = array_merge($item, array(
      'subject' => t('Petition ID discrepancy'),
      'notice' => t("There's something funny about this signature validation. When the signature was first submitted, it was submitted with one petition ID. When the signature was validated, the URL included a different petition ID. Either the logic in our code has changed, or someone is tampering with validation URLs (for example, to trick people into signing one petition when they think they signed a different one)."),
    ));
    _signatures_queue_notify_admin($params);
    $legitimate = FALSE;
  }
  return $legitimate;
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
  if ($user = user_load_by_mail($item['email'])) {
    return $user;
  }

  // Create users in the same format as wh_core_user_register_validate().
  $user = (object) array(
    'is_new' => TRUE,
    'mail' => trim($item['email']),
    'name' => preg_replace("/[^\x80-\xF7 [:alnum:]@]/", '_', trim($item['email'])),
    'status' => 1,
  );
  $new_user = user_save($user);

  // Confirm that Profile2 is enabled.
  if (module_exists('profile2')) {
    // Invoke profile2_get_types() to retrieve an array of all profiles.
    foreach (profile2_get_types() as $type_name => $profile_type) {
      // Confirm that the profile is available on user registration forms.
      if (empty($profile_type->data['registration'])) {
        continue;
      }
      // Save profile data.
      $profile = profile2_create(array('type' => $type_name, 'user' => $new_user));
      $profile->field_first_name[LANGUAGE_NONE][0]['value'] = $item['first_name'];
      $profile->field_last_name[LANGUAGE_NONE][0]['value'] = $item['last_name'];
      $profile->field_zip[LANGUAGE_NONE][0]['value'] = $item['zip'];
      // Invoke petitions_signatures_display_location() for format location.
      if (module_exists('petitions_signatures_display')) {
        $location = petitions_signatures_display_location($item['zip']);
        if (is_array($location)) {
          $loc = reset($location);
          $profile->field_city[LANGUAGE_NONE][0]['value'] = $loc['city'];
          $profile->field_state[LANGUAGE_NONE][0]['value'] = $loc['state'];
          ($loc['country'] == 'us') AND $loc['country'] = 'United States';
          $profile->field_country[LANGUAGE_NONE][0]['value'] = $loc['country'];
        }
        else {
          $profile->field_city[LANGUAGE_NONE][0]['value'] = $item['city'];
          $profile->field_state[LANGUAGE_NONE][0]['value'] = $item['state'];
          $profile->field_country[LANGUAGE_NONE][0]['value'] = $item['country'];
        }
      }
      profile2_save($profile);
    }
  }

  return $new_user;
}

/**
 * Create signature record.
 *
 * If a signature record with this email does not exist in petition_signatures
 * create one, and get the signature id.
 *
 * Then signature_count module increments the cached signature count. @@?
 *
 * @param string $item
 *   The item from the database query.
 * @param object $user
 *   Drupal user object for checking petition_signatures.
 *
 * @return string
 *   $signature_id The ID of the signature created
 */
function _signatures_queue_process_signatures_create_record($item, $user) {
  // The wh_petitions_create_signature function called by
  // wh_petitions_create_new_signature determines if this user has already
  // signed the petition.
  $signature_id = wh_petitions_create_new_signature($item['petition_id'], $user, $item['client_ip']);

  return $signature_id;
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
 *
 * @return bool
 *   Returns TRUE on success.
 */
function _signatures_queue_process_signatures_add_to_signatures_validations($item) {
  // Save to database.
  $id = db_insert('signature_validations')
    ->fields(array(
      'secret_validation_key' => $item['secret_validation_key'],
      'signature_id' => $item['signature_id'],
      'petition_id' => $item['petition_id'],
      'signature_source_api_key' => $item['signature_source_api_key'],
      'email' => $item['email'],
      'timestamp_processed' => time(),
    ))
    ->execute();

  return TRUE;
}


/**
 * Move items to processed tables.
 *
 * Delete the items from signatures_pending_validation and validations
 * and add them to signatures_processed and validations_processed.
 *
 * @param string $item
 *   The item from the database query.
 *
 * @return bool
 *   Returns TRUE on success.
 */
function _signatures_queue_process_signatures_move_to_processed($item) {
  // Set the active database to the signatures_processing db.
  signatures_queue_set_db();

  // Add to processed tables.
  $id = db_insert('signatures_processed')
    ->fields(array(
      'signature_source_api_key' => $item['signature_source_api_key'],
      'timestamp_petition_close' => $item['timestamp_petition_close'],
      'timestamp_validation_close' => $item['timestamp_validation_close'],
      'petition_id' => $item['petition_id'],
      'first_name' => $item['first_name'],
      'last_name' => $item['last_name'],
      'zip' => $item['zip'],
      'email' => $item['email'],
      'signup' => $item['signup'],
      'timestamp_validation_email_sent' => $item['timestamp_validation_email_sent'],
      'timestamp_submitted' => $item['timestamp_submitted'],
      'secret_validation_key' => $item['secret_validation_key'],
    ))
    ->execute();
  $id = db_insert('validations_processed')
    ->fields(array(
      'secret_validation_key' => $item['secret_validation_key'],
      'timestamp_validated' => $item['timestamp_validated'],
      'timestamp_validation_close' => $item['timestamp_validation_close'],
      'client_ip' => $item['client_ip'],
      'petition_id' => $item['petition_id'],
    ))
    ->execute();

  // Delete from pre-process tables.
  $deleted = db_delete('signatures_pending_validation')
    ->condition('sid', $item['sid'])
    ->execute();
  $deleted = db_delete('validations')
    ->condition('vid', $item['vid'])
    ->execute();

  // Set the active database back to default.
  db_set_active();
}
