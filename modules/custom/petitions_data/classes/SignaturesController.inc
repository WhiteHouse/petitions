<?php

/**
 * @file
 * Contains SignaturesController.
 */

/**
 * Class SignaturesController.
 *
 * Provides methods for interacting with SignatureItem objects.
 */
class SignaturesController {

  /**
   * Saves a signature.
   *
   * @param SignatureItem $signature
   *   The signature to be saved. This should have the $petitionId and $uid
   *   properties defined.
   *
   * @return SignatureItem|false
   *   Returns the given SignatureItem, augmented with any newly-given IDs, if
   *   successful or FALSE if not.
   *
   * @throws InvalidArgumentException
   *   Throws an exception in case of an incomplete signature given.
   */
  public static function save(SignatureItem $signature) {
    if (!$signature->getPetitionId()) {
      throw new InvalidArgumentException('Cannot save signature without a petition ID (either a legacy ID or an entity ID).');
    }

    if (!$signature->getUid() && !$signature->getUser()) {
      throw new InvalidArgumentException('Cannot save signature without a user (either a "user" or "uid" property).');
    }

    // Fail if the signature does not specify a valid petition.
    $petition = PetitionsController::load($signature->getPetitionId());
    if (empty($petition)) {
      return FALSE;
    }

    if (self::isDuplicate($signature)) {
      return FALSE;
    }

    // Add city, state, and country if missing.
    static::ensureLocationDetails($signature);

    // The Mongo save must come first because it adds a legacy ID which the
    // MySQL save will use if present.
    if (petitions_data_mongo_writes_are_enabled()) {
      $signature = self::saveToMongo($signature);
    }

    if (petitions_data_mysql_writes_are_enabled()) {
      $signature = self::saveToMysql($signature);
    }

    return $signature;
  }

  /**
   * Determines whether a given a signature is a duplicate.
   *
   * @param SignatureItem $signature
   *   A signature to test for duplicity.
   *
   * @return bool
   *   Returns TRUE if the given signature is a duplicate or FALSE if not.
   */
  protected static function isDuplicate(SignatureItem $signature) {
    $duplicates = SignaturesSelectQueryFactory::create()
      ->setPetitionId($signature->getPetitionId())
      ->setUid($signature->getUid())
      ->execute()
      ->getResult();
    return (bool) $duplicates;
  }

  /**
   * Adds location details to a given signature if missing.
   *
   * @param SignatureItem $signature
   *   The signature to be augmented.
   */
  protected static function ensureLocationDetails(SignatureItem &$signature) {
    if ($signature->getCity() && $signature->getState() && $signature->getUserCountry()) {
      // All location data is already present.
      return;
    }

    if (!$signature->getZip()) {
      // No additional location data can be retrieved without a Zip-code.
      return;
    }

    $locations = wh_zipcodelookup_get_location_details_from_zipcode($signature->getZip());
    if (!empty($locations[0])) {
      $signature
        ->setCity($locations[0]['city'])
        ->setState($locations[0]['state'])
        ->setUserCountry($locations[0]['country']);
    }

  }

  /**
   * Saves a signature to MongoDB.
   *
   * @param SignatureItem $signature
   *   The signature to be saved.
   *
   * @return SignatureItem
   *   The given signature, augmented with its newly-given legacy ID.
   */
  protected static function saveToMongo(SignatureItem $signature) {
    // Save the signature.
    $legacy_id = wh_petitions_mongo2mysql_create_new_signature($signature->getPetitionId(), $signature->getUser(), $signature->getIpAddress());

    // If successful, save the new legacy ID back into the SignatureItem.
    if ($legacy_id) {
      $signature->setLegacyId($legacy_id);
    }
    // Otherwise log failure.
    else {
      watchdog('petitions_data', 'Failed to save signature to MongoDB in SignaturesController::saveToMongo: !entity', array(
        '!entity' => petitionslog_format_for_watchdog($signature->toEntity()),
      ));
    }

    return $signature;
  }

  /**
   * Saves a signature to MySQL.
   *
   * @param SignatureItem $signature
   *   The signature to be saved.
   *
   * @return SignatureItem
   *   The given signature, augmented with its newly-given legacy ID.
   */
  protected static function saveToMysql(SignatureItem $signature) {
    // If mongo reads are enabled, we won't have the nid of the petition.
    // Try to get it and add to signature if it exists.
    if (petitions_data_mongo_reads_are_enabled()) {
      $legacy_petition_id = $signature->getLegacyPetitionId();
      if ($legacy_petition_id != '') {
        $petition_nid = petition_get_nid($legacy_petition_id);
        if ($petition_nid !== FALSE && $petition_nid != 0) {
          $signature->setPetitionId($petition_nid);
        }
      }
    }
    // Create and save the entity.
    $entity = $signature->toEntity();
    try {
      $success = $entity->save();
    }
    catch (Exception $e) {
      watchdog('petitions_data', 'Failed to save signature to MySQL in SignaturesController::saveToMySQL. Signature: !entity Exception: !exception', array(
        '!entity' => petitionslog_format_for_watchdog($entity),
        '!exception' => petitionslog_format_for_watchdog($e),
      ));
      return $signature;
    }

    // If successful, save the new entity ID back into the SignatureItem.
    if ($success) {
      $signature->setId($entity->id);
    }
    // Otherwise log failure.
    else {
      watchdog('petitions_data', 'Failed to save signature to MySQL in SignaturesController::saveToMySQL. Signature: !entity', array(
        '!entity' => petitionslog_format_for_watchdog($entity),
      ));
    }

    return $signature;
  }

  /**
   * Loads a single signature.
   *
   * @param string $sid
   *   The signature id.

   * @param bool $realtime
   *   Whether or not realtime accuracy is required. See
   *   SignatureSelectQueryFactory::create() for details. Defaults to TRUE.
   *
   * @return SignatureItem
   *   The loaded SignatureItem.
   */
  public static function load($sid, $realtime = TRUE) {
    return SignaturesController::loadMultiple(array($sid), $realtime);
  }

  /**
   * Loads multiple signatures.
   *
   * @param array $sids
   *   An array signature ids.

   * @param bool $realtime
   *   Whether or not realtime accuracy is required. See
   *   SignatureSelectQueryFactory::create() for details. Defaults to TRUE.
   *
   * @return array
   *   An array of SignatureItem objects.
   */
  public static function loadMultiple(array $sids, $realtime = TRUE) {
    $signatures_query = SignaturesSelectQueryFactory::create($realtime);
    $signatures = $signatures_query->setSignatureIds($sids)
      ->execute()->getResult();

    return $signatures;
  }

}
