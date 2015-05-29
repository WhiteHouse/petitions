<?php

/**
 * @file
 * Contains forms to replicate the streamlined petition workflow form for an
 * individual petition from MySql VBO for Mongo based petitions.
 */


/**
 * Form with confirmation step.
 */
function wh_petitions_removal_form($form, &$form_state) {
  $query_parameters = drupal_get_query_parameters();
  if (!empty($query_parameters['petition_id'])) {
    $petition_id = $query_parameters['petition_id'];
  }

  // Load the petition.
  $conn = wh_petitions_mongo_petition_connection();
  $petition = wh_petitions_load_petition($conn, $petition_id);

  if (empty($petition)) {
    watchdog('petition_remove', t('Attempted to remove non-existent petition @petition_id.', array(
      '@petition_id' => $petition_id,
    )));
    return drupal_not_found();
  }
  $form_state['storage']['petition_id'] = $petition_id;
  $form_state['storage']['petition_title'] = $petition['title'];
  $form_state['storage']['petition_uid'] = $petition['uid'];;

  // Path to return to if "cancel" is pressed.
  $path = 'admin/petitions';

  // Initial form first.
  if (!isset($form_state['storage']['confirm'])) {
    $form = petitionevents_hide_petition_action_form(NULL);
    $form['#validate'][] = 'petitionevents_hide_petition_action_validate';
    $form['remove_email']['#description'] = t("The following message will be sent to the petitions you’ve selected to be hidden.");
    drupal_set_title('Petitions Moderation');

    // Add confirmation button to the form.
    $form['confirm'] = array(
      '#type' => 'submit',
      '#value' => 'Next',
      '#submit' => array('wh_petitions_form_submit'),
    );

    return $form;
  }
  else {
    // The form shown during the confirmation step:
    // Normally don't like this much HTML outside a template but this is a
    // temporary measure that will ensure a smooth transition to the mysql
    // VBO.
    $petition_title = $form_state['storage']['petition_title'];
    $confirmation_msg = "<div id=\"edit-description\" class=\"form-item form-type-item\">
        <div class=\"item-list\">
            <h3>You selected the following <strong>item</strong>:</h3>
            <ul>
                <li class=\"first last\">$petition_title</li>
            </ul>
        </div>
      </div>";

    $form['#title'] = 'Petitions Moderation';
    $form['#submit'][] = 'wh_petitions_form_submit';

    return confirm_form($form, "Petitions Moderation", $path, $confirmation_msg, $yes = "Confirm");
  }
}

/**
 * Form submit callback for form submission.
 */
function wh_petitions_form_submit($form, &$form_state) {

  if (empty($form_state['storage']['confirm'])) {

    // This will cause the form to be rebuilt, entering the confirm part of
    // the form.
    $form_state['storage']['confirm'] = TRUE;
    $form_state['rebuild'] = TRUE;
    $form_state['storage']['context']  = petitionevents_hide_petition_action_submit($form, $form_state);
  }
  else {
    $petition_id = $form_state['storage']['petition_id'];
    $petition_uid = $form_state['storage']['petition_uid'];

    $context = $form_state['storage']['context'];
    wh_petitions_hide_petition_action($petition_id, $petition_uid, $context);

    // This message is designed to mimic the current VBO message.
    drupal_set_message(t('Performed Remove petitions on 1 item.'));

    $path = "admin/petitions/$petition_id";
    drupal_goto($path);
  }
}

/**
 * Custom action to unpublish a petition.
 *
 * @param array $petition_id
 *   Mongo petition id.
 *
 * @param array $petition_uid
 *   uid of the petition's owner.
 *
 * @param array $context
 *   Contextual information passed into the action.  Values set in a VBO form
 *   would be present here.
 */
function wh_petitions_hide_petition_action($petition_id, $petition_uid, $context) {
  try{
    $petition = wh_petitions_mongo2mysql_hide($petition_id);
  }

  catch (Exception $e) {
    watchdog('petitionevents', $e->getMessage());

    drupal_set_message(t("An error occurred with removing petitions.  If
    this happens again please contact an administrator"), 'error');
    return;
  }

  $context['petition_title'] = $petition['title'];
  $context['petition_uid'] = $petition_uid;

  if (!$context['skip_notification']) {
    petitionevents_send_hidden_email($context);
  }

  watchdog("petition_removal", "Removing petition: @title mongo_petition_id: @nid", array('@title' => $petition['title'], '@nid' => $petition_id));
}
