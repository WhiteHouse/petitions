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
 * @param string $server_name
 *   The name of the active server.
 * @param string $worker_name
 *   An arbitrary worker identifier.
 *
 * @return bool
 *   Returns TRUE if the function completes or FALSE if not.
 */
function signatures_queue_receive_signature_validation($server_name, $worker_name, $options) {
  // Construct the validated signature data array.
  $signature_data = _signatures_queue_validated_signature_data($options['signature'], $options['petition_id']);

  // Queue validated signatures for next step.
  $queue = SignaturesQueue::get('signatures_validated_queue');
  $queue->createQueue();
  $queue->createItem($signature_data);

  return TRUE;
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
