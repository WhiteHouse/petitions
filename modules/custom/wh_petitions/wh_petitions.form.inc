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
  $url = $base_url . '/' . drupal_get_path('theme', 'petitions44');

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
 * Callback for create a petition form.  Displays each step of the form based on where the user is.
 *
 * Step 1: Title + Issues
 * Step 2: Similar Petitions
 * Step 3: Description + Keywords
 * Step 4: Review and Publish/Edit/Delete
 * Step 5: Thank You Page
 */
function wh_petitions_create_petition($form, &$form_state) {
  global $base_url, $user;
  $url = $base_url . '/' . drupal_get_path('theme', 'petitions44');

  // Only let registered users here.  Access here should be restricted by the 'create petition' permission anyway, but can't hurt to double-check.
  if (empty($user->uid)) {
    // This is unfornately hardcoded because of the iframe's path. Subtly, the path is
    // petitionS/create, vs, petition/create which loads petitions/create. In this
    // case, you can't do a drupal_get_destination() because it loads the wrong path due
    // to the iframe.
    $destination = array('destination' => '/petition/create');
    $markup = '<div class="top-msg-bar top-msg-bar-blue">' . t('You must verify your email before you can create a White House petition') . '</div>';
    $markup .= '<div id="sign-in-or-create-account"><div id="sign-in" class="col-2"><h3>' . t('Already Have an Account?') . '</h3>';
    $markup .= '<p>&nbsp;</p>';
    $markup .= l(t('Sign In'),  'user', array('query' => $destination, 'attributes' => array('class' => array('no-follow'), 'id' => 'button-sign-in', 'target' => array('_parent'))));
    $markup .= '</div>';
    $markup .= '<div class="or"><img src="' . $url . '/img/or.gif" alt="or"/></div>';
    $markup .= '<div id="no-account" class="col-2"><h3>' . t("Don't Have an Account?") . '</h3>';
    $markup .= '<p>&nbsp;</p>';
    $markup .= l(t('Create an Account'), 'register', array('query' => $destination, 'attributes' => array('class' => array('no-follow'), 'id' => 'button-create-an-account', 'target' => array('_parent'))));
    $markup .= '</div></div>';

    return array(
      'description' => array(
        '#type' => 'markup',
        '#markup' => $markup,
      ),
    );
  }

  if (!empty($form_state['create_page'])) {
    if ($form_state['create_page'] == 2) {
      return wh_petitions_create_petition_step_2($form, $form_state);
    }
    elseif ($form_state['create_page'] == 3) {
      return wh_petitions_create_petition_step_3($form, $form_state);
    }
    elseif ($form_state['create_page'] == 4) {
      return wh_petitions_create_petition_step_4($form, $form_state);
    }
    elseif ($form_state['create_page'] == 5) {
      return wh_petitions_create_petition_thank_you($form, $form_state);
    }
  }

  // Display the first page
  return wh_petitions_create_petition_step_1($form, $form_state);
}

/**
 * First page of the create a petition form.  Contains title and issues.  Saves the petition as a draft when the user submits.
 */
function wh_petitions_create_petition_step_1($form, &$form_state) {
  global $user;
  global $base_url;

  // We're on the first page.
  $form_state['create_page'] = 1;
  $form['#prefix'] = theme('wh_petitions_create_header', array('step' => 1));
  $form['#attributes'] = array('id' => 'create-petition-form-step-1');

  // If we have a petition saved in the form state, pre-populate with those values.
  if (!empty($form_state['petition'])) {
    $form_state['values']['petition_title'] = $form_state['petition']['title'];
    $issues = array();
    foreach ($form_state['petition']['issues'] as $key) {
      $issues[$key] = $key;
    }
    $form_state['values']['petition_issues'] = $issues;
  }
  // If the form hasn't been submitted - we're coming here for the first time.
  // Check to see if there's an id in the url - if so, we're coming to edit an existing petition off the user's dashboard page.
  elseif (empty($form_state['submitted'])) {
    $arg_petition_id = arg(2);
    if (!empty($arg_petition_id)) {
      $petition_item = PetitionsController::loadObject($arg_petition_id);
      $petition = ($petition_item) ? $petition_item->toLegacyArray() : array();

      // Make sure we got a petition and that the creator is the current user, and that it's a draft.
      if (!empty($petition) && $user->uid == $petition['uid'] && $petition['petition_status'] == WH_PETITION_STATUS_DRAFT) {
        $form_state['petition'] = $petition;
        $form_state['petition_id'] = $arg_petition_id;
        $form_state['values']['petition_title'] = $form_state['petition']['title'];
        $issues = array();
        foreach ($form_state['petition']['issues'] as $key) {
          $issues[$key] = $key;
        }
        $form_state['values']['petition_issues'] = $issues;
      }
      else {
        return '';
      }
    }
  }

  // Set the default petition title to go into the field based on whether we're editing a petition, have already had user input, or use the default.
  $default_petition_title = '';
  if (!empty($form_state['values']['petition_title'])) {
    $default_petition_title = check_plain($form_state['values']['petition_title']);
  }
  elseif (!empty($form_state['input']['petition_title'])) {
    $default_petition_title = check_plain($form_state['input']['petition_title']);
  }
  else {
    $default_petition_title = t('finish the statement here');
  }

  // Petition Title - This will be themed in a separate function ( wh_petition_title ) because of unusual design requirements.
  $form['petition_title'] = array(
    '#type' => 'textarea',
    '#title' => t('Enter a title for your petition'),
    '#required' => FALSE,
    '#resizable' => FALSE,
    '#theme' => 'wh_petition_title',
    '#default_value' => $default_petition_title,
    '#attributes' => array('id' => 'form-item-create-petition-title'),
  );

  // Issues
  $issues = array();
  $vocab = taxonomy_vocabulary_machine_name_load('issues');
  if (!empty($vocab)) {
    $issues_list = taxonomy_get_tree($vocab->vid);
    foreach ($issues_list as $issue) {
      $issues[$issue->tid] = check_plain($issue->name);
    }
  }
  $form['petition_issues'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Choose up to 3 issues to describe your petition'),
    '#required' => FALSE,
    '#options' => $issues,
    '#prefix' => '<div id="title-tooltip" class="tooltip display-none">' . variable_get('wh_petitions_tooltip_title', '') . '</div><div class="clearfix">',
    '#suffix' => '</div>',
    '#after_build' => array('wh_petitions_column_checks'),
    '#default_value' => !empty($form_state['values']['petition_issues']) ? $form_state['values']['petition_issues'] : array(),
    '#attributes' => array('id' => 'form-item-issues-describe'),
  );

  // Next Button
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Save & Continue to Step 2'),
    '#submit' => array('wh_petitions_create_petition_step_1_submit'),
    '#prefix' => '<div class="clear p-fuzzy-divide"></div>',
    '#attributes' => array('class' => array('input-button-steps'), 'id' => 'continue-to-step-2'),
  );
  $form['#validate'] = array('wh_petitions_create_petition_step_1_validate');

  $form['#action'] = $base_url . '/' . implode('/', arg());

  return $form;
}

