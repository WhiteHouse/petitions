<?php
/****************** CREATE A PETITION ************************/

/**
 * Displays the iframe for the create petition form.
 */
function wh_petitions_create_petition_iframe() {
  drupal_set_title(t('Create A Petition'));

  $url = '/petitions/create';
  $petition_id = arg(2);

  if (!empty($petition_id)) {
    $url .= '/' . check_plain($petition_id);
  }

  return '<iframe src= "' . $url . '?t=' . microtime() . '" width="687" height="1500" frameborder="0"></iframe>';
}

/**
 * Displays the create petition form in the iframe
 */
function wh_petitions_create_petition_iframe_html() {
  global $base_url;
  $styles = drupal_get_css();
  $scripts = drupal_get_js();
  $form = drupal_get_form('wh_petitions_create_petition');
  $form_html = drupal_render($form);
  $messages = theme('status_messages');
  $url = $base_url . '/' . drupal_get_path('theme', 'petitions_responsive');

  $html = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
  <html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" dir="ltr">
  <head><title>' . t('Create A Petition') . '</title>
  ' . $styles . $scripts . '
  <!--[if IE 7]>
  <link type="text/css" rel="stylesheet" media="all" href="' . $url . '/css/ie7.css" />
  <![endif]-->
  <script type="text/javascript">
  (function ($) {
    $(document).ready(function() {
      var pos = $("#petition-tool-content", window.parent.document).position().top;
      pos = pos + 163;
      $("html,body", window.parent.document).animate({scrollTop:pos}, "slow");
    });
  })(jQuery);
  </script>
  <style type="text/css">
    #petition-inner { padding:0px; }
  </style>
  </head>
  <body><div id="petition-inner">
  ' . $messages . '
  ' . $form_html . '
  </div></body>
  </html>';
  print $html;
  exit;
}

/**
 * Themes the petition title area which has different design requirements that drupal doesn't inherantly handle with the attributes available to the form.
 */
function theme_wh_petition_title($variables) {
  global $base_url;
  $element = $variables['element'];
  element_set_attributes($element, array('id', 'name', 'cols', 'rows'));
  _form_set_class($element, array('form-textarea'));
  $url = $base_url . '/' . drupal_get_path('theme', 'petitions_responsive');

  $default = !empty($element['#default_value']) ? $element['#default_value'] : t('finish the statement here');

  $output = '<div id="form-item-create-petition-title" class="clearfix">';
  $output .= '  <div class="we-petition-msg">' . t('We Petition the Obama Administration To...') . '</div>';
  $output .= '  <div class="form-field-wrapper clearfix">';
  $output .= '    <img class="form-arrow" alt="' . t('finish the statement here') . '" src="' . $url . '/img/textarea-arrow.gif" />';
  $output .= '    <textarea rows="5" cols="40" id="edit-petition-title" class="trigger" name="petition_title" onfocus="if(this.value==\'' . t('finish the statement here') . '\'){this.value=\'\'};">' . $default . '</textarea>';
  $output .= '    <div class="help-text">';
  $output .= '      <div class="view-some"><a href="#">' . t('Tips for creating good petition headlines:') . '</a></div>';
  $output .= '      <div class="character-limit">' . t('120 characters') . '</div>';
  $output .= '</div></div></div>';

  return $output;
}

/**
 * Thank-You page ( Step 5 of the process ).  Just display the petition.
 */
