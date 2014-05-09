<?php

/**
 * @file
 * Defines PetitionsSelectQuery class.
 */

/**
 * Class Petitions.
 *
 * This should contain methods for querying petitions that are NOT specific to
 * a backend, e.g., MongoDB.
 */
abstract class PetitionsSelectQuery extends SelectQueryBase {

  protected $title;
  protected $body;
  protected $issueID;
  protected $issue;
  protected $petitionIds;
  protected $baseURL;
  protected $responseID;
  protected $createdDate;
  protected $startDate;
  protected $endDate;
  protected $signatureThresholdCeiling;
  protected $signatureThreshold;
  protected $signatureThresholdFloor;
  protected $signatureCountCeiling;
  protected $signatureCount;
  protected $signatureCountFloor;
  protected $minSignatureCount;
  protected $signaturesNeededCeiling;
  protected $signaturesNeeded;
  protected $signaturesNeededFloor;
  protected $url;
  protected $deadlineBefore;
  protected $deadline;
  protected $deadlineAfter;
  protected $status;
  protected $isSignable;
  protected $isPublic;

  /**
   * Constructor. This must be explicitly called by subclasses.
   */
  public function __construct() {
    parent::__construct();
  }

  /**
   * Set accessor for $this->url.
   *
   * @param string $url
   *   The petition URL used to filter results.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setURL($url) {
    $this->url = $url;
    return $this;
  }

  /**
   * Set accessor for $this->createdAt.
   *
   * @param int $date
   *   Epoch (UNIX style) time stamp.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setCreatedAt($date) {
    if (is_numeric($date)) {
      $this->createdDate = $date;
    }
    return $this;
  }

  /**
   * Get accessor for $this->createdDate.
   *
   * @return int
   *   Epoch (UNIX style) time stamp.
   */
  protected function getCreatedDate() {
    return intval($this->createdDate);
  }

  /**
   * Get accessor for $this->startDate.
   *
   * @return int
   *   Epoch (UNIX style) time stamp.
   */
  protected function getStartDate() {
    return intval($this->startDate);
  }

  /**
   * Set accessor for $this->startDate.
   *
   * @param int $date
   *   Epoch (UNIX style) time stamp.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setStartDate($date) {
    if (is_numeric($date)) {
      $this->startDate = $date;
    }
    return $this;
  }

  /**
   * Get accessor for $this->endDate.
   *
   * @return int
   *   Epoch (UNIX style) time stamp.
   */
  protected function getEndDate() {
    return intval($this->endDate);
  }

  /**
   * Set accessor for $this->endDate.
   *
   * @param int $date
   *   Epoch (UNIX style) time stamp.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setEndDate($date) {
    if (is_numeric($date)) {
      $this->endDate = $date;
    }
    return $this;
  }

  /**
   * Get accessor for $this->title.
   *
   * @return string
   *   The petition title used to filter results.
   */
  protected function getTitle() {
    return $this->title;
  }

  /**
   * Set accessor for $this->title.
   *
   * @param string $title
   *   The petition title used to filter results.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setTitle($title) {
    $this->title = $title;
    return $this;
  }

  /**
   * Get accessor for $this->body.
   *
   * @return string
   *   The petition body used to filter results.
   */
  protected function getBody() {
    return $this->body;
  }

  /**
   * Set accessor for $this->body.
   *
   * @param string $body
   *   The petition body used to filter results.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setBody($body) {
    $this->body = $body;
    return $this;
  }

  /**
   * Get accessor for $this->isSignable.
   *
   * @return string
   *   The petition isSignable used to filter results.
   */
  protected function getIsSignable() {
    return $this->isSignable;
  }

  /**
   * Set accessor for $this->isSignable.
   *
   * @param string $is_signable
   *   The signable status of the petition.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setIsSignable($is_signable) {
    // Case race: First to evaluate TRUE wins.
    switch (TRUE) {
      // These are all the possible true cases.
      case intval($is_signable) == 1:
      case strtolower($is_signable) == 'true':
      case $is_signable === TRUE:
        $this->isSignable = TRUE;
        break;

      // These are all the possible false cases.
      case $is_signable === '0':
      case $is_signable === 0:
      case strtolower($is_signable) == 'false':
      case $is_signable === FALSE:
        $this->isSignable = FALSE;
        break;

      case $is_signable === NULL:
      default:
        break;
    }

    return $this;
  }

  /**
   * Get accessor for $this->isPublic.
   *
   * @return string
   *   The petition isPublic used to filter results.
   */
  protected function getIsPublic() {
    return $this->isPublic;
  }

