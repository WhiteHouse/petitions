<?php

/**
 * @file
 * Defines PetitionsController class.
 */

/**
 * Class PetitionsController.
 *
 * Provides methods for interacting with Peteition data objects.
 */
class PetitionsController {

  /**
   * Saves a petition.
   *
   * Upon save, this should update $petition->id, $petition->niceUrl, and
   * $petition->shortUrl.
   *
   * @param Petition $petition
   *   A petition object to save.
   *
   * @return Petition
   *   Returns the Petition with id property set if save is successful.
   */
  public static function save(Petition $petition) {

    $petition_form['uid'] = $petition->getUid();
    $petition_form['created'] = $petition->getCreated();
    $petition_form['title'] = $petition->getTitle();
    $petition_form['body'] = $petition->getBody();
    $petition_form['body_keywords'] = wh_petitions_generate_keywords($petition->getBody());
    $petition_form['title_keywords'] = wh_petitions_generate_keywords($petition->getTitle());
    $petition_form['petition_status'] = $petition->getStatus();
    $petition_form['response_status'] = $petition->getResponseStatus();
    $petition_form['user_tags'] = $petition->getUserTags();
    $petition_form['issues'] = $petition->getIssues();
    $petition_form['signature_count'] = $petition->getSignatureCount();
    $petition_form['public_signatures'] = $petition->getSignaturePublicThreshold();
    $petition_form['response_signatures'] = $petition->getSignatureThreshold();
    $petition_form['review_timeframe'] = $petition->getReviewTimeframe();
    $petition_form['reached_public'] = $petition->getReachedPublic();
    $petition_form['published'] = $petition->getPublished();

    // Save the petition to MongoDB.
    $conn = wh_petitions_mongo_petition_connection();
    $conn->setSlaveOkay(FALSE);
    $petition_id = wh_petitions_save_petition($conn, $petition_form);

    // Update petition with new MongoDB id.
    $petition_form['_id'] = new MongoId($petition_id);
    $petition->setId($petition_id);

    // Generate a Friendly & Short URL. These require MongoDB ID to be set.
    wh_petitions_generate_nice_url($petition_form);
    wh_petitions_generate_short_url($petition_form);
    $petition->setNiceUrl($petition_form['nice_url']);
    $petition->setShortUrl($petition_form['short_url']);

    // Re-save with updated values.
    wh_petitions_save_petition($conn, $petition_form);

    return $petition;
  }

  /**
   * Loads a single petition.
   *
   * @param string $pid
   *   A petition id.
   *
   * @return Petition
   *   A single petition object, or FALSE if none is found.
   */
  public static function load($pid) {
    return PetitionsController::loadMultiple(array($pid));
  }

  /**
   * Loads multiple petitions.
   *
   * @param array $pids
   *   An array of petition ids.
   *
   * @return array
   *   An array of Petition objects.
   */
  public static function loadMultiple(array $pids) {
    $petitions_query = PetitionsSelectQueryFactory::create();
    $petitions = $petitions_query->setPetitionIds($pids)
      ->execute()->getResult();

    return $petitions;
  }
}
