<?php

/**
 * @file
 * This file provides an administration form for Petition Admin Dashboard.
 */

/**
 * Page callback: Form constructor for the admin settings form.
 *
 * @ingroup forms
 */
function petitionadmin_dashboard_settings_form() {
  $form = array();
  $form['petitionadmin_dashboard_daily_report_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject for the Daily Petition Report email.'),
    '#description' => '',
    '#default_value' => variable_get('petitionadmin_dashboard_daily_report_subject', t('Daily Petitions Report')),
    '#required' => TRUE,
  );

  $form['petitionadmin_dashboard_daily_report_emails'] = array(
    '#type' => 'textarea',
    '#title' => t('Petition Dashboard Report Email Addresses'),
    '#description' => t('Enter a comma-separated list of email addresses who should receive a daily petition stats report.'),
    '#default_value' => variable_get('petitionadmin_dashboard_daily_report_emails', variable_get('site_mail', '')),
    '#required' => FALSE,
  );

  return system_settings_form($form);
}
