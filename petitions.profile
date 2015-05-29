<?php

/**
 * @file
 * Enables modules and site configuration for a Petitions site installation.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function petitions_form_install_configure_form_alter(&$form, &$form_state, $form_id) {
  $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];
  $form['admin_account']['account']['name']['#default_value'] = 'admin';
  $form['server_settings']['site_default_country']['#default_value'] = 'US';
  $form['update_notifications']['update_status_module']['#default_value'] = array(0);
}
