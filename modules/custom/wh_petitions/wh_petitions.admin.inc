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

  // Homepage Blocks.
  $form['homepage'] = array(
    '#type' => 'fieldset',
    '#title' => 'Homepage video/block texts/header text',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['homepage']['wh_petitions_homepage_left'] = array(
    '#type' => 'textarea',
    '#title' => t('Homepage - Lower left block'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_homepage_left', '<h2><a href="/how-why/introduction">What\'s this all about?</a></h2><p>The right to petition your government is guaranteed by the First Amendment of the United States Constitution.</p><a href="/how-why/introduction" class="more">Learn more about petitions</a>'),
    '#required' => TRUE,
  );
  $form['homepage']['wh_petitions_homepage_center'] = array(
    '#type' => 'textarea',
    '#title' => t('Homepage - Lower middle block'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_homepage_center', '<h2><a href="/how-why/step-step-guide">It\'s easy to participate</a></h2><p>Discovering, creating and signing petitions is simple.  Find out how to get started.</p><a href="/how-why/step-step-guide" class="more">Step-by-step guide</a>'),
    '#required' => TRUE,
  );
  $form['homepage']['wh_petitions_homepage_right'] = array(
    '#type' => 'textarea',
    '#title' => t('Homepage - Lower right block'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_homepage_right', ''),
    '#required' => FALSE,
  );
  $form['homepage']['wh_petitions_homepage_content'] = array(
    '#type' => 'textarea',
    '#title' => t('Homepage - Middle content area (video)'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_homepage_content', '&nbsp;'),
    '#required' => TRUE,
  );
  $form['homepage']['wh_petitions_top_right_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Homepage - Top Right Text'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_top_right_text', 'White House petitions is <br />a brand new platform for you <br />to tell the Obama Administration about the issues that matter to you.'),
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
  $form['petition_admin'] = array(
    '#type' => 'fieldset',
    '#title' => t('Petition Administration Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['petition_admin']['wh_petitions_compliance_review_threshold'] = array(
    '#type' => 'textfield',
    '#title' => t('Compliance Review Threshold'),
    '#description' => t('The number of signatures on a petition which will flag it for Compliance Review.'),
    '#default_value' => variable_get('wh_petitions_compliance_review_threshold', '3'),
    '#required' => TRUE,
  );
  $form['petition_admin']['wh_petitions_compliance_reviewer_emails'] = array(
    '#type' => 'textarea',
    '#title' => t('Compliance Reviewer Email Addresses'),
    '#description' => t('Enter a comma-separated list of email addresses who should be notified when a petition passes the compliance review threshold.'),
    '#default_value' => variable_get('wh_petitions_compliance_reviewer_emails', variable_get('site_mail', '')),
    '#required' => FALSE,
  );
  $form['petition_admin']['wh_petitions_review_threshold_passed_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject for the email sent when a petition receives enough signatures to pass the compliance review threshold.'),
    '#description' => '',
    '#default_value' => variable_get('wh_petitions_review_threshold_passed_subject', 'Petition ready for compliance review'),
    '#required' => TRUE,
  );
  $form['petition_admin']['wh_petitions_review_threshold_passed_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Text for the email sent when a petition receives enough signatures to pass the compliance review threshold.'),
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

  // Settings related to petition administration.
  $form['mongo2mysql'] = array(
    '#type' => 'fieldset',
    '#title' => t('MongoDB to MySQL Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['mongo2mysql']['wh_petitions_mysql_toggle'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow data to be saved to MySQL as well as MongoDB'),
    '#default_value' => variable_get('wh_petitions_mysql_toggle', 0),
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
 * Petition Admin Dashboard page.
 *
 * Allows admins to search for petitions based on:
 *  -Petition Status ( default: Under Review )
 *  -Response Status ( default: Pending )
 *  -Date Range
 *  -Bookmarked petitions by current user
 *  -Featured petitions
 *
 * Admins from this page can bookmark or feature petitions, remove
 * them from lists so they're only available by direct url, override
 * flagged status, or go to the petition edit screen for more options.
 *
 * The petition edit screen shows all the information associated with
 * a petition, plus allows administrators to edit certain information
 * and apply a response.
 */
function wh_petitions_admin_petitions() {
  global $user;
  drupal_add_js(drupal_get_path('module', 'wh_petitions') . "/wh_petitions.admin.js", array('scope' => 'header', 'weight' => 50));

  // Retrieve filter settings.
  $session_settings = isset($_SESSION['petition_overview_filter']) ? $_SESSION['petition_overview_filter'] : array();
  $submitted = isset($_GET['submitted']) && is_numeric($_GET['submitted']) && $_GET['submitted'] == 1;

  if ($submitted) {
    foreach ($_GET as $key => $value) {
      $value = check_plain($value);

      if (is_numeric($value)) {
        $value = (int) $value;
      }
      elseif (stripos($value, ',')) {
        $values = explode(',', $value);
        $final_values = array();

        foreach ($values as $value_item) {
          $final_values[] = is_numeric($value_item) ? (int) $value_item : $value_item;
        }

        $value = $final_values;
      }

      $session_settings[$key] = $value;
    }
  }

  // Results per page and page we're on.
  $results_per_page = 25;
  $page = 0;
  if (!empty($_GET['page'])) {
    $get_page = preg_replace('/[^0-9]/', '', $_GET['page']);
    if (is_int((int) $get_page)) {
      $page = intval($get_page);
    }
  }
  $offset = $page * $results_per_page;

  // Filter Fieldset.
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Choose filters'),
    '#theme' => 'exposed_filters__node',
  );

  // Petition Id.
  $default_petition_id = isset($session_settings['petition_id']) ? check_plain($session_settings['petition_id']) : '';
  $form['filters']['petition_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Petition ID'),
    '#description' => t('The long, alphanumeric ID of the petition'),
    '#default_value' => $default_petition_id,
    '#size' => 25,
    '#maxlength' => 25,
  );

  // Petition Id.
  $default_short_url = isset($session_settings['short_url']) ? check_plain($session_settings['short_url']) : '';
  $form['filters']['short_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Petition Short URL'),
    '#description' => t('The wh.gov url of the petition, including the http://. For example, http://wh.gov/xxx'),
    '#default_value' => $default_short_url,
    '#size' => 25,
    '#maxlength' => 25,
  );

  // Petition Keywords.
  if (isset($session_settings['petition_keywords'])) {
    $default_petition_keywords = is_array($session_settings['petition_keywords']) ? $session_settings['petition_keywords'] : array($session_settings['petition_keywords']);
  }
  else {
    $default_petition_keywords = array();
  }
  $form['filters']['petition_keywords'] = array(
    '#type' => 'textfield',
    '#title' => t('Petition Keywords'),
    '#description' => t('Enter a comma-separated list of keywords to search against the petition title and body'),
    '#default_value' => implode(',', $default_petition_keywords),
    '#size' => 25,
  );

  // Petition Statuses.
  $petition_statuses = array();
  $status_array = array(
    WH_PETITION_STATUS_DRAFT,
    WH_PETITION_STATUS_PRIVATE,
    WH_PETITION_STATUS_PUBLIC,
    WH_PETITION_STATUS_CLOSED,
    WH_PETITION_STATUS_UNDER_REVIEW,
    WH_PETITION_STATUS_REVIEWED,
    WH_PETITION_STATUS_FLAGGED,
  );
  foreach ($status_array as $status) {
    $petition_statuses[$status] = wh_petitions_display_status_name($status);
  }
  if (isset($session_settings['petition_status'])) {
    $default_petition_status = is_array($session_settings['petition_status']) ? $session_settings['petition_status'] : array($session_settings['petition_status']);
  }
  else {
    $default_petition_status = array(WH_PETITION_STATUS_UNDER_REVIEW);
  }
  $form['filters']['petition_status'] = array(
    '#type' => 'select',
    '#options' => $petition_statuses,
    '#title' => t('Petition Status'),
    '#default_value' => $default_petition_status,
    '#multiple' => TRUE,
  );

  // Response Statuses.
  $response_statuses = array();
  $status_array = array(
    WH_PETITION_RESPONSE_STATUS_UNANSWERED,
    WH_PETITION_RESPONSE_STATUS_PENDING,
    WH_PETITION_RESPONSE_STATUS_ANSWERED,
  );
  foreach ($status_array as $status) {
    $response_statuses[$status] = wh_petitions_display_status_name($status, 'response');
  }
  if (isset($session_settings['response_status'])) {
    $default_response_status = is_array($session_settings['response_status']) ? $session_settings['response_status'] : array($session_settings['response_status']);
  }
  else {
    $default_response_status = array(WH_PETITION_RESPONSE_STATUS_PENDING);
  }
  $form['filters']['response_status'] = array(
    '#type' => 'select',
    '#options' => $response_statuses,
    '#title' => t('Response Status'),
    '#default_value' => $default_response_status,
    '#multiple' => TRUE,
  );

  // Date Published After.
  $default_pub_after = isset($session_settings['published_after']) ? check_plain($session_settings['published_after']) : '';
  $form['filters']['published_after'] = array(
    '#type' => 'textfield',
    '#title' => t('Published After:'),
    '#size' => 20,
    '#description' => t('Format: YYYY-MM-DD'),
    '#default_value' => $default_pub_after,
  );

  // Date Published Before.
  $default_pub_before = isset($session_settings['published_before']) ? check_plain($session_settings['published_before']) : '';
  $form['filters']['published_before'] = array(
    '#type' => 'textfield',
    '#title' => t('Published Before:'),
    '#size' => 20,
    '#description' => t('Format: YYYY-MM-DD'),
    '#default_value' => $default_pub_before,
  );

  // Whether to only show bookmarked.
  $default_bookmarked = isset($session_settings['show_bookmarked']) && is_numeric($session_settings['show_bookmarked']) ? $session_settings['show_bookmarked'] : 0;
  $form['filters']['show_bookmarked'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Bookmarked:'),
    '#default_value' => $default_bookmarked,
    '#suffix' => '<div class="clear"> </div><br />',
  );

  // Whether to only show featured.
  $default_featured = isset($session_settings['show_featured']) && is_numeric($session_settings['show_featured']) ? $session_settings['show_featured'] : 0;
  $form['filters']['show_featured'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Featured:'),
    '#default_value' => $default_featured,
    '#suffix' => '<div class="clear"> </div><br />',
  );

  // Whether to only show hidden.
  $default_hidden = isset($session_settings['show_hidden']) && is_numeric($session_settings['show_hidden']) ? $session_settings['show_hidden'] : 0;
  $form['filters']['show_hidden'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Removed From Lists:'),
    '#default_value' => $default_hidden,
    '#suffix' => '<div class="clear"> </div><br />',
  );

  // Whether to only show petitions that have received flags.
  $default_flags = isset($session_settings['show_flags']) && is_numeric($session_settings['show_flags']) ? $session_settings['show_flags'] : 0;
  $form['filters']['show_flags'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Only With Flags:'),
    '#default_value' => $default_flags,
    '#suffix' => '<div class="clear"> </div><br />',
  );

  // Filter Button.
  $form['filters']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );
  $form['filters']['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
  );

  // Batch Keyword Form.
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Batch Apply Keywords'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['options']['private_tags'] = array(
    '#type' => 'textfield',
    '#title' => t('Private Keywords'),
    '#default_value' => '',
    '#size' => 25,
    '#description' => t('Separate keywords with a comma.'),
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#validate' => array('wh_petitions_admin_petitions_tags_validate'),
    '#submit' => array('wh_petitions_admin_petitions_tags_submit'),
  );

  // Build the sortable table header.
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'title'),
    'signature_count' => array('data' => t('Signatures'), 'field' => 'signature_count'),
    'petition_status' => t('Petition Status'),
    'response_status' => t('Response Status'),
    'flags' => t('Abuse Flags'),
    'reached_ready' => array('data' => t('Date Ready For Response'),'field' => 'reached_ready'),
    'actions' => t('Actions'),
  );

  // Build the query.
  $conn = wh_petitions_mongo_petition_connection();
  $query = array();
  if (!empty($default_petition_id)) {
    $query['_id'] = new MongoId($default_petition_id);
  }
  if (!empty($default_short_url)) {
    $query['short_url'] = $default_short_url;
  }

  // Keyword searching.
  if (!empty($default_petition_keywords)) {
    $query_or = array();
    $terms = array();
    // Set all terms to lowercase.
    foreach ($default_petition_keywords as $keyword) {
      $kwd = trim(strtolower($keyword));
      $terms[] = $kwd;
    }
    $terms2 = array('horse', 'iran');
    // Append the field elements to the query array.
    foreach (array('title_keywords', 'body_keywords') as $field) {
      $query_or[] = array($field => array('$in' => $terms));
    }
    $query['$or'] = $query_or;
  }

  if (!empty($default_petition_status)) {
    $query['petition_status'] = array('$in' => $default_petition_status);
  }
  if (!empty($default_response_status)) {
    $query['response_status'] = array('$in' => $default_response_status);
  }
  if (!empty($default_pub_after)) {
    $query['published'] = array('$gt' => (int) strtotime($default_pub_after));
  }
  if (!empty($default_pub_before)) {
    $query['published'] = array('$lt' => (int) strtotime($default_pub_before));
  }
  if (!empty($default_bookmarked)) {
    $query['bookmarked'] = array('$in' => array((int) $user->uid));
  }
  if (!empty($default_featured)) {
    $query['featured'] = 1;
  }
  if ($default_hidden == 1) {
    $query['hidden'] = 1;
  }
  if ($default_flags == 1) {
    $query['abuse_count'] = array('$gt' => 0);
  }

  // Sort.
  $sort_query = array();
  if (empty($_GET['sort']) || empty($_GET['order'])) {
    if ($default_flags == 1) {
      $sort_query['abuse_count'] = -1;
    }
    else {
      $sort_query['reached_ready'] = -1;
    }
  }
  // For the table sorts we need to catch the sort from the
  // url and apply it here ( since we're querying mongo instead
  // of building a sql query through drupal's interface, which
  // would do it for us.
  else {
    $asc_desc = -1;
    if ($_GET['sort'] == 'asc') {
      $asc_desc = 1;
    }

    switch ($_GET['order']) {
      case t('Title'):
        $sort_query['title'] = $asc_desc;
        break;

      case t('Signatures'):
        $sort_query['signature_count'] = $asc_desc;
        break;

      case t('Date Ready For Response'):
        $sort_query['reached_ready'] = $asc_desc;
        break;
    }
  }
  $retrieve_fields = array(
    'title',
    'signature_count',
    'reached_ready',
    'bookmarked',
    'petition_status',
    'abuse_count',
    'response_status',
    'featured',
    'hidden',
    'nice_url',
  );

  // If this is a specific edit petition page instead of the
  // list of petitions, the second argument will be the petition id.
  // If this is set, show the batch keywords, plus apply response,
  // and get more information about the petition to display at the bottom.
  $arg2 = arg(2);
  if (!empty($arg2)) {
    array_push($retrieve_fields, 'body', 'abuse_count', 'uid', 'user_tags', 'private_tags', 'review_timeframe', 'public_signatures', 'response_signatures', 'published', 'reached_public', 'closed');

    // Link to return to the petition list.
    $form['filters'] = array(
      '#type' => 'markup',
      '#markup' => '<div style="margin-bottom:10px;">' . l(t('Return To All Petitions'), 'admin/petitions') . '</div>',
    );

    // Apply Response Form.
    $form['apply'] = array(
      '#type' => 'fieldset',
      '#title' => t('Apply Response To This Petition'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#attributes' => array('class' => array('container-inline')),
    );
    $form['apply']['response_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Response Title'),
      '#size' => 60,
      '#maxlength' => 128,
      '#default_value' => '',
      '#autocomplete_path' => 'response-autocomplete',
    );
    $form['apply']['intro'] = array(
      '#type' => 'textarea',
      '#title' => t('Response Introduction'),
      '#rows' => 5,
      '#cols' => 40,
      '#prefix' => '<div style="margin-top:15px;"> </div>',
      '#default_value' => '',
    );
    $form['apply']['apply_petition_id'] = array(
      '#type' => 'hidden',
      '#value' => check_plain($arg2),
    );
    $form['apply']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Apply'),
      '#validate' => array('wh_petitions_admin_petitions_apply_validate'),
      '#submit' => array('wh_petitions_admin_petitions_apply_submit'),
    );

    // Determine if we're getting a mongo petition id or a short url in arg2.
    if (strlen($arg2) <= 5) {
      // If the key is this short, it's most likely a short
      // url rather than a mongo id.
      $query = array(
        // Concat the arg2 with our short url base.
        'short_url' => 'http://wh.gov/' . check_plain($arg2),
      );
    }
    else {
      $query = array(
        '_id' => new MongoId($arg2),
      );
    }
    $results = $conn->find($query, $retrieve_fields);
  }
  // Query all the results instead of the single petition.
  else {
    $results = $conn->find($query, $retrieve_fields)->sort($sort_query)->skip($offset)->limit($results_per_page);
  }

  // Iterate through the results and construct each line for the table.
  $options = array();
  foreach ($results as $res) {
    $id = $res['_id']->__toString();

    // Determine if we need to remove the 'apply response' form.
    if (!empty($arg2)) {
      if (!(!empty($res['reached_ready']) && $res['response_status'] != WH_PETITION_RESPONSE_STATUS_ANSWERED && $res['petition_status'] == WH_PETITION_STATUS_UNDER_REVIEW)) {
        unset($form['apply']);
      }
    }

    // When they reached ready for a response.
    // Should only show a date if it has reached ready.
    $ready = '';
    if ($res['reached_ready']) {
      $ready = date('M d, Y', $res['reached_ready']);
    }

    // Bookmark link.
    $bookmark = '<div id="bookmark-' . check_plain($id) . '" class="bookmark" style="display:inline;cursor:pointer;"><a>' . t('Bookmark') . '</a></div>';
    if (!empty($res['bookmarked'])) {
      if (in_array((int) $user->uid, $res['bookmarked'])) {
        $bookmark = '<div id="bookmark-' . check_plain($id) . '" class="bookmark" style="display:inline;cursor:pointer;"><a>' . t('Un-Bookmark') . '</a></div>';
      }
    }

    // Feature link.
    $featured = '<div id="featured-' . check_plain($id) . '" class="featured" style="display:inline;cursor:pointer;"><a>' . t('Feature') . '</a></div>';
    if (!empty($res['featured'])) {
      $featured = '<div id="featured-' . check_plain($id) . '" class="featured" style="display:inline;cursor:pointer;"><a>' . t('Un-Feature') . '</a></div>';
    }

    if (!petitions_data_mysql_writes_are_enabled()) {
      // Flag ( Remove from Site )
      $hidden_link = '<div id="remove-lists-' . check_plain($id) . '" class="remove-lists" style="display:inline;cursor:pointer;"><a href="/remove-mongo-petition?petition_id=' . $id . '">' . t('Remove from Site') . '</a></div>';
      if ($res['petition_status'] == WH_PETITION_STATUS_FLAGGED) {
        $hidden_link = '<div id="remove-lists-' . check_plain($id) . '" class="remove-lists" style="display:inline;cursor:pointer;"><a>' . t('Enable on Site') . '</a></div>';
      }
    }

    else {
      // Flag ( Remove from Site )
      $node = petition_load_from_legacy_id($id);

      $nid = "";
      if (!empty($node)) {
        $nid = $node->nid;
      }

      $text = ($res['petition_status'] == WH_PETITION_STATUS_FLAGGED) ? t('Enable via MySQL') : t('Disable via MySQL');
      $link = l($text, 'admin/moderation-tools', array(
        'query' => array('nid' => $nid),
      ));
      $hidden_link = '<div id="remove-lists-' . check_plain($id) . '" class="remove-lists" style="display:inline;cursor:pointer;">' . $link . '</div>';
    }

    $action_string = $bookmark . ' | ' . $featured . ' | ' . $hidden_link;
    $action_string .= ' | ' . l(t('Download Signature Email CSV'), 'admin/petitions/download_signatures/' . check_plain($id) . '/email_csv', array('attributes' => array('target' => '_blank')));
    $action_string .= ' | ' . l(t('Download Signature List'), 'admin/petitions/download_signatures/' . check_plain($id), array('attributes' => array('target' => '_blank')));
    $action_string .= ' | ' . l(t('Edit'), 'admin/petitions/' . check_plain($id));

    // Add the line for this petition to the table.
    $options[$id] = array(
      'title' => l($res['title'], $res['nice_url'], array('attributes' => array('target' => '_blank'))),
      'signature_count' => wh_petitions_format_number($res['signature_count']),
      'petition_status' => '<div id="petition-status-' . $id . '">' . wh_petitions_display_status_name($res['petition_status']) . '</div>',
      'response_status' => '<div id="response-status-' . $id . '">' . wh_petitions_display_status_name($res['response_status'], 'response') . '</div>',
      'flags' => !empty($res['abuse_count']) ? wh_petitions_format_number($res['abuse_count']) : 0,
      'reached_ready' => $ready,
      'actions' => $action_string,
    );
  }

  // Find totals to feed to the pager.
  $page_total = 1;
  $result_count = 0;
  if (!empty($options)) {
    $result_count = $conn->find($query)->count();
    $page_total = ceil($result_count / $results_per_page);
  }

  // Compose the table.
  $form['petitions'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No petitions available.'),
  );

  // Initialize the pager.
  pager_default_initialize($result_count, $results_per_page);
  $form['pager'] = array('#markup' => theme('pager'));

  // If this is a single result, display extra information about the petition.
  if (!empty($arg2)) {

    $markup = '';
    $markup .= '<p><strong>' . t('Description:') . '</strong> ' . wh_petitions_format_description($res['body']) . '</p>';

    $uid = intval($res['uid']);
    $creator = user_load($uid);
    $creator_profile = profile2_load_by_user($creator, 'main');
    $creator_name = array();
    if ($creator) {
      if (!empty($creator_profile->field_last_name[LANGUAGE_NONE][0]['safe_value'])) {
        $creator_name[] = check_plain($creator_profile->field_last_name[LANGUAGE_NONE][0]['safe_value']);
      }
      if (!empty($creator_profile->field_first_name[LANGUAGE_NONE][0]['safe_value'])) {
        $creator_name[] = check_plain($creator_profile->field_first_name[LANGUAGE_NONE][0]['safe_value']);
      }
    }
    $creator_name = implode(', ', $creator_name);
    $markup .= '<p>' . t('<strong>Petition Creator:</strong> <a href="@url">@name</a> (@email)', array(
      '@url' => url("user/{$uid}"),
      '@name' => $creator_name,
      '@email' => $creator->mail,
    )) . '</p>';

    $markup .= '<p><strong>' . t('User Keywords:') . '</strong> ' . check_plain(implode(', ', $res['user_tags'])) . '</p>';
    $markup .= '<p><strong>' . t('Private Keywords:') . '</strong> ' . check_plain(implode(', ', $res['private_tags'])) . '</p>';
    $markup .= '<p><strong>' . t('Review Timeframe') . '</strong> ' . number_format(intval($res['review_timeframe'])) . ' Days</p>';
    $markup .= '<p><strong>' . t('Signatures for Public') . '</strong> ' . number_format(intval($res['public_signatures'])) . ' Days</p>';
    $markup .= '<p><strong>' . t('Signatures for Response') . '</strong> ' . number_format(intval($res['response_signatures'])) . ' Days</p>';
    if (!empty($res['published'])) {
      $markup .= '<p><strong>' . t('Published Date:') . '</strong> ' . date('M d, Y', $res['published']) . '</p>';
    }
    if (!empty($res['reached_public'])) {
      $markup .= '<p><strong>' . t('Reached Public:') . '</strong> ' . date('M d, Y', $res['reached_public']) . '</p>';
    }
    if (!empty($res['reached_ready'])) {
      $markup .= '<p><strong>' . t('Ready for Response:') . '</strong> ' . date('M d, Y', $res['reached_ready']) . '</p>';
    }
    if (!empty($res['closed'])) {
      $markup .= '<p><strong>' . t('Archived Date:') . '</strong> ' . date('M d, Y', $res['closed']) . '</p>';
    }
    $form['petition'] = array(
      '#type' => 'fieldset',
      '#title' => t('Details'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      'details' => array(
        '#type' => 'markup',
        '#markup' => $markup,
      ),
    );
  }

  if (petitions_data_mysql_writes_are_enabled()) {
    drupal_set_message(t('MySQL based petitions are enabled, and using this page will introduce inconsistencies between Mongo- and MySQL-based petitions.<br />We recommend that you use the new <a href="@link">MySQL-based petitions moderation tools</a> to keep petitions in sync.', array(
      '@link' => url('admin/moderation-tools'),
    )), 'warning');
  }

  $form['#submit'][] = 'wh_petitions_admin_petitions_submit';
  return $form;
}