  /**
   * Set accessor for $this->isPublic.
   *
   * @param string $is_public
   *   The public property of the petition.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setIsPublic($is_public) {
    // Case race: First to evaluate TRUE wins.
    switch (TRUE) {
      // These are all the possible true cases.
      case intval($is_public) == 1:
      case strtolower($is_public) == 'true':
      case $is_public === TRUE:
        $this->isPublic = TRUE;
        break;

      // These are all the possible false cases.
      case $is_public === '0':
      case $is_public === 0:
      case strtolower($is_public) == 'false':
      case $is_public === FALSE:
        $this->isPublic = FALSE;
        break;

      case $is_public === NULL:
      default:
        break;
    }

    return $this;
  }

  /**
   * Get accessor for $this->issueID.
   *
   * @return int
   *   The issue ID used to filter results.
   */
  protected function getIssueID() {
    return $this->issueID;
  }

  /**
   * Set accessor for $this->issueID.
   *
   * @param int $issue_id
   *   The issue ID used to filter results.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setIssueID($issue_id) {
    $this->issueID = $issue_id;
    return $this;
  }

  /**
   * Get accessor for $this->issue.
   *
   * @return string
   *   The issue used to filter results.
   */
  protected function getIssue() {
    return $this->issue;
  }

  /**
   * Set accessor for $this->petitionIds.
   *
   * @param array $pids
   *   An array of petition ids.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setPetitionIds(array $pids) {
    $this->petitionIds = $pids;

    return $this;
  }

  /**
   * Get accessor for $this->petitionIds.
   *
   * @return array
   *   Array of petitionIds.
   */
  public function getPetitionIds() {
    return $this->petitionIds;
  }

  /**
   * Pass through to set $this->petitionIds.
   *
   * @param string $pid
   *   A petition ids.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setPetitionId($pid) {
    $this->setPetitionIds(array($pid));

    return $this;
  }

  /**
   * Set accessor for $this->issue.
   *
   * @param string $issue
   *   The issue used to filter results.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setIssue($issue) {
    $this->issue = $issue;
    return $this;
  }

  /**
   * Get accessor for $this->signatureThresholdCeiling.
   *
   * @return int
   *   The maximum signature threshold used to filter results.
   */
  protected function getSignatureThresholdCeiling() {
    return $this->signatureThresholdCeiling;
  }

  /**
   * Set accessor for $this->signatureThresholdCeiling.
   *
   * @param int $signature_threshold_ceiling
   *   The maximum signature threshold used to filter results.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setSignatureThresholdCeiling($signature_threshold_ceiling) {
    $this->signatureThresholdCeiling = $signature_threshold_ceiling;
    return $this;
  }

  /**
   * Get accessor for $this->signatureThreshold.
   *
   * @return int
   *   The exact signature threshold used to filter results.
   */
  protected function getSignatureThreshold() {
    return $this->signatureThreshold;
  }

  /**
   * Set accessor for $this->signatureThreshold.
   *
   * @param int $signature_threshold
   *   The exact signature threshold used to filter results.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setSignatureThreshold($signature_threshold) {
    $this->signatureThreshold = $signature_threshold;
    return $this;
  }

  /**
   * Get accessor for $this->signatureThresholdFloor.
   *
   * @return int
   *   The minimum signature threshold used to filter results.
   */
  protected function getSignatureThresholdFloor() {
    return $this->signatureThresholdFloor;
  }

  /**
   * Set accessor for $this->signatureThresholdFloor.
   *
   * @param int $signature_threshold_floor
   *   The minimum signature threshold used to filter results.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setSignatureThresholdFloor($signature_threshold_floor) {
    $this->signatureThresholdFloor = $signature_threshold_floor;
    return $this;
  }

  /**
   * Get accessor for $this->signatureCountCeiling.
   *
   * @return int
   *   The maximum signature count used to filter results.
   */
  protected function getSignatureCountCeiling() {
    return $this->signatureCountCeiling;
  }

