<?php

/**
 * @file
 * Contains PetitionItem.
 */

/**
 * Class PetitionItem.
 */
class PetitionItem {

  protected $entityId;
  protected $legacyId;
  protected $uid;
  protected $title;
  protected $abuseFlags;
  protected $bookmarked;
  protected $body;
  protected $issues;
  protected $legacyPath;
  protected $privateTags;
  protected $published;
  protected $signaturePublicThreshold;
  protected $signatureThreshold;
  protected $signatureCount;
  protected $reviewTimeframe;
  protected $reviewThresholdMailSent;
  protected $niceUrl;
  protected $shortUrl;
  protected $featured;
  protected $status;
  protected $reachedPublic;
  protected $reachedReady;
  protected $created;
  protected $closed;
  protected $response;
  protected $responseStatus = WH_PETITION_RESPONSE_STATUS_UNANSWERED;
  protected $userTags;

  /**
   * Sets abuse flags for the petition.
   *
   * @param int[] $uids
   *   An array of UIDs of users who have flagged the petition as abusive.
   *
   * @return $this
   */
  public function setAbuseFlags($uids) {
    $this->abuseFlags = $uids;

    return $this;
  }

  /**
   * Get abuse flags for the petition.
   *
   * @return int[]
   *   An array of UIDs of users who have flagged the petition as abusive.
   */
  public function getAbuseFlags() {
    return (array) $this->abuseFlags;
  }

  /**
   * Sets the petition body.
   *
   * @param string $body
   *   The petition body.
   *
   * @return $this
   */
  public function setBody($body) {
    $this->body = $body;

    return $this;
  }

  /**
   * Gets the petition body.
   *
   * @return string
   *   The petition body.
   */
  public function getBody() {
    return $this->body;
  }

  /**
   * Gets an array of keywords from the petition body.
   *
   * @return string[]
   *   An indexed array of keywords.
   */
  public function getBodyKeywords() {
    return wh_petitions_generate_keywords($this->getBody());
  }

  /**
   * Sets the users who have bookmarked the petition.
   *
   * @param int[] $uids
   *   An array of UIDs of users who have bookmarked the petition.
   *
   * @return $this
   */
  public function setBookmarked($uids) {
    $this->bookmarked = $uids;

    return $this;
  }

  /**
   * Gets the users who have bookmarked the petition.
   *
   * @return int[]
   *   An array of UIDs of users who have bookmarked the petition.
   */
  public function getBookmarked() {
    return (array) $this->bookmarked;
  }

  /**
   * Sets the time the petition was closed.
   *
   * @param int $timestamp
   *   The UNIX timestamp when this petition was closed.
   *
   * @return $this
   */
  public function setClosed($timestamp) {
    $this->closed = $timestamp;

    return $this;
  }

  /**
   * Gets the time the petition was closed.
   *
   * @return int
   *   The UNIX timestamp when this petition was closed.
   */
  public function getClosed() {
    return $this->closed;
  }

  /**
   * Sets the time the petition was created.
   *
   * See PetitionItem::getCreated() for a note on the "created" property.
   *
   * @param int $timestamp
   *   The UNIX timestamp when this petition was created.
   *
   * @return $this
   *
   * @see PetitionItem::getCreated()
   */
  public function setCreated($timestamp) {
    $this->created = $timestamp;

    return $this;
  }

  /**
   * Gets the time the petition was created.
   *
   * Note: PetitionItem::toRestResponseItemArray() does NOT use this method to
   * get the "created" property--it overwrites it with the "published" property
   * for reasons explained in that method. The value set with this method,
   * therefore, will never surface in parts of the application that rely on the
   * array transformation--only those that individually invoke this method
   * directly.
   *
   * @todo On account of the above note, this method should be deprecated or
   *   just become a wrapper around PetitionItem::getPublished() as soon as the
   *   implications of such a change are better understood.
   *
   * @return int
   *   The UNIX timestamp when this petition was created.
   *
   * @see PetitionItem::toRestResponseItemArray()
   */
  public function getCreated() {
    return (int) $this->created;
  }

