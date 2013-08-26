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
 *   The name of the server processing the workflow.
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
 * Logs signature processing events to the database.
 *
 * @param string $job_id
 *   A unique identifier for the job, as created by
 *   _signatures_queue_create_job_id().
 * @param string $workflow
 *   The name of the workflow to invoke, as defined in
 *   signatures_queue_get_workflow_names().
 * @param string $server_name
 *   The name of the server processing the workflow.
 * @param string $worker_name
 *   An arbitrary identifier for the invoking process (e.g., cron job).
 * @param string $status
 *   Either "started" or "completed".
 */
function _signatures_queue_log_event($job_id, $workflow, $server_name, $worker_name, $status) {
  // Ignore if $status is invalid.
  if (!in_array($status, array('started', 'completed'))) {
    return;
  }

  // Log to watchdog regardless of whether event logging is enabled or not.
  $watchdog_suffix = _signatures_queue_watchdog_suffix($workflow, $job_id, $server_name, $worker_name);
  watchdog('signatures_queue', '@status job. @suffix', array(
    '@status' => ucfirst($status),
    '@job_id' => $job_id,
    '@suffix' => $watchdog_suffix,
  ), WATCHDOG_INFO);

  // Return if event logging is disabled.
  $logging_enabled = variable_get('signatures_queue_log_events', SIGNATURES_QUEUE_LOG_EVENTS_DEFAULT);
  if (!$logging_enabled) {
    return;
  }

  // Log to database.
  signatures_queue_set_db();
  db_insert('signatures_queue_log')
    ->fields(array(
      'job_id' => $job_id,
      'workflow' => $workflow,
      'server_name' => $server_name,
      'worker_name' => $worker_name,
      'timestamp' => time(),
      'status' => $status,
    ))
    ->execute();
  // Switch back to default db (if signatures_processing is separate).
  db_set_active();
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
 *   The name of the server processing the workflow.
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
    return token_replace($text, $variables, array('language' => $language, 'callback' => 'signatures_queue_validation_email_tokens', 'sanitize' => FALSE, 'clear' => TRUE));
  }

  return $text;
}