function wh_petitions_create_petition_thank_you($form, &$form_state) {
  global $base_url;
  global $user;

  $form_state['create_page'] = 5;
  $form['#attributes'] = array('id' => 'create-petition-form-thank-you');

  // Load the petition and display the thank-you message.
  $petition_item = PetitionsController::loadObject($form_state['petition_id']);
  $petition = ($petition_item) ? $petition_item->toLegacyArray() : array();
  $petition_id = $petition_item->getId();
  $date = strtotime('+' . $petition['review_timeframe'] . ' days', $petition['published']);
  $date = date("F d, Y", $date);
  $link = $base_url . '/' . $petition['nice_url'];
  $email_url = $base_url . '/petition/email/' . $petition_id;

  $full_link = l($link, $link, array('html' => TRUE, 'attributes' => array('target' => '_top')));

  // Use the full URL in cases where the short_url service was not available at the time of publishing
  $short_url = '';
  if (empty($petition['short_url'])) {
    // Service wasn't available, use the full URL
    $short_url = $full_link;
    drupal_set_message(t('The ShortURL service is currently unavailable. We apologize for the inconvenience.'), 'warning');
  }
  else {
    $short_url = l($petition['short_url'], $petition['short_url'], array('html' => TRUE, 'attributes' => array('target' => '_top')));
  }

  $email_address = $user->mail;
  $email_link = l(t('Email This Petition'), $email_url, array('attributes' => array('target' => '_top')));

  // Display the Thank You message
  $form['thank-you'] = array(
    '#type' => 'markup',
    '#markup' => theme('wh_petitions_page_thank_you', array(
      'days' => check_plain($petition['review_timeframe']),
      'total_needed' => wh_petitions_format_number($petition['response_signatures']),
      'public_needed' => wh_petitions_format_number($petition['public_signatures']),
      'link' => $full_link,
      'short_url' => $short_url,
      'petition_id' => $petition_id,
      'petition_title' => check_plain($petition['title']),
      'date' => check_plain($date),
      'email_address' => check_plain($email_address),
      'twitter_link' => wh_petition_tool_twitter_link($petition_id, check_plain($petition['title']), 'petition', '', $petition['short_url'], $petition['nice_url']),
      'facebook_link' => wh_petition_tool_facebook_link($petition_id, check_plain($petition['title']), 'petition', '', $petition['nice_url']),
      'email_link' => $email_link,
    )),
  );
  $form['#action'] = $base_url . '/' . implode('/', arg());
  return $form;
}

/**
 * Delete a petition handler.  Save to a separate collection.
 */
function wh_petitions_create_petition_delete_petition($form, &$form_state) {
  PetitionsController::delete($form_state['petition_id']);
  drupal_goto('petition/deleted');
}

/****************** OPEN PETITIONS PAGE FILTERS ************************/

/**
 * Callback for the page that has the issues filter form.
 */
function wh_petitions_filter_issue_standalone() {
  $form = drupal_get_form('wh_petitions_list_page_issues_form');
  $form = drupal_render($form);
  drupal_set_title(t('Filter By Issues'));

  return '<div id="filter-page"><h3>' . t('Filter By Issues') . '</h3><div class="drop-down">' . $form . '</div></div>';
}

/**
 * Callback for the page that has the search filter form.
 */
function wh_petitions_filter_search_standalone() {
  $form = drupal_get_form('wh_petitions_list_page_search_form');
  $form = drupal_render($form);
  drupal_set_title(t('Petition Search'));

  return '<div id="filter-page"><h3>' . t('Search') . '</h3><div class="drop-down">' . $form . '</div></div>';
}

/**
 * Callback for the saerch and issues filter standalone forms.
 */
function wh_petitions_filter_results() {
  $issues = array();
  if (!empty($_POST['filter_issues'])) {
    $tmp = check_plain($_POST['filter_issues']);
    foreach ($tmp as $key => $val) {
      if ($val > 0) {
        array_push($issues, intval($key));
      }
    }
  }

  // Stick something in the url if they didn't select any.
  if (empty($issues)) {
    $issues = array(0);
  }

  $search = 0;
  if (!empty($_POST['filter_search'])) {
    $search = urlencode(strip_tags($_POST['filter_search']));
  }

  $arr[0] = 'petitions';
  $arr[1] = 'all';
  $arr[2] = 0;
  $arr[3] = 2;

  $url = implode('/', $arr) . '/' . implode('+', $issues) . '/' . $search;

  // Redirect to the main site or the petition tool site, depending on where this request came from
  $url =  '/petitions/' . $url;
  drupal_goto($url);
}

/**
 * Filter by issues form on open petitions list page
 */
function wh_petitions_list_page_issues_form($form, &$form_state) {
  global $base_url;
  $form['#attributes'] = array('id' => 'form-item-issues-filter', 'class' => array('petition-issue-form'));
  $form['#action'] = $base_url . '/filter/results/petitions';

  // Issues
  $issues = array();
  $vocab = taxonomy_vocabulary_machine_name_load('issues');
  $counts = variable_get('wh_petitions_petition_issue_counts', array());
  if (!empty($vocab)) {
    $issues_list = taxonomy_get_tree($vocab->vid);
    foreach ($issues_list as $issue) {
      $issues[$issue->tid] = check_plain($issue->name);
      $total = 0;
      if (!empty($counts[$issue->tid])) {
        $total = $counts[$issue->tid];
      }
      $issues[$issue->tid] .= '<span class="num"> - ' . $total . '</span>';
    }
  }

  // If there are issues in the url, pre-populate the values in the form
  $arg_issues = arg(4);
  if (!empty($arg_issues)) {
    $arg_issues = explode(' ', $arg_issues);
    foreach ($arg_issues as $key => $val) {
      $form_state['values']['filter_issues'][$val] = $val;
    }
  }

  // Issues checkboxes
  $form['filter_issues'] = array(
    '#type' => 'checkboxes',
    '#title' => '',
    '#prefix' => '<div class="clearfix">',
    '#suffix' => '</div>',
    '#required' => FALSE,
    '#options' => $issues,
    '#after_build' => array('_wh_petitions_checkbox_3column_format'),
    '#default_value' => !empty($form_state['values']['filter_issues']) ? $form_state['values']['filter_issues'] : array(),
  );

  // Submit Button
  $form['report'] = array(
    '#type' => 'submit',
    '#value' => t('Filter by Issues'),
    '#attributes' => array('id' => 'button-filter-petitions', 'class' => array('button')),
  );

  return $form;
}