/**
 * Page callback: Displays form for petitions settings page.
 */
function wh_petitions_admin_petitions_settings_form() {
  $form = array();
  $form['petitions_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Petitions settings'),
  );
  $form['petitions_settings']['petitions_signing_method'] = array(
    '#type' => 'radios',
    '#title' => t('Signing method'),
    '#description' => t('The "Registraion required" signing method requires users to explicitly create an account and log in to sign petitions. The "Simplified signing" method allows them to sign via a simple form and to validate their signature via email instead of logging in. A Drupal cache clear is required to make a change in this setting take effect. Note: the registration-base signing method has a known security vulnerability: <a href="!url">SA-CONTRIB-2014-060- Petitions - Cross Site Request Forgery (CSRF)</a>', array('!url'=> 'https://www.drupal.org/node/2284571')),
    '#default_value' => variable_get('petitions_signing_method', WH_PETITIONS_SIGNING_METHOD_REGISTRATION_REQUIRED),
    '#options' => array(
      WH_PETITIONS_SIGNING_METHOD_REGISTRATION_REQUIRED => t('Registration required'),
      WH_PETITIONS_SIGNING_METHOD_SIMPLIFIED_SIGNING => t('Simplified signing'),
    ),
    '#access' => module_exists('petitionssignatureform'),
  );
  $form['related_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Related settings'),
  );
  $form['related_settings']['links'] = array(
    '#type' => 'markup',
    '#markup' => wh_petitions_admin_petitions_settings_links(),
  );
  return system_settings_form($form);
}

