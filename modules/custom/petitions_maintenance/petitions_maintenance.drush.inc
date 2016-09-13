<?php

/**
 * @file
 * Drush commands for the petitions maintenance module.
 */

/**
 * Implements hook_drush_command().
 */
function petitions_maintenance_drush_command() {
  $items = array();

  $items['petitions-maintenance-add-type'] = array(
    'description' => 'Set value for new petition type field on petition nodes.',
    'aliases' => array('pat'),
    'arguments' => array(
      'filepath' => 'The path to the json file with the type to node mapping. Use "-" to use STDIN to pass a
        file from a local machine.',
    ),
    'examples' => array(
      'drush pat ../mapping.json' => 'Set type value for petition nodes according to mapping in mapping.json.',
      'drush pat - < ../mapping.json' => 'Set type value for petition nodes according to mapping in local mapping.json.',
    ),
  );

  return $items;
}

/**
 * Drush callback function.
 *
 * @param string $filepath
 *   Path to json file with type to petition mapping. If a '-' is passed as the
 *   argument, the function will attempt to read the file content from STDIN.
 */
function drush_petitions_maintenance_add_type($filepath) {
  _petitions_maintenance_add_type($filepath);
}
