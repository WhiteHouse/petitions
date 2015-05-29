<?php

/**
 * @file
 * Contains drush hook implementations for Petitions Data module.
 */

/**
 * Implements hook_drush_command().
 */
function petitions_data_drush_command() {
  $items = array();

  $items['petitions-data-petition-get'] = array(
    'description' => 'Get the details of a given petition.',
    'arguments' => array(
      'petition_id' => 'A petition ID.',
    ),
    'required-arguments' => TRUE,
    'examples' => array(
      'drush petitions-data-petition-get 50a3fd762f2c88cd65000015' => 'Get a petition.',
    ),
    'aliases' => array('pdpg'),
  );

  return $items;
}

/**
 * Implements drush_hook_COMMAND_validate().
 *
 * @see petitions_data_drush_command()
 */
function drush_petitions_data_petition_get_validate() {
  $args = drush_get_arguments();
  $petition_id = $args[1];
  if (!petitions_data_is_valid_petition_id($petition_id)) {
    drush_set_error('PETITIONS_DATA_INVALID_PETITION_ID', dt('Invalid petition ID "@id".', array(
      '@id' => $petition_id,
    )));
  }
}

/**
 * Implements drush_hook_COMMAND().
 *
 * @see petitions_data_drush_command()
 */
function drush_petitions_data_petition_get() {
  $args = drush_get_arguments();
  $petition_id = $args[1];
  drush_print_r(PetitionsController::load($petition_id));
}

/**
 * Implements hook_drush_cache_clear().
 */
function petitions_data_drush_cache_clear(&$types) {
  $types['petitions'] = 'petitions_data_clear_caches';
}
