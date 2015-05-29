<?php

/**
 * @file
 * Contains PetitionsController.
 */

/**
 * Class PetitionsController.
 *
 * Provides methods for interacting with Petition data objects.
 */
class PetitionsController {

  /**
   * Increments the signature count for a given petition.
   *
   * @param string|int $petition_id
   *   A petition ID.
   */
  public static function incrementSignatureCount($petition_id) {
    // Increment in MongoDB.
    if (petitions_data_mongo_writes_are_enabled()) {
      wh_petitions_mongo2mysql_increment_count($petition_id);
    }
    // Increment in MySQL.
    if (petitions_data_mysql_writes_are_enabled()) {
      petition_increment_count($petition_id, 1);
    }
  }

  /**
   * Deletes a petition.
   *
   * @param int|string $petition_id
   *   The unique identifier for the petition to delete.
   *
   * @throws PetitionNotFoundException
   *   Throws an exception if the given petition ID does not exist.
   */
  public static function delete($petition_id) {
    /** @var PetitionItem $petition */
    $petition = PetitionsController::loadObject($petition_id);

    if (!$petition) {
      throw new PetitionNotFoundException();
    }

    if (petitions_data_mongo_writes_are_enabled() && $petition->getLegacyId()) {
      try {
        $conn = wh_petitions_mongo_petition_connection();
        $conn->setSlaveOkay(FALSE);
        wh_petitions_delete_petition($conn, $petition->getLegacyId());
      }
      catch (Exception $e) {
        watchdog('petitions_data', 'Failed to delete petition @petition_id from MongoDB in PetitionsController::delete().', array(
          '@petition_id' => $petition_id,
        ), WATCHDOG_WARNING);
      }
    }

    if (petitions_data_mysql_writes_are_enabled()) {
      // If MySQL writes and Mongo reads are simultaneously enabled (phase 2),
      // the above loadObject() call was issued against Mongo and therefore had
      // no access to the entity ID. Manually re-issue against MySQL.
      if (petitions_data_mongo_reads_are_enabled()) {
        $petitions_query = new PetitionsSelectQueryMysql();
        $petitions = $petitions_query
          ->setPetitionId($petition_id)
          ->execute()
          ->getResultObjects();
        $petition = $petitions[0];
      }

      if ($petition->getEntityId()) {
        node_delete($petition->getEntityId());
      }
    }
  }

  /**
   * Saves a petition.
   *
   * Saves a petition to the active database backend(s) and updates the entity
   * ID, legacy ID, nice URL, and short URL properties as appropriate.
   *
   * @param PetitionItem $petition
   *   A petition object to save.
   */
  public static function save(PetitionItem $petition) {
    // The Mongo save must come first because it adds a legacy ID which the
    // MySQL save will use if present.
    if (petitions_data_mongo_writes_are_enabled()) {
      self::saveToMongo($petition);
    }

    if (petitions_data_mysql_writes_are_enabled()) {
      self::saveToMySql($petition);
    }
  }

  /**
   * Saves a petition to MongoDB.
   *
   * @param PetitionItem $petition
   *   The petition to save.
   */
  protected static function saveToMongo(PetitionItem $petition) {
    $petition_form = $petition->toLegacyArray();

    // Save the petition to MongoDB.
    $conn = wh_petitions_mongo_petition_connection();
    $conn->setSlaveOkay(FALSE);
    $petition_id = wh_petitions_save_petition($conn, $petition_form);

    // Update petition with new MongoDB id.
    $petition_form['_id'] = new MongoId($petition_id);
    $petition->setLegacyId($petition_id);

    // Generate a Friendly & Short URL. These require MongoDB ID to be set.
    wh_petitions_generate_nice_url($petition_form);
    $petition->setNiceUrl($petition_form['nice_url']);

    if ($petition->getStatus() != WH_PETITION_STATUS_DRAFT) {
      wh_petitions_generate_short_url($petition_form);
      $petition->setShortUrl($petition_form['short_url']);
    }

    $petition->setLegacyPath($petition->getNiceUrl());

    // Re-save with updated values.
    wh_petitions_save_petition($conn, $petition_form);
  }

  /**
   * Saves a petition to MySQL.
   *
   * @param PetitionItem $petition
   *   The petition to save.
   */
  protected static function saveToMysql(PetitionItem $petition) {
    $node = $petition->toEntity();
    node_save($node);
    $petition->setEntityId($node->nid);

    $nice_url = url("node/{$node->nid}");
    $petition->setNiceUrl($nice_url);

    if ($petition->getStatus() != WH_PETITION_STATUS_DRAFT) {
      $short_url = wh_petition_tool_shortenurl($petition->getNiceUrl(FALSE));
      $petition->setShortUrl($short_url);

      $node = $petition->toEntity();
      node_save($node);
    }
  }

  /**
   * Loads a single petition.
   *
   * @param string $pid
   *   A petition id.
   * @param bool $realtime
   *   Whether or not realtime accuracy is required. See
   *   PetitionsSelectQueryFactory::create() for details. Defaults to TRUE.
   *
   * @return array
   *   An array of petition data, or an empty array if no matches are found.
   */
  public static function load($pid, $realtime = TRUE) {
    $petitions = PetitionsController::loadMultiple(array($pid), $realtime);

    if (empty($petitions)) {
      return array();
    }

    return $petitions[0];
  }

  /**
   * Loads multiple petitions.
   *
   * @param array $pids
   *   An array of petition ids.
   * @param bool $realtime
   *   Whether or not realtime accuracy is required. See
   *   PetitionsSelectQueryFactory::create() for details. Defaults to TRUE.
   *
   * @return array
   *   An array of petition data arrays.
   */
  public static function loadMultiple(array $pids, $realtime = TRUE) {
    $petitions_query = PetitionsSelectQueryFactory::create($realtime);
    $petitions = $petitions_query->setPetitionIds($pids)
      ->execute()->getResult();

    return $petitions;
  }

  /**
   * Loads a single petition as an object.
   *
   * @param string $pid
   *   A petition ID.
   * @param bool $realtime
   *   Whether or not realtime accuracy is required. See
   *   PetitionsSelectQueryFactory::create() for details. Defaults to TRUE.
   *
   * @return PetitionItem|false
   *   A PetitionItem object if found or FALSE if not.
   */
  public static function loadObject($pid, $realtime = TRUE) {
    $petitions = PetitionsController::loadObjectMultiple(array($pid), $realtime);

    if (empty($petitions)) {
      return FALSE;
    }

    return $petitions[0];
  }

  /**
   * Loads multiple petitions as objects.
   *
   * @param array $pids
   *   An array of petition IDs.
   * @param bool $realtime
   *   Whether or not realtime accuracy is required. See
   *   PetitionsSelectQueryFactory::create() for details. Defaults to TRUE.
   *
   * @return PetitionItem[]
   *   An array of PetitionItem objects.
   */
  public static function loadObjectMultiple(array $pids, $realtime = TRUE) {
    $petitions_query = PetitionsSelectQueryFactory::create($realtime);
    $petitions = $petitions_query->setPetitionIds($pids)
      ->execute()->getResultObjects();

    return $petitions;
  }

}
