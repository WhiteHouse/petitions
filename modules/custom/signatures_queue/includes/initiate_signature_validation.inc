<?php

/**
 * @file
 * The "initiate signature validation" step.
 *
 * Retrieve and delete batches of signatures from signatures_submitted_queue,
 * send a validation email, then store signature info in
 * signatures_pending_validation_queue.
 */

/**
 * Initiates signature validation.
 *
 * @param string $server_name
 *   The name of the active server.
 * @param string $worker_name
 *   An arbitrary worker identifier.
 *
 * @return bool
 *   Returns TRUE if the function completes or FALSE if not.
 */
function signatures_queue_initiate_signature_validation($job_id, $server_name, $worker_name) {
  $watchdog_suffix = _signatures_queue_watchdog_suffix('initiate_signature_validation', $job_id, $server_name, $worker_name);

  // Retrieve and delete signatures from the signatures_submitted_queue.
  $signatures = _signatures_queue_retrieve_submitted_signatures();

  // Return if there's nothing to do.
  if (empty($signatures)) {
    return TRUE;
  }

  // Assign secret validation keys to each signature.
  $signatures = _signatures_queue_assign_validation_keys($signatures);

  // Add md5 evaluation key to prevent spam.
  $signatures = _signatures_queue_add_md5_evaluation_keys($signatures);

  // Send validation emails.
  $signatures = _signatures_queue_send_validation_emails($signatures);

  // Advance successfully-processed signatures to the next workflow.
  _signatures_queue_advance_successful_signatures($signatures['sent']);

  // Return failed signatures to the queue for re-processing.
  _signatures_queue_return_failed_signatures($signatures['not sent']);

  return TRUE;
}

/**
 * Retrieve and delete signatures from the signatures_submitted_queue.
 *
 * @param int $batch_size
 *   The number of signatures to process in this batch.
 *
 * @return array
 *   Array of signature submissions.
 */
function _signatures_queue_retrieve_submitted_signatures($batch_size = 0) {
  // If this function is called directly, the calling function can determine
  // batch size by passing it as a parameter. Otherwise use admin-defined batch
  // size, or default to 1.
  if (empty($batch_size)) {
    $batch_size = variable_get('signatures_queue_initiate_signature_validation_batch_size', SIGNATURES_QUEUE_BATCH_SIZE_DEFAULT);
  }

  // Retrieve items from signatures_submitted_queue.
  $signatures = array();
  $queue = SignaturesQueue::get('signatures_submitted_queue');
  $queue->createQueue();
  for ($i = 0; $i < $batch_size; $i++) {
    $item = $queue->claimItem();

    // claimItem() returns FALSE when there are no more items in the queue.
    if (!$item) {
      break;
    }

    $signatures[] = $item->data;

    // Remove item from signatures_submitted_queue (AWS SQS only guarantees you
    // can retrieve each message once, if we don't successfully complete
    // processing, we should re-queue, rather than just leave the item sitting
    // in the queue).
    $queue->deleteItem($item);
  }

  return $signatures;
}

/**
 * Assign secret validation keys to each signature.
 *
 * @param array $signatures
 *   Array of submitted signatures.
 *
 * @return array
 *   Array of submitted signatures with secret validation keys assigned.
 */
function _signatures_queue_assign_validation_keys(array $signatures) {
  $signatures_with_keys = array();

  // Generate secret validation keys.
  foreach ($signatures as $signature) {
    $email = $signature['email'];
    $timestamp = $signature['timestamp_submitted'];
    $petition_id = $signature['petition_id'];
    $salt = variable_get('signatures_queue_salt', '');
    $signature['secret_validation_key'] = sha1($email . $timestamp . $petition_id . $salt);
    $signatures_with_keys[] = $signature;
  }

  return $signatures_with_keys;
}

/**
 * Add md5 evaluation key to prevent spam.
 *
 * @param array $signatures
 *   Array of submitted signatures.
 *
 * @return array
 *   Array of submitted signatures with evaluation keys.
 */
function _signatures_queue_add_md5_evaluation_keys(array $signatures) {
  $signatures_with_keys = array();

  // Generate secret validation keys.
  foreach ($signatures as $signature) {
    $signature['md5_encryption_key'] = md5($signature['secret_validation_key']);
    $signatures_with_keys[] = $signature;
  }

  return $signatures_with_keys;
}

/**
 * Send validation emails.
 *
 * @param array $signatures
 *   Array of submitted signatures.
 *
 * @return array
 *   Array of signatures organized like this:
 *     array('sent' => $signatures_sent, 'not sent' => $signatures_not_sent)
 */
function _signatures_queue_send_validation_emails(array $signatures) {
  $signatures_mailed = array('sent' => array(), 'not sent' => array());

  // TODO Looks like we can only call drupal_mail() one-by-one. Is there any way
  // to submit these in bulk? Or do we need to make an API call to for every
  // send?
  foreach ($signatures as $signature) {
    // Send validation email.
    $to = $signature['email'];
    global $language;
    $params = array('signature info' => $signature);
    $result = drupal_mail('signatures_queue', 'initiate_signature_validation', $to, $language, $params);

    // TODO Figure out how we determine success/failure here...
    $text = 'This is where a message gets sent to ' . $signature['first_name'] . ' ' . $signature['last_name'];
    drupal_set_message($text);
    // VERIFY SUCCESS HERE.
    if ($result) {
      $signatures_mailed['sent'][] = $signature;
    }
    else {
      $signatures_mailed['not sent'][] = $signature;
    }
  }

  return $signatures_mailed;
}

/**
 * Advances signatures to "signatures_pending_validation_queue".
 *
 * @param array $signatures
 *   An array of signatures.
 */
function _signatures_queue_advance_successful_signatures(array $signatures) {
  $queue = SignaturesQueue::get('signatures_pending_validation_queue');
  $queue->createQueue();
  foreach ($signatures as $signature) {
    $queue->createItem($signature);
  }
}

/**
 * Returns signatures to "signatures_submitted_queue".
 *
 * @param array $signatures
 *   An array of signatures.
 */
function _signatures_queue_return_failed_signatures(array $signatures) {
  $queue = SignaturesQueue::get('signatures_submitted_queue');
  $queue->createQueue();
  foreach ($signatures as $signature) {
    $queue->createItem($signature);
  }
}