/**
 * Page callback: Displays links to various petitions settings pages.
 */
function wh_petitions_admin_petitions_settings_links() {
  $links['items'][] = l(t('Petition Signatures Queue'), 'admin/config/system/signatures-queue');
  $links['items'][] = l(t('Petition Tool'), 'admin/config/system/petitions');
  $links['items'][] = l(t('Petition Tool Hub'), 'admin/config/system/petition-tool-hub');
  $links['items'][] = l(t('Petitions Data'), 'admin/config/services/petitions_data');
  if (module_exists('petitionssignatureform') && user_access('administer petitionssignatureform')) {
    $links['items'][] = l(t('Petitions Signature Form'), 'admin/config/services/petitionssignatureform');
  }
  $links['items'][] = l(t('Petitions Users Statistics'), 'admin/config/services/stats/users');
  return theme('item_list', $links);
}

/**
 * Adds new filter selections to the session.
 *
 * Adds new filter selections to the session. Note: This needs to be the
 * last submit handler called as it issues a drupal_goto.
 */
function wh_petitions_admin_petitions_submit($form, &$form_state) {
  // On Reset go to petitions admin with no GET variables.
  if ($form_state['clicked_button']['#value'] == 'Reset') {
    drupal_goto('admin/petitions');
  }

  $filter_values = array();

  if (!empty($form_state['values']['petition_id'])) {
    $filter_values['petition_id'] = $form_state['values']['petition_id'];
  }

  if (!empty($form_state['values']['short_url'])) {
    $filter_values['short_url'] = $form_state['values']['short_url'];
  }

  if (!empty($form_state['values']['petition_keywords'])) {
    $filter_values['petition_keywords'] = $form_state['values']['petition_keywords'];
  }

  if (!empty($form_state['values']['petition_status'])) {
    $filter_values['petition_status'] = implode(',', $form_state['values']['petition_status']);
  }

  if (!empty($form_state['values']['response_status'])) {
    $filter_values['response_status'] = implode(',', $form_state['values']['response_status']);
  }

  if (!empty($form_state['values']['published_after'])) {
    $filter_values['published_after'] = $form_state['values']['published_after'];
  }

  if (!empty($form_state['values']['published_before'])) {
    $filter_values['published_before'] = $form_state['values']['published_before'];
  }

  if (!empty($form_state['values']['show_bookmarked'])) {
    $filter_values['show_bookmarked'] = $form_state['values']['show_bookmarked'];
  }

  if (!empty($form_state['values']['show_featured'])) {
    $filter_values['show_featured'] = $form_state['values']['show_featured'];
  }

  if (!empty($form_state['values']['show_hidden'])) {
    $filter_values['show_hidden'] = $form_state['values']['show_hidden'];
  }

  if (!empty($form_state['values']['show_flags'])) {
    $filter_values['show_flags'] = $form_state['values']['show_flags'];
  }

  $filter_values['submitted'] = TRUE;

  drupal_goto('admin/petitions', array('query' => $filter_values));
}

