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
 *   Returns TRUE if the function completes or FALSE if not, e.g., if it
 *   terminates early due to a shunt trip.
 */
function signatures_queue_initiate_signature_validation($server_name, $worker_name) {
  if (shunt_is_enabled('signature_queue_initiate_signature_validation')) {
    return FALSE;
  }

  // Retrieve and delete signatures from the signatures_submitted_queue.
  $signatures = _signatures_queue_retrieve_submitted_signatures();

  // Assign secret validation keys to each signature.
  $signatures = _signatures_queue_assign_validation_keys($signatures);

  // Add md5 evaluation key to prevent spam.
  $signatures = _signatures_queue_add_md5_evaluation_keys($signatures);

  // Send validation emails.
  $signatures = _signatures_queue_send_validation_emails($signatures);

  // Queue signatures for next step.
  if (!empty($signatures['sent'])) {
    // Successfully sent records move to next queue.
    $queue = SignaturesQueue::get('signatures_pending_validation_queue');
    $queue->createQueue();
    foreach ($signatures['sent'] as $signature) {
      $queue->createItem($signature);
    }
  }
  if (!empty($signatures['not sent'])) {
    // Unsuccessful mail gets re-queued with signatures submitted.
    $queue = SignaturesQueue::get('signatures_submitted_queue');
    $queue->createQueue();
    foreach ($signatures['not sent'] as $signature) {
      $queue->createItem($signature);
    }
  }

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
    $batch_size = variable_get('signatures_queue_initiate_signature_validation_batch_size', 10);
  }

  // Retrieve items from signatures_submitted_queue.
  $signatures = array();
  $queue = SignaturesQueue::get('signatures_submitted_queue');
  $queue->createQueue();
  for ($i = 0; $i < $batch_size; $i++) {
    $item = $queue->claimItem();
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
function _signatures_queue_assign_validation_keys($signatures) {
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
function _signatures_queue_add_md5_evaluation_keys($signatures) {
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
function _signatures_queue_send_validation_emails($signatures) {
  $signatures_mailed = array();

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