  /**
   * Set accessor for $this->signatureCountCeiling.
   *
   * @param int $signature_count_ceiling
   *   The maximum signature count used to filter results.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setSignatureCountCeiling($signature_count_ceiling) {
    if (is_numeric($signature_count_ceiling)) {
      // Enforce absolute minimum allowed signature count.
      if ($signature_count_ceiling < $this->getMinSignatureCount()) {
        $signature_count_ceiling = $this->getMinSignatureCount();
      }
      $this->signatureCountCeiling = $signature_count_ceiling;
    }
    return $this;
  }

  /**
   * Get accessor for $this->signatureCount.
   *
   * @return int
   *   The absolute minimum signature count used to filter results.
   */
  public function getMinSignatureCount() {
    return $this->minSignatureCount;
  }

  /**
   * Set accessor for $this->minSignatureCount.
   *
   * @param int $min_signature_count
   *   The absolute minimum signature count used to filter results.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setMinSignatureCount($min_signature_count) {
    $this->minSignatureCount = $min_signature_count;
    return $this;
  }

  /**
   * Get accessor for $this->signatureCount.
   *
   * @return int
   *   The exact signature count used to filter results.
   */
  protected function getSignatureCount() {
    return $this->signatureCount;
  }

  /**
   * Set accessor for $this->signatureCount.
   *
   * @param int $signature_count
   *   The exact signature count used to filter results.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setSignatureCount($signature_count) {
    if (is_numeric($signature_count)) {
      // Enforce absolute minimum allowed signature count.
      if ($signature_count < $this->getMinSignatureCount()) {
        $signature_count = $this->getMinSignatureCount();
      }
      $this->signatureCount = $signature_count;
    }
    return $this;
  }

  /**
   * Get accessor for $this->signatureCountFloor.
   *
   * @return int
   *   The minimum signature count used to filter results.
   */
  protected function getSignatureCountFloor() {
    return $this->signatureCountFloor;
  }

  /**
   * Set accessor for $this->signatureCountFloor.
   *
   * @param int $signature_count_floor
   *   The minimum signature count used to filter results.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setSignatureCountFloor($signature_count_floor) {
    if (is_numeric($signature_count_floor)) {
      // Enforce absolute minimum allowed signature count.
      if ($signature_count_floor < $this->getMinSignatureCount()) {
        $signature_count_floor = $this->getMinSignatureCount();
      }
      $this->signatureCountFloor = $signature_count_floor;
    }
    return $this;
  }

  /**
   * Get accessor for $this->signaturesNeededCeiling.
   *
   * @return int
   *   The maximum signatures needed used to filter results.
   */
  protected function getSignaturesNeededCeiling() {
    return $this->signaturesNeededCeiling;
  }

  /**
   * Set accessor for $this->signaturesNeededCeiling.
   *
   * @param int $signatures_needed_ceiling
   *   The maximum signatures needed used to filter results.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setSignaturesNeededCeiling($signatures_needed_ceiling) {
    $this->signaturesNeededCeiling = $signatures_needed_ceiling;
    return $this;
  }

  /**
   * Get accessor for $this->signaturesNeeded.
   *
   * @return int
   *   The exact signatures needed used to filter results.
   */
  protected function getSignaturesNeeded() {
    return $this->signaturesNeeded;
  }

  /**
   * Set accessor for $this->signaturesNeeded.
   *
   * @param int $signatures_needed
   *   The exact signatures needed used to filter results.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setSignaturesNeeded($signatures_needed) {
    $this->signaturesNeeded = $signatures_needed;
    return $this;
  }

  /**
   * Get accessor for $this->signaturesNeededFloor.
   *
   * @return int
   *   The minimum signature count used to filter results.
   */
  protected function getSignaturesNeededFloor() {
    return $this->signaturesNeededFloor;
  }

  /**
   * Set accessor for $this->signaturesNeededFloor.
   *
   * @param int $signatures_needed_floor
   *   The minimum signature count used to filter results.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setSignaturesNeededFloor($signatures_needed_floor) {
    $this->signaturesNeededFloor = $signatures_needed_floor;
    return $this;
  }

  /**
   * Get accessor for $this->url.
   *
   * @return string
   *   The petition URL used to filter results.
   */
  protected function getURL() {
    return $this->url;
  }

