<?php

/*
 * @file
 * Contains forms to replicate the streamlined petition workflow form for an
 * individual petition from MySql VBO for Mongo based petitions.
 */


/**
 *  Form with confirmation step
 */
function wh_petitions_removal_form($form, &$form_state) {
  $query_parameters = drupal_get_query_parameters();
  if (!empty($query_parameters['petition_id'])) {
    $petition_id = $query_parameters['petition_id'];
  }


  // Load the petition
  $conn = wh_petitions_mongo_petition_connection();
  $petition = wh_petitions_load_petition($conn, $petition_id);

  $form_state['storage']['petition_id'] = $petition_id;
  $form_state['storage']['petition_title'] = $petition['title'];
  $form_state['storage']['petition_uid'] = $petition['uid'];;


  $path = 'admin/petitions'; // path to return to if "cancel" is pressed

  // create a form with a "confirm" step.
  // Initial form first:
  if(!isset( $form_state['storage']['confirm'])) {
    $form = petitionevents_unpublish_petition_action_form(NULL);
    $form['#validate'][] = 'petitionevents_unpublish_petition_action_validate';
    $form['remove_email']['#description'] = t("The following message will be sent to the petitions you’ve selected for removal.");
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

    return confirm_form($form, $question="Petitions Moderation", $path , $confirmation_msg, $yes = "Confirm");
  }


  return $form;
} // end of function
// Our submit function
// (Name of the submit function has to be the name of above function + "_submit"!)

function wh_petitions_form_submit($form, &$form_state) {

  if(empty($form_state['storage']['confirm'] )) {
    // NOTE: The variable is no longer in $form_state['values']['your-field'] by the time I'm printing out the final confirmation message. I'm
    // not sure session variables are the best solution, but this worked for me.
    $form_state['storage']['confirm'] = TRUE; // this will cause the form to be rebuilt, entering the confirm part of the form
    $form_state['rebuild'] = TRUE;
    $form_state['storage']['context']  = petitionevents_unpublish_petition_action_submit($form, $form_state);
  }
  else {
    $petition_id = $form_state['storage']['petition_id'];
    $petition_uid = $form_state['storage']['petition_uid'];

    $context = $form_state['storage']['context'];
    wh_petitions_unpublish_petition_action($petition_id, $petition_uid, $context);

    // This message is designed to mimic the current VBO message.
    drupal_set_message('Performed Remove petitions on 1 item.');

    $path = "admin/petitions/$petition_id";
    drupal_goto($path);
  }
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
function wh_petitions_unpublish_petition_action($petition_id, $petition_uid, $context) {
  try{
    $petition = wh_petitions_mongo2mysql_unpublish($petition_id);
  }

  catch (Exception $e) {
    watchdog('petitionevents', $e->getMessage());

    drupal_set_message(t("An error occurred with removing petitions.  If
    this happens again please contact an administrator"), 'error');
    return;
  }

  if (!$context['skip_notification']) {
    petitionevents_send_remove_email($petition_uid, $context);
  }

  else {
    $msg = strtr("Removing petition: @title nid: @nid", array('@title' => $petition['title'], '@nid' => $petition_id));
    watchdog("petition_removal", $msg);
  }
}

