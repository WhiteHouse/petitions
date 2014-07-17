<?php
/**
 * @file
 * Contains VBO actions related to petitions.
 */

/**
 * Implements hook_action_info().
 */
function petitionevents_action_info() {
  return array(
    'petitionevents_unpublish_petition_action' => array(
      'type' => 'node',
      'label' => t('Remove petitions'),
      'configurable' => TRUE,
      'vbo_configurable' => TRUE,
    ),
    'petitionevents_publish_petition_action' => array(
      'type' => 'node',
      'label' => t('Restore petitions'),
    ),
  );
}

/**
 * Custom action to unpublish a petition.
 *
 * @param array $node
 *   Array representing a petition node.
 *
 * @param array $context
 *   Contextual information passed into the action.  Values set in a VBO form
 *   would be present here.
 */
function petitionevents_unpublish_petition_action(&$node, $context) {
  try{
    petition_unpublish($node);
  }

  catch (Exception $e) {
    watchdog('petitionevents', $e->getMessage());

    drupal_set_message(t("An error occurred with removing petitions.  If
    this happens again please contact an administrator"), 'error');
    return;
  }

  if (!$context['skip_notification']) {
    petitionevents_send_remove_email($node->uid, $context);
  }

  else {
    $msg = strtr("Removing petition: @title nid: @nid", array('@title' => $node->title, '@nid' => $node->nid));
    watchdog("petition_removal", $msg);
  }
}

/**
 * Sends a removal email to a petition owner.
=======

/**
 * Custom action to publish a petition.
 *
 * @param array $node
 *  Array representing a petition node.
 *
 * @param array $context
 *  Contextual information passed into the action.  Values set in a VBO form
 *  would be present here.
 * */
function petitionevents_publish_petition_action(&$node, $context) {
  try{
    petition_publish($node);
  }

  catch (Exception $e) {
    watchdog('petitionevents', $e->getMessage());

    drupal_set_message(t("An error occurred with restoring petitions.  If
    this happens again please contact and administrator"), 'error');
    return;
  }

  watchdog("petition_publish", "Publishing petition: " . $node->title . " nid:" . $node->nid );
}

/*
 * Sends a takedown email to a petition owner.
 *
 * @param array $node
 *   Array representing a petition node.
 *
 * @param array $context
 *   Contextual information passed into the action.  Values set in a VBO form
 *   would be present here.
 */
function petitionevents_send_remove_email($uid, $context) {
  // If an address was given, send a test email message.
  $user = user_load($uid);

  $petition_owner_email = $user->mail;
  $petition_owner_name = $user->name;
  $petition_owner_full_email = "$petition_owner_name <$petition_owner_email>";

  global $language;
  $params['subject'] = check_plain($context['subject']);
  $params['body'] = filter_xss_admin($context['body']);
  $params['bcc'] = check_plain($context['bcc']);
  $result = drupal_mail('petitionevents', 'send_remove_email', $petition_owner_full_email, $language, $params);
}

/**
 * Implements hook_mail().
 */
function petitionevents_mail($key, &$message, $params) {

  if ($key == 'send_remove_email') {
    $message['subject'] = $params['subject'];
    $message['body'] = $params['body'];
    $message['bcc'] = $params['bcc'];
  }
}

/**
 * Implements mandrill_mail().
 */
function petitionevents_mandrill_mail_alter(&$mandrill_params, $message) {

  // This code enables BCC for petitionevents_send_remove_email().
  if ($message['key'] == 'send_remove_email') {
    $bcc_array = explode(',', $message['bcc']);

    foreach ($bcc_array as $bcc_email) {
      if (preg_match(MANDRILL_EMAIL_REGEX, $bcc_email, $matches)) {
        $mandrill_params['message']['to'][] = array(
          'email' => $matches[2],
          'name' => $matches[1],
          'type' => 'bcc',
        );
      }

      elseif (!empty($bcc_email)) {
        $mandrill_params['message']['to'][] = array(
          'email' => $bcc_email,
          'type' => 'bcc',
        );
      }
    }
  }
}

/**
 * Creates form for emailing removal notice to petition owner.
 *
 * @param array $settings
 *   Settings passed in from VBO action.
 *
 * @return array
 *   Form of the array.
 */
function petitionevents_unpublish_petition_action_form($settings) {
  $form = array();
  $form['petitionevents_remove_email_skip_notification'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove without notification.'),
    '#required' => FALSE,
  );
  petitionevents_remove_email_settings_form($form);

  $form['remove_email']['#collapsible'] = FALSE;
  $form['remove_email']['#states'] = array(
  // Hide the settings when the cancel notify checkbox is disabled.
    'invisible' => array(
      ':input[name="petitionevents_remove_email_skip_notification"]' => array('checked' => TRUE),
    ),
  );

  // In this case we don't want the form items to be required by default due
  // to custom validation.
  unset($form['remove_email']['petitionevents_remove_email_from_name']['#required']);
  unset($form['remove_email']['petitionevents_remove_email_from_address']['#required']);
  unset($form['remove_email']['petitionevents_remove_email_subject']['#required']);
  unset($form['remove_email']['petitionevents_remove_email_body']['#required']);
  return $form;
}

/**
 * Implements hook_action_validate().
 */
function petitionevents_unpublish_petition_action_validate($form, &$form_state) {
  $skip_notification = (boolean) $form_state['values']['petitionevents_remove_email_skip_notification'];
  if ($skip_notification) {
    return;
  }

  $name_set = !empty($form_state['values']['petitionevents_remove_email_from_name']);
  $email_set = !empty($form_state['values']['petitionevents_remove_email_from_address']);
  $subject_set = !empty($form_state['values']['petitionevents_remove_email_subject']);
  $message_set = !empty($form_state['values']['petitionevents_remove_email_body']);

  if (!$name_set) {
    form_set_error('petitionevents_remove_email_from_name', t('You must enter a name.'));
  }

  if (!$email_set) {
    form_set_error('petitionevents_remove_email_from_address', t('You must enter a email.'));
  }

  if (!$subject_set) {
    form_set_error('petitionevents_remove_email_subject', t('You must enter a subject.'));
  }

  if (!$message_set) {
    form_set_error('petitionevents_remove_email_body', t('You must enter a remove petition message.'));
  }
}

/**
 * Submit callback for actions form.
 */
function petitionevents_unpublish_petition_action_submit($form, $form_state) {
  $action_context = array();
  $action_context['skip_notification'] = $form_state['values']['petitionevents_remove_email_skip_notification'];
  $action_context['name'] = $form_state['values']['petitionevents_remove_email_from_name'];
  $action_context['email'] = $form_state['values']['petitionevents_remove_email_from_address'];
  $action_context['subject'] = $form_state['values']['petitionevents_remove_email_subject'];
  $action_context['bcc'] = $form_state['values']['petitionevents_remove_email_bcc'];
  $action_context['body'] = $form_state['values']['petitionevents_remove_email_body'];
  return $action_context;
}