  /**
   * Get accessor for $this->deadlineBefore.
   *
   * @return int
   *   Epoch (UNIX style) time stamp.
   */
  protected function getDeadlineBefore() {
    return $this->deadlineBefore;
  }

  /**
   * Set accessor for $this->deadlineBefore.
   *
   * @param int $deadline_before
   *   Epoch (UNIX style) time stamp.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setDeadlineBefore($deadline_before) {
    $this->deadlineBefore = $deadline_before;
    return $this;
  }

  /**
   * Get accessor for $this->deadline.
   *
   * @return int
   *   Epoch (UNIX style) time stamp.
   */
  protected function getDeadline() {
    return $this->deadline;
  }

  /**
   * Set accessor for $this->deadline.
   *
   * @param int $deadline
   *   Epoch (UNIX style) time stamp.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setDeadline($deadline) {
    $this->deadline = $deadline;
    return $this;
  }

  /**
   * Get accessor for $this->deadlineAfter.
   *
   * @return int
   *   Epoch (UNIX style) time stamp.
   */
  protected function getDeadlineAfter() {
    return $this->deadlineAfter;
  }

  /**
   * Set accessor for $this->deadlineAfter.
   *
   * @param int $deadline_after
   *   Epoch (UNIX style) time stamp.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setDeadlineAfter($deadline_after) {
    $this->deadlineAfter = $deadline_after;
    return $this;
  }

  /**
   * Get accessor for $this->status.
   *
   * @return string
   *   The petition status used to filter results.
   */
  protected function getStatus() {
    return $this->status;
  }

  /**
   * Set accessor for $this->status.
   *
   * @param string $status
   *   The petition status used to filter results.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setStatus($status) {
    $this->status = $status;
    return $this;
  }

  /**
   * Get accessor for $this->responseID.
   *
   * @return int
   *   The response ID used to filter results.
   */
  protected function getResponseID() {
    return $this->responseID;
  }

  /**
   * Set accessor for $this->responseID.
   *
   * @param int $response_id
   *   The response ID used to filter results.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setResponseID($response_id) {
    $this->responseID = $response_id;
    return $this;
  }

  /**
   * Helper function to format return taxonomies.
   *
   * @param array $issues
   *   Taxonomy term ids for specific issues stored in Drupal.
   *
   * @return array
   *   Ids and term names of the related petitions.
   */
  protected function formatReturnIssues($issues) {
    $terms = array();
    if (!is_array($issues)) {
      $issues = array($issues);
    }
    foreach ($issues as $issue_id) {
      $term = taxonomy_term_load($issue_id);

      if (empty($term)) {
        continue;
      }

      $row = array(
        'id' => $term->tid,
        'name' => petitions_data_sanitize_output($term->name),
      );
      $terms[] = $row;
    }
    return $terms;
  }

  /**
   * Helper function to format URL.
   *
   * @param string $uri
   *   Petition path.
   *
   * @return string
   *   Full Petition URL.
   */
  protected function formatReturnURL($uri) {
    return $this->getBaseURL() . '/' . $uri;
  }

  /**
   * Get accessor for $this->base_url.
   *
   * @return string
   *   The base url for the site that absolute URLs will be constructed with.
   */
  protected function getBaseURL() {
    return $this->baseURL;
  }

  /**
   * Set accessor for $this->base_url.
   *
   * @param string $base_url
   *   The base url for the site that absolute URLs will be constructed with.
   *
   * @return PetitionsSelectQuery
   *   Returns current instance of object.
   */
  public function setBaseURL($base_url) {
    $this->baseURL = $base_url;
    return $this;
  }

  /**
   * Helper function to calculate and load info for responses.
   *
   * @param int $response_id
   *   The node id of the response to return information about
   *
   * @return array
   *   Basic information and a link to the associated petition response.
   */
  protected function formatReturnResponse($response_id = 0) {
    if (!$response_id) {
      return array();
    }
    $uri = drupal_lookup_path('alias', 'node/' . $response_id);
    $created_time = db_select('node', 'n')
      ->fields('n', array('created'))
      ->condition('nid', $response_id)
      ->orderBy('created', 'DESC')
      ->range(0, 1)
      ->execute()
      ->fetchField();
    $response = array(
      'id' => $response_id,
      'url' => $this->getBaseURL() . '/' . $uri,
      'associationTime' => $created_time,
    );
    return $response;
  }
}