  /**
   * Gets the deadline for accepting signatures for the petition.
   *
   * @return int
   *   The UNIX timestamp when this petition no longer accepts signatures.
   */
  public function getDeadline() {
    return $this->calcDeadline($this->getPublished(), $this->getReviewTimeframe());
  }

  /**
   * Sets the featured flag on the petition.
   *
   * @param int $flag
   *   The "featured" flag value for the petition--1 for featured or 0 for not.
   *
   * @return $this
   */
  public function setFeatured($flag) {
    $this->featured = (int) $flag;

    return $this;
  }

  /**
   * Gets the featured flag on the petition.
   *
   * @return int
   *   The "featured" flag value for the petition--1 for featured or 0 for not.
   */
  public function getFeatured() {
    return (int) $this->featured;
  }

  /**
   * Gets a unique identifier for the petition.
   *
   * Gets one of the unique identifiers for the petition--either the legacy
   * (Mongo) ID or the entity (node) ID--according to preference and
   * availability.
   *
   * @param bool $prefer_legacy_id
   *   TRUE to prefer the legacy (Mongo) ID or FALSE to prefer the entity (node)
   *   ID. Defaults to TRUE for backward compatibility.
   *
   * @return string|int|null
   *   Returns a legacy (Mongo) petition ID or an entity (node) ID, or NULL if
   *   no ID has been set at all.
   */
  public function getId($prefer_legacy_id = TRUE) {
    $preferred_id = ($prefer_legacy_id) ? $this->getLegacyId() : $this->getEntityId();
    $fallback_id = ($prefer_legacy_id) ? $this->getEntityId() : $this->getLegacyId();
    return ($preferred_id) ? $preferred_id : $fallback_id;
  }

  /**
   * Sets the legacy ID of the petition.
   *
   * @param string $legacy_id
   *   The legacy (MongoDB) ID.
   *
   * @return $this
   */
  public function setLegacyId($legacy_id) {
    if (!empty($legacy_id)) {
      $this->legacyId = $legacy_id;
    }

    return $this;
  }

  /**
   * Gets the legacy ID of the petition.
   *
   * @return string|null
   *   Returns a legacy (MongoDB) ID if set or NULL if not.
   */
  public function getLegacyId() {
    return $this->legacyId;
  }

  /**
   * Sets the legacy path of the petition.
   *
   * @param string $path
   *   The legacy (MongoDB) path, without a leading forward slash (/).
   *
   * @return $this
   */
  public function setLegacyPath($path) {
    $this->legacyPath = $path;

    return $this;
  }

  /**
   * Gets the legacy path of the petition.
   *
   * @return string|null
   *   The legacy (MongoDB) path, without a leading forward slash (/), if
   *   available, of NULL if not.
   */
  public function getLegacyPath() {
    return $this->legacyPath;
  }

  /**
   * Sets the entity (node) ID.
   *
   * @param int $id
   *   The entity ID.
   *
   * @return $this
   */
  public function setEntityId($id) {
    $this->entityId = $id;

    return $this;
  }

  /**
   * Gets the petition's entity ID.
   *
   * @return int
   *   Returns the petition's entity (node) ID.
   */
  public function getEntityId() {
    return $this->entityId;
  }

  /**
   * Sets the issues the petition is tagged with.
   *
   * @param array[] $issues
   *   An array of issue arrays, containing the following key/value pairs:
   *   - id: The issue id.
   *   - name: The issue name.
   *
   * @return $this
   */
  public function setIssues($issues) {
    $this->issues = $issues;

    return $this;
  }

  /**
   * Sets the issues the petition is tagged with.
   *
   * @return array[]
   *   An array of issue arrays, containing the following key/value pairs:
   *   - id: The issue id.
   *   - name: The issue name.
   */
  public function getIssues() {
    return $this->issues;
  }

  /**
   * Sets the time the petition was published.
   *
   * @param int $timestamp
   *   The UNIX timestamp when this petition was published.
   *
   * @return $this
   */
  public function setPublished($timestamp) {
    $this->published = $timestamp;

    return $this;
  }

  /**
   * Sets the private tags for the petition.
   *
   * @param string[] $tags
   *   An indexed array of tags for the petition, set by an administrator.
   *
   * @return $this
   */
  public function setPrivateTags($tags) {
    $this->privateTags = $tags;

    return $this;
  }

