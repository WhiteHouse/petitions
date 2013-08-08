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
  $items['signatures-queue-invoke-workflow'] = array(
    'callback' => 'drush_signatures_queue_invoke_workflow',
    'description' => 'Invoke a signatures queue processing workflow.',
    'arguments' => array(
      'workflow' => 'The name of the workflow to invoke: one of "receive_new_signatures", "initiate_signature_validation", "receive_signature_validation", "preprocess_signatures", "process_signatures", or "archive_signatures".',
    ),
    'required-arguments' => TRUE,
    'options' => array(
      'server-name' => '(optional) The name of the server running the command, strictly for logging purposes. Defaults to "default".',
      'worker-name' => '(optional) An arbitrary identifier, strictly for logging purposes. This may be used to identify the invoking cron job, for example. Defaults to "default".',
    ),
    'examples' => array(
      'drush signatures-queue-invoke-workflow initiate_signature_validation --server-name=M5 --worker-name=w42' => 'Invoke the "initiate_signature_validation" queue.',
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

    case 'meta:signatures_queue:title':
      return dt('Signature queue Drush commands');
  }
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
  if (!in_array($workflow, signatures_queue_get_workflow_names())) {
    drush_set_error('SIGNATURES_QUEUE_INVALID_ARGUMENT', dt('"@workflow" is not a valid workflow choice.', array('@workflow' => $workflow)));
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

  // Dispatch specific workflow commands.
  module_load_include('inc', 'signatures_queue', "includes/{$workflow}");
  $function_name = "signatures_queue_{$workflow}";
  if (function_exists($function_name)) {
    $server_name = drush_get_option('server-name', 'default');
    $worker_name = drush_get_option('worker-name', 'default');
    $success = call_user_func($function_name, $server_name, $worker_name);
  }
}
