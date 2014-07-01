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
