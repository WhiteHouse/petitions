<?php

/**
 * @file
 * Creates the help text field in the theme settings UI.
 */

  require_once drupal_get_path('theme', 'petitions44') . '/template.php';

/**
 * Implements hook_form_system_theme_settings_alter().
 */
function petitions44_form_system_theme_settings_alter(&$form, &$form_state, $form_id = NULL) {

  // Get headers and footer values.
  // - $petitions44_help_text
  $petitions44_help_text = _petitions44_help_text();

  $form['petitions44_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('Petitions: General Assets'),
    '#collapsible' => TRUE,
    '#weight' => -5,
  );

  $form['petitions44_general']['description'] = array(
    '#type' => 'markup',
    '#title' => t('Default Settings'),
    '#collapsible' => TRUE,
    '#weight' => -2,
  );

  $form['petitions44_general']['petitions44_help_text'] = array(
    '#type' => 'textarea',
    '#title' => t('HTML Code to use for help text'),
    '#description' => t('Check this to default to the built-in configuration for these settings (for migration purposes)'),
    '#default_value' => $petitions44_help_text,
    '#weight' => 0,
  );
}
