<?php
/**
 * @file
 * petitionadmin.drush.inc
 * Drush commands.
 */

/**
 * Drush command index array.
 *
 * @return array
 *   Defines drush command details.
 */
function petitionadmin_drush_command() {
  $items = array();

  $items['petition-signatures-export'] = array(
    'description' => 'Provide csv export of signatures by petition.',
    'aliases' => array('pse'),
    'callback' => 'petitionadmin_drush_callback',
    'arguments' => array(
      'petition_id' => 'Unique ID of the petition',
    ),
    'options' => array(
      'filepath' => 'The path to write the output to.',
      'name' => 'Name for the report.',
    ),
    'examples' => array(
      'drush pse 54bfde14d063554d04d63af1' => 'Export petition signatures to current directory.',
      'drush pse 54bfde14d063554d04d63af1 --filepath=~/directory' => 'export petition signatures specifying path.',
    ),
  );

  return $items;
}

/**
 * Drush callback function.
 *
 * @param mixed $petition_id
 *   Either a string or int of the petition Id.
 *
 * @return mixed
 *   Returns a string of data to a file.
 */
function petitionadmin_drush_callback($petition_id = 'ALL') {
  $filepath = drush_get_option('filepath');
  $name = drush_get_option('name');

  if (!$filepath) {
    $filepath = getcwd();
  }

  if (!$name) {
    $name = 'signature-export';
  }

  $args = drush_get_arguments();
  if (count($args) > 0) {
    $petition_id = $args[1];
  }

  $results = petitionadmin_drush_export_signatures($petition_id);

  $output = "";
  while ($values = $results->fetchAssoc()) {
    $output .= $values['uid'] . ',' . $values['email'] . "\n";
  }

  $run_time = date('m-d-Y--H-i-s', time());
  $file_name = $filepath . '/petition-export-' . $run_time . '.' . $petition_id . '.' . $name . '.csv';

  if (count($results)) {
    drush_print('Writing results to file: ' . $file_name);
    file_put_contents($file_name, $output);
  }
  else {
    drush_print('No signatures where found -- no file was written.');
  }

}

/**
 * Wrapper function to trigger callback.
 *
 * @param mixed $petition_id
 *   Either a string or int petition ID.
 *
 * @return mixed
 *   either a string output to a file or notices.
 */
function drush_petitionadmin_drush_callback($petition_id) {
  petitionadmin_drush_callback($petition_id);
}

/**
 * Returns a query result set.
 *
 * @param mixed $petition_id
 *   Either a string or int petition id.
 *
 * @return DatabaseStatementInterface|null
 *   Either a database result set or nothing.
 */
function petitionadmin_drush_export_signatures($petition_id) {

  try {
    $query = db_select('signature_mail', 'sm');
    $query->distinct();
    $query->addJoin('INNER', 'users', 'us', 'us.uid = sm.uid');
    $query->addField('sm', 'uid');
    $query->addField('us', 'mail', 'email');

    if (is_numeric($petition_id)) {
      $query->condition('sm.petition_id', $petition_id);
    }
    elseif (is_string($petition_id) && !is_int($petition_id) && $petition_id != 'ALL') {
      $query->condition('sm.legacy_petition_id', $petition_id);
    }

    return $query->execute();
  }
  catch (Exception $e) {
    watchdog('petitionadmin', "Error.\n\n@exception", array(
      '@exception' => print_r($e, TRUE),
    ), WATCHDOG_ERROR);
  }
}
