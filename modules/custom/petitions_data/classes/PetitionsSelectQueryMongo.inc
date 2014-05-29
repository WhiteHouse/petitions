<?php

/**
 * @file
 * Defines PetitionsQueryMongo class.
 */

/**
 * Class PetitionsQueryMongo.
 *
 * This should contain MongoDB-specific logic for querying petitions.
 */
class PetitionsSelectQueryMongo extends PetitionsSelectQuery {

  protected $collection;
  protected $query = array();


  /**
   * Constructor.
   */
  public function __construct() {
    parent::__construct();
    $collection = mongodb_collection('petitions');
    $this->setCollection($collection);
  }


  /**
   * Get accessor for $this->collection.
   *
   * @return array
   *   MongoCollection resource for querying against a collection.
   */
  protected function getCollection() {
    return $this->collection;
  }

  /**
   * Set accessor for $this->collection.
   *
   * @param MongoCollection $collection
   *   MongoCollection resource for querying against a collection.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  protected function setCollection($collection) {
    $this->collection = $collection;
    return $this;
  }

  /**
   * Get accessor for $this->query.
   *
   * @return array
   *   Results of query to MongoDB.
   */
  protected function getQuery() {
    return $this->query;
  }

  /**
   * Set accessor for $this->query.
   *
   * @param array $query
   *   Array of query parameters to get passed to mongodb.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  protected function setQuery(array $query) {
    $this->query = $query;
    return $this;
  }

  /**
   * Primary processing function of class. Makes query to MongoDB.
   *
   * @return PetitionsQueryMongo
   *   Returns current instance of object.
   */
  public function execute() {
    $results = array();

    if (($this->getIsSignable() === TRUE) || ($this->getIsSignable() === FALSE)) {
      $this->addIsSignableToQuery();
    }

    if (($this->getIsPublic() === TRUE) || ($this->getIsPublic() === FALSE)) {
      $this->addIsPublicToQuery();
    }

    if ($this->getCreatedDate()) {
      $this->addCreatedDateToQuery();
    }
    else {
      $this->addStartDateToQuery();
      $this->addEndDateToQuery();
    }

    if ($this->getSignatureThreshold()) {
      $this->addSignatureThresholdToQuery();
    }
    else {
      $this->addSignatureThresholdCeilingToQuery();
      $this->addSignatureThresholdFloorToQuery();
    }

    if ($this->getSignatureCount()) {
      $this->addSignatureCountToQuery();
    }
    else {
      $this->addSignatureCountCeilingToQuery();
      $this->addSignatureCountFloorToQuery();
    }

    $this->addTitleToQuery();
    $this->addBodyToQuery();
    $this->addURLToQuery();
    $this->addStatusToQuery();
    $this->addResponseIDToQuery();
    $this->addPetitionIdsToQuery();

    $fields = array(
      'title',
      'body',
      'issues',
      'signature_count',
      'response_signatures',
      'nice_url',
      'short_url',
      'closed',
      'created',
      'petition_status',
      'review_timeframe',
      'response_id',
    );

    // WARNING: All query additions must happen prior to calling formatStatus().
    $this->formatStatus();
    $query = $this->getQuery();

    $mongo_results = $this->getCollection()
      ->find($query, $fields)
      ->limit($this->getLimit())
      ->skip($this->getOffset())
      ->sort(array('created' => -1));

    if (!$mongo_results->hasNext()) {
      $this->setResult(array());
      return $this;
    }

    foreach ($mongo_results as $doc) {

      $petition = new Petition();
      $petition->setId($this->formatReturnId($doc['_id']))
        ->setTitle($doc['title'])
        ->setBody($doc['body'])
        ->setIssues($this->formatReturnIssues($doc['issues']))
        ->setSignatureThreshold($doc['response_signatures'])
        ->setSignatureCount($doc['signature_count'])
        ->setNiceUrl($this->formatReturnUrl($doc['nice_url']))
        ->setReviewTimeframe($doc['review_timeframe'])
        ->setStatus($doc['petition_status'])
        ->setResponse(isset($doc['response_id']) ? $this->formatReturnResponse($doc['response_id']) : NULL)
        ->setCreated($doc['created']);

      $results[] = $petition->toArray();
    }

    $this->setCount($mongo_results->count());
    $this->setResult($results);

    return $this;
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
   * Helper function to add $this->signatureThresholdCeiling as a query param.
   */
  protected function addSignatureThresholdCeilingToQuery() {
    if ($this->getSignatureThresholdCeiling()) {
      $query = $this->getQuery();
      // Do not overwrite any other parameters that use 'response_signatures'.
      if (!isset($query['response_signatures'])) {
        $query['response_signatures'] = array();
      }
      $query['response_signatures'] += array('$lt' => new MongoInt64($this->getSignatureThresholdCeiling()));
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->signatureThresholdFloor as a query parameter.
   */
  protected function addSignatureThresholdFloorToQuery() {
    if ($this->getSignatureThresholdFloor()) {
      $query = $this->getQuery();
      // Do not overwrite any other parameters that use 'response_signatures'.
      if (!isset($query['response_signatures'])) {
        $query['response_signatures'] = array();
      }
      $query['response_signatures'] += array('$gt' => new MongoInt64($this->getSignatureThresholdFloor()));
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
   * Helper function to add $this->signatureCountCeiling as a query parameter.
   */
  protected function addSignatureCountCeilingToQuery() {
    if ($this->getSignatureCountCeiling()) {
      $query = $this->getQuery();
      // Do not overwrite any other parameters that use 'signature_count'.
      if (!isset($query['signature_count'])) {
        $query['signature_count'] = array();
      }
      $query['signature_count'] += array('$lt' => new MongoInt64($this->getSignatureCountCeiling()));
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->signatureCountFloor as a query parameter.
   */
  protected function addSignatureCountFloorToQuery() {
    if ($this->getSignatureCountFloor()) {
      $query = $this->getQuery();
      // Do not overwrite any other parameters that use 'signature_count'.
      if (!isset($query['signature_count'])) {
        $query['signature_count'] = array();
      }
      $query['signature_count'] += array('$gt' => new MongoInt64($this->getSignatureCountFloor()));
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->title as a query parameter.
   */
  protected function addTitleToQuery() {
    if ($this->getTitle()) {
      $query = $this->getQuery();
      $query += array(
        'title' => new MongoRegex("/" . $this->getTitle() . "/i"),
      );
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->body as a query parameter.
   */
  protected function addBodyToQuery() {
    if ($this->getBody()) {
      $query = $this->getQuery();
      $query += array(
        'body' => new MongoRegex("/" . $this->getBody() . "/i"),
      );
      $this->setQuery($query);
    }
  }

  /**
   * Helper function filter results by petition ids.
   */
  protected function addPetitionIdsToQuery() {
    if ($this->getPetitionIds()) {
      $pids = $this->getPetitionIds();
      $mongo_ids = array();
      foreach ($pids as $pid) {
        $mongo_ids[] = new MongoId($pid);
      }
      $query = $this->getQuery();
      $query += array(
        '_id' => array('$in' => $mongo_ids),
      );
      $this->setQuery($query);
      // Set statuses to disallow "flagged"/removed petitions.
      $this->setStatusExclude(array(WH_PETITION_STATUS_FLAGGED));
    }
  }

  /**
   * Helper function to add $this->url as a query parameter.
   */
  protected function addURLToQuery() {
    // Filter by URL.
    if ($this->getURL()) {
      $nice_url = petitions_data_get_nice_url_from_full_url($this->getURL());
      $query = $this->getQuery();
      $query += array(
        'nice_url' => $nice_url,
      );
      $this->setQuery($query);
      // Set statuses to disallow "flagged"/removed petitions.
      $this->setStatusExclude(array(WH_PETITION_STATUS_FLAGGED));
    }
  }

  /**
   * Helper function to add $this->status as a query parameter.
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

      if (!empty($status)) {
        // A status request has been made, so add it to included statuses.
        $this->setStatusInclude((Array) $status);
      }
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
   * Helper function to add $this->isPublic as a query parameter.
   */
  protected function addIsPublicToQuery() {
    $query = $this->getQuery();
    // Petitions with any of the following status, are considered public.
    $array_public_statuses = wh_petitions_public_statuses();

    if ($this->getIsPublic()) {
      // Request is for petitions that are public.
      $this->setStatusInclude($array_public_statuses);

    }
    elseif ($this->getIsPublic() === FALSE) {
      // Request is for petitions that are NOT public.
      // Non-public petitions are not available through the index api.
      // Return access denied with a message.
      $developer_message = t('You need a petition id or url to access non-public petitions.');
      api_errors_throw_error(403, $developer_message);
    }
  }

  /**
   * Helper function to add $this->isSignable as a query parameter.
   */
  protected function addIsSignableToQuery() {
    $query = $this->getQuery();
    // Petitions with any of the following status, are considered signable.
    $array_signable_statuses = wh_petitions_signable_statuses();

    if ($this->getIsSignable()) {
      // Request is for petitions that are signable.
      $this->setStatusInclude($array_signable_statuses);
    }
    elseif ($this->getIsSignable() === FALSE) {
      // Request is for petitions that are NOT signable.
      $this->setStatusExclude($array_signable_statuses);
    }
  }

  /**
   * Helper function to format return response ID field.
   *
   * @param MongoId $id
   *   Unformatted mongo id object.
   *
   * @return string
   *   Unique string identifier for a petition.
   */
  protected function formatReturnId($id) {
    return $id->{'$id'};
  }


  /**
   * Helper function to remove petition statuses from another array.
   *
   * @param array $original_status
   *   The status(es) that already exist in the query from prior sets.
   *
   * @param array $status_to_remove
   *   The status(es) that should be removed from the filter.
   *
   * @return array
   *   The difference of the two arrays or $existing_status if
   *   $additional_status is empty.
   */
  private function removeNINStatuses($original_status = array(), $status_to_remove = array()) {
    if (empty($original_status) && !empty($status_to_remove)) {
      // There is no status(es) to include , but some to exclude. Diff the
      // $status_to_remove against the full array of possible statuses to end up
      // with just the statuses we want.
      $statuses_all_possible = wh_petitions_all_statuses();
      return array_diff($statuses_all_possible, $status_to_remove);
    }
    elseif (!empty($status_to_remove)) {
      return array_diff($original_status, $status_to_remove);
    }
    else {
      return $original_status;
    }

  }

  /**
   * Get accessor for $query->statusInclude.
   *
   * @return array
   *   Array of status ids to include in the query.
   */
  protected function getStatusInclude() {
    $query = $this->getQuery();

    if (!empty($query['petition_status_in'])) {
      return $query['petition_status_in'];
    }
    else {
      return array();
    }
  }

  /**
   * Set accessor for $query->statusInclude.
   *
   * @param array $statuses_to_add
   *   An array of statuses to add to (intersect with) the include filter.
   */
  protected function setStatusInclude($statuses_to_add = array()) {

    // Check that there is a status to be set.
    if (!empty($statuses_to_add)) {

      // Make sure it is an array, and not just a single status id.
      $statuses_to_add = (is_array($statuses_to_add)) ? $statuses_to_add : (Array) $statuses_to_add;

      // Load the current query.
      $query = $this->getQuery();

      // Get any existing include status(es).
      $status_original = $this->getStatusInclude();

      // Check to see if status(es) have already been set.
      if (empty($status_original)) {
        // There is no original status, so just set the status to the new one.
        $statuses_processed = $statuses_to_add;
      }
      elseif (!empty($statuses_to_add)) {
        // There is an original status(es) and a new status(es) to be added.
        $statuses_processed = array_intersect($status_original, $statuses_to_add);

      }
      else {
        // There are no new status(es) to add so pass the original through.
        $statuses_processed = $status_original;
      }

      // This is a non-mongo style query array at this time.  It will get
      // converted to mongo style just before the query is executed.
      $query['petition_status_in'] = $statuses_processed;
    }

    $this->setQuery($query);
  }


  /**
   * Get accessor for $query->statusExclude.
   *
   * @return array
   *   Array of status ids to exclude from the query.
   */
  protected function getStatusExclude() {
    $query = $this->getQuery();
    if (!empty($query['petition_status_exclude'])) {
      return $query['petition_status_exclude'];
    }
    else {
      return array();
    }
  }


  /**
   * Set accessor for $query->status exclude.
   *
   * @param array $statuses_to_exclude
   *   An array of statuses to extract from the include filter.
   *
   * @return array
   *   Returns current instance of query array.
   */
  protected function setStatusExclude($statuses_to_exclude = array()) {
    // Check that there is a status to be set.
    if (!empty($statuses_to_exclude)) {
      // Make sure it is an array, and not just a single status id.
      $statuses_to_exclude = (is_array($statuses_to_exclude)) ? $statuses_to_exclude : (Array) $statuses_to_exclude;
      // Load the current query.
      $query = $this->getQuery();
      // Get any existing excluded status(es).
      $status_exclude_original = $this->getStatusExclude();
      // Check to see if some status(es) have already been set.
      if (empty($status_exclude_original)) {
        // There is no original status, so just set the status to the new one.
        $statuses_processed = $statuses_to_exclude;
      }
      elseif (!empty($statuses_to_exclude)) {
        // There is an original status(es) and a new status(es) to be excluded
        // as a union of all statuses not wanted.
        $statuses_processed = array_unique(array_merge($status_exclude_original, $statuses_to_exclude));
      }
      else {
        // There are no new status(es) to add so pass the original through.
        $statuses_processed = $statuses_to_exclude;
      }

      // This is a non-mongo style query array at this time.  It will get
      // converted to mongo style just before the query is executed.
      $query['petition_status_exclude'] = $statuses_processed;
    }
    $this->setQuery($query);
  }

  /**
   * Helper function to format the include and exclude statuses for Mongo.
   */
  protected function formatStatus() {
    // Load the current query.
    $query = $this->getQuery();
    // Intercept the $query['petition_status_in'] & ['petition_status_exclude']
    // which are a pair of flat arrays, to include and exclude.
    $status_in = $this->getStatusInclude();
    $status_nin = $this->getStatusExclude();

    $status_to_query = $this->removeNINStatuses($status_in, $status_nin);
    // Renumber keys so mongo doesn't throw a 500 error if the keys don't start
    // with 0 and go in order.
    sort($status_to_query);
    // Remove non-mongofied statuses in the query array.
    unset($query['petition_status_in']);
    unset($query['petition_status_exclude']);
    if (!empty($status_to_query)) {
      $query['petition_status'] = array('$in' => (Array) $status_to_query);
    }
    else {
      // Some combination of status filters have resulted in no filters,
      // which is impossible to return.  Set this to nin for the entire status.
      $query['petition_status'] = array('$nin' => wh_petitions_all_statuses());
    }
    $this->setQuery($query);
  }
}