/**
 * Themes the petition title area which has different design requirements that drupal doesn't inherantly handle with the attributes available to the form.
 */
function theme_wh_petition_title($variables) {
  global $base_url;
  $element = $variables['element'];
  element_set_attributes($element, array('id', 'name', 'cols', 'rows'));
  _form_set_class($element, array('form-textarea'));
  $url = $base_url . '/' . drupal_get_path('theme', 'petitions44');

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
 * Places the checkboxes into 3 (mostly-equal) columms.
 */
function wh_petitions_column_checks($form_element, &$form_state) {
  $size = sizeof($form_element['#options']);

  if ($size > 3) {
    $in_each = ceil($size / 3);
    $i = 1;
    foreach ($form_element['#options'] as $key => $val) {
      if ($i == 1) {
        $form_element[$key]['#prefix'] = '<div class="group">';
      }
      elseif ($i == ($in_each + 1)) {
        $form_element[$key]['#prefix'] = '</div><div class="group">';
      }
      elseif ($i == (($in_each * 2) + 1)) {
        $form_element[$key]['#prefix'] = '</div><div class="group">';
      }
      elseif ($i == $size) {
        $form_element[($key)]['#suffix'] = '</div>';
      }

      $form_element[$key]['#attributes']['id'] = $form_element['#id']. '-' . $key;
      $i++;
    }

    return $form_element;
  }
}

/**
 * Validation handler - makes sure we have a title with the right number of characters and have between 1 and 3 issues, inclusive
 */
function wh_petitions_create_petition_step_1_validate($form, &$form_state) {
  $title = trim(strip_tags($form_state['values']['petition_title']));
  if (empty($title) || $title == 'finish the statement here') {
    form_set_error('petition_title', t('Please enter a petition title.'));
  }
  elseif (wh_petitions_exceed_char_count($title, 120)) {
    form_set_error('petition_title', t('Please enter a title that is 120 characters or less.'));
  }

  $count = 0;
  $issues = $form_state['values']['petition_issues'];
  foreach ($issues as $key => $val) {
    if ($val > 0) {
      $count++;
    }
  }
  if ($count > 3) {
    form_set_error('petition_issues', t('Please select a maximum of 3 issues.'));
  }
  elseif ($count == 0) {
    // @todo Make petition_issues optional and easy to enable/disable.
    //form_set_error('petition_issues', t('Please select at least 1 issue.'));
    $text = t('This petition has not been tagged with any petition issues keywords. Please check relevant keywords below.');
    watchdog('wh_petitions', $text, array(), WATCHDOG_WARNING);
    form_set_error('petition_issues', $text);
  }

  // If the user got here other than by clicking the Save and Continue button ( i.e. by clicking the back button in the browser and trying
  // to submit a different step ), clear the validation error messages and give them a message of where to go to perform their edits.
  if ($form_state['input']['op'] != t('Save & Continue to Step 2')) {
    drupal_get_messages('error');
    form_set_error(NULL, '', TRUE);

    $link = '/dashboard';
    drupal_set_message(t('You have clicked the back-button while in the middle creating a petition. The petition you were working on can be continued') . ' ' . l(t('here'), $link, array('attributes' => array('target' => '_top'))) . ' ' . t('or you can begin a new petition on this page.'), 'status');
    return FALSE;
  }
}

/**
 * Submit handler for step 1 of the create a petition form.
 *
 * Create a draft petition object and rebuild the form to go to step 2.
 */
function wh_petitions_create_petition_step_1_submit($form, &$form_state) {
  // Save the petition as a draft and add the id to the form state
  global $user;

  // Get a default petition object or load the one we're editing.
  if (empty($form_state['petition_id'])) {
    $petition = wh_petitions_default_petition();
  }
  else {
    $petition_item = PetitionsController::loadObject($form_state['petition_id']);
    if (empty($form_state['entity_id'])) {
      // Try to see it petition already has the entity ID (phases 3/4 should).
      $entity_id = $petition_item->getEntityId();
      if (!empty($entity_id)) {
        $form_state['entity_id'] = $petition_item->getEntityId();
      }
      elseif (petitions_data_mysql_writes_are_enabled()) {
        // To get entity id in Phase 2, need to call Mysql direct since reads are disabled.
        $petitions_query = new PetitionsSelectQueryMysql();
        $petitions = $petitions_query->setPetitionIds(array($form_state['petition_id']))
          ->execute()->getResultObjects();

        if ($petitions) {
          $petition_item_mysql = $petitions[0];
          $form_state['entity_id'] = $petition_item_mysql->getEntityId();
        }
      }
    }
    $petition = ($petition_item) ? $petition_item->toLegacyArray() : array();

    // Make sure we got a petition and that the creator is the current user, and that it's a draft.
    if (!empty($petition) && $user->uid == $petition['uid'] && $petition['petition_status'] == WH_PETITION_STATUS_DRAFT) {
      $petition['_id'] = new MongoId($form_state['petition_id']);
    }
    else {
      $petition = array();
    }
  }

  if (!empty($petition)) {
    $petition['uid'] = (int)$user->uid;
    $petition['title'] = (string)trim(strip_tags($form_state['values']['petition_title']));
    $petition['title_keywords'] = wh_petitions_generate_keywords($petition['title']);
    $petition['petition_status'] = (int)WH_PETITION_STATUS_DRAFT;
    $petition['response_status'] = (int)WH_PETITION_RESPONSE_STATUS_UNANSWERED;

    $issues = array();
    foreach ($form_state['values']['petition_issues'] as $key => $val) {
      if ($val > 0) {
        array_push($issues, intval($key));
      }
    }
    $petition['issues'] = $issues;

    // Save and retrieve the ID for future use.
    $petition_item = PetitionItem::fromLegacyArray($petition);
    if (!empty($form_state['entity_id'])) {
      $petition_item->setEntityId($form_state['entity_id']);
    }
    PetitionsController::save($petition_item);
    $form_state['entity_id'] = $petition_item->getEntityId();
    $id = $petition_item->getId();
    if (!empty($id)) {
      $form_state['petition_id'] = $id;
      $form_state['petition'] = $petition;

      // Create a default nice url, it will get something better once
      // published but we need this for consistent url handling.
      $petition_item->setNiceUrl("petition/detail/{$id}");
      PetitionsController::save($petition_item);
    }

    // Go to the next page
    $form_state['create_page'] = 2;
    $form_state['rebuild'] = TRUE;
  }
  else {
    return '';
  }
}

/**
 * Second page of the create a petition form.
 *
 * Try to find petitions that:
 * -Are Open and Public
 * -Match at least one of the same issues
 * -Match at least one word from the title to the title keywords, omitting common words
 * -Sorted by most popular (most signatures), Limit 3
 */
function wh_petitions_create_petition_step_2($form, &$form_state) {
  // We're on the second page.
  $form_state['create_page'] = 2;
  $form['#prefix'] = theme('wh_petitions_create_header', array('step' => 2));
  $form['#attributes'] = array('id' => 'create-petition-form-step-2');

  $petition_id = $form_state['petition_id'];

  // Get the issues into an array of keys
  $issues = array();
  foreach ($form_state['petition']['issues'] as $key => $val) {
    if ($val > 0) {
      array_push($issues, intval($val));
    }
  }

  // Removed common words from the string of title keywords
  $title_keywords = $form_state['petition']['title_keywords'];
  $title_terms = array();
  foreach ($title_keywords as $term) {
    if (!in_array($term, array("and", "in", "this", "or", "a", "the", "an", "as", "at", "but", "by", "before", "for", "from", "is", "like", "of", "on", "onto", "than", "this", "that", "to", "with"))) {
      array_push($title_terms, $term);
    }
  }

  // Find three similar petitions.
  $similar = '';
  $similar_html = '';
  $petition_query = PetitionsSelectQueryFactory::create(FALSE);
  $petitions = $petition_query
    ->setIsPublic(TRUE)
    ->setIsSignable(TRUE)
    ->setIssueIDs($issues)
    // @todo Match at least one word from $title_terms in the petition title.
    ->setLimit(3)
    ->setOrderBy(PetitionsSelectQuery::SELECT_QUERY_ORDER_BY_FIELD_SIGNATURE_COUNT, PetitionsSelectQuery::SELECT_QUERY_ORDER_BY_DESC)
    ->execute()
    ->getResultObjects();

  $petition_count = 0;
  if ($petitions) {
    /** @var PetitionItem $petition */
    foreach ($petitions as $petition) {
      $res = $petition->toLegacyArray();
      $link = $petition->getNiceUrl();

      $similar_html .= theme('wh_petitions_display_list_petition', array(
        'entry_num' => $petition_count,
        'title' => check_plain($res['title']),
        'signature_count' => wh_petitions_format_number($res['signature_count']),
        'nice_url' => $res['signature_count'],
        'petition_link' => l(t('Find out more'), $link, array('attributes' => array('target' => '_top'))),
        'cols' => 2,
        'petition_id' => $petition->getId(),
        'target' => '_top',
        'title_link' => $link,
      ));
      $petition_count++;
    }
  }
  /**
   * @todo - Ben - This feature doesn't work in production so take changes with a grain of salt.
   */
  // Retrieve up to 2 related responses based on title matches that match at least one of the issue terms sorted by petition_count desc
  $query = db_select('node', 'n', array('target' => 'slave'));
  $query->innerJoin('field_data_field_petition_count', 'c', 'n.nid = c.entity_id');
  $query->addField('c', 'field_petition_count_value', 'field_petition_count_value');
  $query->fields('n', array('nid', 'title'))
    ->condition('n.type', 'wh_response')
    ->condition('n.status', 0, '<>')
    ->orderBy('c.field_petition_count_value', 'DESC')
    ->orderBy('n.title', 'ASC');

  // Join to the Issues terms to match at least one
  $query->innerJoin('field_data_field_issues', 'i', 'n.nid = i.entity_id');
  $query->condition('i.field_issues_tid', $issues, 'IN');

  // Join to private keywords to match a title keyword from the draft to a private keyword
  $query->innerJoin('field_data_field_private_tags', 't', 'n.nid = t.entity_id');
  $query->innerJoin('taxonomy_term_data', 'td', 't.field_private_tags_tid = td.tid');
  $query->condition('td.name', $title_terms, 'IN');
  $query->distinct(TRUE);
  $result = $query->range(0, 2)
    ->execute();

  $response_count = 0;
  $entry_num = 0;
  if ($petition_count % 2 == 1) {
    $entry_num = 1;
  }

  foreach ($result as $res) {
    $link = url('node/' . $res->nid);

    $similar_html .= theme('wh_response_display_list_response', array(
      'entry_num' => $response_count,
      'cols' => 2,
      'nid' => intval($res->nid),
      'title' => l($res->title, $link, array('attributes' => array('target' => '_top'))),
      'response_link' => l(t('Read Full Response'), $link, array('attributes' => array('target' => '_top'))),
    ));
    $response_count++;
    $entry_num++;
  }

  // Determine what text to show based on what we found.
  if ($petition_count > 0 || $response_count > 0) {
    $similar = '<div class="alert-msg"><span class="red">' . t('Important:') . '</span> ';

    if ($petition_count > 0 && $response_count == 0) {
      $similar .= t('Below are a few petitions that may be similar to yours.');
    }
    elseif ($petition_count == 0 && $response_count > 0) {
      $similar .= t('Below are a few responses that may be similar to your petition.');
    }
    else {
      $similar .= t('Below are a few petitions and responses that may be similar to your petition.');
    }

    $similar .= '</div>';
    $similar .= '<div class="petition-instructions either-or-msg">' . t('Consider signing an existing petition instead of creating a new one.  Duplicate petitions on the same topic will result in fewer signatures for each petition.  Focusing your efforts on signing a petition that already has signatures increases the likelihood that you\'ll get a response from the Obama Administration.') . '</div>';
    $similar .= '<div class="petition-list clearfix">' . $similar_html . '</div>';
    $similar .= '<div class="clear p-fuzzy-divide"> </div>';
    $similar .= '<div class="petition-instructions continue-text">' . t("Didn't find a similar petition? Click below to continue creating a new petition.") . '</div>';
  }
  else {
    $similar = '<div class="petition-instructions either-or-msg">' . t('No similar petitions or responses have been found.') . '</div>';
  }

  $form['similar_issues'] = array(
    '#type' => 'markup',
    '#markup' => '<div>' . $similar . '</div>',
  );

  // Next Button
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Save & Continue to Step 3'),
    '#submit' => array('wh_petitions_create_petition_step_2_submit'),
    '#attributes' => array('class' => array('input-button-steps'), 'id' => 'continue-to-step-3'),
  );

  $form['#action'] = '/' . implode('/', arg());

  return $form;
}

