<?php
/**
 * @file
 * Administration interface for migration settings.
 */

/**
 * System settings form to manage migration variables.
 */
function _migrate_mongo2mysql_settings() {
  $form['description'] = array(
    '#markup' => '<p>' . t('Change migration settings for petition and signature migrations.') . '</p>',
  );

  // Petition Configuration Settings.
  $form['petition_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Petition Migration Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['petition_config']['migrate_mongo2mysql_pet_unpublish'] = array(
    '#type' => 'checkbox',
    '#title' => t('Migrate unpublished petitions'),
    '#description' => t('By default, only published petitions are migrated. Check this box to include unpublished migrations during the final delta migration.'),
    '#default_value' => (bool) variable_get('migrate_mongo2mysql_pet_unpublish', 0),
  );

  $form['signature_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Signature Migration Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $date_format = 'Y-m-d';

  $default_start_date = '2005-01-01 00:00:00';
  $form['signature_config']['migrate_mongo2mysql_sig_date_start'] = array(
    '#type' => 'date_select',
    '#date_format' => $date_format,
    '#title' => t('Signatures created after'),
    '#description' => t('By default, all signatures are migrated. Use this field to restrict signature migrations to signatures that were created after a specified date.'),
    '#default_value' => variable_get('migrate_mongo2mysql_sig_date_start', $default_start_date),
    '#date_year_range' => '-10:+1',
  );

  $default_end_date = '2014-12-08 00:00:00';
  $form['signature_config']['migrate_mongo2mysql_sig_date_end'] = array(
    '#type' => 'date_select',
    '#date_format' => $date_format,
    '#title' => t('Signatures created before'),
    '#description' => t('By default, all signatures are migrated. Use this field to restrict signature migrations to signatures that were created before a specified date.'),
    '#default_value' => variable_get('migrate_mongo2mysql_sig_date_end', $default_end_date),
    '#date_year_range' => '-10:+1',
  );

  $form['signature_config']['migrate_mongo2mysql_sig_depends_on_pet'] = array(
    '#type' => 'checkbox',
    '#title' => t('Signature migrations depend on petitions'),
    '#description' => t('By default, signatures cannot be imported while petitions are available to be imported. Uncheck this box to remove this dependency. *Unchecking this box can lead to data corruption - only use this box if you understand the implications.*
      The drush command: {drush mreg} must be run for this change to take effect.'),
    '#default_value' => (bool) variable_get('migrate_mongo2mysql_sig_depends_on_pet', 1),
  );

  $form['signature_config']['migrate_mongo2mysql_sig_all'] = array(
    '#type' => 'checkbox',
    '#title' => t('Migrate ALL signatures'),
    '#description' => t('Ignore all date restrictions on signatures, including the settings directly above this one. This can have performance implications, including importing signatures throwing exceptions.'),
    '#default_value' => (bool) variable_get('migrate_mongo2mysql_sig_all', 0),
  );

  return system_settings_form($form, FALSE);
}