  /**
   * Gets the private tags for the petition.
   *
   * @return string[]
   *   An indexed array of tags for the petition, set by an administrator.
   */
  public function getPrivateTags() {
    return (array) $this->privateTags;
  }

  /**
   * Gets the published date for the petition.
   *
   * @return int
   *   The UNIX timestamp when this petition was published.
   */
  public function getPublished() {
    return $this->published;
  }

  /**
   * Sets the time the review threshold email for the petition was sent.
   *
   * @param int $timestamp
   *   The UNIX timestamp when the review threshold email for this petition was
   *   sent.
   *
   * @return $this
   */
  public function setReviewThresholdMailSent($timestamp) {
    $this->reviewThresholdMailSent = $timestamp;

    return $this;
  }

  /**
   * Gets the time the review threshold email for the petition was sent.
   *
   * @return int
   *   The UNIX timestamp when the review threshold email for this petition was
   *   sent.
   */
  public function getReviewThresholdMailSent() {
    return $this->reviewThresholdMailSent;
  }

  /**
   * Sets the review timeframe for the petition.
   *
   * @param int $days
   *   The length of time in days between when a petition reaches threshold
   *   and when a response must be posted (business rule).
   *
   * @return $this
   */
  public function setReviewTimeframe($days) {
    $this->reviewTimeframe = $days;

    return $this;
  }

  /**
   * Gets the review timeframe for the petition.
   *
   * @return int
   *   The petition review timeframe in days.
   */
  public function getReviewTimeframe() {
    return $this->reviewTimeframe;
  }

  /**
   * Sets the signature threshold for making the petition public.
   *
   * @param int $number
   *   The number of signatures required for this petition to become public.
   *
   * @return $this
   */
  public function setSignaturePublicThreshold($number) {
    $this->signaturePublicThreshold = $number;

    return $this;
  }

  /**
   * Gets the signature threshold for making the petition public.
   *
   * @return int
   *   The number of signatures required for this petition to become public.
   */
  public function getSignaturePublicThreshold() {
    return $this->signaturePublicThreshold;
  }

  /**
   * Sets the signature threshold for getting a response.
   *
   * @param int $number
   *   The number of signatures required for this petition to get a response.
   *
   * @return $this
   */
  public function setSignatureThreshold($number) {
    $this->signatureThreshold = $number;

    return $this;
  }

  /**
   * Gets the signature threshold for getting a response.
   *
   * @return int
   *   The number of signatures required for this petition to get a response.
   */
  public function getSignatureThreshold() {
    return $this->signatureThreshold;
  }

  /**
   * Sets the current signature count on the petition.
   *
   * @param int $number
   *   The number of signatures submitted to this petition.
   *
   * @return $this
   */
  public function setSignatureCount($number) {
    $this->signatureCount = $number;

    return $this;
  }

  /**
   * Gets the signature count on the petition.
   *
   * @return int
   *   The number of signatures submitted to this petition.
   */
  public function getSignatureCount() {
    return $this->signatureCount;
  }

  /**
   * Gets the number of signatures needed to reach the response threshold.
   *
   * @return int
   *   The number of signatures needed for this petition to get a response.
   */
  public function getSignaturesNeeded() {
    return $this->calcSignaturesNeeded($this->getSignatureThreshold(), $this->getSignatureCount());
  }

  /**
   * Sets the petition status.
   *
   * @param int $status
   *   An integer representing the petition status. See
   *   PetitionItem::formatStatus() for possible values.
   *
   * @return $this
   *
   * @see PetitionItem::formatStatus()
   */
  public function setStatus($status) {
    $this->status = $status;

    return $this;
  }

  /**
   * Gets the petition status.
   *
   * @return int
   *   An integer representing the petition status. See
   *   PetitionItem::formatStatus() for possible values.
   *
   * @see PetitionItem::formatStatus()
   */
  public function getStatus() {
    return (int) $this->status;
  }

