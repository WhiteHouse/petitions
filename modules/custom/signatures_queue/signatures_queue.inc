<?php

/**
 * @file
 * Helper functions for the signatures_queue module.
 */

/**
 * Creates a unique job ID.
 *
 * @param string $workflow
 *   The name of the workflow to invoke, as defined in
 *   signatures_queue_get_workflow_names().
 * @param string $server_name
 *   The name of the server processing the workflow.
 * @param string $worker_name
 *   An arbitrary identifier for the invoking process (e.g., cron job).
 *
 * @return int
 *   An md5 hash (a 32-character hexadecimal number).
 */
function _signatures_queue_create_job_id($workflow, $server_name, $worker_name) {
  return md5($workflow . $server_name . $worker_name . time());
}

/**
 * Logs signature processing events to the database.
 *
 * @param int $job_id
 *   A unique identifier for the job as created by
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
  // Just return if logging is disabled.
  $logging_enabled = variable_get('signatures_queue_log_events', SIGNATURES_QUEUE_LOG_EVENTS_DEFAULT);
  if (!$logging_enabled) {
    return;
  }

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
