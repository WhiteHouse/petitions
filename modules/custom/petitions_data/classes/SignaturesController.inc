<?php

/**
 * @file
 * Defines SignaturesController class.
 */

/**
 * Class SignaturesController
 *
 * Provides methods for interacting with Signature data objects.
 */
class SignaturesController {

  /**
   * Saves a signature.
   *
   * @param Signature $signature
   *   The signature to be saved. This should have the $petitionId and $uid
   *   properties defined.
   *
   * @return Signature
   *   Returns Signature object if successful.
   *
   * @throws InvalidArgumentException
   */
  public static function save($signature) {
    if (!$signature->getPetitionId()) {
      throw new InvalidArgumentException('The $this->petitionId and $this->uid or $this->user properties must be set before saving a signature.');
    }

    $signature_id = wh_petitions_mongo2mysql_create_new_signature($signature->getPetitionId(), $signature->getUser(), ip_address());
    $signature->setId($signature_id);

    return $signature;
  }

  /**
   * Loads a single signature.
   *
   * @param string $sid
   *   The signature id.
   *
   * @return Signature
   *   The loaded signature.
   */
  public static function load($sid) {
    return SignaturesController::loadMultiple(array($sid));
  }

  /**
   * Loads multiple signatures.
   *
   * @param array $sids
   *   An array signature ids.
   *
   * @return array
   *   An array of Signature objects.
   */
  public static function loadMultiple(array $sids) {
    $signatures_query = SignaturesSelectQueryFactory::create();
    $signatures = $signatures_query->setSignatureIds($sids)
      ->execute()->getResult();

    return $signatures;
  }

}
