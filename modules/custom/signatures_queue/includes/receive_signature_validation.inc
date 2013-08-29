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
 *   (optional) The name of the server processing the workflow. Defaults to
 *   SIGNATURES_QUEUE_SERVER_NAME_DEFAULT.
 * @param string $worker_name
 *   (optional) An arbitrary identifier for the invoking process (e.g., cron
 *   job). Defaults to SIGNATURES_QUEUE_WORKER_NAME_DEFAULT.
 * @param array $options
 *   (optional) An array of arbitrary options data as required by particular
 *   workflows. Defaults to an empty array.
 *
 * @return string
 *   Returns "thank-you/{$petition_id}" if hashes match or "error" if they
 *   don't. @todo Since signatures_queue_invoke_workflow() expects a boolean
 *   return, this is actually a violation of its interface and suggests that
 *   something somewhere needs to be refactored.
 *
 * @see signatures_queue_invoke_workflow()
 */
function _signatures_queue_receive_signature_validation($job_id, $server_name = SIGNATURES_QUEUE_SERVER_NAME_DEFAULT, $worker_name = SIGNATURES_QUEUE_WORKER_NAME_DEFAULT, array $options = array()) {
  $is_valid = FALSE;
  $return = '';

  // Confirm that the md5 passed to Drupal through the validation URL is the MD5
  // hash of the secret_validation_key. (This is a simple check to try and keep
  // spam out. NOTE: More complex checks can be added here if we need to harden the 
  // application.
  if ($options['md5'] == md5($options['secret_validation_key'])) {
    $is_valid = TRUE;
  }

  // Confirm the petition is still open for signature confirmations. If not,
  // send user to the petition-error page.
  if ($options['validation_expiration'] < time()) {
    $return = 'petition-error';
  }

  // If request is valid, process it.
  if ($is_valid && empty($return)) {
    // Construct the validated signature data array  for signatures_validated_queue.
    $signature_data = _signatures_queue_validated_signature_data($options['secret_validation_key'], $options['petition_id']);

    // Queue validated signatures for next step.
    $queue = SignaturesQueue::get('signatures_validated_queue');
    $queue->createQueue();
    $queue->createItem($signature_data);

    // Build redirect path where thank you message will be displayed.
    $return = 'thank-you/' . $options['petition_id'];
  }
  elseif (empty($return)) {
    $return = 'error';
  }

  return $return;
}

/**
 * Constructs the validated signature data array.
 *
 * @param string $secret_validation_key
 *   The validated secret validation key.
 *
 * @param string $petition_id
 *   The petition ID associated with the signature.
 *
 * @return array
 *   Data associated to the validated signature as an array.
 */
function _signatures_queue_validated_signature_data($secret_validation_key, $petition_id) {
  $signature_data = array();

  $signature_data['secret_validation_key'] = $secret_validation_key;
  $signature_data['timestamp_validated'] = time();
  $signature_data['client_ip'] = ip_address();
  $signature_data['petition_id'] = $petition_id;

  return $signature_data;
}
