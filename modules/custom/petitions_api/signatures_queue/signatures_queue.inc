<?php

/**
 * @file
 * Helper functions for the signatures_queue module.
 */

/**
 * Creates a unique job ID.
 *
 * @param string $workflow
 *   The name of the workflow being invoked, as defined in
 *   signatures_queue_get_workflow_names().
 * @param string $server_name
 *   An arbitrary identifier for the server processing the workflow.
 * @param string $worker_name
 *   An arbitrary identifier for the invoking process (e.g., cron job).
 *
 * @return string
 *   An md5 hash (a 32-character hexadecimal number).
 */
function _signatures_queue_create_job_id($workflow, $server_name, $worker_name) {
  return md5($workflow . $server_name . $worker_name . time());
}

/**
 * Builds a string of job details for use in watchdog messages.
 *
 * This ensures consistency in the formatting of the job details appended to
 * watchdog messages about signature processing events.
 *
 * Example usage:
 * @code
 * $watchdog_suffix = _signatures_queue_watchdog_suffix('initiate_signature_validation', $job_id, $server_name, $worker_name);
 * watchdog('signatures_queue', 'Performed example action. @suffix', array(
 *   '@suffix' => $watchdog_suffix,
 * ), WATCHDOG_INFO);
 * @endcode
 *
 * @param string $workflow
 *   The name of the workflow being invoked, as defined in
 *   signatures_queue_get_workflow_names().
 * @param string $job_id
 *   A unique identifier for the job, as created by
 *   _signatures_queue_create_job_id().
 * @param string $server_name
 *   An arbitrary identifier for the server processing the workflow.
 * @param string $worker_name
 *   An arbitrary identifier for the invoking process (e.g., cron job).
 *
 * @return string
 *   A string of job details.
 */
function _signatures_queue_watchdog_suffix($workflow, $job_id, $server_name, $worker_name) {
  $suffix = "Workflow: {$workflow}. Job: {$job_id}. Server name: {$server_name}. Worker name: {$worker_name}.";
  return $suffix;
}

/**
 * Returns a mail string for a variable name.
 *
 * Used by the Signatures Queue settings form to retrieve strings.
 */
