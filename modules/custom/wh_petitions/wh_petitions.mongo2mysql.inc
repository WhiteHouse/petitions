<?php
/**
 * @file
 * Transitional support for functions with mongo dependencies, to be removed.
 */

/**
 * Check to see if signature exists already.
 *
 * @param string $petition_id
 *   The unique identifier of the petition.
 * @param int $uid
 *   A user ID.
 *
 * @return string|false
 *   Signature ID or FALSE if it doesn't exist yet.
 */
function wh_petitions_mongo2mysql_get_signature_id($petition_id, $uid) {
    $signature_id = FALSE;

    // Create connection.
    $sig_connection = wh_petitions_mongo_petition_signatures_connection();
    $sig_connection->setSlaveOkay(FALSE);

    // Determine if this user has already signed the petition.
    $query = array(
        'uid' => (int) $uid,
        'petition_id' => (string) $petition_id,
    );
    $result = $sig_connection->findOne($query);
    if (!empty($result)) {
        $signature_id = $result['_id']->__toString();
        // TODO make sure this ^^ works.
    }

    return $signature_id;
}

/**
 * @see wh_petitions_mongo2mysql_get_signature_id()
 */
function wh_petitions_get_signature_id($petition_id, $uid) {
  throw new Exception(t('wh_petitions_get_signature_id() is deprecated. Use wh_petitions_mongo2mysql_get_signature_id(). See ABOUT-7.x-3.x.md for more info.'));
}

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
 *
 * @return string|false
 *   The ID of the created signature record if one is created, or FALSE if not.
 *
 * @todo Replace usages of this function with method call to
 *   SignaturesQueryFactory::create->save().
 */
function wh_petitions_mongo2mysql_create_new_signature($petition_id, $user, $client_ip) {
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

    $signature_id = wh_petitions_create_signature($petition, $user, $sig_connection, $client_ip);
    if (!$signature_id) {
        watchdog('wh_petitions', 'Failed to create signature for @user on @petition', array('@user' => $user->uid, '@petition' => $petition['_id']));
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