/**
 * Submit handler for step 2 of the create a petition form.  Don't need to save any values, but update the page and rebuild.
 */
function wh_petitions_create_petition_step_2_submit($form, &$form_state) {
  // Go to the next page
  $form_state['create_page'] = 3;
  $form_state['rebuild'] = TRUE;
}

/**
 * Third page of the create a petition form.  Contains body and user-defined keywords.
 */
function wh_petitions_create_petition_step_3($form, &$form_state) {
  global $base_url;
  // We're on the third page.
  $form_state['create_page'] = 3;
  $form['#prefix'] = theme('wh_petitions_create_header', array('step' => 3));
  $form['#attributes'] = array('id' => 'create-petition-form-step-3');
  $url = $base_url . '/' . drupal_get_path('theme', 'petitions44');

  // Populate with the values from a saved petition
  if (!empty($form_state['petition']['body'])) {
    $form_state['values']['petition_description'] = $form_state['petition']['body'];
  }
  if (!empty($form_state['petition']['user_tags'])) {
    $form_state['values']['petition_tags'] = implode(', ', $form_state['petition']['user_tags']);
  }

  // Description
  $form['petition_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Enter a description for your petition'),
    '#description' => '<div class="help-text">' . t('Please limit your description to 800 characters.') . '    <span class="character-count"></span></div><div class="view-tooltip"><a href="#">Tips for writing good descriptions</a><div id="description-tooltip" class="tooltip display-none no-follow">' . variable_get('wh_petitions_tooltip_description', '') . '<div class="tip-arrow"><img src="' . $url . '/img/tip-arrow.gif"></div>
</div></div>',
    '#required' => FALSE,
    '#resizable' => FALSE,
    '#default_value' => !empty($form_state['values']['petition_description']) ? $form_state['values']['petition_description'] : '',
  );

  // Free Tags
  $form['petition_tags'] = array(
    '#type' => 'textfield',
    '#access' => petitions_data_mongo_reads_are_enabled(),
    '#title' => t('Enter keywords that best describe your petition'),
    '#description' => '<div class="help-text">' . t('Separate keywords by commas.') . ' <span class="character-count">' . t('Limited to 120 characters.') . '</span></div><div class="view-tooltip-tag"><a href="#">Tips for writing good keywords</a><div id="tags-tooltip" class="tooltip display-none no-follow">' . variable_get('wh_petitions_tooltip_tags', '') . '<div class="tip-arrow-up"><img src="' . $url . '/img/tip-arrow-up.gif"></div>
</div></div>',
    '#required' => FALSE,
    '#size' => 50,
    '#maxlength' => 120,
    '#attributes' => array('class' => array('input-text')),
    '#default_value' => !empty($form_state['values']['petition_tags']) ? $form_state['values']['petition_tags'] : '',
  );

  // Next Button
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Save & Continue to Final Step'),
    '#submit' => array('wh_petitions_create_petition_step_3_submit'),
    '#prefix' => '<div class="clear p-fuzzy-divide"></div><!--/p-fuzzy-divide-->',
    '#attributes' => array('class' => array('input-button-steps'), 'id' => 'continue-to-final-step'),
  );
  $form['#validate'] = array('wh_petitions_create_petition_step_3_validate');

  $form['#action'] = $base_url . '/' . implode('/', arg());

  return $form;
}

