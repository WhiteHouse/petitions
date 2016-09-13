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
