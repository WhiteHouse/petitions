<?php

/**
 * @file
 * Contains PetitionsQueryMongo.
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
   * OrderBy ASC/DESC mapping
   *
   * @var array
   */
  protected $orderByDirMap = array(
    self::SELECT_QUERY_ORDER_BY_ASC => 1,
    self::SELECT_QUERY_ORDER_BY_DESC => -1,
  );

  /**
   * OrderBy Field mapping
   *
   * Maps Order By constants to appropriate database column name.
   *
   * @var array
   *   An array containing 'field' element with field name.
   */
  protected $orderByFieldsMap = array(
    self::SELECT_QUERY_ORDER_BY_FIELD_ID => array('field' => '_id'),
    self::SELECT_QUERY_ORDER_BY_FIELD_TITLE => array('field' => 'title'),
    self::SELECT_QUERY_ORDER_BY_FIELD_DATE_CREATED => array('field' => 'created'),
    self::SELECT_QUERY_ORDER_BY_FIELD_PUBLISHED => array('field' => 'published'),
    self::SELECT_QUERY_ORDER_BY_FIELD_SIGNATURE_COUNT => array('field' => 'signature_count'),
    self::SELECT_QUERY_ORDER_BY_FIELD_DATE_REACHED_PUBLIC => array('field' => 'reached_public'),
  );

  /**
   * {@inheritdoc}
   */
  public function __construct() {
    parent::__construct();
    try {
      $collection = mongodb_collection('petitions');
    }
    catch (Exception $e) {
      petitionslog_event('exception.petitions_data.e54cea3');
    }
    $this->setCollection($collection);
    $this->setOrderBy(self::SELECT_QUERY_ORDER_BY_FIELD_DATE_CREATED, self::SELECT_QUERY_ORDER_BY_DESC);
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
   * @return $this
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
   * @return $this
   */
  protected function setQuery(array $query) {
    $this->query = $query;
    return $this;
  }

  /**
   * Primary processing function of class. Makes query to MongoDB.
   *
   * @return $this
   */
  public function execute() {
    if (($this->getIsSignable() === TRUE) || ($this->getIsSignable() === FALSE)) {
      $this->addIsSignableToQuery();
    }

    if (($this->getIsPublic() === TRUE) || ($this->getIsPublic() === FALSE)) {
      $this->addIsPublicToQuery();
    }

    if (($this->getIsOpen() === TRUE) || ($this->getIsOpen() === FALSE)) {
      $this->addIsOpenToQuery();
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
    $this->addIssuesToQuery();
    $this->addBodyToQuery();
    $this->addURLToQuery();
    $this->addStatusToQuery();
    $this->addResponseIDToQuery();
    $this->addPetitionIdsToQuery();
    $this->addUidToQuery();

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
      'published',
      'petition_status',
      'review_timeframe',
      'response_id',
      'response_status',
      'reached_public',
      'reached_ready',
      'public_signatures',
      'uid',
      'bookmarked',
      'featured',
      'private_tags',
      'user_tags',
      'abuse_flags',
      'review_threshold_mail_sent',
    );

    // WARNING: All query additions must happen prior to calling formatStatus().
    $this->formatStatus();
    $query = $this->getQuery();

    $mongo_results = $this->getCollection()
      ->find($query, $fields)
      ->limit($this->getLimit())
      ->skip($this->getOffset())
      ->sort($this->getSort());

    $result_objects = array();
    $result_arrays = array();
    foreach ($mongo_results as $doc) {

      $petition = new PetitionItem();
      $petition->setLegacyId($this->formatReturnId($doc['_id']))
        ->setTitle($doc['title'])
        ->setBody($doc['body'])
        ->setIssues($this->formatReturnIssues($doc['issues']))
        ->setSignatureThreshold($doc['response_signatures'])
        ->setSignatureCount($doc['signature_count'])
        ->setNiceUrl($this->formatReturnUrl($doc['nice_url']))
        ->setShortUrl($doc['short_url'])
        ->setReviewTimeframe($doc['review_timeframe'])
        ->setStatus($doc['petition_status'])
        ->setCreated($doc['created'])
        ->setPublished($doc['published'])
        ->setUid($doc['uid'])
        ->setResponseStatus($doc['response_status'])
        ->setReachedPublic($doc['reached_public'])
        ->setSignaturePublicThreshold($doc['public_signatures'])
        ->setUserTags($doc['user_tags']);
      if (!empty($doc['abuse_flags'])) {
        $petition->setAbuseFlags($doc['abuse_flags']);
      }
      if (!empty($doc['bookmarked'])) {
        $petition->setBookmarked($doc['bookmarked']);
      }
      if (!empty($doc['closed'])) {
        $petition->setClosed($doc['closed']);
      }
      if (!empty($doc['featured'])) {
        $petition->setFeatured($doc['featured']);
      }
      if (!empty($doc['private_tags'])) {
        $petition->setPrivateTags($doc['private_tags']);
      }
      if (!empty($doc['reached_ready'])) {
        $petition->setReachedReady($doc['reached_ready']);
      }
      if (!empty($doc['response_id'])) {
        $petition->setResponse(PetitionsSelectQuery::formatReturnResponse($doc['response_id']));
      }
      if (!empty($doc['review_threshold_mail_sent'])) {
        $petition->setReviewThresholdMailSent($doc['review_threshold_mail_sent']);
      }

      $result_objects[] = $petition;
      $result_arrays[] = $petition->toRestResponseItemArray();
    }

    $this->setResultObjects($result_objects);
    $this->setResult($result_arrays);
    $this->setCount($mongo_results->count());

    return $this;
  }

  /**
   * Helper function to add $this->createdDate as a query parameter.
   */
  protected function addCreatedDateToQuery() {
    if ($this->getCreatedDate()) {
      $query = $this->getQuery();
      $query += array(
        'published' => $this->getCreatedDate(),
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
      // Do not overwrite any other parameters that use 'published'.
      if (!isset($query['published'])) {
        $query['published'] = array();
      }
      $query['published'] += array('$gt' => $this->getStartDate());
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->endDate as a query parameter.
   */
  protected function addEndDateToQuery() {
    if ($this->getEndDate()) {
      $query = $this->getQuery();
      // Do not overwrite any other parameters that use 'published'.
      if (!isset($query['published'])) {
        $query['published'] = array();
      }
      $query['published'] += array('$lt' => $this->getEndDate());
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
   * Adds the issues argument to the query.
   */
  protected function addIssuesToQuery() {
    if ($this->getIssueIDs()) {
      $query = $this->getQuery();
      $query += array(
        'issues' => array('$in' => $this->getIssueIDs()),
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
      $nice_url = petitions_data_get_path_from_full_url($this->getURL());
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
      $developer_message = t('You need a petition ID or URL to access non-public petitions.');
      api_errors_throw_error(403, $developer_message);
    }
  }

  /**
   * Helper function to add $this->isOpen as a query parameter.
   */
  protected function addIsOpenToQuery() {
    // Petitions with any of the following status, are considered open.
    $array_open_statuses = wh_petitions_open_statuses();

    if ($this->getIsOpen()) {
      // Request is for petitions that are open.
      $this->setStatusInclude($array_open_statuses);
    }
    elseif ($this->getIsOpen() === FALSE) {
      // Request is for petitions that are NOT open.
      $this->setStatusExclude($array_open_statuses);
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
   * Helper function to add $this->uid as a query parameter.
   */
  protected function addUidToQuery() {
    if ($this->getUid()) {
      $query = $this->getQuery();
      $query += array(
        'uid' => (int) $this->getUid(),
      );
      $this->setQuery($query);
    }
  }

  /**
   * Adds the orderBy arguments to the query.
   */
  protected function getSort() {
    $sort_array = array();
    if (!$this->getOrderBy()) {
      return $sort_array;
    }

    foreach ($this->getOrderBy() as $order_by) {
      $field = $order_by->field;
      $order_by_dir = $order_by->direction;

      if (!$this->isValidOrderByField($field)) {
        throw new Exception('getSort - Not a valid field: ' . $field);
      }
      if (!$this->isValidOrderByDirection($order_by_dir)) {
        throw new Exception('getSort - Not a valid direction: ' . $order_by_dir);
      }
      $mapped_field = $this->orderByFieldsMap[$field]['field'];
      $mapped_dir = $this->orderByDirMap[$order_by_dir];
      $sort_array[$mapped_field] = $mapped_dir;
    }
    return $sort_array;
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
    elseif (!count($query)) {
      // Some combination of status filters have resulted in no filters,
      // which is impossible to return.  Set this to nin for the entire status.
      $query['petition_status'] = array('$nin' => wh_petitions_all_statuses());
    }
    $this->setQuery($query);
  }

  /**
   * Gets a petition ID from a given petition URL.
   *
   * @param $url
   *   The URL to get a petition ID from.
   *
   * @return mixed|false
   *   A petition ID corresponding to the given URL, or FALSE if there is no
   *   corresponding petition.
   */
  public static function getPetitionIdFromUrl($url) {
    $nice_url = petitions_data_get_path_from_full_url($url);
    $conn = wh_petitions_mongo_petition_connection();
    $petition = $conn->findOne(array('nice_url' => $nice_url), array('title'));
    return (string) $petition['_id'];
  }
}
