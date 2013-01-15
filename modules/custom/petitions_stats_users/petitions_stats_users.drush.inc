<?php

/**
 * @file
 * Drush commands for petitions_stats_users module.
 *
 * This file provides commands for queueing and wiping data. Other modules
 * may be used to interact with queued tasks via drush or via a web UI.
 *
 * @see http://drupal.org/project/drush_queue
 * @see http://drupal.org/project/queue_ui
 */

/**
 * Implements hook_drush_command().
 */
function petitions_stats_users_drush_command() {
  $items = array();

  $items['psu-wipe-data'] = array(
    'callback' => 'petitions_stats_users_wipe_drush',
    'description' => 'Wipes stored Petitions Users Statistics data from disk and resets variables.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );
  $items['psu-queue-fetch'] = array(
    'callback' => 'petitions_stats_users_queue_drush',
    'description' => 'Queues Petitions Users Statistics to be fetched via Petitions API.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  return $items;
}

/**
 * Callback for psu-wipe-data drush command.
 */
function petitions_stats_users_wipe_drush() {
  if (_petitions_stats_users_wipe_data()) {
    drush_log('Petitions Users Statistics data wiped from disk', 'success');
    drush_log('Use petitions-stas-users-fetch to fetch new data.');
  }
  else {
    drush_log('There was an error wiping Petitions Stats Users data.', 'error');
  }
}

/**
 * Callback for psu-queue-fetch drush command.
 */
function petitions_stats_users_queue_drush() {
  // Queue data fetching tasks.
  if (_petitions_stats_users_queue_tasks()) {
    drush_log('Petitions Users Statistics data have been queued for fetching. Running cron will initiate the queue.', 'success');
  }
  else {
    drush_log('There are no new Petitions Users Statistics data to fetch.', 'error');
  }
}