/**
 * Filter by search form on open petitions list page
 */
function wh_petitions_list_page_search_form($form, &$form_state) {
  $form['#attributes'] = array('id' => 'form-item-search-drop');
  global $base_url;
  $form['#action'] = $base_url . '/filter/results/petitions';

  // Prepopulate if there was something in the url
  $search = arg(5);
  if (!empty($search)) {
    $search = strip_tags(urldecode($search));
    $form_state['values']['filter_search'] = check_plain($search);
  }

  $form['filter_search'] = array(
    '#type' => 'textfield',
    '#title' => t('Search Terms'),
    '#required' => FALSE,
    '#attributes' => array('onfocus' => array("if(this.value=='" . t('Enter your search terms') . "'){this.value=''};")),
    '#default_value' => !empty($form_state['values']['filter_search']) ? $form_state['values']['filter_search'] : t('Enter your search terms'),
    '#autocomplete_path' => 'taxonomy/autocomplete/field_issues',
  );

  // Submit Button
  $form['go'] = array(
    '#type' => 'submit',
    '#value' => t('Go'),
    '#attributes' => array('id' => 'search-go-button', 'class' => array('button')),
  );

  return $form;
}

/**
 * Clear form button
 */
function wh_petitions_list_page_clear_form($form, &$form_state) {
  $form['#attributes'] = array('id' => 'form-item-clear', 'class' => array('petitions-clear-form'));

  $form['result_clear'] = array(
    '#type' => 'submit',
    '#value' => t('clear'),
    '#executes_submit_callback' => 'wh_petitions_clear_responses_submit',
  );

  return $form;
}

/****************** SIGN A PETITION ************************/

/**
 * Sign A Petition form
 */
function wh_petitions_sign_petition_form($form, &$form_state, $petition_id) {
  $form['petition_id'] = array(
    '#type' => 'hidden',
    '#value' => check_plain($petition_id),
  );

  // Sign Button
  $form['sign'] = array(
    '#type' => 'submit',
    '#value' => t('Sign This Petition'),
    '#attributes' => array('id' => 'button-sign-this-petition'),
    '#submit' => array('wh_petitions_sign_petition_form_submit'),
  );

  return $form;
}

/************** MODAL FORMS *************/

/**
 * Report this petition as inappropriate form
 */
function wh_petitions_report_inappropriate_form($form, &$form_state, $petition_id) {
  global $base_url;
  $form['#action'] = $base_url . '/petition/inappropriate/' . check_plain($petition_id);
  $form['petition_id'] = array(
    '#type' => 'hidden',
    '#value' => check_plain($petition_id),
  );

  // Report Button
  $form['report'] = array(
    '#type' => 'submit',
    '#value' => t('Report This Petition'),
    '#prefix' => '<p>' . variable_get('wh_petitions_report_modal_text', '') . '</p>',
    '#attributes' => array('id' => 'button-report-this-petition', 'class' => array('button', 'no-follow')),
  );

  return $form;
}

/**
 * Implements the page version of the delete form
 */
function wh_petitions_delete_form($form, &$form_state, $petition_id) {
  $form = drupal_get_form('wh_petitions_delete_petition_form');
  $form = drupal_render($form);

  return $form;
}

/**
 * Delete this petition.  Called from the 4th page of the Create A Petition form, or from the user dashboard page.
 */