  /**
   * Translates petition status codes into readable string formats.
   *
   * @param int $status_code
   *   Status code as stored.
   *
   * @return string
   *   String representing status code.
   */
  static public function formatStatus($status_code) {
    switch ($status_code) {
      // Published, but does not have the required number of signatures to
      // become public, only visible by direct URL.
      case WH_PETITION_STATUS_PRIVATE:
        // Has the required number of signatures to become public,
        // visible everywhere on site.
      case WH_PETITION_STATUS_PUBLIC:
        return "open";

      // Has received the required number of signatures, visible on site.
      case WH_PETITION_STATUS_UNDER_REVIEW:
        return "pending response";

      // Has received a response.
      case WH_PETITION_STATUS_REVIEWED:
        return "responded";

      // Has been removed by administrative action.
      case WH_PETITION_STATUS_FLAGGED:
        return "removed";

      default:
        return "closed";
    }
  }

  /**
   * Sets the petition title.
   *
   * @param string $title
   *   The petition title.
   *
   * @return $this
   */
  public function setTitle($title) {
    $this->title = $title;

    return $this;
  }

  /**
   * Gets the petition title.
   *
   * @return string
   *   The petition title.
   */
  public function getTitle() {
    return $this->title;
  }

  /**
   * Gets an array of keywords from the petition title.
   *
   * @return string[]
   *   An indexed array of keywords.
   */
  public function getTitleKeywords() {
    return wh_petitions_generate_keywords($this->getTitle());
  }

  /**
   * Sets the "nice URL" of the petition.
   *
   * The "nice URL" is a legacy concept from the Mongo implementation that
   * basically corresponds to a path alias in Drupal terms. It is the path
   * component of the petition URL, without the leading forward slash (/).
   *
   * @param string $path
   *   The petition's nice URL path.
   *
   * @return $this
   */
  public function setNiceUrl($path) {
    $this->niceUrl = $path;

    return $this;
  }

  /**
   * Gets the "nice URL" of the petition.
   *
   * See PetitionItem::setNiceUrl() for a description of the "nice URL" concept.
   *
   * @param bool $absolute
   *   Whether to return an absolute link (beginning with http:). Useful for
   *   links that will be displayed outside the site, such as in an API response
   *   or email. If FALSE, returns a path fragment without the leading forward
   *   slash (/). Defaults to TRUE.
   *
   * @return string
   *   The petition's URL if available, or an empty string if not.
   *
   * @see PetitionItem::setNiceUrl()
   */
  public function getNiceUrl($absolute = TRUE) {
    if (!$this->niceUrl) {
      return '';
    }

    return petitions_data_url($this->niceUrl, $absolute);
  }

  /**
   * Sets the short URL for the petition.
   *
   * @param string $url
   *   A full URL from a URL shortener service, pointing to the petition.
   *
   * @return $this
   */
  public function setShortUrl($url) {
    $this->shortUrl = $url;

    return $this;
  }

  /**
   * Sets the short URL for the petition.
   *
   * @return string
   *   A full URL from a URL shortener service, pointing to the petition.
   */
  public function getShortUrl() {
    return $this->shortUrl;
  }

  /**
   * Sets the time the petition reached public status.
   *
   * @param int $timestamp
   *   A UNIX timestamp representing when this petition gained public status.
   *
   * @return $this
   */
  public function setReachedPublic($timestamp) {
    $this->reachedPublic = $timestamp;

    return $this;
  }

  /**
   * Gets the time the petition reached public status.
   *
   * @return int
   *   A UNIX timestamp representing when this petition gained public status.
   */
  public function getReachedPublic() {
    return $this->reachedPublic;
  }

  /**
   * Sets the response details.
   *
   * @param array $response
   *   An array of response details as returned by
   *   PetitionsSelectQuery::formatReturnResponse(), with the follow key/value
   *   pairs:
   *   - id: The response node ID (nid).
   *   - url: The full URL of the response.
   *   - associationTime: The time the response was associated with the
   *     petition, in the form of a UNIX timestamp.
   *
   * @return $this
   *
   * @see PetitionsSelectQuery::formatReturnResponse()
   */
  public function setResponse($response) {
    $this->response = $response;

    return $this;
  }

  /**
   * Sets the time when the petition reached response ready status.
   *
   * @param int $timestamp
   *   A UNIX timestamp representing when this petition crossed the signature
   *   response threshold.
   *
   * @return $this
   */
  public function setReachedReady($timestamp) {
    $this->reachedReady = $timestamp;

    return $this;
  }

