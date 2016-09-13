<?php

/**
 * @file
 * Petition Creation Form Steps.
 */

/**
 * Callback for create petition form. Displays each step of create process.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state.
 *
 * @return array
 *   Form array.
 */
function wh_petitions_create_petition($form, &$form_state) {
  // Redirect anonymous users to login/register
  if (user_is_anonymous()) {
    drupal_goto('user/login', array('query' => array('destination' => 'petition/create')));
    exit;
  }

  if (!isset($form_state['step'])) {
    logger_event('petitioncreateform.form.started');
  }

  $form_state['step'] = isset($form_state['step']) ? $form_state['step'] : 1;
  $current_form = _wh_petitions_get_create_form($form_state['step'], $form_state);
  logger_event('petitioncreateform.form.step_' . $form_state['step']);

  return $current_form;
}

/**
 * Get create form for step.
 *
 * @param int $step
 *   Form step to get.
 *
 * @param array $form_state
 *   Form State array.
 *
 * @return array
 *   Form Array
 */
function _wh_petitions_get_create_form($step, $form_state) {
  $form = array();
  $form['#prefix'] = '<div id="petition-create-formwrapper">';
  $form['#suffix'] = '</div>';
  $form['#attributes']['class'][] = 'petition-create-form-step-' . $step;
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'wh_petitions') . '/wh_petitions.form.js',
  );

  $func = '_wh_petitions_get_create_form_step_' . $step;
  $form['step_' . $step] = $func($form_state);

  return $form;
}

/**
 * Get number of petition create form steps.
 *
 * @return int
 *   # of form steps.
 */
function _wh_petitions_get_num_create_form_steps() {
  $steps = 0;
  while (function_exists('_wh_petitions_get_create_form_step_' . ++$steps)) {
  }
  return $steps - 1;
}

/**
 * Create Petition Form submit handler.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state.
 */
function wh_petitions_create_petition_form_submit($form, &$form_state) {
  $step_id = 'step_' . $form_state['step'];
  $form_state['storedvalues'][$step_id] = $form_state['values'];
  if (!isset($form_state['storedvalues']['step_all'])) {
    $form_state['storedvalues']['step_all'] = array();
  }

  $form_state['storedvalues']['step_all'] = array_merge($form_state['storedvalues']['step_all'], $form_state['values']);

  // Submit data prior to last step, which is thank you page.
  if ($form_state['step'] == (_wh_petitions_get_num_create_form_steps() - 1)) {
    global $user;
    logger_event('petitioncreateform.form.submitted');

    // Final step reached.
    $final_form_values = $form_state['storedvalues']['step_all'];

    $issues = taxonomy_term_load_multiple($final_form_values['petition_issues']);
    $issues_array = array();
    if ($issues) {
      foreach ($issues as $tid => $term) {
        $issues_array[] = array(
          'id' => $tid,
          'name' => petitions_data_sanitize_output($term->name),
        );
      }
    }

    $type = taxonomy_term_load_multiple(array($final_form_values['petition_type']));
    $petition_type = array();
    if ($type) {
      foreach ($type as $tid => $term) {
        $petition_type[] = array(
          'id' => $tid,
          'name' => petitions_data_sanitize_output($term->name),
        );
      }
    }

    // Save new petition.
    $petition_item = new PetitionItem();

    $petition_item->setUid((int) $user->uid);
    $petition_item->setTitle((string) trim(strip_tags($final_form_values['petition_title'])));
    $petition_item->setBody((string) trim(strip_tags($final_form_values['petition_description'])));
    $petition_item->setType($petition_type);
    $petition_item->setIssues($issues_array);
    $petition_item->setStatus((int) WH_PETITION_STATUS_PRIVATE);
    $petition_item->setResponseStatus((int) WH_PETITION_RESPONSE_STATUS_UNANSWERED);
    $petition_item->setSignatureCount(0);
    $petition_item->setPublished((int) time());

    $petition_item->setSignaturePublicThreshold((int) variable_get('wh_petitions_public_signatures', 20));
    $petition_item->setSignatureThreshold((int) variable_get('wh_petitions_review_signatures', 500));
    $petition_item->setReviewTimeframe((int) variable_get('wh_petitions_review_timeframe', 30));

    // Make it available publicly if the threshhold is 1 signature.
    if ($petition_item->getSignaturePublicThreshold() == 1) {
      $petition_item->setStatus((int) WH_PETITION_STATUS_PUBLIC);
      $petition_item->setReachedPublic((int) time());
    }

    PetitionsController::save($petition_item);

    $form_state['petition_id'] = $petition_item->getEntityId();

    // Self sign petition.
    $user_load = user_load($user->uid);
    $profile = profile2_load_by_user($user_load);

    $signature = new SignatureItem();
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

    if (isset($user_load->mail)) {
      petition_update_email_ip_metrics($petition_item->getEntityId(), $user_load->mail, (string) ip_address());
    }

    module_invoke_all('petition_signatures_updated', $petition_item->getEntityId());

    // Send an email to the creator.
    if ($petition_item->getEntityId()) {
      wh_petitions_email_published($petition_item->getEntityId(), $user->uid);
    }
  }
  $form_state['rebuild'] = TRUE;
  $form_state['step']++;

}

