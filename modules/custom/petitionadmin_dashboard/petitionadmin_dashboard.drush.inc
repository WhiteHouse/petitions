<?php

/**
 * Implements hook_drush_command().
 */
function petitionadmin_dashboard_drush_command() {
  $items = array();

  $items['petitionadmin-dashboard-send-report'] = array(
    'description' => 'Send petitionadmin dashboard report to recipients set in admin.',
    'examples' => array(
      'drush petitionadmin-dashboard-send-report' => 'Send report.',
    ),
    'aliases' => array('pdsr'),
  );

  $items['petitionadmin-dashboard-clean-event-log'] = array(
    'description' => 'Remove records from event_log table older than the petitions_event_lifetime.',
    'examples' => array(
      'drush petitionadmin-dashboard-clean-event-log' => 'Clean event_log table.'
    ),
    'aliases' => array('pdcel'),
  );

  return $items;
}

/**
 * Implements drush_hook_COMMAND().
 *
 * @see petitionadmin_dashboard_drush_command()
 */
function drush_petitionadmin_dashboard_send_report() {
  petitionadmin_dashboard_send_report();
  drush_print('Petition report sent.');
}

/**
 * Implements drush_hook_COMMAND().
 *
 * @see petitionadmin_dashboard_drush_command()
 */
function drush_petitionadmin_dashboard_clean_event_log() {
  module_load_include('inc', 'petitionadmin_dashboard', 'petitionadmin_dashboard.cron');
  petitionadmin_dashboard_clean_event_log();
  drush_log('Cleaned event_log table.', 'ok');
}