/**
 * Validation handler - makes sure the body is filled out and isn't too long, and keywords aren't too long.
 */
function wh_petitions_create_petition_step_3_validate($form, &$form_state) {
  if (empty($form_state['values']['petition_description'])) {
    form_set_error('petition_description', t('Please enter a petition description.'));
  }
  elseif (wh_petitions_exceed_char_count($form_state['values']['petition_description'], 800)) {
    form_set_error('petition_description', t('Please enter a description that is 800 characters or less.'));
  }

  if (wh_petitions_exceed_char_count($form_state['values']['petition_tags'], 120)) {
    form_set_error('petition_tags', t('Please enter keywords that are a total of 120 characters or less.'));
  }
}

/**
 * Submit handler for step 3 of the create a petition form.
 *
 * Save the the description and keywords to the petition.
 */
function wh_petitions_create_petition_step_3_submit($form, &$form_state) {
  global $user;

  // Add the description and keywords to the petition.
  $petition_item = PetitionsController::loadObject($form_state['petition_id']);
  $petition = ($petition_item) ? $petition_item->toLegacyArray() : array();
  if (!empty($petition) && $user->uid == $petition['uid'] && $petition['petition_status'] == WH_PETITION_STATUS_DRAFT) {
    $petition['user_tags'] = array_unique(wh_petitions_generate_keywords(strip_tags($form_state['values']['petition_tags']), ','));
    $petition['body'] = (string)strip_tags($form_state['values']['petition_description']);
    $petition['body_keywords'] = wh_petitions_generate_keywords(strip_tags($form_state['values']['petition_description']));
    $petition_item = PetitionItem::fromLegacyArray($petition);
    $petition_item->setEntityId($form_state['entity_id']);
    PetitionsController::save($petition_item);
    $form_state['petition'] = $petition;

    module_invoke_all("wh_petitions_petition_body_update", $form_state['petition_id'],
            $petition['body']);
  }

  // Go to the next page
  $form_state['create_page'] = 4;
  $form_state['rebuild'] = TRUE;
}