/**
 * Form validate function for the Batch Apply Keywords.
 */
function wh_petitions_admin_petitions_tags_validate($form, &$form_state) {
  if (empty($form_state['values']['private_tags'])) {
    form_set_error('private_tags', t('Please enter at least one keyword.'));
  }
}

/**
 * Form validate function for the Apply Response.
 */
function wh_petitions_admin_petitions_apply_validate($form, &$form_state) {
  if (empty($form_state['values']['response_title'])) {
    form_set_error('response_title', t('Please enter a response title.'));
  }
}

/**
 * Submit function for the Batch Apply Keywords.
 */
function wh_petitions_admin_petitions_tags_submit($form, &$form_state) {
  $conn = wh_petitions_mongo_petition_connection();

  // Get the private keywords.
  $tags = explode(', ', $form_state['values']['private_tags']);
  for ($i = 0; $i < count($tags); $i++) {
    $tags[$i] = trim(strip_tags(strtolower($tags[$i])));
  }

  if (!empty($tags)) {
    // Iterate through the petitions and apply the private
    // keywords to each ( admin petitions list page )
    if (!empty($form_state['values']['petitions'])) {
      foreach ($form_state['values']['petitions'] as $id => $value) {
        if ($value > 0) {
          $petition = wh_petitions_load_slim_petition($conn, $id, array('private_tags'));
          $petition['private_tags'] = array_merge($petition['private_tags'], $tags);
          $petition['private_tags'] = array_unique($petition['private_tags']);

          $conn->update(
            array('_id' => new MongoId($id)),
            array('$set' => array('private_tags' => $petition['private_tags']))
          );
        }
      }
    }
    // We have a single petition, apply keywords
    // to that one ( edit petition page )
    elseif (!empty($form_state['values']['petition_id'])) {
      $petition = wh_petitions_load_slim_petition($conn, $form_state['values']['petition_id'], array('private_tags'));
      $petition['private_tags'] = array_merge($petition['private_tags'], $tags);
      $petition['private_tags'] = array_unique($petition['private_tags']);
      $conn->update(
        array('_id' => new MongoId($form_state['values']['petition_id'])),
        array('$set' => array('private_tags' => $petition['private_tags']))
      );
    }
  }
}

