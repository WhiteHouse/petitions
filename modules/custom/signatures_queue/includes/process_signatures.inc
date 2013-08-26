<?php
/**
 * @file
 * The "process signatures" workflow.
 *
 * Signature processing includes these steps: 
 *  - Match records from validations to signatures_pending_validation tables.
 *  - If signature (email address) does not have a corresponding user, create one.
 *  - If a signature record with this email does not exist in petition_signatures
 *    create one, and get the signature id.
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
function _signatures_queue_process_signatures(
  $job_id,
  $server_name = SIGNATURES_QUEUE_SERVER_NAME_DEFAULT,
  $worker_name = SIGNATURES_QUEUE_WORKER_NAME_DEFAULT,
  array $options = array()) {

  $limit = signatures_queue_get_queue_batch_size('process_signatures');

  // Set the active database to the signatures_processing db.
  signatures_queue_set_db();

  // Match records from validations to signatures_pending_validation.
  $query = db_select('signatures_pending_validation', 'p');
  $query->join('validations', 'v', 'p.secret_validation_key=v.secret_validation_key');
  $query->fields('p')->range(0, $limit);
  // Add fields from the signatures_validated table.
  $query->addField('v', 'vid');
  $query->addField('v', 'client_ip');
  $query->addField('v', 'petition_id', 'validated_petition_id');
  $query->addField('v', 'timestamp_validated');

  $result = $query->execute();

  // Set the active database back to default, in case queue backend wasn't using
  // Drupal's database.
  db_set_active();

  $count = 0;

  // Proccess the batch.
  while ($item = $result->fetchAssoc()) {
    // Make sure timestamps are valid and petition IDs match.
    if (!_signatures_queue_process_signatures_assert_legitimate($item)) {
      // Skip processing illegitimate item.
      continue;
    }

    // Create user if doesn't exist.
    $user = _signatures_queue_process_signatures_create_user($item);

    // Create record in petition_signatures if doesn't exist.
    $item['signature_id'] = _signatures_queue_process_signatures_create_record($item, $user);

    // Add signature ID and API key to signatures_validations table.
    _signatures_queue_process_signatures_add_to_signatures_validations($item);

    // Set the active database to the signatures_processing db.
    signatures_queue_set_db();

    // Move item to processed tables.
    _signatures_queue_process_signatures_move_to_processed($item);

    // Set the active database back to default.
    db_set_active();

    // Subscribe to list if requested.
    // Turned off until further review...
    // _signatures_queue_process_signatures_list_subscribe($item);

    $count++;
  }

  // Add signature_source_api_key to signature_sources table.
  _signatures_queue_process_signatures_add_signatures_sources();

  // Log successes.
  $watchdog_suffix = _signatures_queue_watchdog_suffix('process_signatures', $job_id, $server_name, $worker_name);
  watchdog('signatures_queue', '@number records from validations were matched to signatures_pending_validation and counted. @suffix', array(
    '@number' => $count,
    '@suffix' => $watchdog_suffix,
  ), WATCHDOG_INFO);

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
  if ($item['timestamp_validated'] > $item['timestamp_petition_close']) {
    // This suggests suspicious activity, notify admin.
    $params = array_merge($item, array(
      'subject' => t('Timestamp discrepancy'),
      'notice' => t('Timestamp validated is greater than timestamp petition close.'),
      ));
    _signatures_queue_notify_admin($params);
    $legitimate = FALSE;
  }

  // Check petition IDs.
  if ($item['petition_id'] != $item['validated_petition_id']) {
    // This suggests suspicious activity, notify admin.
    $params = array_merge($item, array(
      'subject' => t('Petition ID discrepancy'),
      'notice' => t('Validated signature has mismatched petition ID.'),
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

  $user = (object) array(
    'is_new' => TRUE,
    'mail' => $item['email'],
    'name' => $item['first_name'] . ' ' . $item['last_name'],
  );
  $new_user = user_save($user);

  return $new_user;
>>>>>>> PT-994 - completed process signatures workflow, adding admin notification
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
 * @return bool
 *   Returns TRUE on success.
 */