/**
 * Fourth page of the create a petition form - preview + submit/save/delete
 */
function wh_petitions_create_petition_step_4($form, &$form_state) {
  global $user;
  global $base_url;

  // We're on the fourth page.
  $form_state['create_page'] = 4;
  $form['#prefix'] = theme('wh_petitions_create_header', array('step' => 4));
  $form['#attributes'] = array('id' => 'create-petition-form-step-4');

  // Load the petition
  $petition_item = PetitionsController::loadObject($form_state['petition_id']);
  $petition = ($petition_item) ? $petition_item->toLegacyArray() : array();

  // Verify we should be here
  if (empty($petition) || $user->uid != $petition['uid'] || $petition['petition_status'] != WH_PETITION_STATUS_DRAFT) {
    return '';
  }

  // Load variables to display with the petition
  $petition_id = $petition_item->getId();
  $timestamp = $petition_item->getCreated();
  $created = date("M d, Y", $timestamp);
  $issues = array();
  if (!empty($petition['issues'])) {
    for ($i=0;$i<sizeof($petition['issues']);$i++) {
      $term = taxonomy_term_load($petition['issues'][$i]);
      if (!empty($term)) {
        array_push($issues, l($term->name, 'petitions/all/0/2/' . intval($term->tid), array('attributes' => array('target' => '_top'))));
      }
    }
  }
  $issues = implode(', ', $issues);
  $public_needed = variable_get('wh_petitions_public_signatures', 20);
  $total_needed = variable_get('wh_petitions_review_signatures', 500);
  $signatures_needed = $total_needed - $petition['signature_count'];
  $signature_count = $petition['signature_count'];

  // End date is today + number of days to get the signatures ( not from the date it was created - because if it was published today the clock would start from now )
  $end_date = strtotime('+' . variable_get('wh_petitions_review_timeframe', 30) . ' days');
  $end_date = date("F d, Y", $end_date);

  // Show the Petition Info
  $form['preview'] = array(
    '#type' => 'markup',
    '#markup' => theme('wh_petitions_display_petition_info', array(
      'title' => check_plain($petition['title']),
      'body' => wh_petitions_format_description($petition['body']),
      'created' => check_plain($created),
      'issues' => $issues,
      'published' => FALSE,
      'total_needed' => wh_petitions_format_number($total_needed),
      'signatures_needed' => wh_petitions_format_number($signatures_needed),
      'signatures' => wh_petitions_format_number($signature_count),
      'end_date' => check_plain($end_date),
      'archived' => FALSE,
      'responded' => FALSE,
      'flagged' => FALSE,
      'admin' => FALSE,
    )),
  );

  // Publish Button and Text
  $publish_text = '<p>' . t('NOTE: You will not be able to edit or delete this petition once you publish it.') . '</p><p>' . t('By creating this petition you agree to receive email updates related to this petition.') . '</p>';
  //$publish_text .= '<p><strong>' . t('Next Steps') . '</strong></p>';
  //$publish_text .= '<p>' . t('Get !public_count signatures before it becomes viewable by the White House community.', array('!public_count' => wh_petitions_format_number($public_needed))) . '</p>';
  //$publish_text .= '<p>' . t('Get !total_count signatures before the White House will review it.', array('!total_count' => wh_petitions_format_number($total_needed))) . '</p>';
  $form['publish'] = array(
    '#type' => 'submit',
    '#value' => t('Publish my petition'),
    '#prefix' => '<div class="petition-detail-publish-options clearfix"><div class="col-2">',
    '#submit' => array('wh_petitions_create_petition_step_4_submit'),
    '#suffix' => $publish_text . '</div>',
    '#attributes' => array('id' => 'button-publish-my-petition', 'class' => array('button')),
  );

  // Fieldset for the Save / Edit / Delete buttons
  $form['save_edit_delete'] = array(
    '#type' => 'fieldset',
    '#title' => '',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#prefix' => '<div class="col-2" id="no-publish-other-options">',
    '#suffix' => '</div></div>',
  );

  // Redirect to the main site or the petition tool site, depending on where this request came from
  $form['save_edit_delete']['edit'] = array(
    '#type' => 'submit',
    '#prefix' => '<div class="option"><p>' . t('Not ready to publish?') . '</p>' . l(t('Save & Publish Later'), 'dashboard', array('attributes' => array('id' => 'button-save-this-petition-and-publish-later', 'class' => array('button'), 'target' => '_top'))) . '</div><div class="option"><p>' . t('Want to make changes?') . '</p>',
    '#suffix' => '</div>',
    '#value' => t('Edit my petition'),
    '#submit' => array('wh_petitions_create_petition_edit_petition'),
    '#attributes' => array('id' => 'button-back-to-previous-step', 'class' => array('button')),
  );

  $delete_text = variable_get('wh_petitions_delete_modal_text', '');
  $form['save_edit_delete']['delete'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="option" style="position:relative;"><p>' . t('Want to delete this petition?  Note: If you delete this petition all of your information will be lost.') . "</p>" . l(t('Delete'), 'petition/delete/' . $petition_id, array('attributes' => array('id' => 'delete-petition-link-' . $petition_id, 'class' => 'button delete delete-petition-form no-follow button-delete-my-petition', 'rel' => $petition_id))) . '<div class="delete-insert delete-insert-' . $petition_id . '"><div id="delete-petition-' . $petition_id . '" class="delete-petition-overlay display-none">
                <a class="close-button no-follow" rel="' . $petition_id . '">' . t('Close') . '</a>
                <div class="wrap">
                  <h2 class="alert">' . t('You are about to delete your petition draft.') . '</h2>
                  <p>' . $delete_text . '</p>
                </div><div class="buttons">' .
            l(t('Delete This Petition'), 'petition/delete/' . $petition_id, array('attributes' => array('id' => 'button-delete-this-petition', 'target' => '_top', 'class' => array('button')), 'query' => array('process' => 'create_form'))) .'
            <a id="button-no-thanks-keep" rel="' . $petition_id . '">' . t('No thanks. Keep this Draft') . '</a>
           </div></div></div>',
  );

  $form['#action'] = $base_url . '/' . implode('/', arg());

  return $form;
}