/**
 * Create Petition Form Prev button submit handler.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state.
 */
function wh_petitions_create_petition_form_prev_submit($form, &$form_state) {
  $form_state['step']--;
  $form_state['rebuild'] = TRUE;
}

/**
 * Create Petition Form Ajax submit handler.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state.
 *
 * @return array
 *   Form array.
 */
function wh_petitions_create_petition_form_ajax_submit($form, &$form_state) {
  return $form;
}

/**
 * Create Petition Form validation handler.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state.
 */
function wh_petitions_create_petition_form_validate(&$form, &$form_state) {
  // If Previous button is clicked, do not perform validation.
  if ($form_state['values']['op'] == 'Previous') {
    return;
  }

  switch ($form_state['step']) {
    case '1':
      $petition_type = $form_state['values']['petition_type'];
      if (empty($petition_type)) {
        form_set_error('petition_type', t('Please select a Petition Type'));
      }
      break;

    case '2':
      $petition_title = trim(strip_tags(_wh_petitions_get_form_default_value($form_state['step'], 'petition_title', $form_state)));
      if (empty($petition_title)) {
        form_set_error('petition_title', t('Please enter a petition title.'));
      }
      elseif (_wh_petitions_exceed_char_count($petition_title, 120)) {
        form_set_error('petition_title', t('Please enter a title that is 120 characters or less.'));
      }
      break;

    case '3':
      $petition_description = trim(strip_tags(_wh_petitions_get_form_default_value($form_state['step'], 'petition_description', $form_state)));
      if (empty($petition_description)) {
        form_set_error('petition_description', t('Please enter a petition description.'));
      }
      elseif (_wh_petitions_exceed_char_count($petition_description, 800)) {
        form_set_error('petition_description', t('Please enter a description that is 800 characters or less.'));
      }
      break;

    case '4':
      $count = 0;
      $issues = _wh_petitions_get_form_default_value($form_state['step'], 'petition_issues', $form_state);
      if (empty($issues)) {
        form_set_error('petition_issues', t('Please select at least one tag'));
      }
      else {
        foreach ($issues as $key => $val) {
          if ((int) $val > 0) {
            $count++;
          }
        }
        if ($count > 3) {
          form_set_error('petition_issues', t('Please select a maximum of 3 tags.'));
        }
        elseif ($count == 0) {
          form_set_error('petition_issues', t('Please select at least one tag'));
        }
      }
      break;

    case '5':
      break;

    default:
      break;
  }

  $full_form = &$form['step_' . $form_state['step']];
  _wh_petitions_add_field_error_labels($full_form, $form['step_' . $form_state['step']]);
}

/**
 * Implements hook_ajax_render_alter().
 */
function wh_petitions_ajax_render_alter(&$commands) {
  // Scroll top top of multistep form on each step.
  $commands[] = array('command' => 'petitionCreateScroll');
}

/**
 * Get Petition Create Next/Previous Nav Buttons.
 *
 * @param bool $prev
 *   If TRUE, returns previous button, otherwise next button.
 * @param string $button_value_override
 *   String to override button value with.
 * @param string $add_container
 *   If set, will wrap button in container with the string as the class.
 *
 * @return array
 *   Button form array.
 */
