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
 * @throws Exception
 *   Rethrows any exceptions caught.
 *
 * @see signatures_queue_invoke_workflow()
 */
function _signatures_queue_receive_new_signatures($job_id, $server_name, $worker_name, array $options = array()) {
  $signature = (array) $options['signature'];
  $api_key = $signature['signature_source_api_key'];
  $petition_id = !empty($signature['petition_id']) ? $signature['petition_id'] : 'NONE';

  try {
    petitionslog_event("signatures_queue.workflow.receive_new_signatures.api_key.{$api_key}.invoked");
    petitionslog_event("signatures_queue.workflow.receive_new_signatures.petition_id.{$petition_id}.invoked");

    // Debugging. Log signature data received.
    if ($debug = variable_get('signatures_queue_debug', FALSE)) {
      watchdog('signatures_queue', 'DEBUG ENABLED: New signature received: !signature', array(
        '!signature' => petitionslog_format_for_watchdog($signature),
      ), WATCHDOG_DEBUG);
    }

    // Make sure the signature argument is valid. This is done first because
    // it's cheapest.
    if (!_signatures_queue_validate_signature($signature)) {
      petitionslog_event("signatures_queue.workflow.receive_new_signatures.api_key.{$api_key}.bad_request");
      petitionslog_event("signatures_queue.workflow.receive_new_signatures.petition_id.{$petition_id}.bad_request");
      return SIGNATURES_QUEUE_STATUS_BAD_REQUEST;
    }

    if (!shunt_is_enabled('signatures_queue_receive_new_signatures_validate_petition')) {
      $petition = petitions_data_get_petition($petition_id);

      // Make sure the petition exists.
      if (empty($petition)) {
        petitionslog_event("signatures_queue.workflow.receive_new_signatures.api_key.{$api_key}.not_found");
        petitionslog_event("signatures_queue.workflow.receive_new_signatures.petition_id.{$petition_id}.not_found");
        return SIGNATURES_QUEUE_STATUS_NOT_FOUND;
      }

      // Make sure the petition is "signable".
      if (!$petition['isSignable']) {
        petitionslog_event("signatures_queue.workflow.receive_new_signatures.api_key.{$api_key}.forbidden");
        petitionslog_event("signatures_queue.workflow.receive_new_signatures.petition_id.{$petition_id}.forbidden");
        return SIGNATURES_QUEUE_STATUS_FORBIDDEN;
      }
    }

    // Input is all valid. Build the queue item.
    $item = _signatures_queue_build_new_queue_item($signature);

    // Enqueue it.
    _signatures_queue_enqueue_item($item);
    _signatures_queue_log_signatory_details($api_key, $petition_id, $item);
    petitionslog_event("signatures_queue.workflow.receive_new_signatures.api_key.{$api_key}.status.ok");
    petitionslog_event("signatures_queue.workflow.receive_new_signatures.petition_id.{$petition_id}.status.ok");
    return SIGNATURES_QUEUE_STATUS_OK;
  }
  catch (Exception $e) {
    petitionslog_event('exceptions.signatures_queue.3a67b6f');
    throw $e;
  }
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
  $petition = petitions_data_get_petition($signature['petition_id']);
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
  $api_key = $item['signature_source_api_key'];
  $petition_id = $item['petition_id'];

  $queue = SignaturesQueue::get('signatures_submitted_queue');
  $queue->createQueue();
  $queue->createItem($item);

  petitionslog_event('signatures_queue.data_store.signatures_submitted_queue.item_added');
  petitionslog_event("signatures_queue.workflow.receive_new_signatures.api_key.{$api_key}.completed");
  petitionslog_event("signatures_queue.workflow.receive_new_signatures.petition_id.{$petition_id}.completed");
}

/**
 * Logs signatory details from a new signature.
 *
 * @param string $api_key
 *   The API key used to submit the signature.
 * @param string $petition_id
 *   The unique identifier of the petition.
 * @param array $signature
 *   The signature queue item as created by
 *   _signatures_queue_build_new_queue_item().
 */
function _signatures_queue_log_signatory_details($api_key, $petition_id, array $signature) {
  $is_disposable = signatures_queue_is_disposable_email($signature['email']) ? 'true' : 'false';
  $is_subaddressed = signatures_queue_is_subaddressed_email($signature['email']) ? 'true' : 'false';

  // Log in aggregate, per API key, and per petition ID metrics.
  $metric_prefixes = array(
    'signatures_queue.workflow.receive_new_signatures',
    "signatures_queue.workflow.receive_new_signatures.api_key.{$api_key}",
    "signatures_queue.workflow.receive_new_signatures.petition_id.{$petition_id}",
  );
  foreach ($metric_prefixes as $prefix) {
    petitionslog_event("{$prefix}.email_address.is_disposable.{$is_disposable}");
    petitionslog_event("{$prefix}.email_address.is_subaddressed.{$is_subaddressed}");
  }
}
