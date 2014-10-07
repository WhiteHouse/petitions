<?php

/**
 * @file
 * The "receive new signatures" workflow.
 *
 * Receive new signatures via the API and queue them for validation.
 */

/**
 * Receives new signatures.
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
 *   - signature: The signature argument received from the service request.
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
function _signatures_queue_receive_new_signatures($job_id, $server_name, $worker_name, array $options = array()) {
  $signature = (array) $options['signature'];

  // Debugging. Log signature data received.
  if ($debug = variable_get('signatures_queue_enable_debugging', FALSE)) {
    watchdog('signatures_queue', 'DEBUG ENABLED: New signature received: !signature', array(
      '!signature' => petitionslog_format_for_watchdog($signature),
    ), WATCHDOG_DEBUG);
  }

  // Make sure the signature argument is valid. This is done first because it's
  // cheapest.
  if (!_signatures_queue_validate_signature($signature)) {
    return SIGNATURES_QUEUE_STATUS_BAD_REQUEST;
  }

  $petition = petitions_data_mongo2mysql_get_petition($signature['petition_id']);

  // Make sure the petition exists.
  if (empty($petition)) {
    return SIGNATURES_QUEUE_STATUS_NOT_FOUND;
  }

  // Make sure the petition is "signable".
  if (!$petition['isSignable']) {
    return SIGNATURES_QUEUE_STATUS_FORBIDDEN;
  }

  // Input is all valid. Build the queue item.
  $item = _signatures_queue_build_new_queue_item($signature);

  // Enqueue it.
  _signatures_queue_enqueue_item($item);

  return SIGNATURES_QUEUE_STATUS_OK;
}

/**
 * Validates a signature array.
 *
 * @param array $signature
 *   The signature argument received from the service request.
 *
 * @return bool
 *   Return TRUE if the signature array is valid or FALSE if it isn't.
 */
function _signatures_queue_validate_signature(array $signature) {
  // Make sure all required fields are supplied.
  $required_fields = array(
    'signature_source_api_key',
    'petition_id',
    'first_name',
    'last_name',
    'email',
  );
  foreach ($required_fields as $key) {
    if (empty($signature[$key])) {
      return FALSE;
    }
  }

  // Make sure email address is valid.
  if (!valid_email_address($signature['email'])) {
    return FALSE;
  }

  return TRUE;
}

/**
 * Builds a queue item from a validated service request signature argument.
 *
 * @param array $signature
 *   The signature argument received from the service request.
 *
 * @return array
 *   Returns an array suitable for use as a signature queue item.
 */
function _signatures_queue_build_new_queue_item(array $signature) {
  // Remove unexpected fields from signature array.
  $expected_fields = array(
    'signature_source_api_key' => '',
    'petition_id' => '',
    'email' => '',
    'first_name' => '',
    'last_name' => '',
    'zip' => '',
    'signup' => '',
  );
  $signature_trimmed = array_intersect_key($signature, $expected_fields);

  // Initialize the item with signature data.
  $item = array_merge($expected_fields, $signature_trimmed);

  // Add application data to the item.
  $petition = petitions_data_mongo2mysql_get_petition($signature['petition_id']);
  $validation_grace_period = (int) variable_get('signatures_queue_validation_grace_period', 2);
  $item['timestamp_received_new_signature'] = time();
  $item['timestamp_petition_close'] = $petition['deadline'];
  // The validation link is valid for today plus grace period (converted from
  // days to unix time).
  $item['timestamp_validation_close'] = time() + $validation_grace_period * 24 * 60 * 60;
  $item['secret_validation_key'] = '';

  return $item;
}

/**
 * Insert a signature queue item into the signatures_submitted_queue.
 *
 * @param array $item
 *   A signature queue item as built by
 *   _signatures_queue_build_new_queue_item().
 */
function _signatures_queue_enqueue_item(array $item) {
  $queue = SignaturesQueue::get('signatures_submitted_queue');
  $queue->createQueue();
  $queue->createItem($item);
  petitionslog_event('signatures_queue.data_store.signatures_submitted_queue.item_added');
}