  /**
   * Gets the time when the petition reached response ready status.
   *
   * @return int
   *   A UNIX timestamp representing when this petition crossed the signature
   *   response threshold.
   */
  public function getReachedReady() {
    return $this->reachedReady;
  }

  /**
   * Gets the response details.
   *
   * @return array
   *   An array of response details as returned by
   *   PetitionsSelectQuery::formatReturnResponse(), with the follow key/value
   *   pairs:
   *   - id: The response node ID (nid).
   *   - url: The full URL of the response.
   *   - associationTime: The time the response was associated with the
   *     petition, in the form of a UNIX timestamp.
   *
   * @see PetitionsSelectQuery::formatReturnResponse()
   */
  public function getResponse() {
    return $this->response;
  }

  /**
   * Gets the response ID for the petition.
   *
   * @return int|null
   *   The node ID (nid) of the response to the petition, if available, or NULL
   *   if not.
   */
  public function getResponseId() {
    $response = $this->getResponse();

    $response_id = NULL;
    if (!empty($response['id'])) {
      $response_id = $response['id'];
    }

    return $response_id;
  }

  /**
   * Sets the petition's response status.
   *
   * @param int $response_status
   *   The petition's response status. One of these values:
   *   - WH_PETITION_RESPONSE_STATUS_UNANSWERED: Unanswered.
   *   - WH_PETITION_RESPONSE_STATUS_PENDING: Awaiting action.
   *   - WH_PETITION_RESPONSE_STATUS_ANSWERED: Responded.
   *
   * @return $this
   */
  public function setResponseStatus($response_status) {
    $this->responseStatus = $response_status;

    return $this;
  }

  /**
   * Gets the petition's response status.
   *
   * @return int
   *   The petition's response status. One of these values:
   *   - WH_PETITION_RESPONSE_STATUS_UNANSWERED: Unanswered.
   *   - WH_PETITION_RESPONSE_STATUS_PENDING: Awaiting action.
   *   - WH_PETITION_RESPONSE_STATUS_ANSWERED: Responded.
   */
  public function getResponseStatus() {
    return $this->responseStatus;
  }

  /**
   * Sets user UID of the petition creator.
   *
   * @param int $uid
   *   The user UID of the petition creator.
   *
   * @return $this
   */
  public function setUid($uid) {
    $this->uid = $uid;

    return $this;
  }

  /**
   * Sets user UID of the petition creator.
   *
   * @return int
   *   The user UID of the petition creator.
   */
  public function getUid() {
    return $this->uid;
  }

  /**
   * Sets the user tags on the petition.
   *
   * @param string[] $tags
   *   An array of tags for this petition, created by petition creator.
   *
   * @return $this
   */
  public function setUserTags($tags) {
    $this->userTags = $tags;

    return $this;
  }

  /**
   * Gets the user tags on the petition.
   *
   * @return string[]
   *   An array of tags for this petition.
   */
  public function getUserTags() {
    return (array) $this->userTags;
  }

  /**
   * Calculates the number of needed signatures.
   *
   * @param int $threshold
   *   Signatures required to solicit a response.
   * @param int $current
   *   Current number of signatures.
   *
   * @return int
   *   Signatures remaining to required to reach response threshold.
   */
  public static function calcSignaturesNeeded($threshold, $current) {
    return ($current >= $threshold) ? 0 : $threshold - $current;
  }

  /**
   * Calculate the deadline.
   *
   * @param int $published
   *   Epoch (UNIX style) time stamp.
   * @param int $days
   *   Number of days from publication that the Petition will remain open.
   *
   * @return int
   *   Epoch (UNIX style) time stamp.
   */
  public static function calcDeadline($published, $days) {
    return strtotime("+{$days} days", $published);
  }