/**
 * Submit function for the Apply Response.
 */
function wh_petitions_admin_petitions_apply_submit($form, &$form_state) {
  $pet_conn = wh_petitions_mongo_petition_connection();
  $res_conn = wh_response_mongo_response_connection();

  $nid = '';
  $response_title = $form_state['values']['response_title'];
  preg_match('/^(?:\s*|(.*) )?\[\s*(.+)\s*\]$/', $response_title, $matches);
  if (!empty($matches[2])) {
    $nid = intval($matches[2]);
  }
  $petition_id = $form_state['values']['apply_petition_id'];

  if (!empty($nid) && !empty($petition_id)) {
    // Verify it's not already part of the node ( trying to attach twice )
    $obj = array();
    $obj = wh_response_load_petition_response($res_conn, $nid, $petition_id);
    if (empty($obj) && !empty($nid)) {
      // Verify this petition can be attached to this node.
      // ( reached_ready > 0, no response already, UNDER_REVIEW status )
      $petition_obj = array();
      $petition_obj = wh_petitions_load_petition($pet_conn, $petition_id);
      if (!empty($petition_obj['reached_ready'])  && $petition_obj['response_status'] != WH_PETITION_RESPONSE_STATUS_ANSWERED && $petition_obj['petition_status'] == WH_PETITION_STATUS_UNDER_REVIEW) {
        $obj = wh_response_default_petition_relationship();
        $obj['petition'] = $petition_id;
        $obj['intro'] = $form_state['values']['intro'];
        $obj['nid'] = $nid;

        // Set the statuses on the petition to answered and reviewed.
        if (wh_response_save_petition_response($res_conn, $obj)) {
          $petition_obj['response_status'] = WH_PETITION_RESPONSE_STATUS_ANSWERED;
          $petition_obj['petition_status'] = WH_PETITION_STATUS_REVIEWED;
          $petition_obj['response_id'] = $nid;
          wh_petitions_save_petition($pet_conn, $petition_obj);
        }
      }
    }
  }
}

