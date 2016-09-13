<?php

/**
 * Implements hook_drush_command().
 */
function petitionsdevel_drush_command() {
  $items = array();

  $items['generate-petitions'] = array(
    'description' => 'Generate dummy petitions.',
    'aliases' => array('gp', 'generate-petitions'),
    'arguments' => array(
      'num' => 'Number of petitions to generate, default is 25',
      'sigs' => 'Number of signatures to generate per petition, default is 20',
    ),
  );

  $items['remove-generated-petitions'] = array(
    'description' => 'Remove generated petitions',
    'aliases' => array('rgp', 'remote-generated-petitions'),
  );

  $items['generate-responses'] = array(
    'description' => 'Generate dummy responses.',
    'aliases' => array('gr', 'generate-responses'),
    'arguments' => array(
      'num' => 'Number of responses to generate, default is 25',
    ),
  );

  $items['remove-generated-responses'] = array(
    'description' => 'Remove generated responses',
    'aliases' => array('rgr', 'remote-generated-responses'),
  );

  return $items;
}


/**
 * Implements drush_hook_COMMAND().
 */
function drush_petitionsdevel_generate_petitions($num = 25, $sigs = 20) {
  petitionsdevel_generate_petitions($num, $sigs);
}

/**
 * Implements drush_hook_COMMAND().
 */
function drush_petitionsdevel_remove_generated_petitions() {
  petitionsdevel_remove_petitions();
}

/**
 * Implements drush_hook_COMMAND().
 */
function drush_petitionsdevel_generate_responses($num = 25) {
  petitionsdevel_generate_responses($num);
}

/**
 * Implements drush_hook_COMMAND().
 */
function drush_petitionsdevel_remove_generated_responses() {
  petitionsdevel_remove_responses();
}