function _wh_petitions_get_create_nav_button($prev = FALSE, $button_value_override = '', $add_container = '') {
  $button_value = $prev ? t('Go Back') : t('Next Step');
  $button_value = ($button_value_override == '') ? $button_value : $button_value_override;
  $button_id = drupal_html_class($button_value);

  $button[$button_id] = array(
    '#type' => 'submit',
    '#value' => $button_value,
    '#attributes' => array('class' => array('input-button-' . $button_id)),
    '#ajax' => array(
      'callback' => 'wh_petitions_create_petition_form_ajax_submit',
      'wrapper' => 'petition-create-formwrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  if ($prev) {
    $button[$button_id]['#submit'] = array('wh_petitions_create_petition_form_prev_submit');
  }
  else {
    $button[$button_id]['#validate'] = array('wh_petitions_create_petition_form_validate');
    $button[$button_id]['#submit'] = array('wh_petitions_create_petition_form_submit');
  }

  if ($add_container != '') {
    $button_container = array(
      '#type' => 'container',
      '#attributes' => array('class' => array($add_container)),
    );

    $button_container[$button_id] = $button[$button_id];
    return $button_container;
  }
  else {
    return $button;
  }
}

/**
 * Get default value from multistep form field.
 *
 * @param int $step
 *   Step number of form.
 * @param string $field
 *   Field name to get value of.
 * @param array $form_state
 *   Form state array.
 * @param string $default_value
 *   Initial default value to use.
 *
 * @return mixed
 *   Default value of field.
 */
function _wh_petitions_get_form_default_value($step, $field, $form_state, $default_value = '') {
  if (isset($form_state['values'][$field])) {
    $default_value = $form_state['values'][$field];
  }
  elseif (isset($form_state['storedvalues']['step_' . $step]) && isset($form_state['storedvalues']['step_' . $step][$field])) {
    $default_value = $form_state['storedvalues']['step_' . $step][$field];
  }

  return $default_value;
}

/**
 * Petition Create Step 1 - Type.
 *
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function _wh_petitions_get_create_form_step_1($form_state) {
  $current_step = substr(__FUNCTION__, -1, 1);
  $form = array();

  // Petition Types.
  $petition_types = array();
  $initial_petition_type_tid = 0;
  $vocab = taxonomy_vocabulary_machine_name_load('petition_type');
  if (!empty($vocab)) {
    $petition_types_list = taxonomy_get_tree($vocab->vid);
    foreach ($petition_types_list as $petition_type) {
      $petition_types[$petition_type->tid] = '<h5>' . check_plain($petition_type->name) . '</h5>';
      if (strtolower($petition_type->name) == 'change an existing law or policy') {
        $initial_petition_type_tid = $petition_type->tid;
      }
    }
  }

  $default_petition_type_value = _wh_petitions_get_form_default_value($current_step, 'petition_type', $form_state, $initial_petition_type_tid);

  $form['breadcrumb'] = _wh_petitions_get_step_nav($current_step);
  $form['breadcrumb']['#type'] = 'container';
  $form['breadcrumb']['#attributes']['class'] = array('step-breadcrumbs');
  $form['breadcrumb']['next'] = _wh_petitions_get_create_nav_button(FALSE, '', 'top-submit-button');

  $form['petition_type_header'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('petition-type-header')),
    '#prefix' => t('!html1This field is required!html2', array(
      '!html1' => '<span class="element-invisible screenreader-prompt" aria-hidden="false">',
      '!html2' => '</span>',
    )),
    '#children' => t('!html1We the People ask the federal government to:!html2', array(
      '!html1' => '<h1>',
      '!html2' => '</h1>',
    )),
  );

  $form['petition_type'] = array(
    '#type' => 'radios',
    '#title' => '',
    '#options' => $petition_types,
    '#default_value' => $default_petition_type_value,
    '#attributes' => array('class' => array('create-petition-type')),
  );



  $form['bottom_navigation'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('petition-bottom-navigation')),
  );

  $form['bottom_navigation']['petition_type_help'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('petition-create-help', 'petition-type-help')),
    '#children' => l(t('See past successful petitions for help getting started'), '/about', array(
      'fragment' => 'success',
      'attributes' => array(
        'target' => '_blank',
      ),
    )),
  );

  // &nbsp necessary to leave an empty space for mobile.
  $form['bottom_navigation']['navigation_left'] = array();
  $form['bottom_navigation']['navigation_left']['#children'] = '&nbsp;';

  $form['bottom_navigation']['navigation_left']['#type'] = 'container';
  $form['bottom_navigation']['navigation_left']['#attributes'] = array(
    'class' => array('petition-navigation-left'),
  );

  $form['bottom_navigation']['navigation_right'] = _wh_petitions_get_create_nav_button();
  $form['bottom_navigation']['navigation_right']['#type'] = 'container';
  $form['bottom_navigation']['navigation_right']['#attributes'] = array(
    'class' => array('petition-navigation-right'),
  );

  return $form;
}

/**
 * Petition Create Step 2 - Title.
 *
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function _wh_petitions_get_create_form_step_2($form_state) {
  $current_step = substr(__FUNCTION__, -1, 1);
  $form = array();

  $default_petition_title = check_plain(_wh_petitions_get_form_default_value($current_step, 'petition_title', $form_state));

  $form['breadcrumb'] = _wh_petitions_get_step_nav($current_step);
  $form['breadcrumb']['#type'] = 'container';
  $form['breadcrumb']['#attributes']['class'] = array('step-breadcrumbs');
  $form['breadcrumb']['next'] = _wh_petitions_get_create_nav_button(FALSE, '', 'top-submit-button');

  $term = taxonomy_term_load($form_state['storedvalues']['step_1']['petition_type']);

  $petition_type_markup = _wh_petitions_get_petition_type_text($term->name);
  $form['petition_type'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('petition-type')),
    '#prefix' => t('!html1This field is required!html2', array(
      '!html1' => '<span class="element-invisible screenreader-prompt" aria-hidden="false">',
      '!html2' => '</span>',
    )),
    '#children' => t('!html1@markup!html2', array(
      '!html1' => '<h1>',
      '@markup' => $petition_type_markup,
      '!html2' => '</h1>',
    )),
  );

  // Field can't be required or prev submit won't work. Check in validation.
  $form['petition_title'] = array(
    '#type' => 'textarea',
    '#title' => t('!html1 Add a title to your petition !html2 Title must be 120 characters or less !html3', array(
      '!html1' => '<p class="large">',
      '!html2' => '<span class="element-invisible screenreader-prompt" aria-hidden="false">',
      '!html3' => '</span></p>',
    )),
    '#required' => FALSE,
    '#resizable' => FALSE,
    '#default_value' => $default_petition_title,
    '#attributes' => array(
      'class' => array('create-petition-title'),
      'placeholder' => _wh_petitions_get_create_form_placeholder('petition_title'),
    ),
  );

  $form['bottom_navigation'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('petition-bottom-navigation')),
  );

  $form['bottom_navigation']['petition_title_help'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('petition-create-help', 'petition-title-help')),
    '#children' => t('Title must be 120 characters or less:') . ' <span class="character-count">120</span>',
  );

  $form['bottom_navigation']['navigation_left'] = _wh_petitions_get_create_nav_button(TRUE);
  $form['bottom_navigation']['navigation_left']['#type'] = 'container';
  $form['bottom_navigation']['navigation_left']['#attributes'] = array(
    'class' => array('petition-navigation-left'),
  );

  $form['bottom_navigation']['navigation_right'] = _wh_petitions_get_create_nav_button();
  $form['bottom_navigation']['navigation_right']['#type'] = 'container';
  $form['bottom_navigation']['navigation_right']['#attributes'] = array(
    'class' => array('petition-navigation-right'),
  );

  return $form;
}

/**
 * Petition Create Step 3 - Description.
 *
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function _wh_petitions_get_create_form_step_3($form_state) {
  $current_step = substr(__FUNCTION__, -1, 1);
  $form = array();

  $default_petition_description = check_plain(_wh_petitions_get_form_default_value($current_step, 'petition_description', $form_state));

  $form['breadcrumb'] = _wh_petitions_get_step_nav($current_step);
  $form['breadcrumb']['#type'] = 'container';
  $form['breadcrumb']['#attributes']['class'] = array('step-breadcrumbs');
  $form['breadcrumb']['next'] = _wh_petitions_get_create_nav_button(FALSE, '', 'top-submit-button');

  $petition_title = $form_state['storedvalues']['step_2']['petition_title'];

  $form['petition_title'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('petition-title')),
    '#prefix' => t('!html1This field is required!html2', array(
      '!html1' => '<span class="element-invisible screenreader-prompt" aria-hidden="false">',
      '!html2' => '</span>',
    )),
    '#children' => t('!html1@markup!html2', array(
      '!html1' => '<h1>',
      '@markup' => ucfirst(strtolower($petition_title)),
      '!html2' => '</h1>',
    )),
  );

  // Field can't be required or prev submit won't work. Check in validation.
  $form['petition_description'] = array(
    '#type' => 'textarea',
    '#title' => t('!html1 Add a description to your petition!html2Please limit your description to 800 characters!html3', array(
      '!html1' => '<p class="large">',
      '!html2' => '<span class="element-invisible screenreader-prompt" aria-hidden="false">',
      '!html3' => '</span></p>',
    )),
    '#required' => FALSE,
    '#resizable' => FALSE,
    '#default_value' => $default_petition_description,
    '#attributes' => array(
      'class' => array('create-petition-description'),
      'placeholder' => _wh_petitions_get_create_form_placeholder('petition_description'),
    ),
  );

  $form['bottom_navigation'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('petition-bottom-navigation')),
  );

  $form['bottom_navigation']['petition_description_help'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('petition-create-help', 'petition-description-help')),
    '#children' => t('Please limit your description to 800 characters.') . ' <span class="character-count">800</span>',
  );

  $form['bottom_navigation']['navigation_left'] = _wh_petitions_get_create_nav_button(TRUE);
  $form['bottom_navigation']['navigation_left']['#type'] = 'container';
  $form['bottom_navigation']['navigation_left']['#attributes'] = array(
    'class' => array('petition-navigation-left'),
  );

  $form['bottom_navigation']['navigation_right'] = _wh_petitions_get_create_nav_button();
  $form['bottom_navigation']['navigation_right']['#type'] = 'container';
  $form['bottom_navigation']['navigation_right']['#attributes'] = array(
    'class' => array('petition-navigation-right'),
  );

  return $form;
}

/**
 * Petition Create Step 4 - Tags.
 *
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function _wh_petitions_get_create_form_step_4($form_state) {
  $current_step = substr(__FUNCTION__, -1, 1);
  $form = array();

  // Get Issue Tags.
  $issues = array();
  $vocab = taxonomy_vocabulary_machine_name_load('issues');
  if (!empty($vocab)) {
    $issues_list = taxonomy_get_tree($vocab->vid);
    foreach ($issues_list as $issue) {
      $issues[$issue->tid] = check_plain($issue->name);
    }
  }

  $default_petition_issues = _wh_petitions_get_form_default_value($current_step, 'petition_issues', $form_state);

  $form['breadcrumb'] = _wh_petitions_get_step_nav($current_step);
  $form['breadcrumb']['#type'] = 'container';
  $form['breadcrumb']['#attributes']['class'] = array('step-breadcrumbs');
  $form['breadcrumb']['next'] = _wh_petitions_get_create_nav_button(FALSE, '', 'top-submit-button');

  $form['petition_issues_header'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('petition-issues-header')),
    '#prefix' => t('!html1Select at least 1 and no more than 3 tags from the list below.!html2', array(
      '!html1' => '<span class="element-invisible screenreader-prompt" aria-hidden="false">',
      '!html2' => '</span>',
    )),
    '#children' => t('!html1Add one to three tags to help categorize your petition:!html2', array(
      '!html1' => '<h1>',
      '!html2' => '</h1>',
    )),
  );

  $form['petition_issues'] = array(
    '#type' => 'checkboxes',
    '#title' => '',
    '#description' => '',
    '#required' => FALSE,
    '#options' => $issues,
    '#default_value' => ($default_petition_issues == '') ? array() : $default_petition_issues,
    '#attributes' => array('class' => array('create-petition-issue-tags')),
    '#theme' => array('multicolumn_options'),
    '#multicolumn_numcols' => '3',
  );

  $form['bottom_navigation'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('petition-bottom-navigation')),
  );

  $form['bottom_navigation']['petition_issues_help'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('petition-create-help', 'petition-issues-help')),
    '#children' => t('Please choose 1-3 tags.'),
  );

  $form['bottom_navigation']['navigation_left'] = _wh_petitions_get_create_nav_button(TRUE);
  $form['bottom_navigation']['navigation_left']['#type'] = 'container';
  $form['bottom_navigation']['navigation_left']['#attributes'] = array(
    'class' => array('petition-navigation-left'),
  );

  $form['bottom_navigation']['navigation_right'] = _wh_petitions_get_create_nav_button();
  $form['bottom_navigation']['navigation_right']['#type'] = 'container';
  $form['bottom_navigation']['navigation_right']['#attributes'] = array(
    'class' => array('petition-navigation-right'),
  );

  return $form;
}

/**
 * Petition Create Step 5 - Preview/Submit.
 *
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function _wh_petitions_get_create_form_step_5($form_state) {
  global $user;

  $current_step = substr(__FUNCTION__, -1, 1);
  $form = array();

  $petition_title = _wh_petitions_get_form_default_value('all', 'petition_title', $form_state);
  $petition_description = _wh_petitions_get_form_default_value('all', 'petition_description', $form_state);
  $petition_issues = _wh_petitions_get_form_default_value('all', 'petition_issues', $form_state);

  // Simulate node field items for theming.
  $issues = array(
    'container' => array(
      '#type' => 'container',
      '#attributes' => array('class' => array('field-items')),
    ),
  );

  if (!empty($petition_issues)) {
    foreach ($petition_issues as $tid => $value) {
      if ($value) {
        $term = taxonomy_term_load($tid);
        if (!empty($term)) {
          $issues['container'][$term->tid] = array(
            '#type' => 'container',
            '#attributes' => array('class' => array('field-item')),
            '#children' => '<h6>' . $term->name . '</h6>',
          );
        }
      }
    }
  }

  $issues_markup = drupal_render($issues);

  $form['breadcrumb'] = _wh_petitions_get_step_nav($current_step);
  $form['breadcrumb']['#type'] = 'container';
  $form['breadcrumb']['#attributes']['class'] = array('step-breadcrumbs');
  $form['breadcrumb']['next'] = _wh_petitions_get_create_nav_button(TRUE, "Publish", 'top-submit-button');

  $term = taxonomy_term_load($form_state['storedvalues']['step_1']['petition_type']);
  $petition_type_markup = _wh_petitions_get_petition_type_text($term->name);

  $date_str = date('F j, Y');
  $profile = profile2_load_by_user($user);
  $fields = array('first_name', 'last_name');
  $user_fields = array();
  foreach ($fields as $f) {
    if (!empty($profile['main']->{'field_' . $f}[LANGUAGE_NONE][0]['value'])) {
      $user_fields[$f] = $profile['main']->{'field_' . $f}[LANGUAGE_NONE][0]['value'];
    }
  }

  $first_initial = isset($user_fields['first_name']) ? strtoupper($user_fields['first_name'][0]) : '';
  $last_initial = isset($user_fields['last_name']) ? strtoupper($user_fields['last_name'][0]) : '';
  $created_by_text = sprintf('Created by %s.%s. on %s', $first_initial, $last_initial, $date_str);
  $form['preview_header'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('petition-preview-header')),
    '#children' => '<h6>' . $petition_type_markup . '</h6><h1>' . ucfirst(strtolower($petition_title)) . '</h1><h4>' . $created_by_text . '</h4>',
  );

  $form['preview_body'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('petition-preview-body')),
    '#children' => check_markup($petition_description, filter_default_format()),
  );

  $form['preview_issues'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('petition-preview-issues', 'field field-name-field-petition-issues')),
    '#children' => $issues_markup,
  );

  $form['bottom_navigation'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('petition-bottom-navigation')),
  );

  $form['bottom_navigation']['publish_help'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('petition-create-help', 'petition-publish-help')),
    '#children' => t('Once you publish, you will not be able to edit. Please review and consider sharing the text in an email to friends for feedback before you publish.'),
  );

  $form['bottom_navigation']['navigation_left'] = _wh_petitions_get_create_nav_button(TRUE, "Edit");
  $form['bottom_navigation']['navigation_left']['#type'] = 'container';
  $form['bottom_navigation']['navigation_left']['#attributes'] = array(
    'class' => array('petition-navigation-left'),
  );

  $form['bottom_navigation']['navigation_right'] = _wh_petitions_get_create_nav_button(FALSE, "Publish");
  $form['bottom_navigation']['navigation_right']['#type'] = 'container';
  $form['bottom_navigation']['navigation_right']['#attributes'] = array(
    'class' => array('petition-navigation-right'),
  );

  return $form;
}

/**
 * Petition Create Step 6 - Thank You.
 *
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function _wh_petitions_get_create_form_step_6($form_state) {
  $current_step = substr(__FUNCTION__, -1, 1);
  $form = array();

  $petition_item = PetitionsController::loadObject($form_state['petition_id']);

  // Call the share_links render array.
  $node = node_load($form_state['petition_id']);
  $share_links = theme('petitions_share_links', array('node' => $node));

  $hpw_block = block_load('petitions_blocks', 'how_petitions_work_block');
  $hpw_render_array = _block_get_renderable_array(_block_render_blocks(array($hpw_block)));
  $hpw_output = drupal_render($hpw_render_array);

  $popular_title = array(
    '#theme' => 'html_tag',
    '#tag' => 'h2',
    '#value' => t('Popular Petitions'),
    '#attributes' => array(
      'class' => array(
        'title',
      ),
    ),
  );
  $popular_output = views_embed_view('petitions_listing', 'popular_petitions');

  // Done here because view Custom Link doesn't allow anchors.
  $more_link = array(
    '#theme' => 'html_tag',
    '#tag' => 'div',
    '#value' => l(t('View more popular petitions'), '/', array('fragment' => 'signapetition')),
    '#attributes' => array(
      'class' => array(
        'more-link',
      ),
    ),
  );
  $popular_output .= drupal_render($more_link);

  $form['#attributes'] = array('id' => 'create-petition-form-thank-you');
  $form['thank-you'] = array(
    '#type' => 'markup',
    '#markup' => theme('wh_petitions_page_thank_you', array(
      'thank_you_text' => t('Thank you for creating the petition:'),
      'petition_url' => $petition_item->getNiceUrl(TRUE),
      'short_url' => $petition_item->getShortUrl(),
      'petition_title' => check_plain($petition_item->getTitle()),
      'share_links' => $share_links,
    )),
  );

  $form['how-petitions-work'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('how-petitions-work')),
    '#children' => $hpw_output,
  );

  $form['popular-petitions'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'popular-petitions',
        'block-popular-petitions',
        'block',
        'block-views',
      ),
    ),
    '#children' => drupal_render($popular_title) . $popular_output,
  );

  $form['#action'] = '/' . implode('/', arg());

  return $form;
}

/**
 * Get step navigation breadcrumb.
 *
 * @param int $current_step
 *   Current step of petition creation.
 *
 * @return array
 *   Form array of step navigation
 */
function _wh_petitions_get_step_nav($current_step) {
  $visited_class = 'visited-step';
  $active_class = 'active-step';
  $inactive_class = 'inactive-step';
  $form = array();

  $step_titles = array(
    1 => t('Choose Request'),
    2 => t('Add A Title'),
    3 => t('Add A Description'),
    4 => t('Add Tags'),
    5 => t('Preview'),
  );

  for ($step = 1; $step < _wh_petitions_get_num_create_form_steps(); $step++) {
    $classes = array('step-breadcrumb');

    if ($step < $current_step) {
      // For all steps less than current steps show visited class.
      $classes[] = $visited_class;
    }
    elseif ($step == $current_step) {
      // Show active class if on current step.
      $classes[] = $active_class;
    }
    elseif ($step > $current_step) {
      // Show inactive class for steps after current step.
      $classes[] = $inactive_class;
    }

    $form['step' . $step . '_breadcrumb'] = array(
      '#markup' => '<div class="' . implode(" ", $classes) . '"><h3>Step ' . $step . '<span> of ' . (_wh_petitions_get_num_create_form_steps() - 1) . '</span></h3><h6>' . check_plain($step_titles[$step]) . '</h6></div>',
    );
  }

  return $form;
}
/**
 * Get placeholder string for a field.
 *
 * @param string $field
 *   Field to get placeholder for.
 *
 * @return string
 *   Placeholder string.
 */
function _wh_petitions_get_create_form_placeholder($field) {
  $placeholder = '';
  switch ($field) {
    case 'petition_title':
      $placeholder = 'Keep your title brief and compelling, and be sure it clearly describes the goal of the petition';
      break;

    case 'petition_description':
      $placeholder = 'Explain why this petition is important and the people it impacts. Please note if you are petitioning on behalf of an organization.';
      break;
  }

  return $placeholder;
}

/**
 * Get petition type text for an issue.
 *
 * @param string $term_name
 *   Petition type term text.
 *
 * @return string
 *   Petition type text.
 */
function _wh_petitions_get_petition_type_text($term_name) {
  return 'We the People ask the federal government to ' . strtolower($term_name);
}