/**
 * Callback function for the Bookmark/Un-Bookmark ajax request.
 */
function wh_petitions_bookmark_petitions($petition_id) {
  global $user;

  if (!empty($petition_id) && !empty($user->uid) && user_access('administer petitions')) {
    $conn = wh_petitions_mongo_petition_connection();
    $petition = array();
    $petition = wh_petitions_load_slim_petition($conn, $petition_id, array('bookmarked'));
    if (!in_array((int) $user->uid, $petition['bookmarked'])) {
      array_push($petition['bookmarked'], (int) $user->uid);
      $conn->update(
        array('_id' => $petition['_id']),
        array('$set' => array('bookmarked' => $petition['bookmarked']))
      );
      print '<a>' . t('Un-Bookmark') . '</a>';
      exit;
    }
    else {
      $size = count($petition['bookmarked']);
      $new_array = array();
      for ($i = 0; $i < $size; $i++) {
        if ($user->uid != $petition['bookmarked'][$i]) {
          array_push($new_array, $petition['bookmarked'][$i]);
        }
      }
      $petition['bookmarked'] = $new_array;
      $conn->update(
        array('_id' => $petition['_id']),
        array('$set' => array('bookmarked' => $petition['bookmarked']))
      );
      print '<a>' . t('Bookmark') . '</a>';
      exit;
    }
  }

  print '';
}