  /**
   * Determine if petition is signable.
   *
   * Confirm petition has not passed deadline for accepting signatures. If it
   * hasn't, petitions considered signable will be of one of these statuses:
   *  - WH_PETITION_STATUS_PRIVATE: Is signable but only visible by direct link.
   *  - WH_PETITION_STATUS_PUBLIC: Is public and signable.
   *  - WH_PETITION_STATUS_UNDER_REVIEW: Has reached enough signatures for a
   *    response, but has not been responded to, so can get more signatures.
   *
   * @return bool
   *   TRUE if petition is signable, FALSE if not.
   *
   * @throws Exception
   *   Throws an exception if the petition status can't be determined.
   */
  public function isSignable() {
    if (!isset($this->status)) {
      throw new Exception("Can't determine if petition is signable if petition status is not set.");
    }

    // If the signature threshold has been passed and the petition has not
    // received a response.
    $signatures_passed_threshold = $this->getSignaturesNeeded() == 0;
    $pending_response = $this->getStatus() == WH_PETITION_STATUS_UNDER_REVIEW;
    if ($signatures_passed_threshold && $pending_response) {
      return TRUE;
    }

    // Has the petition passed its deadline for accepting signatures?
    if ($this->getDeadline() < time()) {
      return FALSE;
    }

    // A petition that has not passed its deadline is considered signable if its
    // status can be found in the array returned by
    // wh_petitions_signable_statuses().
    $array_signable_statuses = wh_petitions_signable_statuses();
    return (in_array($this->status, $array_signable_statuses)) ? TRUE : FALSE;
  }


  /**
   * Determine if petition is public by looking at its numeric status.
   *
   * Petitions considered public will be of one of these statuses:
   *  - WH_PETITION_STATUS_PUBLIC: By definition this is public.
   *  - WH_PETITION_STATUS_UNDER_REVIEW:  No action has been taken on this.
   *  - WH_PETITION_STATUS_REVIEWED: Has been reviewed and responded to.
   *  - WH_PETITION_STATUS_CLOSED: Is closed but should still be visible.
   *
   * @return bool
   *   TRUE if petition is public, FALSE if not.
   *
   * @throws Exception
   *   Throws an exception if the petition status can't be determined.
   */
  public function isPublic() {
    if (!isset($this->status)) {
      throw new Exception("Can't determine if petition is public if petition status is not set.");
    }
    // A petition is considered public if its status can be found in the array
    // returned by wh_petitions_public_statuses().
    return (in_array($this->status, wh_petitions_public_statuses())) ? TRUE : FALSE;
  }


  /**
   * Gets the petition in the form of a REST response item array.
   *
   * @return array
   *   An array as used by the REST API.
   */
  public function toRestResponseItemArray() {
    $output = array(
      'id' => $this->getId(),
      'type' => 'petition',
      'title' => petitions_data_sanitize_output($this->getTitle()),
      'body' => petitions_data_sanitize_output($this->getBody()),
      'issues' => $this->getIssues(),
      'signatureThreshold' => $this->getSignatureThreshold(),
      'signatureCount' => $this->getSignatureCount(),
      'signaturesNeeded' => $this->getSignaturesNeeded(),
      'url' => $this->getNiceUrl(),
      'deadline' => $this->getDeadline(),
      'status' => $this->formatStatus($this->getStatus()),
      'response' => $this->getResponse(),

      // The "created" timestamp was never meant to be publicly exposed.
      // Stakeholders intended to expose when a petition was PUBLISHED, not
      // when it was created. Simply return the published timestamp instead.
      // @todo An array transformation is not the right place to implement this
      //   kind of business logic. Find a better place for it.
      'created' => $this->getPublished(),

      'isSignable' => $this->isSignable(),
      'isPublic' => $this->isPublic(),
    );

    return $output;
  }