function _signatures_queue_validation_mail_text($key, $language = NULL, $variables = array(), $replace = TRUE) {
  $langcode = isset($language) ? $language->language : NULL;

  if ($admin_setting = variable_get('user_mail_' . $key, FALSE)) {
    // An admin setting overrides the default string.
    $text = $admin_setting;
  }
  else {
    // No override, return default string.
    switch ($key) {
      case 'initiate_signature_validation_subject':
        $text = t('Petition signature validation instructions for [petition:title] at [site:name]', array(), array('langcode' => $langcode));
        break;

      case 'initiate_signature_validation_body':
        $text = t("[signature:first-name],

A signature has been submitted to [petition:title] on your behalf by [signature:signature-source-name] and requires your confirmation. Please validate your signature by clicking this link or copying and pasting it to your browser:

[signature:validation-url]

You can view [petition:title] by clicking this link or copying and pasting it to your browser:

[petition:url]

--  [site:name] team", array(), array('langcode' => $langcode));
        break;
    }
  }

  if ($replace) {
    // We do not sanitize the token replacement, since the output of this
    // replacement is intended for an e-mail message, not a web browser.
    return token_replace($text, $variables, array(
      'language' => $language,
      'callback' => 'signatures_queue_validation_email_tokens',
      'sanitize' => FALSE,
      'clear' => TRUE,
    ));
  }

  return $text;
}

/**
 * Send an notification to a configured admin email address.
 *
 * Ideally this will be refactored to sending a digest of alerts and a dashboard
 * of recent notifications.
 */
function _signatures_queue_notify_admin($params) {
  global $language;

  $to = variable_get('signatures_queue_notify_email');
  if (empty($to)) {
    // Fall back to address of user 1.
    $user = user_load(1);
    $to = $user->mail;
  }
  drupal_mail('signatures_queue', 'notify_admin', $to, $language, $params);
}

/**
 * Provide the text for sending admin alerts.
 */
function _signatures_queue_notify_admin_text($part, $language, $params) {
  switch ($part) {
    case 'subject':
      return t('ALERT: Signature Queue !subject', array('!subject' => $params['subject']));

    case 'body':
      $required = array(
        'first_name',
        'last_name',
        'email',
        'secret_validation_key',
        'petition_id',
        'timestamp_petition_close',
      );
      foreach ($required as $r) {
        if (empty($params[$r])) {
          $params[$r] = t('missing !r', array('!r' => $r));
        }
      }
      $body = array(
        t('The Signature Queue found an item which suggests tampering. Please review the following signature:'),
        t('!fn !ln !email', array(
          '!fn' => $params['first_name'],
          '!ln' => $params['last_name'],
          '!email' => $params['email'])),
        t('secret_validation_key: !s', array('!s' => $params['secret_validation_key'])),
        t('petition_id: !p', array('!p' => $params['petition_id'])),
        t('timestamp_petition_close: !t', array('!t' => $params['timestamp_petition_close'])),
        '',
      );
      if (!empty($params['notice'])) {
        $body[] = t('Notice:') . ' ' . $params['notice'];
      }
      return $body;
  }
}

/**
 * Find out when queue(s) were last emptied.
 *
 * @param string $name
 *   (Optional) The machine name of a signatures queue.
 *
 * @return array
 *   Returns array keyed by queue name(s). Value is a timestamp of when queue
 *   was last emptied.
 */
function _signatures_queue_check_empty($name = NULL) {
  $last_emptied = array();

  // If a queue name was specified, check only that queue.
  if ($name) {
    $queue_names = array($name);
  }
  // Otherwise, check all queues.
  else {
    $queue_names = array_keys(signatures_queue_get_queue_names());
  }

  foreach ($queue_names as $queue_name) {
    if (!SignaturesQueue::isValidQueueName($queue_name)) {
      continue;
    }

    // Make sure every queue exists. There is no harm in trying to recreate an
    // existing queue.
    $queue = SignaturesQueue::get($queue_name);
    $queue->createQueue();

    $now = time();
    $num_items = $queue->numberOfItems();
    _signatures_queue_data_store_size_event($queue_name, $num_items);

    // For any empty queue, updated corresponding last_emptied variable.
    if ($num_items == 0) {
      $last_emptied[$queue_name] = $now;
      variable_set('signatures_queue_' . $queue_name . '_last_emptied', $now);
    }
    // Otherwise, get existing last_emptied variable for later comparison.
    else {
      $last_emptied[$queue_name] = variable_get('signatures_queue_' . $queue_name . '_last_emptied', 0);
    }
  }

  // If all queues were checked, update signatures_queue_oldest_emptied.
  if (!$name) {
    $oldest_emptied = min($last_emptied);
    variable_set('signatures_queue_oldest_emptied', $oldest_emptied);
    watchdog('signatures_queue', 'Empty check was performed on all signature queues and logged. Oldest emptied queue was logged as @oldest_emptied.', array('@oldest_emptied' => $oldest_emptied));
  }
  else {
    watchdog('signatures_queue', 'Empty check was performed on @queue_name queue and logged.', array('@queue_name' => $name));
  }

  return $last_emptied;
}

/**
 * Generates a validation link.
 *
 * @param string $url
 *   The website URL, without a trailing slash.
 * @param array $data
 *   The signature data.
 * @param bool $absolute
 *   (optional) TRUE to return an absolute URL or FALSE to return only the link
 *   (with a leading slash). Defaults to TRUE.
 *
 * @return string
 *   The validation link.
 */
function _signatures_queue_validation_link($url, $data, $absolute = TRUE) {
  $key = $data['secret_validation_key'];
  $md5 = $data['md5_encryption_key'];
  $petition_id = $data['petition_id'];
  $validation_expiration = $data['timestamp_validation_close'];
  $link = "/thank-you?k={$key}&m={$md5}&p={$petition_id}&d={$validation_expiration}";
  return $absolute ? $url . $link : $link;
}
