<?php
/**
 * @file
 * Transitional support for functions with mongo dependencies, to be removed.
 */

/**
 * Creates a signature record.
 *
 * Wrapper around wh_petitions_create_signature(). This can be called by modules
 * who don't know anything about mongo.
 *
 * @param string $petition_id
 *   The unique identifier of the petition to create a signature for.
 * @param object $user
 *   A Drupal user object.
 * @param string $client_ip
 *   The IP address of the signatory.
 * @param int $timestamp
 *   Time when signature was signed. If FALSE, defaults to time().
 *
 * @return string|false
 *   The ID of the created signature record if one is created, or FALSE if not.
 */
function wh_petitions_mongo2mysql_create_new_signature($petition_id, $user, $client_ip, $timestamp = FALSE) {
    // Setup connections for signatures and petitions.
    $connection = wh_petitions_mongo_petition_connection();
    $connection->setSlaveOkay(FALSE);
    $sig_connection = wh_petitions_mongo_petition_signatures_connection();
    $sig_connection->setSlaveOkay(FALSE);

    $slim_petition_elements = array(
        'petition_status',
        'signature_count',
        'response_signatures',
        'public_signatures',
    );

    // Load the petition.
    $petition = wh_petitions_load_slim_petition($connection, $petition_id, $slim_petition_elements);

    // If it wasn't a valid petition id, exit.
    if (empty($petition)) {
        return FALSE;
    }

    $signature_id = wh_petitions_create_signature($petition, $user, $sig_connection, $client_ip, '', $timestamp);
    if (!$signature_id) {
        return FALSE;
    };

    return $signature_id;
}

/**
 * @see wh_petitions_mongo2mysql_create_new_signature()
 */
function wh_petitions_create_new_signature($petition_id, $user, $client_ip) {
  throw new Exception(t('wh_petitions_create_new_signature() is deprecated. Use wh_petitions_mongo2mysql_create_new_signature().'));
}

/**
 * Wrapper around wh_petitions_increment_signature_count().
 *
 * This function takes care of creating a mongo connection and looking up the
 * properly formatted $petition object.
 *
 * @param string $petition_id
 *   The unique identifier of the petition.
 */
function wh_petitions_mongo2mysql_increment_count($petition_id) {
    // Create connection.
    $connection = wh_petitions_mongo_petition_connection();
    $connection->setSlaveOkay(FALSE);

    $slim_petition_elements = array(
        'petition_status',
        'signature_count',
        'response_signatures',
        'public_signatures',
    );

    // Load the petition.
    $petition = wh_petitions_load_slim_petition($connection, $petition_id, $slim_petition_elements);

    // Increment signature count.
    wh_petitions_increment_signature_count($petition, $connection);
}

/**
 * @see wh_petitions_mongo2mysql_increment_count()
 */
function wh_petitions_increment_count($petition_id) {
  throw new Exception(t('wh_petitions_increment_count() is deprecated. Use wh_petitions_mongo2mysql_increment_count().'));
}

/**
 * List un-notified compliant petitions.
 *
 * Pulls a list of petitions with more signatures than the compliance review
 * threshold and which have not been sent compliance review notifications. 
 */
function wh_petitions_mongo2mysql_compliance_review_threshold_check($threshold, $compliance_reviewer_emails) {
  $petition_connection = wh_petitions_mongo_petition_connection();

  $where = array(
    // Have to cast it because otherwise mongo treats it as string.
    'signature_count' => array('$gte' => (int) $threshold),
    '$or' => array(
      array('review_threshold_mail_sent' => 0),
      array('review_threshold_mail_sent' => array('$exists' => FALSE)),
    ),
    'petition_status' => array(
      '$in' => array(
        WH_PETITION_STATUS_DRAFT,
        WH_PETITION_STATUS_PRIVATE,
        WH_PETITION_STATUS_PUBLIC,
      ),
    ),
  );
  $fields = array(
    '_id',
    'signature_count',
    'review_threshold_mail_sent',
  );
  $notification_list = $petition_connection->find($where, $fields);
  foreach ($notification_list as $notification) {
    $petition = wh_petitions_load_slim_petition($petition_connection, $notification['_id'], array('body'));

    $message = drupal_mail('wh_petitions', 'review_threshold_passed', $compliance_reviewer_emails, language_default(),
      array(
        'petition' => $petition,
      )
    );

    // Set the var on the petition record so we know the mail has been sent.
    $review_threshold_mail_sent = time();
    $petition_connection->update(
      array('_id' => $petition['_id']),
      array('$set' => array('review_threshold_mail_sent' => $review_threshold_mail_sent))
    );
    // Also update the corresponding petition node.
    if (petitions_data_mysql_writes_are_enabled()) {
      $legacy_id = $petition['_id']->__toString();
      $nid = petition_get_nid($legacy_id);
      if (!empty($nid)) {
        $petition_node = node_load($nid);
        $petition_node->field_review_threshold_mail_sent[$petition->language][0]['value'] = $review_threshold_mail_sent;
        node_save($petition_node);
      }
    }

    watchdog('wh_petitions', 'Petition (@id) Review Threshold Notification Sent.', array('@id' => $notification['_id']));

  }
}
