<?php

/**
 * @file
 * Transitional support for functions with mongo dependencies, to be removed.
 */

/**
 * Submit handler to handle petitions when responses are saved.
 *
 * -If the petition id in the field wasn't blank but is now, remove the petition from the response.
 * -If the petition id's are the same but the intro is different, save with the new intro
 * -If the petition id's are different, save one and remove the other.
 * -If the field was blank but now there's a petition, save a new petition-response relationship.
 */
function wh_response_mongo2mysql_petition_submit($form, &$form_state) {
  $pet_conn = wh_petitions_mongo_petition_connection();
  $res_conn = wh_response_mongo_response_connection();

  if (!empty($form_state['values']['nid'])) {
    $i = 0;
    $k = 0;
    $petition_nodes_to_reference = array();
    $petitions = $form_state['values']['petitions']['petition'];
    $set_values = array();
    foreach ($petitions as $petition_arr) {
      $petition = $petition_arr['title'];
      preg_match('/^(?:\s*|(.*) )?\[\s*(.+)\s*\]$/', $petition, $matches);
      preg_match('/^(?:\s*|(.*) )?\[\s*(.+)\s*\]$/', $form['petitions']['petition'][$i]['title']['#default_value'], $matches_pre);
      // Prev ID - Petition ID that was in the form element by default.
      $prev_id = '';
      if (!empty($matches_pre[2])) {
        $prev_id = $matches_pre[2];
      }
      // Cur ID - Petition ID that is in the form element now.
      $cur_id = '';
      if (!empty($matches[2])) {
        $cur_id = $matches[2];
      }
      $intro = $petitions[$i]['intro'];

      // Check for one that got removed.
      if (!empty($prev_id) && empty($petition)) {
        $obj = wh_response_load_petition_response($res_conn, $form_state['values']['nid'], $prev_id);
        wh_response_mongo2mysql_delete_petition_response($res_conn, $obj['_id']->__toString(), $prev_id);
      }

      // Check to see if this one already exists.
      elseif (!empty($cur_id) && !empty($prev_id) && $cur_id == $prev_id) {
        // If the intros don't match, save with new intro.
        if ($intro != $form['petitions']['petition'][$i]['intro']['#default_value']) {
          $obj = array();
          $obj = wh_response_load_petition_response($res_conn, $form_state['values']['nid'], $cur_id);
          if (!empty($obj)) {
            $obj['intro'] = $intro;
            wh_response_save_petition_response($res_conn, $obj);
          }
        }

        // Otherwise we're set, they match.
      }

      // One changed from one petition to another.
      elseif (!empty($cur_id) && !empty($prev_id) && $cur_id != $prev_id) {
        // Need to remove one.
        $obj = wh_response_load_petition_response($res_conn, $form_state['values']['nid'], $prev_id);
        wh_response_mongo2mysql_delete_petition_response($res_conn, $obj['_id']->__toString(), $prev_id);

        // Then add one.
        $prev_id = '';
      }

      // Add a new one.
      if (!empty($cur_id) && empty($prev_id)) {
        // Verify it's not already part of the node ( trying to attach twice ).
        $obj = array();
        $obj = wh_response_load_petition_response($res_conn, $form_state['values']['nid'], $cur_id);
        if (empty($obj)) {
          // Verify this petition can be attached to this node.  ( under_review or public, no response already attached ).
          $petition_obj = array();
          $petition_obj = wh_petitions_load_petition($pet_conn, $cur_id);
          if (empty($petition_obj['response_id']) && in_array($petition_obj['petition_status'], array(WH_PETITION_STATUS_PUBLIC, WH_PETITION_STATUS_UNDER_REVIEW))) {
            $obj = wh_response_default_petition_relationship();
            $obj['petition'] = $cur_id;
            $obj['intro'] = $intro;
            $obj['nid'] = $form_state['values']['nid'];

            // Set the statuses on the petition to answered and reviewed.
            if (wh_response_save_petition_response($res_conn, $obj)) {
              $petition_obj['response_status'] = WH_PETITION_RESPONSE_STATUS_ANSWERED;
              $petition_obj['petition_status'] = WH_PETITION_STATUS_REVIEWED;
              $petition_obj['response_id'] = $form_state['values']['nid'];
              wh_petitions_save_petition($pet_conn, $petition_obj);
            }
          }
        }
      }
      if (petitions_data_mysql_writes_are_enabled() && !empty($cur_id)) {
        $petition_nid = petition_get_nid($cur_id);
        if (!empty($petition_nid)) {
          // Petition nodes used to populate the entity reference field.
          $petition_nodes_to_reference[$k] = array('target_id' => $petition_nid);
          $k++;
        }
      }
      $i++;
    }
    if (petitions_data_mysql_writes_are_enabled() && !empty($petition_nodes_to_reference)) {
      // Populate the hidden entity reference field.
      $form['field_petition_id']['#parents'] = array('field_petition_id');
      form_set_value($form['field_petition_id'], array(LANGUAGE_NONE => $petition_nodes_to_reference), $form_state);
    }
  }
}

/**
 * Deletes a petition_response object.
 *
 * Also needs to set the petition response status back to pending
 * response and petition status back to under review.
 */
function wh_response_mongo2mysql_delete_petition_response(&$collection, $obj_id, $petition_id) {
  if (!empty($obj_id) && !empty($collection) && !empty($petition_id)) {
    $collection->remove(array('_id' => new MongoId($obj_id)));

    $conn = wh_petitions_mongo_petition_connection();
    $petition = wh_petitions_load_petition($conn, $petition_id);

    // Check if we should make this petition public, archived or under review based on signatures and timeframes.
    $timestamp = strtotime('- ' . $petition['review_timeframe'] . ' days');

    if ($petition['signature_count'] >= $petition['response_signatures']) {
      $petition['petition_status'] = WH_PETITION_STATUS_UNDER_REVIEW;
      $petition['response_status'] = WH_PETITION_RESPONSE_STATUS_PENDING;
    }
    elseif ($petition['signature_count'] < $petition['response_signatures']) {
      $petition['petition_status'] = WH_PETITION_STATUS_PUBLIC;
      $petition['response_status'] = WH_PETITION_RESPONSE_STATUS_UNANSWERED;
    }
    elseif ($petition['published'] < $timestamp) {
      $petition['petition_status'] = WH_PETITION_STATUS_CLOSED;
    }
    $petition['response_id'] = 0;
    wh_petitions_save_petition($conn, $petition);

    return TRUE;
  }

  return FALSE;
}
