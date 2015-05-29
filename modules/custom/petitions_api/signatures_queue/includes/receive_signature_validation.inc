<?php

/**
 * @file
 * The "receive signature validation" workflow.
 *
 * Compares the second parameter in the request to the MD5 hash of the first
 * parameter (the unique signature validation key) and stores timestamp, client
 * IP, secret validation, and petition ID in the signatures validated queue.
 */

/**
 * Receives valid signatures.
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
 *   (optional) An array of options. Defaults to an empty array. These (key,
 *   value) pairs are expected:
 *   - petition_id: The petition ID associated with the signature.
 *   - validation_expiration: A Unix timestamp of the date the validation
 *     expires.
 *   - secret_validation_key: A secret key for validating a signature via email.
 *   - md5: An MD5 hash of the secret validation key.
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
function _signatures_queue_receive_signature_validation($job_id, $server_name, $worker_name, array $options = array()) {
  // Make sure required URL arguments are present.
  if (empty($_GET['k']) || empty($_GET['m']) || empty($_GET['p']) || empty($_GET['d']) || !is_numeric($_GET['d'])) {
    return SIGNATURES_QUEUE_STATUS_BAD_REQUEST;
  }

  $is_valid = FALSE;
  $return = '';

  // Confirm that the md5 passed to Drupal through the validation URL is the MD5
  // hash of the secret_validation_key. (This is a simple check to try and keep
  // spam out. NOTE: More complex checks can be added here if we need to harden
  // the application.
  if ($options['md5'] == md5($options['secret_validation_key'])) {
    $is_valid = TRUE;
  }

  // Confirm the petition is still open for signature confirmations. If not,
  // send user to the petition-error page.
  if ($options['validation_expiration'] < time()) {
    $return = SIGNATURES_QUEUE_STATUS_BAD_REQUEST;
  }

  // If request is valid, process it.
  if ($is_valid && empty($return)) {
    // Construct the validated signature data array  for validations_queue.
    $signature_data = _signatures_queue_validated_signature_data($options['secret_validation_key'], $options['petition_id']);

    // Queue validated signatures for next step.
    $queue = SignaturesQueue::get('validations_queue');
    $queue->createQueue();
    $queue->createItem($signature_data);
    petitionslog_event('signatures_queue.data_store.validations_queue.item_added');

    $return = SIGNATURES_QUEUE_STATUS_OK;
  }
  elseif (empty($return)) {
    $return = SIGNATURES_QUEUE_STATUS_NOT_FOUND;
  }

  // Debugging. Log options and signature data.
  if ($debug = variable_get('signatures_queue_debug', FALSE)) {
    watchdog('signatures_queue', 'DEBUG ENABLED: Options passed to receive_signature_validation workflow: !options', array(
      '!options' => petitionslog_format_for_watchdog($options),
    ), WATCHDOG_DEBUG);
    watchdog('signatures_queue', 'DEBUG ENABLED: Validation data added to validations_queue: !validation', array(
      '!validation' => petitionslog_format_for_watchdog($signature_data),
    ), WATCHDOG_DEBUG);
  }

  return $return;
}

/**
 * Constructs the validated signature data array.
 *
 * @param string $secret_validation_key
 *   The validated secret validation key.
 * @param string $petition_id
 *   The petition ID associated with the signature.
 *
 * @return array
 *   Data associated to the validated signature as an array.
 */
function _signatures_queue_validated_signature_data($secret_validation_key, $petition_id) {
  $signature_data = array();

  $signature_data['secret_validation_key'] = $secret_validation_key;
  $signature_data['timestamp_received_signature_validation'] = time();
  $signature_data['client_ip'] = ip_address();
  $signature_data['petition_id'] = $petition_id;

  return $signature_data;
}
