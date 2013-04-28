<?php
/**
 * @file
 * File containing class that accesses data using mongodb queries.
 */

class PetitionsIndexBase extends PetitionsBase {

  protected $startDate;
  protected $endDate;
  protected $createdDate;
  protected $limit;
  protected $maxReturnLimit = 1000;
  protected $offset;
  protected $title;
  protected $body;
  protected $issueID;
  protected $issue;
  protected $signatureThresholdCeiling;
  protected $signatureThreshold;
  protected $signatureThresholdFloor;
  protected $signatureCountCeiling;
  protected $signatureCount;
  protected $signatureCountFloor;
  protected $minSignatureCountFloor = 150;
  protected $signaturesNeededCeiling;
  protected $signaturesNeeded;
  protected $signaturesNeededFloor;
  protected $url;
  protected $deadlineBefore;
  protected $deadline;
  protected $deadlineAfter;
  protected $status;
  protected $responseID;
  protected $responseAssociationTimeBefore;
  protected $responseAssociationTime;
  protected $responseAssociationTimeAfter;

  /**
   * Set accessor for $this->startDate.
   *
   * @param int $date
   *   Epoch (UNIX style) time stamp.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setStartDate($date) {
    if (is_numeric($date)) {
      $this->startDate = $date;
    }
    return $this;
  }

  /**
   * Set accessor for $this->endDate.
   *
   * @param int $date
   *   Epoch (UNIX style) time stamp.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setEndDate($date) {
    if (is_numeric($date)) {
      $this->endDate = $date;
    }
    return $this;
  }

  /**
   * Set accessor for $this->createdAt.
   *
   * @param int $date
   *   Epoch (UNIX style) time stamp.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setCreatedAt($date) {
    if (is_numeric($date)) {
      $this->createdDate = $date;
    }
    return $this;
  }

  /**
   * Set accessor for $this->limit.
   *
   * @param int $limit
   *   Number of results to return
   *
   * @return SignaturesMongoRaw
   *   Returns current instance of object.
   */
  public function setLimit($limit) {
    if (is_numeric($limit)) {
      if ($limit > $this->maxReturnLimit) {
        $limit = $this->maxReturnLimit;
      }
      $this->limit = $limit;
    }
    return $this;
  }