/**
 * Submit handler for step 4 of the create a petition form.
 *
 * Publish the petition.
 */
function wh_petitions_create_petition_step_4_submit($form, &$form_state) {
  global $user;

  $petition_id = $form_state['petition_id'];
  $petition_item = PetitionsController::loadObject($petition_id);
  $petition = ($petition_item) ? $petition_item->toLegacyArray() : array();

  if (!empty($petition) && $user->uid == $petition['uid'] && $petition['petition_status'] == WH_PETITION_STATUS_DRAFT) {
    $petition['petition_status'] = (int)WH_PETITION_STATUS_PRIVATE;
    $petition['published'] = (int)time();

    // By default, a user signs their own petition
    $user_load = user_load($user->uid);

    // Set the count to 0 as the mongo implementation of SignaturesController::save()
    // sets the mongo signature number to the petition signature count + 1
    $petition['signature_count'] = 0;

    // Petition should always use the review timeframe, public/review signature counts that are set when the petition is published.
    $petition['public_signatures'] = (int)variable_get('wh_petitions_public_signatures', 20);
    $petition['response_signatures'] = (int)variable_get('wh_petitions_review_signatures', 500);
    $petition['review_timeframe'] = (int)variable_get('wh_petitions_review_timeframe', 30);

    // Make it available publicly if the threshhold is 1 signature.
    if ($petition['public_signatures'] == 1) {
      $petition['petition_status'] = (int)WH_PETITION_STATUS_PUBLIC;
      $petition['reached_public'] = (int)time();
    }

    $petition_item = PetitionItem::fromLegacyArray($petition);
    $petition_item->setEntityId($form_state['entity_id']);
    PetitionsController::save($petition_item);
    $new_id = $petition_item->getId();

    // Create a new GovDelivery list
    if (!empty($new_id)) {
      govdelivery_subscriber_api_add_topic('petitions_' . $new_id);
    }

    $form_state['petition'] = $petition;

    $profile = profile2_load_by_user($user_load);

    $signature = new SignatureItem();
    $signature->setLegacyPetitionId($petition_item->getLegacyId());
    $signature->setPetitionId($petition_item->getEntityId());
    $signature->setIpAddress((string) ip_address());
    $signature->setUserAgent((string) $_SERVER['HTTP_USER_AGENT']);
    $signature->setUid((int) $user->uid);
    $signature->setCreated((int) time());
    if (is_array($profile) && count($profile) > 0) {
      $first_name = isset($profile['main']->field_first_name[LANGUAGE_NONE][0]['value']) ? $profile['main']->field_first_name[LANGUAGE_NONE][0]['value'] : NULL;
      $last_name = isset($profile['main']->field_last_name[LANGUAGE_NONE][0]['value']) ? $profile['main']->field_last_name[LANGUAGE_NONE][0]['value'] : NULL;
      $city = isset($profile['main']->field_city[LANGUAGE_NONE][0]['value']) ? $profile['main']->field_city[LANGUAGE_NONE][0]['value'] : NULL;
      $state = isset($profile['main']->field_state[LANGUAGE_NONE][0]['value']) ? $profile['main']->field_state[LANGUAGE_NONE][0]['value'] : NULL;
      $zip = isset($profile['main']->field_zip[LANGUAGE_NONE][0]['value']) ? $profile['main']->field_zip[LANGUAGE_NONE][0]['value'] : NULL;

      $signature->setFirstName($first_name);
      $signature->setLastName($last_name);
      $signature->setCity($city);
      $signature->setState($state);
      $signature->setZip($zip);
    }
    $signature->setUser($user);

    SignaturesController::save($signature);

    // Increment the signature count on the petition now that we've saved it.
    PetitionsController::incrementSignatureCount($petition_item->getId());

    // Add creator's signature
    if (!empty($new_id)) {
      govdelivery_subscriber_api_add_subscriber($user->mail, 'petitions_' . $new_id);
    }

    // Send an email to the creator
    if (!empty($new_id)) {
      wh_petitions_email_published($new_id, $user->uid);
    }

    // Go to the thank-you page
    $form_state['create_page'] = 5;
    $form_state['rebuild'] = TRUE;
  }

  return '';
}

