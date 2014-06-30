<?php
/**
 * @file
 * A drush command that generates signatures and petitions both for mongodb and
 * mysql.
 */

/**
 * Define a drush command.
 */
function generate_mongo2mysql_petsig_drush_command() {
  $items = array();
  $items['generate-petitions-content'] = array(
    'description' => "Generate Petitions and Signatures for MongoDB or MySQL.",
    'arguments' => array(
      'db_type' => 'Do you want to generate mongodb or mysql content?',
      'content_type' => 'Do you want to generate petitions or signatures?',
      'number' => 'How many pieces of content would you like to generate',
    ),
    // @TODO Create an option to choose the user generation strategy.
    'options' => array(
      'seed' => 'Seed for the random number generator',
    ),
    'aliases' => array('genpetcon'),
  );
  return $items;
}

/**
 * Callback to generate content.
 *
 * @param string $db_type
 *   either mongodb or mysql
 * @param string $content_type
 *   either petition or signature
 * @param int $number
 *   the number of pieces of content to be generated
 */
function drush_generate_mongo2mysql_petsig_generate_petitions_content($db_type, $content_type, $number) {
  module_load_include("php", "generate_mongo2mysql_petsig", "PetitionContentGenerator");

  $lib = libraries_load('faker');
  // Check to see if Faker library is available.
  if (!empty($lib['installed'])) {

    $faker = Faker\Factory::create();

    if (!($seed = drush_get_option('seed'))) {
      $faker->seed(1234);
    }
    else {
      $faker->seed($seed);
    }

    try{
      $content_generator = new PetitionContentGenerator($faker, $db_type, $content_type);
    }
    catch(Exception $e) {
      return drush_set_error('INCORRECT_TYPE', dt($e->getMessage()));
    }

    $init = microtime(TRUE);
    for ($i = 0; $i < $number; $i++) {
      $id = $content_generator->generate();
    }
    $end = microtime(TRUE);
    $time_spent = $end - $init;
    drush_log("Time: {$time_spent} seconds", 'success');
  }
  else {
    // The Faker library is not available.
    $message = t('The function drush_generate_petitions_content() requires the use of the Faker library present at /vendors/Faker It can be obtained from https://github.com/fzaninotto/Faker');
    drush_log($message, 'failed');
  }
}