/**
 * Callback function for the Feature/Un-Feature ajax request.
 */
function wh_petitions_feature_petitions($petition_id) {
  global $user;

  if (!empty($petition_id) && !empty($user->uid) && user_access('administer petitions')) {
    $conn = wh_petitions_mongo_petition_connection();
    $petition = array();
    $petition = wh_petitions_load_slim_petition($conn, $petition_id, array('featured'));
    if ($petition['featured']) {
      $petition['featured'] = 0;
      $text = t('Feature');
    }
    else {
      $petition['featured'] = 1;
      $text = t('Un-Feature');
    }
    $conn->update(
      array('_id' => $petition['_id']),
      array('$set' => array('featured' => $petition['featured']))
    );
    print '<a>' . $text . '</a>';
    exit;
  }

  print '';
}

/**
 * Callback function for the Remove Lists/Enable Lists ajax request.
 */
function wh_petitions_remove_lists($petition_id) {
  global $user;

  $return_value = '';

  if (!empty($petition_id) && !empty($user->uid) && user_access('administer petitions')) {
    $conn = wh_petitions_mongo_petition_connection();
    $petition = array();
    $petition = wh_petitions_load_petition($conn, $petition_id);

    // Don't love this logic for determining if something should be
    // published/unpublished. However, since it will go away with mongo we can
    // tolerate it for now.

    if ($petition['petition_status'] == WH_PETITION_STATUS_FLAGGED) {
      $petition = wh_petitions_mongo2mysql_show($petition_id);
      $link = l(t('Remove from site'), 'remove-mongo-petition', array(
        'query' => array('petition_id' => $petition_id),
      ));
    }
    else {
      drupal_goto('remove-mongo-petition', array(
        'query' => array('petition_id' => $petition_id),
      ));
    }

    // Now that PHP is done with the petition, convert some
    // of the attribute values to text so JS can display them easier.
    $petition['petition_status'] = wh_petitions_display_status_name($petition['petition_status']);
    $petition['response_status'] = wh_petitions_display_status_name($petition['response_status'], 'response');

    $js_petition = $petition;

    $return_value = drupal_json_output(
      array(

        // The text of the link for the remove-lists div.
        'link' => $link,

        // A representation of the petition for JS to display.
        'petition' => $petition,
      )
    );
  }

  drupal_json_output($return_value);
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

/**
 * Generate a csv list of signatures on the given petition.
 */
function wh_petitions_download_signatures_run_batch($petition_id, $report_type = '') {
  $limit = 500;
  $time_stamp = time();

  $file_name = variable_get('file_temporary_path', '/tmp') . '/subscriber_export_' . $petition_id . '_' . $time_stamp . '.csv';

  $batch = array(
    'title' => t('Exporting Petition Signatures'),
    'progress_message' => t('Dowload of the file will begin automatically when processing is complete.  After downloading, close this window.'),
    'operations' => array(
      array(
        'wh_petitions_download_signatures_batch',
        array($petition_id, $limit, $file_name, $report_type),
      ),
    ),
    'file' => drupal_get_path('module', 'wh_petitions') . '/wh_petitions.admin.inc',
  );

  batch_set($batch);
  batch_process('admin/petitions/download_signatures_finish/' . $petition_id . '/' . $time_stamp);
}

/**
 * Redirection page for when Batch API completes.
 */
function wh_petitions_download_signatures($petition_id, $time_stamp) {
  $file_name = variable_get('file_temporary_path', '/tmp') . '/subscriber_export_' . $petition_id . '_' . $time_stamp . '.csv';

  if ($_GET['download']) {
    if ($handle = fopen($file_name, 'r')) {
      drupal_add_http_header('Content-Type', 'text/csv; utf-8');
      drupal_add_http_header('Content-Disposition', 'attachment; filename="' . $file_name . '"');
      while ($data = fread($handle, 4096)) {
        echo $data;
      }
      fclose($handle);
    }
    else {
      return "There was an error retrieving your file. Please contact the site administrator." . htmlentities($filename);
    }
  }
  else {
    $link = l(t('download your file'), current_path(), array('query' => array('download' => 1)));
    return t("Processing complete. You may now !link", array('!link' => $link));
  }
}

/**
 * Batch API call-back for generating a list of petition signatures.
 *
 * @param string $petition_id
 *   The ID of the petiton for which signatures should be pulled.
 * @param int $limit
 *   Number of elements to process in each run of the operation.
 * @param string $file_name
 *   Name of the file to which results should be written.
 * @param array $context
 *   Batch API state holder.
 */
function wh_petitions_download_signatures_batch($petition_id, $limit, $file_name, $report_type, &$context) {

  $signatures_collection = wh_petitions_mongo_petition_signatures_connection();

  $query = array('petition_id' => $petition_id);
  $fields = array('uid', 'timestamp');

  // Get us a file to which to write output.
  if (!$file_handle = fopen($file_name, 'a')) {
    watchdog('wh_petitions', 'Unable to open file for writing subscriber export');
    return FALSE;
  }

  // Setup initial sandbox.
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = $signatures_collection->count($query);
  }

  $mongo_signatures = $signatures_collection->find($query, $fields)->skip($context['sandbox']['progress'])->limit($limit);
  $id_list = array();
  $timestamps = array();
  foreach ($mongo_signatures as $signature) {
    $context['sandbox']['progress']++;
    $id_list[] = $signature['uid'];
    $timestamps[$signature['uid']] = $signature['timestamp'];
  }

  $mysql_query = db_select('users', 'u');
  $mysql_query->join('field_data_field_first_name', 'fn', 'u.uid = fn.entity_id');
  $mysql_query->join('field_data_field_last_name', 'ln', 'u.uid = ln.entity_id');
  $mysql_query->join('field_data_field_city', 'fc', 'u.uid = fc.entity_id');
  $mysql_query->join('field_data_field_state', 'fs', 'u.uid = fs.entity_id');
  $mysql_query->join('field_data_field_zip', 'fz', 'u.uid = fz.entity_id');
  $mysql_query->fields('u', array('uid', 'mail'));
  $mysql_query->fields('fn', array('field_first_name_value'));
  $mysql_query->fields('ln', array('field_last_name_value'));
  $mysql_query->fields('fc', array('field_city_value'));
  $mysql_query->fields('fs', array('field_state_value'));
  $mysql_query->fields('fz', array('field_zip_value'));
  $mysql_query->condition('u.uid', array($id_list), 'IN');
  $result = $mysql_query->execute();

  while ($record = $result->fetchAssoc()) {

    switch ($report_type) {
      case 'email_csv':
        fwrite($file_handle, $record['mail'] . PHP_EOL);
        break;

      default:
        $row = array(
          'uid' => $record['uid'],
          'mail' => $record['mail'],
          'firstname' => csv_sanitize($record['field_first_name_value']),
          'lastname' => csv_sanitize($record['field_last_name_value']),
          'city' => csv_sanitize($record['field_city_value']),
          'state' => csv_sanitize($record['field_state_value']),
          'zip' => $record['field_zip_value'],
          'signature_timestamp' => $timestamps[$record['uid']],
        );
        fwrite($file_handle, implode(',', $row) . PHP_EOL);
        break;
    }
  }

  $context['results']['filename'] = $context['sandbox']['output_file'];
  $context['results']['signatures'] = $context['sandbox']['progress'];
  $context['message'] = t('Processed !progress signatures', array('!progress' => $context['sandbox']['progress']));
  $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
}

/**
 * Sanitize strings for export to CSV format.
 */
function csv_sanitize($data) {
  $data = check_plain($data);
  $data = str_replace(',', '&#44;', $data);
  return $data;
}