  /**
   * Gets the petition as an entity (node) object.
   *
   * It does not automatically save the result.
   *
   * @return object
   *   A node object created from the petition.
   */
  public function toEntity() {
    $abuse_flags = array();
    foreach ((array) $this->getAbuseFlags() as $uid) {
      $abuse_flags[]['target_id'] = $uid;
    }

    $issues = array();
    foreach ((array) $this->getIssues() as $issue) {
      $issues[] = array('tid' => $issue['id']);
    }

    $node_status = !in_array($this->getStatus(), wh_petitions_unpublished_statuses());

    $values = array(
      'type' => 'petition',
      'nid' => $this->getEntityId(),
      'uid' => $this->getUid(),
      'title' => $this->getTitle(),
      'body' => array(LANGUAGE_NONE => array(0 => array('value' => $this->getBody()))),
      'created' => $this->getCreated(),
      'status' => $node_status,
      'field_abuse_count' => array(LANGUAGE_NONE => array(0 => array('value' => count($abuse_flags)))),
      'field_abuse_flags' => array(LANGUAGE_NONE => $abuse_flags),
      'field_legacy_id' => array(LANGUAGE_NONE => array(0 => array('value' => $this->getLegacyId()))),
      'field_petition_issues' => array(LANGUAGE_NONE => $issues),
      'field_petition_public_signatures' => array(LANGUAGE_NONE => array(0 => array('value' => $this->getSignaturePublicThreshold()))),
      // 'field_petition_related_petitions' corresponds to an unused Mongo field
      // and is therefore unimplemented in MySQL.
      'field_petition_response_sign' => array(LANGUAGE_NONE => array(0 => array('value' => $this->getSignatureThreshold()))),
      'field_petition_review_timeframe' => array(LANGUAGE_NONE => array(0 => array('value' => $this->getReviewTimeframe()))),
      'field_petition_signature_count' => array(LANGUAGE_NONE => array(0 => array('value' => $this->getSignatureCount()))),
      'field_petition_status' => array(LANGUAGE_NONE => array(0 => array('value' => $this->getStatus()))),
      'field_response_status' => array(LANGUAGE_NONE => array(0 => array('value' => $this->getResponseStatus()))),
      'field_short_url' => array(LANGUAGE_NONE => array(0 => array('value' => $this->getShortUrl()))),
      'field_timestamp_published' => array(LANGUAGE_NONE => array(0 => array('value' => $this->getPublished()))),
      'field_timestamp_reached_public' => array(LANGUAGE_NONE => array(0 => array('value' => $this->getReachedPublic()))),
      'field_timestamp_reached_ready' => array(LANGUAGE_NONE => array(0 => array('value' => $this->getReachedReady()))),
      'field_timestamp_responded' => array(LANGUAGE_NONE => array(0 => array('value' => $this->getClosed()))),
      'field_legacy_path' => array(LANGUAGE_NONE => array(0 => array('value' => $this->getLegacyPath()))),
      'field_review_threshold_mail_sent' => array(LANGUAGE_NONE => array(0 => array('value' => $this->getReviewThresholdMailSent()))),
      // The following Mongo fields have no equivalent in MySQL:
      // - bookmarked
      // - deadline
      // - featured
      // - hidden
      // - nice_url has been replaced with core path aliasing.
      // - private_tags
      // - user_tags
    );

    if ($response = $this->getResponse()) {
      $response_id = $response['id'];
      $values['field_response_id'] = array(LANGUAGE_NONE => array(0 => array('target_id' => $response_id)));
    }

    // If it already has an entity ID, it's an UPDATE not an INSERT.
    if ($this->getEntityId()) {
      $values['is_new'] = FALSE;
      $nid = $this->getEntityId();


      // Ensure node has vid property set if it previously existed in a node.
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'node')
        ->propertyCondition('nid', $nid);
      $result = $query->execute();
      if ($result['node'][$nid]->vid) {
        $values['vid'] = $result['node'][$nid]->vid;
      }
    }
    else {
      $this->setCreated($created = time());
    }