/**
 * Save as Draft submit handler. The petition is saved as a draft with every form page, so we've up to date.  Just redirect to user dashboard.
 */
function wh_petitions_create_petition_save_petition($form, &$form_state) {
  global $user;
  drupal_goto('dashboard');
}

/**
 * Edit my petition submit handler.  Go back to the first step of the form.
 */
function wh_petitions_create_petition_edit_petition($form, &$form_state) {
  // Load the updated petition into the form_state
  $petition_item = PetitionsController::loadObject($form_state['petition_id']);
  $petition_item->setEntityId($form_state['entity_id']);
  $petition = ($petition_item) ? $petition_item->toLegacyArray() : array();
  $form_state['petition'] = $petition;

  // Go back to the first page
  $form_state['create_page'] = 1;
  $form_state['rebuild'] = TRUE;
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
    '#after_build' => array('wh_petitions_column_checks'),
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
  );

  // Submit Button
  $form['go'] = array(
    '#type' => 'submit',
    '#value' => t('Go'),
    '#attributes' => array('id' => 'search-go-button', 'class' => array('button')),
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

/**
 * Add A Comment standalone page ( 508 compliance )
 */
function wh_petitions_add_comment_form($petition_id) {
  $form = drupal_get_form('wh_petitions_comment_petition_form', $petition_id);
  $form = drupal_render($form);

  return '<div id="filter-page"><h3>' . t('Add A Comment') . '</h3><div class="drop-down">' . $form . '</div>';
}

/**
 * Comment on A Petition form
 */
function wh_petitions_comment_petition_form($form, &$form_state, $petition_id) {
  /*
  $form['signature_comment'] = array(
    '#type' => 'textarea',
    '#title' => t('You are welcome to leave a comment with your signature'),
    '#description' => '<div class="help-text"><strong>240</strong> ' . t('characters remaining') . '</div>',
    '#required' => FALSE,
    '#resizable' => FALSE,
    '#default_value' => t('Type your comment here ...'),
    '#attributes' => array('onfocus' => array("if(this.value==this.defaultValue){this.value=''};"), 'onblur' => array("if(this.value==''){this.value=this.defaultValue;}")),
  );

  $form['petition_id'] = array(
    '#type' => 'hidden',
    '#value' => check_plain($petition_id),
  );

  global $user;
  $form['user_id'] = array(
    '#type' => 'hidden',
    '#value' => check_plain($user->uid),
  );

  // Next Button
  $form['sign'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#attributes' => array('class' => array('blue-button'), 'id' => 'submit-comment-with-sig'),
    '#submit' => array('wh_petitions_comment_petition_form_submit'),
  );
  */
  $text = '<p>' . variable_get('wh_petitions_comment_modal_text', '') . '</p>';
  $form['additional'] = array(
    '#type' => 'markup',
    '#markup' => $text,
  );

  global $base_url;
  $form['#action'] = $base_url . '/petition/comment/' . check_plain($petition_id);

  return $form;
}

/**
 * Callback for the submit comment form, for a user to add a comment to their signature.
 *
 * @todo - Ben - Can't test this so verify the redirect works if it is ever used.
 */
function wh_petitions_add_comment() {
  global $user;
  $petition_id = strip_tags($_POST['petition_id']);

 // Verify we got a petition id and that it's not blank and that the petition_id in the url matches the one from the POST
  if (empty($petition_id) || empty($user->uid)) {
    if (!empty($_POST['js']) && $_POST['js'] == TRUE) {
      print '';
      exit;
    }
    else {
      return '';
    }
  }

  // Verify the user_id matches the one from the POST
  if (empty($_POST['user_id']) || $_POST['user_id'] != $user->uid) {
    if (!empty($_POST['js']) && $_POST['js'] == TRUE) {
      print '';
      exit;
    }
    else {
      return '';
    }
  }

  // Load the petition
  $conn = wh_petitions_mongo_petition_connection();
  $conn->setSlaveOkay(FALSE);
  $petition = wh_petitions_load_slim_petition($conn, $petition_id, array('petition_status'));

  // If it wasn't a valid petition id, exit
  if (empty($petition)) {
    if (!empty($_POST['js']) && $_POST['js'] == TRUE) {
      print '';
      exit;
    }
    else {
      return '';
    }
  }

  // Can only sign petitions that are open - private, public, under review
  if ($petition['petition_status'] != WH_PETITION_STATUS_PRIVATE && $petition['petition_status'] != WH_PETITION_STATUS_PUBLIC && $petition['petition_status'] != WH_PETITION_STATUS_UNDER_REVIEW) {
    if (!empty($_POST['js']) && $_POST['js'] == TRUE) {
      print '';
      exit;
    }
    else {
      return '';
    }
  }

  // Retrieve the comment and trim it to 255 characters if needed ( someone had js disabled )
  $comment = strip_tags($_POST['signature_comment']);
  if (wh_petitions_exceed_char_count($comment, 255)) {
    $comment = substr($comment, 0, 255);
  }

  // Update the comment
  $sig_conn = wh_petitions_mongo_petition_signatures_connection();
  $sig_conn->setSlaveOkay(FALSE);
  $sig_conn->update(
    array('petition_id' => (string)$petition_id, 'uid' => (int)$user->uid),
    array('$set' => array('comment' => $comment))
  );

  // If this was an AJAX request, pass some data back to browser that we had a success
  if (!empty($_POST['js']) && $_POST['js'] == TRUE) {
    print '<p>' . t('Your comment has been added to your signature.') . '</p>';
    exit;
  }
  // Redirect to the main site or the petition tool site, depending on where this request came from
  else {
    drupal_goto($petition['nice_url']);
  }
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

/**
 * Ensures that all character counting is the same.  Returns true if the string exceeds the count.
 */
function wh_petitions_exceed_char_count($string, $chars) {
  $string = str_replace("\r\n", "\n", $string);
  if (mb_strlen($string) > $chars) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}
