<?php
/**
 * @file
 * Administration interface for wh_petitions module.
 */

/**
 * System settings form to manage petition variables.
 */
function wh_petitions_admin_petition_settings() {
  $form['description'] = array(
    '#markup' => '<p>' . t('Change petition and signature settings and text that displays on the site.') . '</p>',
  );

  // Petition Configuration Settings.
  $form['config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Petition Configuration Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['config']['wh_petitions_review_timeframe'] = array(
    '#type' => 'textfield',
    '#title' => t('Review Timeframe'),
    '#description' => t('The number of days that a petition has to gather enough signatures to receive a response.'),
    '#default_value' => variable_get('wh_petitions_review_timeframe', '30'),
    '#required' => TRUE,
  );
  $form['config']['wh_petitions_public_signatures'] = array(
    '#type' => 'textfield',
    '#title' => t('Public Signatures'),
    '#description' => t('The number of signatures a petition must gather in order to become public.'),
    '#default_value' => variable_get('wh_petitions_public_signatures', '20'),
    '#required' => TRUE,
  );
  $form['config']['wh_petitions_review_signatures'] = array(
    '#type' => 'textfield',
    '#title' => t('Response Signatures'),
    '#description' => t('The number of signatures that trigger a petition to be reviewed for a response.'),
    '#default_value' => variable_get('wh_petitions_review_signatures', '500'),
    '#required' => TRUE,
  );
  $form['config']['wh_petitions_email_address'] = array(
    '#type' => 'textfield',
    '#title' => t('From Email Address'),
    '#description' => t('From email address used in emails sent from the site (except Email to a Friend).'),
    '#default_value' => variable_get('wh_petitions_email_address', ''),
    '#required' => TRUE,
  );
  $form['config']['wh_petitions_abuse_flag_threshhold'] = array(
    '#type' => 'textfield',
    '#title' => t('Abuse Threshhold'),
    '#description' => t('The number of report abuse flags that automatically remove a petition from display.'),
    '#default_value' => variable_get('wh_petitions_abuse_flag_threshhold', '5'),
    '#required' => TRUE,
  );

  // Tooltips.
  $form['tooltips'] = array(
    '#type' => 'fieldset',
    '#title' => 'Tooltip texts',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tooltips']['wh_petitions_tooltip_title'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for Title Tooltip'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_tooltip_title', '<p>Keep the title brief and compelling, this will be the first thing people see about this petition.  Be sure the title clearly describes the goal of the petition.  Double check your spelling, avoid using all capital letters, and avoid acronyms whenever possible.</p>'),
    '#required' => TRUE,
  );
  $form['tooltips']['wh_petitions_tooltip_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for Description Tooltip'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_tooltip_description', '<p>Start by clearly articulating a position, and then include additional information or research.  Keep the petition description brief, no more than 800 characters, including spaces, are allowed.</p><p>Note: If you are starting your petition on behalf of an organization, please indicate that here.</p>'),
    '#required' => TRUE,
  );
  $form['tooltips']['wh_petitions_tooltip_tags'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for Keywords Tooltip'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_tooltip_tags', '<p>Keywords will help further define the issues addressed in the petition, and will make it easier for others to find it. The keywords should be short (one or two words), and should be descriptive of the content of the petition.  Try to use no more than five keywords per petition. For example, if a petition is about energy policy, relevant keywords might be "clean energy," "solar power," or "oil."</p>'),
    '#required' => TRUE,
  );
  $form['tooltips']['wh_petitions_tooltip_why'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for WHY? Tooltip in the Sign Petition area'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_tooltip_why', '<p>Why Sign up?</p>'),
    '#required' => TRUE,
  );
  $form['tooltips']['wh_petitions_tooltip_threshold'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for Thresholds? Tooltip on the Petition Details page'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_tooltip_threshold', '<p>In order for a petition to be reviewed by the White House and for an official response to be issued, it must get 5,000 signatures in 30 days. That means it\'s up to you to build support for petitions you care about and gather more signatures. In addition, a petition must get 150 signatures in order to be publicly searchable on WhiteHouse.gov.</p><p>We the People is a brand new tool on WhiteHouse.gov, and we\'ve never tried anything like this before. Over time, we may need to adjust the petition signature thresholds, but we\'ll always let you know what the thresholds are.</p>'),
    '#required' => TRUE,
  );

  // Block Bodies.
  $form['blocks'] = array(
    '#type' => 'fieldset',
    '#title' => 'Block Body Texts',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['blocks']['wh_petitions_create_account_block_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for the Create A WH Account block'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_create_account_block_text', ''),
    '#required' => TRUE,
  );
  $form['blocks']['wh_petitions_email_updates_block_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for the Email Updates block'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_email_updates_block_text', t('Sign Up for Updates from President Obama and Other Senior Officials.')),
    '#required' => TRUE,
  );
  $form['blocks']['wh_petitions_history_block_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for the Petition History block'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_history_block_text', ''),
    '#required' => TRUE,
  );

  // By The Numbers block.
  $form['numbers'] = array(
    '#type' => 'fieldset',
    '#title' => 'By The Numbers block config',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['numbers']['wh_petitions_numbers_open_petitions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show number of Open Petitions'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_numbers_open_petitions', ''),
    '#required' => FALSE,
  );
  $form['numbers']['wh_petitions_numbers_responses'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show number of Responses'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_numbers_responses', ''),
    '#required' => FALSE,
  );
  $form['numbers']['wh_petitions_numbers_signatures_24'] = array(
    '#type' => 'checkbox',
    '#title' => t('Total Signatures in Past 24 Hours'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_numbers_signatures_24', ''),
    '#required' => FALSE,
  );

  // Helpful Hints block.
  $form['hints'] = array(
    '#type' => 'fieldset',
    '#title' => 'Helpful Hints block texts',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['hints']['wh_petitions_hints_create_petition'] = array(
    '#type' => 'textarea',
    '#title' => t('Helpful Hints - Create A Petition'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_hints_create_petition', ''),
    '#required' => FALSE,
  );
  $form['hints']['wh_petitions_hints_open_petitions'] = array(
    '#type' => 'textarea',
    '#title' => t('Helpful Hints - Open Petitions'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_hints_open_petitions', ''),
    '#required' => FALSE,
  );
  $form['hints']['wh_petitions_hints_petition_detail'] = array(
    '#type' => 'textarea',
    '#title' => t('Helpful Hints - Petition Detail'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_hints_petition_detail', ''),
    '#required' => FALSE,
  );
  $form['hints']['wh_petitions_hints_responses'] = array(
    '#type' => 'textarea',
    '#title' => t('Helpful Hints - Responses'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_hints_responses', ''),
    '#required' => FALSE,
  );
  $form['hints']['wh_petitions_hints_response_detail'] = array(
    '#type' => 'textarea',
    '#title' => t('Helpful Hints - Response Detail'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_hints_response_detail', ''),
    '#required' => FALSE,
  );
  $form['hints']['wh_petitions_hints_dashboard'] = array(
    '#type' => 'textarea',
    '#title' => t('Helpful Hints - User Dashboard'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_hints_dashboard', ''),
    '#required' => FALSE,
  );
  $form['hints']['wh_petitions_hints_misc'] = array(
    '#type' => 'textarea',
    '#title' => t('Helpful Hints - Misc Pages'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_hints_misc', ''),
    '#required' => FALSE,
  );

  // Misc Text.
  $form['misc'] = array(
    '#type' => 'fieldset',
    '#title' => 'Miscellaneous Text for Modals and Pages',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['misc']['wh_petitions_delete_modal_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for the Delete Petition modal window'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_delete_modal_text', ''),
    '#required' => FALSE,
  );
  $form['misc']['wh_petitions_archived_page_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for the Archived Petition page'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_archived_page_text', '<p>Sorry, this petition has expired. It did not reach the required thresholds within the allotted timeframe.</p>'),
    '#required' => FALSE,
  );
  $form['misc']['wh_petitions_delete_page_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for the Delete Petition page'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_delete_page_text', 'The petition draft is no longer available in your account.'),
    '#required' => FALSE,
  );
  $form['misc']['wh_petitions_report_modal_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for the Report Petition modal window'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_report_modal_text', '<p>Click the button below to report this petition as inappropriate.  Are you sure you want to do this?</p>'),
    '#required' => FALSE,
  );
  $form['misc']['wh_petitions_comment_modal_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for the Comment modal window'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_comment_modal_text', ''),
    '#required' => FALSE,
  );
  $form['misc']['wh_petitions_flagged_page_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for the Flagged ( Removed ) Petition page'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_flagged_page_text', 'Sorry, this petition has been removed from the system for violating our terms of participation.'),
    '#required' => FALSE,
  );
  $form['misc']['wh_petitions_flagged_page_creator_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for the Flagged ( Removed ) Petition page (displays to petition creator only).  If using a link, please add a class of "no-follow" to the <a> tag'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_flagged_page_creator_text', ''),
    '#required' => FALSE,
  );
  $form['misc']['wh_petitions_no_responses_page_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for the Responses page when there are no responses present.'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_no_responses_page_text', ''),
    '#required' => FALSE,
  );
  $form['misc']['wh_petitions_use_no_responses_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the No Responses page text from above instead of the normal responses page.'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_use_no_responses_page', ''),
    '#required' => FALSE,
  );

  // Links.
  $form['links'] = array(
    '#type' => 'fieldset',
    '#title' => 'Links to static pages',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['links']['wh_petitions_learn_more_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Learn about Petition Goals (petition detail page)'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_learn_more_link', ''),
    '#required' => FALSE,
  );
  $form['links']['wh_petitions_participate_rules_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Participation Rules'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_participate_rules_link', ''),
    '#required' => FALSE,
  );
  $form['links']['wh_petitions_read_rules_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Read the Rules (archived petitions)'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_read_rules_link', ''),
    '#required' => FALSE,
  );
  $form['links']['wh_petitions_reported_as_inappropriate_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Reported as Inappropriate bar on petition detail page'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_reported_as_inappropriate_link', ''),
    '#required' => FALSE,
  );

  // Settings related to petition administration.
  $form['petition_admin_compliance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Compliance Review Admin Notification Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['petition_admin_compliance']['wh_petitions_compliance_review_threshold'] = array(
    '#type' => 'textfield',
    '#title' => t('Compliance Review Threshold'),
    '#description' => t('The number of signatures on a petition which will flag it for Compliance Review.'),
    '#default_value' => variable_get('wh_petitions_compliance_review_threshold', '3'),
    '#required' => TRUE,
  );
  $form['petition_admin_compliance']['wh_petitions_compliance_reviewer_emails'] = array(
    '#type' => 'textarea',
    '#title' => t('Compliance Reviewer Email Addresses'),
    '#description' => t('Enter a comma-separated list of email addresses who should be notified when a petition passes the compliance review threshold.'),
    '#default_value' => variable_get('wh_petitions_compliance_reviewer_emails', variable_get('site_mail', '')),
    '#required' => FALSE,
  );
  $form['petition_admin_compliance']['wh_petitions_review_threshold_passed_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_review_threshold_passed_subject', 'Petition ready for compliance review'),
    '#required' => TRUE,
  );
  $form['petition_admin_compliance']['wh_petitions_review_threshold_passed_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_review_threshold_passed_text', "The petition titled \"!petition_title\" has passed the number of signatures necessary for compliance review.

You can review the petition here:<br />!petition_remove_link

=================

TITLE:
!petition_title

BODY:
!petition_body

================="),
    '#required' => TRUE,
  );
  $form['petition_admin_ready_response'] = array(
    '#type' => 'fieldset',
    '#title' => t('Ready For Response Admin Notification Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['petition_admin_ready_response']['wh_petitions_admin_ready_response_emails'] = array(
    '#type' => 'textarea',
    '#title' => t('Ready for Response Notification Email Addresses'),
    '#description' => t('Enter a comma-separated list of email addresses who should be notified when a petition passes the ready for response threshold.'),
    '#default_value' => variable_get('wh_petitions_admin_ready_response_emails'),
    '#required' => FALSE,
  );
  $form['petition_admin_ready_response']['wh_petitions_email_admin_ready_response_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_email_admin_ready_response_subject'),
    '#required' => TRUE,
  );
  $form['petition_admin_ready_response']['wh_petitions_email_admin_ready_response_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_email_admin_ready_response_text'),
    '#required' => TRUE,
  );

  $form['petition_admin']['admin_signature_thresholds'] = array(
    '#type' => 'fieldset',
    '#title' => t('Petition Signature Admin Notification Thresholds'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['petition_admin']['admin_signature_thresholds']['wh_petitions_admin_signature_threshold_crossed_emails'] = array(
    '#type' => 'textarea',
    '#title' => t('Signature Threshold Crossed Notification Email Addresses'),
    '#description' => t('Enter a comma-separated list of email addresses who should be notified when a petition crosses one of the 4 defined thresholds.'),
    '#default_value' => variable_get('wh_petitions_admin_signature_threshold_crossed_emails'),
    '#required' => TRUE,
  );

  $threshold_default_levels = array(
    '1' => 25000,
    '2' => 50000,
    '3' => 75000,
  );

  foreach ($threshold_default_levels as $key => $value) {

    $form['petition_admin']['admin_signature_thresholds']['wh_petitions_admin_signature_threshold_' . $key] = array(
      '#type' => 'textfield',
      '#title' => t('Threshold Level @level', array('@level' => $key)),
      '#description' => t('The number of signatures on a petition which will trigger the notification.'),
      '#default_value' => variable_get('wh_petitions_admin_signature_threshold_' . $key, $value),
      '#required' => TRUE,
    );
  }

  $form['petition_admin']['admin_signature_thresholds']['wh_petitions_email_admin_signature_threshold_crossed_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject for the email sent when a petition receives enough signatures to pass the threshold.'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_email_admin_signature_threshold_crossed_subject'),
    '#required' => TRUE,
  );

  $form['petition_admin']['admin_signature_thresholds']['wh_petitions_email_admin_signature_threshold_crossed_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for the email sent when a petition receives enough signatures to pass the threshold.'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_email_admin_signature_threshold_crossed_text'),
    '#required' => TRUE,
  );

  $form['#validate'][] = 'wh_petitions_admin_petition_settings_validate';

  return system_settings_form($form, FALSE);
}

/**
 * System settings form to manage petitions email settings.
 */
function wh_petitions_admin_petition_email_settings() {
  // Published petition email settings.
  $form['wh_petitions_email_published'] = array(
    '#type' => 'fieldset',
    '#title' => 'Notification: Your petition has been published',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['wh_petitions_email_published']['wh_petitions_email_published_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject for the email sent when a petition is published'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_email_published_subject', ''),
    '#required' => TRUE,
  );
  $form['wh_petitions_email_published']['wh_petitions_email_published_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for the email sent when a petition is published'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_email_published_text', ''),
    '#required' => TRUE,
  );
  $form['wh_petitions_email_published']['wh_petitions_published_email_address'] = array(
    '#type' => 'textfield',
    '#title' => t('New Petition Email Address'),
    '#description' => t('Email Address to send new petitions notifications to.'),
    '#default_value' => variable_get('wh_petitions_published_email_address', variable_get('site_mail', '')),
    '#required' => FALSE,
  );
  // Threshold passed email settings.
  $form['wh_petitions_email_ready_response'] = array(
    '#type' => 'fieldset',
    '#title' => 'Notification: Your petition has passed the threshold for receiving a response',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['wh_petitions_email_ready_response']['wh_petitions_email_ready_response_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject for the email sent when a petition gets enough signatures to receive a response.'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_email_ready_response_subject', ''),
    '#required' => TRUE,
  );
  $form['wh_petitions_email_ready_response']['wh_petitions_email_ready_response_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for the email sent when a petition gets enough signatures to receive a response.'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_email_ready_response_text', ''),
    '#required' => TRUE,
  );
  // Forward petition email settings.
  $form['wh_petitions_email_forward'] = array(
    '#type' => 'fieldset',
    '#title' => 'Forward this petition to your friends',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['wh_petitions_email_forward']['wh_petitions_email_forward_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for the email sent when someone chooses to email a friend ( subject = petition title ).'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_email_forward_text', 'Dear Friends,

I wanted to let you know about an official petition I have signed at WhiteHouse.gov. Will you add your name to mine?  If this petition gets !signatures_needed signatures by !date_needed, the White House will review it and respond!

You can view and sign the petition here: !shorturl

Here\'s some more information about this petition:
!petition_description'),
    '#required' => TRUE,
  );
  $form['wh_petitions_email_forward']['wh_petitions_email_forward_response_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for the email sent when someone chooses to email a friend - response ( subject = response title ).'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_email_forward_response_text', 'Dear Friends,

You can read the response here: !shorturl'),
    '#required' => TRUE,
  );

  return system_settings_form($form, FALSE);
}

/**
 * Validates submitted petition settings form to enforce integer values.
 */
function wh_petitions_admin_petition_settings_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['wh_petitions_review_timeframe'])) {
    form_set_error('wh_petitions_review_timeframe', t("Please use integer values."));
  }
  if (!is_numeric($form_state['values']['wh_petitions_public_signatures'])) {
    form_set_error('wh_petitions_public_signatures', t("Please use integer values."));
  }
  if (!is_numeric($form_state['values']['wh_petitions_review_signatures'])) {
    form_set_error('wh_petitions_review_signatures', t("Please use integer values."));
  }
}

/**
 * Menu callback for the autocomplete results.
 *
 * Retrieves all published responses that match the string
 * that has been typed in.
 */
function wh_petitions_response_autocomplete($string = '') {
  $matches = array();

  // Query.
  $query = db_select('node', 'n', array('target' => 'slave'))
    ->fields('n', array('nid', 'title'))
    ->condition('n.type', 'wh_response')
    ->where("upper(n.title) LIKE upper(:search)", array(':search' => $string . '%'))
    ->orderBy('n.title', 'ASC');
  $result = $query->execute();

  foreach ($result as $res) {
    $matches[check_plain($res->title) . '  [' . check_plain($res->nid) . ']'] = check_plain($res->title);
  }

  drupal_json_output($matches);
}