function wh_petitions_delete_petition_form($form, &$form_state, $petition_id) {
  global $base_url;
  global $user;
  $form['#action'] = $base_url . '/petition/delete/' . check_plain($petition_id);
  if (arg(0) == 'petitions' && arg(1) == 'create') {
    $form['#action'] .= '?process=create_form';
  }

  $form['petition_id'] = array(
    '#type' => 'hidden',
    '#value' => check_plain($petition_id),
  );

  // Delete Button
  $form['delete'] = array(
    '#prefix' => '<div class="buttons">',
    '#type' => 'submit',
    '#value' => t('Delete This Petition'),
    '#attributes' => array('id' => 'button-delete-this-petition'),
  );

  // No Delete Button
  $form['nothanks'] = array(
    '#type' => 'markup',
    '#markup' => l(t('No Thanks, Keep This Draft'), 'dashboard', array('attributes' => array('id' => 'button-no-thanks-keep', 'rel' => check_plain($petition_id), 'class' => array('no-follow')))),
    '#suffix' => '</div>',
  );

  return $form;
}

/************** MODALtaylor CALLBACKS *************/

/**
 * Implements the 'Report as Inappropriate' modal callback
 */
function wh_petitions_inappropriate_modal($petition_id) {
  global $user;
  if (!empty($petition_id) && !empty($user->uid)) {
    if (petitions_data_mysql_writes_are_enabled()) {
      $node = petition_load_from_legacy_id($petition_id);
      $lang = $node->language;

      if (!empty($node)) {
        $abuse_flag = array('target_id' => (int) $user->uid);
        $is_first_abuse_flag = empty($node->field_abuse_flags[$lang]);

        // Ensure user has not already flagged this petition as inappropriate.
        if ($is_first_abuse_flag || !in_array($abuse_flag, $node->field_abuse_flags[$lang])) {
          $node->field_abuse_flags[$lang][] = $abuse_flag;
          $node->field_abuse_count[$lang][0]['value'] = count($node->field_abuse_flags[$lang]);
          node_save($node);
          entity_get_controller('node')->resetCache(array($node->nid));
        }
      }
    }

    if (petitions_data_mongo_writes_are_enabled() || petitions_data_mongo_reads_are_enabled()) {
      // Load the petition
      $conn = wh_petitions_mongo_petition_connection();
      $conn->setSlaveOkay(FALSE);
      $petition = array();
      $petition = wh_petitions_load_slim_petition($conn, $petition_id, array('abuse_flags', 'petition_status'));

      // If it wasn't a valid petition id, exit
      if (!empty($petition)) {
        if (petitions_data_mongo_writes_are_enabled()) {
          array_push($petition['abuse_flags'], (int)$user->uid);

          $conn->update(
            array('_id' => $petition['_id']),
            array('$set' => array('abuse_flags' => $petition['abuse_flags'], 'abuse_count' => count($petition['abuse_flags'])))
          );
        }
      }
    }

    module_invoke_all("wh_petitions_petition_inappropriate", $petition_id);
  }

  // Redirect to the main site or the petition tool site, depending on where this request came from
  if (petitions_data_mongo_reads_are_enabled()) {
    drupal_goto($petition['nice_url']);
  }
  else {
    drupal_goto('node/' . $node->nid);
  }
}

/**
 * Implements the 'Delete Petition modal callback
 */
function wh_petitions_delete_modal($petition_id) {
  global $user;

  if (!empty($petition_id) && !empty($user->uid)) {
    $petition = PetitionsController::loadObject($petition_id);
    if ($petition && $user->uid == $petition->getUid()) {
      PetitionsController::delete($petition_id);
    }
  }

  // MongoDB/MySQL conversion @todo How are we archiving nodes?
  
  $redirect_page = 'dashboard';
  if (!empty($_GET['process'])) {
    if ($_GET['process'] == 'create_form') {
      $redirect_page = 'petition/deleted';
    }
  }

  // Redirect to the main site or the petition tool site, depending on where this request came from
  drupal_goto($redirect_page);
}

/**
 * Signs a petition
 */
function wh_petitions_sign_modal($petition_id) {
  global $user;
  // Verify we got a petition id and that it's not blank and that the petition_id in the url matches the one from the POST
  if (empty($petition_id) || empty($user->uid)) {
     print 0;
     exit;
  }

  wh_petitions_sign_petition($petition_id, $user->uid, ip_address(), $_SERVER['HTTP_USER_AGENT']);

  if (!empty($_POST['js']) && !empty($_POST['petition_id'])) {
    print '<div id="signed-by-user"><div id="sign-this-petition" class="clearfix"><h3>' . t("You've signed this petition") . '</h3><p>' . t("Thank you for participating.  Find other petitions you're interested in or start your own.") . '</p></div></div>';
    exit;
  }

  // Otherwise, just return the output.
  $url = 'petition/add/comment/' . $petition_id;
  drupal_goto($url);
}

