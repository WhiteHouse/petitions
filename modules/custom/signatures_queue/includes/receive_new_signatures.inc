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
 *   (optional) The name of the server processing the workflow. Defaults to
 *   SIGNATURES_QUEUE_SERVER_NAME_DEFAULT.
 * @param string $worker_name
 *   (optional) An arbitrary identifier for the invoking process (e.g., cron
 *   job). Defaults to SIGNATURES_QUEUE_WORKER_NAME_DEFAULT.
 * @param array $options
 *   - $options['signature'], signature submitted via POST request to API.
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
function _signatures_queue_receive_new_signatures($job_id, $server_name, $worker_name, $options) {
  $signature = (array) $options['signature'];

  // Make sure the signature argument is valid. This is done first because it's
  // cheapest.
  if (!_signatures_queue_validate_signature($signature)) {
    return SIGNATURES_QUEUE_STATUS_BAD_REQUEST;
  }

  $petition = petitions_data_get_petition($signature['petition_id']);

  // Make sure the petition exists.
  if (empty($petition)) {
    return SIGNATURES_QUEUE_STATUS_NOT_FOUND;
  }

  // Make sure the petition is "signable".
  $signable_statuses = array(
    WH_PETITION_STATUS_PRIVATE,
    WH_PETITION_STATUS_PUBLIC,
    WH_PETITION_STATUS_UNDER_REVIEW,
  );
  if (!in_array($petition['status'], $signable_statuses)) {
    return SIGNATURES_QUEUE_STATUS_FORBIDDEN;
  }

  // Input is all valid. Build the queue item.
  $item = _signatures_queue_build_new_queue_item($signature);

  // And enqueue it.
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
 * Builds a queue item from a validated service requrest signature argument.
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
  $item['timestamp_submitted'] = time();
  // At this point, petitions_data_petition_is_open() has been invoked and
  // cached the petition close date.
  $petition_close_date = (int) cache_get("petition_close_date:{$signature['petition_id']}", 'cache_petitions');
  $item['timestamp_petition_close'] = $petition_close_date;
  $validation_grace_period = (int) variable_get('signatures_queue_validation_grace_period', 0);
  $item['timestamp_validation_close'] = $petition_close_date + $validation_grace_period;

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
}
