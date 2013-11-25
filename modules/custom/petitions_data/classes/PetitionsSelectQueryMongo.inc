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
    //watchdog('debug', print_r($collection, TRUE));
    // global $conf;
    // watchdog('debug', print_r($conf, TRUE));
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
  protected function setQuery($query) {
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

      $results[] = $petition->__toArray();
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
    }
  }


  /**
   * Helper function to add $this->url as a query parameter.
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
      $query = $this->getQuery();
      $query += array(
        'petition_status' => new MongoInt64($status),
      );
      $this->setQuery($query);
    }
    else {
      $query = $this->getQuery();
      $query += array(
        'petition_status' => array('$ne' => new MongoInt64(WH_PETITION_STATUS_FLAGGED)),
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
}
