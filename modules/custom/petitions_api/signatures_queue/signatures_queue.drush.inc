<?php

/**
 * @file
 * Drush commands for signature queue processing.
 */

/**
 * Implements hook_drush_command().
 */
function signatures_queue_drush_command() {
  $items = array();

  $items['signatures-queue-queue-list'] = array(
    'description' => 'Check the status of the signature queues.',
    'aliases' => array('sqql'),
  );

  $items['signatures-queue-invoke-workflow'] = array(
    'aliases' => array('sqiw'),
    'callback' => 'drush_signatures_queue_invoke_workflow',
    'description' => 'Invoke a signatures queue processing workflow.',
    'arguments' => array(
      'workflow' => 'The name of the workflow to invoke: one of "initiate_signature_validation", "preprocess_signatures", "process_signatures", or "archive_signatures".',
    ),
    'required-arguments' => TRUE,
    'options' => array(
      'server-name' => '(optional) The name of the server running the command, strictly for logging purposes. Defaults to "default".',
      'worker-name' => '(optional) An arbitrary identifier, strictly for logging purposes. This may be used to identify the invoking cron job, for example. Defaults to "default".',
    ),
    'examples' => array(
      'drush signatures-queue-invoke-workflow initiate_signature_validation --server-name=M5 --worker-name=w42' => 'Invoke the "initiate_signature_validation" queue.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
    'drupal dependencies' => array(
      'signatures_queue',
    ),
  );

  $items['signatures-queue-check-empty'] = array(
    'callback' => 'drush_signatures_queue_check_empty',
    'description' => 'Checks for empty signatures queues and logs timestamp if queue is empty.',
    'arguments' => array(
      'queue' => 'The name of the queue to check. Defaults to all queues.',
    ),
    'required-arguments' => FALSE,
    'examples' => array(
      'drush signatures-queue-check-empty' => 'Check all queues.',
      'drush signatures-queue-check-empty signatures_submitted_queue' => 'Check signatures_submitted_queue queue.',
    ),
    'aliases' => array('sqce'),
    'drupal dependencies' => array(
      'signatures_queue',
    ),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function signatures_queue_drush_help($section) {
  switch ($section) {
    case 'drush:signatures-queue-invoke-workflow':
      return dt('Invoke a signatures queue processing workflow.');

    case 'drush:signatures-queue-check-empty':
      return dt('Check for empty signatures queues.');

    case 'meta:signatures_queue:title':
      return dt('Signature queue Drush commands');
  }
}

/**
 * Implements drush_hook_COMMAND().
 */
function drush_signatures_queue_queue_list() {
  // All this would be much cleaner using output formats, but it has to work on
  // Drush 5.
  $column_width = 36;

  drush_print(dt('Queue @space Items', array(
    '@space' => str_pad('', $column_width - 5),
  )));

  signatures_queue_set_db();

  $queue_names = array_keys(signatures_queue_get_queue_names());
  foreach ($queue_names as $queue_name) {
    // The "queue" is a literal queue.
    if (SignaturesQueue::isValidQueueName($queue_name)) {
      // Make sure the queue exists before trying to interact with it.
      $queue = SignaturesQueue::get($queue_name);
      $queue->createQueue();
      $count = (int) $queue->numberOfItems();
    }
    // The "queue" is actually a database table.
    else {
      // Ignore tables that aren't in the "signatures_processing" database.
      if (!db_table_exists($queue_name)) {
        continue;
      }

      $count = (int) db_select($queue_name)->countQuery()->execute()->fetchField();
    }

    drush_print(dt('@queue_name..@count', array(
      '@queue_name' => str_pad($queue_name, $column_width, '.'),
      '@count' => number_format($count),
    )));
  }

  db_set_active();
}

/**
 * Implements drush_hook_COMMAND_validate().
 *
 * @see drush_signatures_queue_invoke_workflow()
 */
function drush_signatures_queue_invoke_workflow_validate() {
  $args = drush_get_arguments();

  // Make sure workflow argument is valid.
  $workflow = $args[1];
  $user_initiated_workflows = array(
    'receive_new_signatures',
    'receive_signature_validation',
  );
  if (!in_array($workflow, signatures_queue_get_workflow_names())) {
    drush_set_error('SIGNATURES_QUEUE_INVALID_ARGUMENT', dt('"@workflow" is not a valid workflow.', array('@workflow' => $workflow)));
  }
  elseif (in_array($workflow, $user_initiated_workflows)) {
    drush_set_error('SIGNATURES_QUEUE_INVALID_ARGUMENT', dt('The "@workflow" workflow is not meant to be invoked from Drush. This is a "user initiated workflow". You can simulate receiving signatures or validating signatures by generating dummy data with the signatures_queue_devel module (admin/config/development/generate/signatures).', array('@workflow' => $workflow)));
  }
}

/**
 * Implements drush_hook_COMMAND().
 *
 * Invokes a signatures queue processing workflow.
 *
 * @see drush_signatures_queue_invoke_workflow_validate()
 */
function drush_signatures_queue_invoke_workflow() {
  $args = drush_get_arguments();
  $workflow = $args[1];
  $options = array(
    'server_name' => drush_get_option('server-name', SIGNATURES_QUEUE_SERVER_NAME_DEFAULT),
    'worker_name' => drush_get_option('worker-name', SIGNATURES_QUEUE_WORKER_NAME_DEFAULT),
  );
  $success = signatures_queue_invoke_workflow($workflow, $options);
  return $success;
}

/**
 * Implements drush_hook_COMMAND_validate().
 */
function drush_signatures_queue_check_empty_validate() {
  $args = drush_get_arguments();
  if (!empty($args[1])) {
    $queue_name = $args[1];
    $queue_names = signatures_queue_get_queue_names();
    return !empty($queue_names[$queue_name]);
  }
  return TRUE;
}

/**
 * Implements drush_hook_COMMAND().
 *
 * @param string $name
 *   The machine name of a signatures queue.
 */
function drush_signatures_queue_check_empty($name = NULL) {
  $last_emptied_timestamps = signatures_queue_check_empty($name);
  foreach ($last_emptied_timestamps as $queue_name => $timestamp) {
    drush_print(dt('@queue_name last emptied: @timestamp', array(
      '@queue_name' => $queue_name,
      '@timestamp' => $timestamp ? date('Y-m-d', $timestamp) : 'never',
    )));
  }
}
