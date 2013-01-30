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

  $account_location = array(
    'petitions_signatures_display_city_state' => t('Display editable city and state fields (Default)'),
    'petitions_signatures_display_city_state_disabled' => t('Display uneditable city and state fields'),
    'petitions_signatures_display_city_state_hidden' => t('Hide city and state fields'),
  );

  $profile_location = array(
    'petitions_signatures_display_profile_user_city_state' => t('Display user-defined city and state fields (Default)'),
    'petitions_signatures_display_profile_db_city_state' => t('Display city and state fields associated to the user-defined zip code'),
  );

  $form['petitions_signatures_display_initials'] = array(
    '#type' => 'radios',
    '#title' => t('Signature display options'),
    '#description' => t('Select the manner in which to display names on all
      lists of signatures. <em>Applies to all Petitions</em>.'),
    '#options' => $display,
    '#default_value' => variable_get('petitions_signatures_display_initials', 'petitions_signatures_display_fname_linitial'),
  );

  $form['petitions_signatures_display_account_location'] = array(
    '#type' => 'radios',
    '#title' => t('Account creation/edit location fields display options'),
    '#description' => t('Select the manner in which to present city and state fields on account creation and edit pages.'),
    '#options' => $account_location,
    '#default_value' => variable_get('petitions_signatures_display_account_location', 'petitions_signatures_display_city_state'),
  );

  $form['petitions_signatures_display_profile_location'] = array(
    '#type' => 'radios',
    '#title' => t('Profile location fields display options'),
    '#description' => t('Select the manner in which to present city, state, and country fields on account creation and edit pages.'),
    '#options' => $profile_location,
    '#default_value' => variable_get('petitions_signatures_display_profile_location', 'petitions_signatures_display_profile_user_city_state'),
  );

  return system_settings_form($form);
}
