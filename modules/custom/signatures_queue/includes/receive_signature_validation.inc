<?php
/**
 * @file
 * The "receive signature validation" step.
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
function signatures_queue_receive_signature_validation($job_id, $server_name = SIGNATURES_QUEUE_SERVER_NAME_DEFAULT, $worker_name = SIGNATURES_QUEUE_WORKER_NAME_DEFAULT, array $options = array()) {
  // Confirm that 'm' is the MD5 hash of 'k'.
  if ($options['md5'] == md5($options['validation_key'])) {
    // Construct the validated signature data array.
    $signature_data = _signatures_queue_validated_signature_data($options['signature'], $options['petition_id']);

    // Queue validated signatures for next step.
    $queue = SignaturesQueue::get('signatures_validated_queue');
    $queue->createQueue();
    $queue->createItem($signature_data);

    $path_redirect = 'thank-you/' . $options['petition_id'];

    return $path_redirect;
  }
  // Hashes do not match. Return error.
  else {
    return 'error';
  }
}

/**
 * Constructs the validated signature data array.
 *
 * @param string $signature
 *   The validated secret validation key.
 *
 * @param string $petition_id
 *   The petition ID associated with the signature.
 *
 * @return array
 *   Data associated to the validated signature as an array.
 */
function _signatures_queue_validated_signature_data($signature, $petition_id) {
  $signature_data = array();

  $signature_data['secret_validation_key'] = $signature;
  $signature_data['timestamp_validated'] = time();
  $signature_data['client_ip'] = ip_address();
  $signature_data['petition_id'] = $petition_id;

  return $signature_data;
}
