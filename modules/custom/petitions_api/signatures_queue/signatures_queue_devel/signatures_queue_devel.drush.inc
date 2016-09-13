<?php
/**
 * @file
 * Provides Drush functionality for signatures_queue_devel module.
 */

/**
 * Implements hook_drush_command().
 */
function signatures_queue_devel_drush_command() {
  $items = array();

  $items['signatures-queue-devel-invoke-workflow'] = array(
    'description' => 'Invokes a workflow with a timer.',
    'aliases' => array('sqdiw', 'sqd-invoke-workflow'),
    'arguments' => array(
      'workflow' => 'The name of the workflow to invoke: one of "initiate_signature_validation", "preprocess_signatures", "process_signatures", or "archive_signatures".',
    ),
    'required-arguments' => TRUE,
  );
  $items['signatures-queue-devel-generate-signatures'] = array(
    'description' => 'Generates signatures for testing',
    'aliases' => array('sqd-generate'),
    'arguments' => array(
      'queue_name' => 'The name of the queue to populate.',
      'count' => 'The number of items to generate. Defaults to 10.',
    ),
    'options' => array(
      'petition' => '(optional) a petition ID to apply to each generated signature.',
    ),
  );
  $items['signatures-queue-devel-cleanup'] = array(
    'description' => 'Removes test signatures from the queue',
    'aliases' => array('sqd-clean'),
    'arguments' => array(
      'queue' => 'the name of the queue to empty',
    ),
  );
  $items['signatures-queue-devel-validate-signatures'] = array(
    'description' => 'Adds validations for any items in the "signatures_pending_validation_queue" queue.',
    'aliases' => array('sqd-validate'),
  );
  $items['signatures-queue-devel-archive-expired-signatures'] = array(
    'description' => 'Archives expired signatures and validations, based on the variable \'signatures_queue_minimum_signature_lifetime\'.',
    'aliases' => array('sqd-archive'),
  );
  $items['signatures-queue-devel-migrate-validations'] = array(
    'description' => 'Moves validations from signatures_pending_validation and validations tables to local db using SignatureValidationProcessorQueue.',
    'aliases' => array('sqd-migrate-validations'),
  );

  return $items;
}

/**
 * Implements drush_hook_COMMAND().
 */
function drush_signatures_queue_devel_generate_signatures($queue_name = 'all', $count = 10) {
  $queues = _signatures_queue_get_queue_list($queue_name);
  $tables = _signatures_queue_get_table_list($queue_name);
  $petition_id = drush_get_option('petition', NULL);

  // Generate items for the queue system.
  if ($queues) {
    $delete_first = FALSE;
    $num_to_match = 0;

    signatures_queue_devel_queue_generate($queues, $count, $delete_first, $num_to_match, array('petition_id' => $petition_id));
  };

  // Generate signatures in the signatures_pending_validation database table.
  if ($tables) {
    signatures_queue_devel_table_generate($tables, $count, array('petition_id' => $petition_id));
  };

  if (!$queues && !$tables) {
    $message = dt('The name \"@queue\" is not a valid queue name or table name. No signatures were generated. Run "@command" to see a list of available queues', array('@queue' => $queue_name, '@command' => 'drush sqql'));
    return drush_set_error('signatures-queue_devel', $message);
  }
}

/**
 * Implements drush_hook_COMMAND().
 */
function drush_signatures_queue_devel_invoke_workflow($workflow) {
  $timer = new SignaturesQueueDevelTimer();

  try {
    $timer->start();
    drush_invoke('signatures-queue-invoke-workflow', array($workflow));
    $timer->stop();

    $duration = $timer->read();
    $message = dt('"@process" completed in @duration ms.', array('@process' => $workflow, '@duration' => $duration));
  }
  catch (Exception $e) {
    $timer->stop();
    $message = dt("The process did not complete successfully:\n@error", array('@error' => $e->getMessage()));
  }

  drush_print($message);
}

/**
 * Implements drush_hook_COMMAND().
 */
function drush_signatures_queue_devel_cleanup($queue = 'all') {
  if ($queue_names = _signatures_queue_get_queue_list($queue)) {
    signatures_queue_devel_delete_queues($queue_names);
  };
}

/**
 * Implements drush_hook_COMMAND().
 */
function drush_signatures_queue_devel_validate_signatures() {
  $result = _signatures_queue_devel_generate_signature_validations();
  drush_print(dt("Generated @validated_count validations.\nUpdated @updated_count signatures without a secret_validation_key in the signatures_pending_validation table.", array(
    '@updated_count' => $result['updated_count'],
    '@validated_count' => $result['validated_count'],
  )));
}

/**
 * Validates a given signature queue name.
 *
 * @param string $queue_name
 *   The name of the queue, or 'all'.
 *
 * @return array|NULL
 *   An array of queue names, or NULL if the queue_name argument is invalid.
 */
function _signatures_queue_get_queue_list($queue_name) {
  $queues = signature_queue_devel_get_queue_names();
  $populate_all_queues = strtolower($queue_name) == 'all';
  if ($populate_all_queues || array_key_exists($queue_name, $queues)) {
    return ($populate_all_queues ? $queues : array($queue_name));
  }
}

/**
 * Validates a given signature table name.
 *
 * @param string $table_name
 *   The name of the table, or 'all'.
 *
 * @return array|NULL
 *   An array of table names, or NULL if the table_name argument is invalid.
 */
function _signatures_queue_get_table_list($table_name) {
  $tables = array('signatures_pending_validation');
  $populate_all_tables = strtolower($table_name) == 'all';
  if ($populate_all_tables || in_array($table_name, $tables)) {
    return ($populate_all_tables ? $tables : array($table_name));
  }
}

/**
 * Wrapper for signatures_queue_devel_archive().
 */
function drush_signatures_queue_devel_archive_expired_signatures() {
  $age_threshold = _signatures_queue_devel_get_age_threshold();
  $expired_validations = _signatures_queue_devel_get_expired_records('validations', $age_threshold, TRUE);
  $expired_signatures = _signatures_queue_devel_get_expired_records('signatures_pending_validation', $age_threshold, TRUE);

  $vars = array(
    '@validations_count' => $expired_validations,
    '@signatures_count' => $expired_signatures,
    '@age' => variable_get('signatures_queue_minimum_signature_lifetime', '2 weeks'),
  );

  if ($expired_signatures == 0 && $expired_validations == 0) {
    return drush_set_error('', "No records available to process.");
  }
  $message = dt('Ready to archive @validations_count validations and @signatures_count signatures older than @age old. Continue?', $vars);
  if (drush_confirm($message)) {
    signatures_queue_devel_archive();
  }
  else {
    drush_user_abort();
  }
}

/**
 * Wrapper for signatures_queue_devel_migrate().
 */
function drush_signatures_queue_devel_migrate_validations() {
  $signatures_pending_validation = _signatures_queue_devel_migrate_get_records(TRUE, TRUE);
  $validations = _signatures_queue_devel_migrate_get_records(FALSE, TRUE);

  $vars = array(
    '@signatures_pending_validation_count' => $signatures_pending_validation,
    '@validations_count' => $validations,
  );

  if ($signatures_pending_validation == 0 && $validations == 0) {
    return drush_set_error('', "No records available to process.");
  }
  $message = dt('Ready to migrate @signatures_pending_validation_count signatures_pending_validation rows and @validations_count validations. Continue?', $vars);
  if (drush_confirm($message)) {
    signatures_queue_devel_migrate();
  }
  else {
    drush_user_abort();
  }
}