  /**
   * Set accessor for $this->offset.
   *
   * @param int $offset
   *   Number of results to skip before return.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setOffset($offset) {
    if (is_numeric($offset)) {
      $this->offset = $offset;
    }
    return $this;
  }

  /**
   * Set accessor for $this->title.
   *
   * @param int $title
   *   The petition title used to filter results.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setTitle($title) {
    $this->title = $title;

    return $this;
  }

  /**
   * Set accessor for $this->body.
   *
   * @param int $body
   *   The petition body used to filter results.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setBody($body) {
    $this->body = $body;

    return $this;
  }

  /**
   * Set accessor for $this->issueID.
   *
   * @param int $issue_id
   *   The issue ID used to filter results.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setIssueID($issue_id) {
    $this->issueID = $issue_id;

    return $this;
  }

  /**
   * Set accessor for $this->issue.
   *
   * @param int $issue
   *   The issue used to filter results.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setIssue($issue) {
    $this->issue = $issue;

    return $this;
  }

  /**
   * Set accessor for $this->signatureThresholdCeiling.
   *
   * @param int $signature_threshold_ceiling
   *   The maximum signature threshold used to filter results.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setSignatureThresholdCeiling($signature_threshold_ceiling) {
    $this->signatureThresholdCeiling = $signature_threshold_ceiling;

    return $this;
  }

  /**
   * Set accessor for $this->signatureThreshold.
   *
   * @param int $signature_threshold
   *   The exact signature threshold used to filter results.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setSignatureThreshold($signature_threshold) {
    $this->signatureThreshold = $signature_threshold;

    return $this;
  }

  /**
   * Set accessor for $this->signatureThresholdFloor.
   *
   * @param int $signature_threshold_floor
   *   The minimum signature threshold used to filter results.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setSignatureThresholdFloor($signature_threshold_floor) {
    $this->signatureThresholdFloor = $signature_threshold_floor;

    return $this;
  }

  /**
   * Set accessor for $this->signatureCountCeiling.
   *
   * @param int $signature_count_ceiling
   *   The maximum signature count used to filter results.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setSignatureCountCeiling($signature_count_ceiling) {
    $this->signatureCountCeiling = $signature_count_ceiling;

    return $this;
  }

  /**
   * Set accessor for $this->signatureCount.
   *
   * @param int $signature_count
   *   The exact signature count used to filter results.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setSignatureCount($signature_count) {
    $this->signatureCount = $signature_count;

    return $this;
  }

  /**
   * Set accessor for $this->signatureCountFloor.
   *
   * @param int $signature_count_floor
   *   The minimum signature count used to filter results.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setSignatureCountFloor($signature_count_floor) {
    if (is_numeric($signature_count_floor)) {
      if ($signature_count_floor < $this->minSignatureCountFloor) {
        $signature_count_floor = $this->minSignatureCountFloor;
      }
      $this->signatureCountFloor = $signature_count_floor;
    }
    return $this;
  }

  /**
   * Set accessor for $this->signaturesNeededCeiling.
   *
   * @param int $signatures_needed_ceiling
   *   The maximum signatures needed used to filter results.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setSignaturesNeededCeiling($signatures_needed_ceiling) {
    $this->signaturesNeededCeiling = $signatures_needed_ceiling;

    return $this;
  }

  /**
   * Set accessor for $this->signaturesNeeded.
   *
   * @param int $signatures_needed
   *   The exact signatures needed used to filter results.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setSignaturesNeeded($signatures_needed) {
    $this->signaturesNeeded = $signatures_needed;

    return $this;
  }

  /**
   * Set accessor for $this->signaturesNeededFloor.
   *
   * @param int $signatures_needed_floor
   *   The minimum signature count used to filter results.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setSignaturesNeededFloor($signatures_needed_floor) {
    $this->signaturesNeededFloor = $signatures_needed_floor;

    return $this;
  }

  /**
   * Set accessor for $this->url.
   *
   * @param int $url
   *   The petition URL used to filter results.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setURL($url) {
    $this->url = $url;

    return $this;
  }

  /**
   * Set accessor for $this->deadlineBefore.
   *
   * @param int $deadline_before
   *   Epoch (UNIX style) time stamp.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setDeadlineBefore($deadline_before) {
    $this->deadlineBefore = $deadline_before;

    return $this;
  }

  /**
   * Set accessor for $this->deadline.
   *
   * @param int $deadline
   *   Epoch (UNIX style) time stamp.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setDeadline($deadline) {
    $this->deadline = $deadline;

    return $this;
  }

  /**
   * Set accessor for $this->deadlineAfter.
   *
   * @param int $deadline_after
   *   Epoch (UNIX style) time stamp.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setDeadlineAfter($deadline_after) {
    $this->deadlineAfter = $deadline_after;

    return $this;
  }

  /**
   * Set accessor for $this->status.
   *
   * @param int $status
   *   The petition status used to filter results.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setStatus($status) {
    $this->status = $status;

    return $this;
  }

  /**
   * Set accessor for $this->responseID.
   *
   * @param int $response_id
   *   The response ID used to filter results.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setResponseID($response_id) {
    $this->responseID = $response_id;

    return $this;
  }

  /**
   * Set accessor for $this->responseAssocationTimeBefore.
   *
   * @param int $response_association_time_before
   *   Epoch (UNIX style) time stamp.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setResponseAssocationTimeBefore($response_association_time_before) {
    $this->responseAssocationTimeBefore = $response_association_time_before;

    return $this;
  }

  /**
   * Set accessor for $this->responseAssocationTime.
   *
   * @param int $response_association_time
   *   Epoch (UNIX style) time stamp.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setResponseAssocationTime($response_association_time) {
    $this->responseAssocationTime = $response_association_time;

    return $this;
  }

  /**
   * Set accessor for $this->responseAssocationTimeAfter.
   *
   * @param int $response_association_time_after
   *   Epoch (UNIX style) time stamp.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setResponseAssocationTimeAfter($response_association_time_after) {
    $this->responseAssocationTimeAfter = $response_association_time_after;

    return $this;
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
   * Get accessor for $this->endDate.
   *
   * @return int
   *   Epoch (UNIX style) time stamp.
   */
  protected function getEndDate() {
    return intval($this->endDate);
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
   * Get accessor for $this->limit.
   *
   * @return int
   *   Number of results to return
   */
  protected function getLimit() {

    if (!intval($this->limit)) {
      return $this->maxReturnLimit;
    }

    return intval($this->limit);
  }

  /**
   * Get accessor for $this->offset.
   *
   * @return int
   *   Number of results to skip before return.
   */
  protected function getOffset() {
    return $this->offset ? intval($this->offset) : 0;
  }

  /**
   * Get accessor for $this->title.
   *
   * @return int
   *   The petition title used to filter results.
   */
  protected function getTitle() {
    return $this->title;
  }

  /**
   * Get accessor for $this->body.
   *
   * @return int
   *   The petition body used to filter results.
   */
  protected function getBody() {
    return $this->body;
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
   * Get accessor for $this->issue.
   *
   * @return int
   *   The issue used to filter results.
   */
  protected function getIssue() {
    return $this->issue;
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
   * Get accessor for $this->signatureThreshold.
   *
   * @return int
   *   The exact signature threshold used to filter results.
   */
  protected function getSignatureThreshold() {
    return $this->signatureThreshold;
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
   * Get accessor for $this->signatureCountCeiling.
   *
   * @return int
   *   The maximum signature count used to filter results.
   */
  protected function getSignatureCountCeiling() {
    return $this->signatureCountCeiling;
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
   * Get accessor for $this->signatureCountFloor.
   *
   * @return int
   *   The minimum signature count used to filter results.
   */
  protected function getSignatureCountFloor() {
    return $this->signatureCountFloor;
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
   * Get accessor for $this->signaturesNeeded.
   *
   * @return int
   *   The exact signatures needed used to filter results.
   */
  protected function getSignaturesNeeded() {
    return $this->signaturesNeeded;
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
   * Get accessor for $this->url.
   *
   * @return int
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
   * Get accessor for $this->deadline.
   *
   * @return int
   *   Epoch (UNIX style) time stamp.
   */
  protected function getDeadline() {
    return $this->deadline;
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
   * Get accessor for $this->status.
   *
   * @return int
   *   The petition status used to filter results.
   */
  protected function getStatus() {
    return $this->status;
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
   * Get accessor for $this->responseAssocationTimeBefore.
   *
   * @return int
   *   Epoch (UNIX style) time stamp.
   */
  protected function getResponseAssocationTimeBefore() {
    return $this->responseAssocationTimeBefore;
  }

  /**
   * Get accessor for $this->responseAssocationTime.
   *
   * @return int
   *   Epoch (UNIX style) time stamp.
   */
  protected function getResponseAssocationTime() {
    return $this->responseAssocationTime;
  }

  /**
   * Get accessor for $this->responseAssocationTimeAfter.
   *
   * @return int
   *   Epoch (UNIX style) time stamp.
   */
  protected function getResponseAssocationTimeAfter() {
    return $this->responseAssocationTimeAfter;
  }

  /**
   * Helper function to add $this->startDate as a query parameter.
   */
  protected function addStartDateToQuery() {
    if ($this->getStartDate()) {
      $query = $this->getQuery();

      // Do not overwrite any other parameters that use 'created'.
      if (!isset($query['created'])) {
        $query['created'] = array();
      }
      $query['created'] += array('$gt' => $this->getStartDate());

      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->endDate as a query parameter.
   */
  protected function addEndDateToQuery() {
    if ($this->getEndDate()) {
      $query = $this->getQuery();

      // Do not overwrite any other parameters that use 'created'.
      if (!isset($query['created'])) {
        $query['created'] = array();
      }
      $query['created'] += array('$lt' => $this->getEndDate());
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->createdDate as a query parameter.
   */
  protected function addCreatedDateToQuery() {
    if ($this->getCreatedDate()) {
      $query = $this->getQuery();

      $query += array(
        'created' => $this->getCreatedDate(),
      );
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->title as a query parameter.
   * @todo add $this->title as a query parameter.
   */
  protected function addTitleToQuery() {
    if ($this->getTitle()) {
      $query = $this->getQuery();

      $query += array(
        'title' => new MongoRegex("/" . $this->getTitle() . "/"),
      );
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->body as a query parameter.
   * @todo add $this->body as a query parameter.
   */
  protected function addBodyToQuery() {
    if ($this->getBody()) {
      $query = $this->getQuery();

      $query += array(
        'body' => new MongoRegex("/" . $this->getBody() . "/"),
      );
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->issueID as a query parameter.
   * @todo add $this->issueID as a query parameter.
   */
  protected function addIssueIDToQuery() {
    // Filter by Issue ID.
  }

  /**
   * Helper function to add $this->issue as a query parameter.
   * @todo add $this->issue as a query parameter.
   */
  protected function addIssueToQuery() {
    // Filter by Issue.
  }

  /**
   * Helper function to add $this->signatureThresholdCeiling as a query parameter.
   */
  protected function addSignatureThresholdCeilingToQuery() {
    if ($this->getSignatureThresholdCeiling()) {
      $query = $this->getQuery();

      $query += array(
        'response_signatures' => array('$lt' => new MongoInt64($this->getSignatureThresholdCeiling())),
      );
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->signatureThreshold as a query parameter.
   */
  protected function addSignatureThresholdToQuery() {
    if ($this->getSignatureThreshold()) {
      $query = $this->getQuery();

      $query += array(
        'response_signatures' => new MongoInt64($this->getSignatureThreshold()),
      );
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->signatureThresholdFloor as a query parameter.
   */
  protected function addSignatureThresholdFloorToQuery() {
    if ($this->getSignatureThresholdFloor()) {
      $query = $this->getQuery();

      $query += array(
        'response_signatures' => array('$gt' => new MongoInt64($this->getSignatureThresholdFloor())),
      );
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->signatureCountCeiling as a query parameter.
   */
  protected function addSignatureCountCeilingToQuery() {
    if ($this->getSignatureCountCeiling()) {
      $query = $this->getQuery();

      $query += array(
        'signature_count' => array('$lt' => new MongoInt64($this->getSignatureCountCeiling())),
      );
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->signatureCount as a query parameter.
   */
  protected function addSignatureCountToQuery() {
    if ($this->getSignatureCount()) {
      $query = $this->getQuery();

      $query += array(
        'signature_count' => new MongoInt64($this->getSignatureCount()),
      );
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->signatureCountFloor as a query parameter.
   */
  protected function addSignatureCountFloorToQuery() {
    if ($this->getSignatureCountFloor()) {
      $query = $this->getQuery();

      $query += array(
        'signature_count' => array('$gt' => new MongoInt64($this->getSignatureCountFloor())),
      );
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->signaturesNeededCeiling as a query parameter.
   * @todo add $this->signaturesNeededCeiling as a query parameter.
   */
  protected function addSignaturesNeededCeilingToQuery() {
    // Filter by signatures needed ceiling.
  }

  /**
   * Helper function to add $this->signaturesNeeded as a query parameter.
   * @todo add $this->signaturesNeeded as a query parameter.
   */
  protected function addSignaturesNeededToQuery() {
    // Filter by signatures needed.
  }

  /**
   * Helper function to add $this->signaturesNeededFloor as a query parameter.
   * @todo add $this->signaturesNeededFloor as a query parameter.
   */
  protected function addSignaturesNeededFloorToQuery() {
    // Filter by signatures needed floor.
  }

  /**
   * Helper function to add $this->url as a query parameter.
   * @todo add $this->url as a query parameter.
   */
  protected function addURLToQuery() {
    // Filter by URL.
    if ($this->getURL()) {

      $uri = explode($this->getBaseURL() . '/', $this->getURL());
      $nice_url = $uri[1];

      $query = $this->getQuery();

      $query += array(
        'nice_url' => $nice_url,
      );
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->deadlineBefore as a query parameter.
   * @todo add $this->deadlineBefore as a query parameter.
   */
  protected function addDeadlineBeforeToQuery() {
    // Filter by deadline before.
  }

  /**
   * Helper function to add $this->deadline as a query parameter.
   * @todo add $this->deadline as a query parameter.
   */
  protected function addDeadlineToQuery() {
    // Filter by deadline.
  }

  /**
   * Helper function to add $this->deadlineAfter as a query parameter.
   * @todo add $this->deadlineAfter as a query parameter.
   */
  protected function addDeadlineAfterToQuery() {
    // Filter by deadline after.
  }

  /**
   * Helper function to add $this->status as a query parameter.
   * @todo add $this->status as a query parameter.
   */
  protected function addStatusToQuery() {
    // Filter by status.
    $status = '';

    if ($this->getStatus()) {
      switch ($this->getStatus()) {
        // Has the required number of signatures to become public,
        // visible everywhere on site.
        case 'open':
          $status = WH_PETITION_STATUS_PUBLIC;

          break;

        // Has received the required number of signatures, visible on site.
        case 'pending response':
          $status = WH_PETITION_STATUS_UNDER_REVIEW;

          break;

        // Has received a response.
        case 'responded':
          $status = WH_PETITION_STATUS_REVIEWED;

          break;

        // Has been closed without a response, not visible on site.
        case 'closed':
          $status = WH_PETITION_STATUS_CLOSED;

          break;
      }

      $query = $this->getQuery();

      $query += array(
        'petition_status' => new MongoInt64($status),
      );
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->responseID as a query parameter.
   */
  protected function addResponseIDToQuery() {
    if ($this->getResponseID()) {
      $query = $this->getQuery();

      $query += array(
        'response_id' => $this->getResponseID(),
      );
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->responseAssociationTimeBefore as a query parameter.
   * @todo add $this->responseAssociationTimeBefore as a query parameter.
   */
  protected function addResponseAssociationTimeBeforeToQuery() {
    // Filter by response association time before.
  }

  /**
   * Helper function to add $this->responseAssociationTime as a query parameter.
   * @todo add $this->responseAssociationTime as a query parameter.
   */
  protected function addResponseAssociationTimeToQuery() {
    // Filter by response association time.
  }

  /**
   * Helper function to add $this->responseAssociationTimeAfter as a query parameter.
   * @todo add $this->responseAssociationTimeAfter as a query parameter.
   */
  protected function addResponseAssociationTimeAfterToQuery() {
    // Filter by response association time after.
  }
}
