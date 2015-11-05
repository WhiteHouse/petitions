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