function _signatures_queue_process_signatures_create_record($item, $user) {
  /*
   * Much of the following pieces are copied from wh_petitions_sign_petition().
   */

  // Setup connections for signatures and petitions.
  $connection = wh_petitions_mongo_petition_connection();
  $connection->setSlaveOkay(FALSE);
  $sig_connection = wh_petitions_mongo_petition_signatures_connection();
  $sig_connection->setSlaveOkay(FALSE);

  $slim_petition_elements = array(
    'petition_status',
    'signature_count',
    'response_signatures',
    'public_signatures',
  );

  // Load the petition.
  $petition = wh_petitions_load_slim_petition($connection, $item['petition_id'], $slim_petition_elements);

  // If it wasn't a valid petition id, exit.
  if (empty($petition)) {
    return FALSE;
  }

  // The wh_petitions_create_signature function determines if this user has
  // already signed the petition.
  $signature_id = wh_petitions_create_signature($petition, $user, $sig_connection, $item['client_ip']);
  if (!$signature_id) {
    watchdog('wh_petitions', 'Failed to create signature for @user on @petition', array('@user' => $user->uid, '@petition' => $petition['_id']));
    return FALSE;
  };

  // Up the signature_count on the petition.
  wh_petitions_increment_signature_count($petition, $connection);

  // Invoke hook_save_signature. @@?
  // module_invoke_all('save_signature', $item);

  return $signature_id;
}

/**
 * Add to signatures_validations.
 *
 * Add signature ID and API key to signatures_validations table. (note: possible
 * many-to-one relationship with signature records in petition_signatures and
 * signatures_sources)
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
      //'timestamp' => time(),  @@should we store a timestamp?
    ))
    ->execute();

  return TRUE;
}


/**
 * Move items to processed tables.
 *
 * Delete the items from signatures_pending_validation and signatures_validated
 * and add them to signatures_processed and validations_processed.
 *
 * @param string $item
 *   The item from the database query.
 *
 * @return bool
 *   Returns TRUE on success.
 */
function _signatures_queue_process_signatures_move_to_processed($item) {
  // Add to processed tables.
  $id = db_insert('signatures_processed')
    ->fields(array(
      'signature_source_api_key' => $item['signature_source_api_key'],
      'timestamp_petition_close' => $item['timestamp_petition_close'],
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
      'client_ip' => $item['client_ip'],
      'petition_id' => $item['petition_id'],
    ))
    ->execute();

  // Delete from tables.
  $deleted = db_delete('signatures_pending_validation')
    ->condition('sid', $item['sid'])
    ->execute();
  $deleted = db_delete('signatures_validated')
    ->condition('vid', $item['vid'])
    ->execute();

}

/**
 * Sign up user to email list if requested.
 *
 * @@ Placeholder function for now.
 *
 * @param string $item
 *   The item from the database query.
 *
 * @return bool
 *   Returns TRUE on success.
 */
function _signatures_queue_process_signatures_list_subscribe($item) {
  // For now we skip this step.
  return;

  // Check if user signed up for email list.
  if ($item['signup']) {
    /*
     * Check for GovDelivery list and add the topic id if it wasn't
     * in the petition object previously.
     */
    $topic_title = 'petitions_' . $petition['_id']->__toString();
    $topic_id = '';

    // Add their email to GovDelivery.
    govdelivery_subscriber_api_add_subscriber($user->mail, $topic_title, $topic_id);
  }
}

/**
 * Add signature sources from the signature_validations table.
 *
 * @@ Placeholder function for now. The signatures_sources table doesn't exist
 * yet.
 *
 * @param string $item
 *   The item from the database query.
 *
 * @return bool
 *   Returns TRUE on success.
 */
function _signatures_queue_process_signatures_add_signatures_sources() {
  // db_select  DISTINCT signature_validations LEFT JOIN signatures_sources
  // WHERE signatures_sources NULL

  // while () {
    // add to signatures_sources...
  // }
}