    return entity_create('node', $values);
  }

  /**
   * Gets the petition as an array in the form expected by legacy Mongo code.
   *
   * @return array
   *   An array as formerly returned by wh_petitions_load_petition().
   *
   * @see wh_petitions_load_petition()
   *
   * @mongo2mysql
   */
  public function toLegacyArray() {
    $issues = array();
    foreach ((array) $this->getIssues() as $issue) {
      $issues[] = $issue['id'];
    }

    $values = array(
      'uid' => $this->getUid(),
      'title' => $this->getTitle(),
      'title_keywords' => $this->getTitleKeywords(),
      'body' => $this->getBody(),
      'body_keywords' => $this->getBodyKeywords(),
      'issues' => $issues,
      'user_tags' => $this->getUserTags(),
      'private_tags' => $this->getPrivateTags(),
      'petition_status' => $this->getStatus(),
      'response_id' => $this->getResponseId(),
      'response_status' => $this->getResponseStatus(),
      'published' => $this->getPublished(),
      'reached_public' => $this->getReachedPublic(),
      'reached_ready' => $this->getReachedReady(),
      'closed' => (int) $this->getClosed(),
      'signature_count' => $this->getSignatureCount(),
      'abuse_flags' => $this->getAbuseFlags(),
      'review_timeframe' => $this->getReviewTimeframe(),
      'response_signatures' => $this->getSignatureThreshold(),
      'public_signatures' => $this->getSignaturePublicThreshold(),
      'bookmarked' => $this->getBookmarked(),
      'featured' => $this->getFeatured(),
      // 'hidden' is unused.
      'nice_url' => $this->getNiceUrl(FALSE),
      'short_url' => $this->getShortUrl(),
      'created' => $this->getCreated(),
      'review_threshold_mail_sent' => $this->getReviewThresholdMailSent(),
      'abuse_count' => count($this->getAbuseFlags()),
    );

    if ($this->getLegacyId()) {
      $values['_id'] = new MongoId($this->getLegacyId());
    }

    // Merge with default values and return.
    return array_merge(wh_petitions_default_petition(), $values);
  }

  /**
   * Creates an object from an array in the form expected by legacy Mongo code.
   *
   * @param array $legacy_array
   *   An array as formerly returned by wh_petitions_load_petition().
   *
   * @return PetitionItem
   *   A PetitionItem created from the given array.
   *
   * @see wh_petitions_load_petition()
   *
   * @mongo2mysql
   */
  public static function fromLegacyArray($legacy_array) {
    // Merge with default petition to fill in any missing fields.
    $legacy_array = array_merge(wh_petitions_default_petition(), $legacy_array);

    $issue_tids = $legacy_array['issues'];
    $issues = taxonomy_term_load_multiple($issue_tids);
    $issues_array = array();
    if ($issues) {
      foreach ($issues as $tid => $term) {
        $issues_array[] = array(
          'id' => $tid,
          'name' => petitions_data_sanitize_output($term->name),
        );
      }
    }

    // Create the object. A few notes on omitted fields:
    // - A legacy array doesn't have an entity ID for setId().
    // - Body and title keywords are generated dynamically on output rather than
    //   being set on object creation.
    // - The "hidden" Mongo field was apparently never used (and therefore is
    //   not handled here).
    $petition = new self();
    if (!empty($legacy_array['_id'])) {
      $petition->setLegacyId($legacy_array['_id']->__toString());
    }
    $petition
      ->setAbuseFlags($legacy_array['abuse_flags'])
      ->setBody($legacy_array['body'])
      ->setBookmarked($legacy_array['bookmarked'])
      ->setClosed($legacy_array['closed'])
      ->setCreated($legacy_array['created'])
      ->setFeatured($legacy_array['featured'])
      ->setIssues($issues_array)

      // In a legacy array, the nice URL and legacy path are the same thing.
      ->setLegacyPath($legacy_array['nice_url'])
      ->setNiceUrl($legacy_array['nice_url'])

      ->setStatus($legacy_array['petition_status'])
      ->setSignaturePublicThreshold($legacy_array['public_signatures'])
      ->setPrivateTags($legacy_array['private_tags'])
      ->setPublished($legacy_array['published'])
      ->setReachedPublic($legacy_array['reached_public'])
      ->setSignatureThreshold($legacy_array['response_signatures'])
      ->setReachedReady($legacy_array['reached_ready'])
      ->setReviewTimeframe($legacy_array['review_timeframe'])
      ->setShortUrl($legacy_array['short_url'])
      ->setSignatureCount($legacy_array['signature_count'])
      ->setTitle($legacy_array['title'])
      ->setUid($legacy_array['uid'])
      ->setUserTags($legacy_array['user_tags']);
    if (!empty($legacy_array['response_id'])) {
      $petition->setResponse(PetitionsSelectQuery::formatReturnResponse($legacy_array['response_id']));
    }
    if (!empty($legacy_array['review_threshold_mail_sent'])) {
      $petition->getReviewThresholdMailSent($legacy_array['review_threshold_mail_sent']);
    }

    return $petition;
  }

}
