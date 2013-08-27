<?php

/**
 * @file
 * The "initiate signature validation" workflow.
 *
 * Retrieve and delete batches of signatures from signatures_submitted_queue,
 * send a validation email, then store signature info in
 * signatures_pending_validation_queue.
 */

/**
 * Initiates signature validation.
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
function signatures_queue_initiate_signature_validation($job_id, $server_name = SIGNATURES_QUEUE_SERVER_NAME_DEFAULT, $worker_name = SIGNATURES_QUEUE_WORKER_NAME_DEFAULT, array $options = array()) {
  $watchdog_suffix = _signatures_queue_watchdog_suffix('initiate_signature_validation', $job_id, $server_name, $worker_name);

  // Retrieve and delete signatures from the signatures_submitted_queue.
  $signatures = _signatures_queue_retrieve_submitted_signatures($watchdog_suffix);

  // Return if there's nothing to do.
  if (empty($signatures)) {
    return TRUE;
  }

  // Assign secret validation keys to each signature.
  $signatures = _signatures_queue_assign_validation_keys($signatures);

  // Add md5 evaluation key to prevent spam.
  $signatures = _signatures_queue_add_md5_evaluation_keys($signatures);

  // Send validation emails.
  $signatures = _signatures_queue_send_validation_emails($signatures, $watchdog_suffix);

  // Advance successfully-processed signatures to the next workflow.
  _signatures_queue_advance_successful_signatures($signatures['sent'], $watchdog_suffix);

  // Leave failed signatures in the queue for re-processing.
  _signatures_queue_leave_failed_signatures($signatures['not sent'], $watchdog_suffix);

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
function _signatures_queue_retrieve_submitted_signatures($watchdog_suffix, $batch_size = 0) {
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
  $number_of_items = $queue->numberOfItems();

  // Return if the queue is empty.
  if (!$number_of_items) {
    watchdog('signatures_queue', 'signatures_submitted_queue is empty. No validation emails will be sent. @suffix', array(
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_INFO);
    return $signatures;
  }

  // Process the queue.
  for ($i = 0; $i < $batch_size; $i++) {
    $item = $queue->claimItem();

    // claimItem() returns FALSE when there are no more items in the queue.
    if (!$item) {
      break;
    }

    $signatures[] = $item;
  }

  if ($i) {
    watchdog('signatures_queue', '@number records were retrieved from signatures_submitted_queue. @suffix', array(
      '@number' => number_format($i),
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_INFO);
  }
  else {
    watchdog('signatures_queue', 'signatures_submitted_queue contains @number record(s), but none were retrieved. @suffix', array(
      '@number' => number_format($number_of_items),
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_ERROR);
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
    $email = $signature->data['email'];
    $timestamp = $signature->data['timestamp_submitted'];
    $petition_id = $signature->data['petition_id'];
    $salt = variable_get('signatures_queue_salt', '');
    $signature->data['secret_validation_key'] = sha1($email . $timestamp . $petition_id . $salt);
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
    $signature->data['md5_encryption_key'] = md5($signature->data['secret_validation_key']);
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
function _signatures_queue_send_validation_emails(array $signatures, $watchdog_suffix) {
  $signatures_mailed = array('sent' => array(), 'not sent' => array());

  // NOTE: We're calling drupal_mail() one-by-one, but we're not sure if this is
  // the best solution here. If the cost charged by our email provider is
  // per-email-recipient (most likely), the real cost to us here is the expense
  // of making so many API calls. TODO, Revisit implementation: If drupal_mail() allows us to send in batches
  // that's probaby more efficient here.
  foreach ($signatures as $signature) {
    // Send validation email.
    $to = $signature->data['email'];
    global $language;
    $params = array('signature info' => $signature->data);
    try {
      $result = drupal_mail('signatures_queue', 'initiate_signature_validation', $to, $language, $params);
      $signatures_mailed['sent'][] = $signature;
      drush_log('Message sent to ' . $signature->data['first_name'] . ' ' . $signature->data['last_name'], 'success');
    }
    catch (Exception $e) {
      $signatures_mailed['not sent'][] = $signature;
    }
  }

  // Identify the active mail system.
  // @todo There should be a nicer way to do this once the broader question of
  //   mail handling has been addressed per PT-1003.
  $mail_systems = variable_get('mail_system', array(
    'default-system' => 'DefaultMailSystem',
  ));
  if (!empty($mail_systems['default-system']) && is_string($mail_systems['default-system'])) {
    $active_mail_system = $mail_systems['default-system'];
  }
  else {
    $active_mail_system = 'DefaultMailSystem';
  }

  // Log successes.
  if (!empty($signatures_mailed['sent'])) {
    watchdog('signatures_queue', '@number signature validation emails were sent via @mail_system. @suffix', array(
      '@number' => number_format(count($signatures_mailed['sent'])),
      '@mail_system' => $active_mail_system,
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_INFO);
  }

  // Log failures.
  if (!empty($signatures_mailed['not sent'])) {
    watchdog('signatures_queue', '@number signature validation emails failed via @mail_system. (Will try again.) @suffix', array(
      '@number' => number_format(count($signatures_mailed['not sent'])),
      '@mail_system' => $active_mail_system,
      '@suffix' => $watchdog_suffix,
    ), WATCHDOG_ERROR);
  }

  return $signatures_mailed;
}

/**
 * Advances signatures to "signatures_pending_validation_queue".
 *
 * @param array $signatures
 *   An array of signatures.
 */
function _signatures_queue_advance_successful_signatures(array $signatures, $watchdog_suffix) {
  $pending_validation_queue = SignaturesQueue::get('signatures_pending_validation_queue');
  $pending_validation_queue->createQueue();
  $submitted_queue = SignaturesQueue::get('signatures_submitted_queue');
  $submitted_queue->createQueue();
  $time = time();
  foreach ($signatures as $signature) {
    // Preprocess data to be stored.
    $record = $signature->data;
    // TODO: NOTE, This is actually probably incorrect. This is, more
    // accurately, the timestamp from when the email was handed off to Mandrill
    // to be queued for sending. Normally this should effectively be the same as
    // timestamp_validation_email_sent. But in the event that emails fail, this
    // is a really misleading bit of data. To be revisited in PT-975.
    $record['timestamp_validation_email_sent'] = $time;
    unset($record['md5_encryption_key']);
    // Add the signature to "signatures_pending_validation_queue".
    $pending_validation_queue->createItem($record);
    // Remove it from "signatures_submitted_queue".
    $submitted_queue->deleteItem($signature);
  }

  watchdog('signatures_queue', '@number records were added to signatures_pending_validation_queue. @suffix', array(
    '@number' => number_format(count($signatures)),
    '@suffix' => $watchdog_suffix,
  ), WATCHDOG_INFO);
}

/**
 * Leaves failed signatures in "signatures_submitted_queue".
 *
 * @param array $signatures
 *   An array of signatures.
 */
function _signatures_queue_leave_failed_signatures(array $signatures, $watchdog_suffix) {
  watchdog('signatures_queue', '@number records could not be processed and were left in signatures_submitted_queue for reprocessing later. @suffix', array(
    '@number' => number_format(count($signatures)),
    '@suffix' => $watchdog_suffix,
  ), WATCHDOG_INFO);
}
