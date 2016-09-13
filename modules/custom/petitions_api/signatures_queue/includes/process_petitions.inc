<?php

/**
 * @file
 * The "process petitions" workflow.
 *
 * Iterates through petition_metadata and performs actions.
 */

/**
 * Process Petitions.
 *
 * This function should not be invoked directly. Use
 * signatures_queue_invoke_workflow() instead.
 *
 * @param string $job_id
 *   A unique identifier for the job, as created by
 *   _signatures_queue_create_job_id().
 * @param string $server_name
 *   (optional) An arbitrary identifier for the server processing the workflow.
 * @param string $worker_name
 *   (optional) An arbitrary identifier for the invoking process (e.g., cron
 *   job).
 * @param array $options
 *   (optional) An array of options. Defaults to an empty array. It is currently
 *   unused by this workflow.
 *
 * @return int
 *   Returns one of the following status codes indicating the workflow outcome:
 *     - SIGNATURES_QUEUE_STATUS_OK
 *
 * @see signatures_queue_invoke_workflow()
 */
function _signatures_queue_process_petitions($job_id, $server_name, $worker_name, array $options = array()) {
  $watchdog_suffix = _signatures_queue_watchdog_suffix('process_petitions', $job_id, $server_name, $worker_name);
  $limit = signatures_queue_get_queue_batch_size('process_petitions');
  $debug = variable_get('signatures_queue_debug', FALSE);
  $count = 0;

  $results = db_select('petition_metadata', 'pm')
    ->fields('pm')
    ->condition('metric', 'petition_signatures_updated')
    ->execute();

  while ($row = $results->fetchObject()) {
    $petition_id = $row->petition_id;
    $count++;

    // Delete queued entry early to allow other workflows to re-add.
    db_delete('petition_metadata')
      ->condition('petition_id', $petition_id)
      ->condition('metric', 'petition_signatures_updated')
      ->execute();

    // Process updated petitions.
    module_invoke_all('petition_signatures_updated', $petition_id);
  }

  // Log successes.
  watchdog('signatures_queue', '@number updated petitions have been processed. @suffix', array(
    '@number' => $count,
    '@suffix' => $watchdog_suffix,
  ), WATCHDOG_INFO);

  return SIGNATURES_QUEUE_STATUS_OK;
}
