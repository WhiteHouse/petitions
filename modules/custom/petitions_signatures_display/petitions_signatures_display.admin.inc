<?php

/**
 * @file
 * This file provides an administration form for Petitions Signatures Display.
 */

/**
 * Menu callback for configuration form (admin/config/petitions/signatures).
 */
function petitions_signatures_display_configure() {
  $display = array(
    'petitions_signatures_display_fname_linitial' => t('First Name, Last Initial'),
    'petitions_signatures_display_finitial_linitial' => t('First Initial, Last Initial'),
  );

  $form['petitions_signatures_display_initials'] = array(
    '#type' => 'radios',
    '#title' => t('Signature display options'),
    '#description' => t('Select the manner in which to display names on all
      lists of signatures. <em>Applies to all Petitions</em>.'),
    '#options' => $display,
    '#default_value' => variable_get('petitions_signatures_display_initials', 'petitions_signatures_display_fname_linitial'),
  );

  return system_settings_form($form);
}
