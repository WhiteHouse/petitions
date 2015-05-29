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
    'petitionevents_hide_petition_action' => array(
      'type' => 'node',
      'label' => t('Petitions Hide (Remove)'),
      'configurable' => TRUE,
      'vbo_configurable' => TRUE,
    ),
    'petitionevents_show_petition_action' => array(
      'type' => 'node',
      'label' => t('Petitions Restore'),
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
function petitionevents_hide_petition_action(&$node, $context) {
  try{
    petition_hide($node);
  }

  catch (Exception $e) {
    watchdog('petitionevents', $e->getMessage());

    drupal_set_message(t("An error occurred with removing petitions.  If
    this happens again please contact an administrator"), 'error');
    return;
  }

  if (!$context['skip_notification']) {
    $context['petition_title'] = $node->title;
    $context['petition_uid'] = $node->uid;
    petitionevents_send_hidden_email($context);
  }

  watchdog('petition_removal', 'Unpublishing petition: @title nid: @nid', array(
    '@title' => $node->title,
    '@nid' => $node->nid,
  ));
}

/**
 * Custom action to publish a petition.
 *
 * @param array $node
 *   Array representing a petition node.
 *
 * @param array $context
 *   Contextual information passed into the action.  Values set in a VBO form
 *   would be present here.
 */
function petitionevents_show_petition_action(&$node, $context) {
  try{
    petition_show($node);
  }

  catch (Exception $e) {
    watchdog('petitionevents', $e->getMessage());

    drupal_set_message(t("An error occurred with restoring petitions.  If
    this happens again please contact and administrator"), 'error');
    return;
  }

  watchdog('petition_show', 'Publishing petition: @title nid: @nid', array(
    '@title' => $node->title,
    '@nid' => $node->nid,
  ));

}

/**
 * Sends a takedown email to a petition owner.
 *
 * @param array $context
 *   Contextual information passed into the action.  Values set in a VBO form
 *   would be present here.
 */
function petitionevents_send_hidden_email($context) {
  // If an address was given, send a test email message.
  $user = user_load($context['petition_uid']);

  $petition_owner_email = $user->mail;
  $petition_owner_name = $user->name;
  $petition_owner_full_email = "$petition_owner_name <$petition_owner_email>";

  $from_email = check_plain($context['from_email']);
  $from_name = check_plain($context['from_name']);
  $from_full_email = "$from_name <$from_email>";

  global $language;
  $message_body = filter_xss_admin($context['body']);

  petitionevents_mail_additions($message_body, $context);

  $params['subject'] = check_plain($context['subject']);
  $params['body'] = $message_body;
  $params['bcc'] = check_plain($context['bcc']);
  $params['from'] = $from_full_email;
  $result = drupal_mail('petitionevents', 'send_hidden_email', $petition_owner_full_email, $language, $params);
}

/**
 * Implements hook_mail().
 */
function petitionevents_mail($key, &$message, $params) {

  if ($key == 'send_hidden_email') {
    $message['subject'] = $params['subject'];
    $message['body'][] = $params['body'];
    $message['bcc'] = $params['bcc'];
    $message['from'] = $params['from'];
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
function petitionevents_hide_petition_action_form($settings) {
  $form = array();
  $form['petitionevents_remove_email_skip_notification'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove without notification.'),
    '#required' => FALSE,
  );
  petitionevents_remove_email_settings_form($form);
  $form['remove_email']['#description'] = t("The following message will be sent to the owners of the petitions you've selected for removal.");

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
function petitionevents_hide_petition_action_validate($form, &$form_state) {
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
function petitionevents_hide_petition_action_submit($form, $form_state) {
  $action_context = array();
  $action_context['skip_notification'] = $form_state['values']['petitionevents_remove_email_skip_notification'];
  $action_context['from_name'] = $form_state['values']['petitionevents_remove_email_from_name'];
  $action_context['from_email'] = $form_state['values']['petitionevents_remove_email_from_address'];
  $action_context['subject'] = $form_state['values']['petitionevents_remove_email_subject'];
  $action_context['bcc'] = $form_state['values']['petitionevents_remove_email_bcc'];
  $action_context['body'] = $form_state['values']['petitionevents_remove_email_body'];
  return $action_context;
}
