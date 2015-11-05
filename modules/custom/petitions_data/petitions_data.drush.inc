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
      'drush petitions-data-petition-get 226511' => 'Get a petition.',
    ),
    'aliases' => array('pdpg'),
  );

  $items['petitions-data-create-test-petition'] = array(
    'description' => 'Create the long term test petition to use for automated testing if it no longer exists.',
    'examples' => array(
      'drush petitions-data-create-test-petition' => 'Create the long term test petition',
    ),
    'aliases' => array('pd-create', 'pdctp')
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

/**
 * Create the long term test petition if it does not exist.
 */
function drush_petitions_data_create_test_petition() {
  $petition_id = '5481d128362c79c931000003';

  $petition = PetitionsController::load($petition_id);

  if (!empty($petition)) {
    drush_print_r($petition);
    drush_set_error('PETITIONS_DATA_DRUSH_ERROR_PETITION_EXISTS', 'The Long-term test petition already exists. Nothing to do here.');
    return;
  }

  try {
    $petition = new PetitionItem();

    $petition->setLegacyId($petition_id);
    $petition->setTitle('Long-term test petition');
    $petition->setSignaturePublicThreshold(1000000000);
    $petition->setSignatureThreshold(1000000000);
    $petition->setStatus(WH_PETITION_STATUS_PRIVATE);
    $petition->setPublished(time());
    $petition->setReviewTimeframe(3650);
    $petition->setBody('This petition should never receive enough signatures to become public or pass the time limit to receive enough signatures for a response.');

    PetitionsController::save($petition);

    // get the petition and display it
    $petition = PetitionsController::load($petition_id);
    drush_print('Petition successfully created.');
    drush_print_r($petition);
  }
  catch (Exception $e) {
    drush_set_error('PETITIONS_DATA_DRUSH_ERROR_PETITION_CREATE',
      dt('!error: !message', array('!error' => $e->getCode(), '!message' => $e->getMessage())));
  }

}
