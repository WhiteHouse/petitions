<?php

/**
 * @file
 * Contains SignaturesQueryMongo.
 */

/**
 * Class SignaturesSelectQueryMongo.
 *
 * This should contain MongoDB-specific logic for querying signatures.
 */
class SignaturesSelectQueryMongo extends SignaturesSelectQuery {

  protected $query = array();
  protected $collection;

  /**
   * {@inheritdoc}
   */
  public function __construct() {
    parent::__construct();
    try {
      $collection = mongodb_collection('petition_signatures');
    }
    catch (Exception $e) {
      petitionslog_event('exception.petitions_data.c968b66');
    }
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
   * @return array
   *   Results of query to MongoDB.
   */
  public function execute() {
    $result_arrays = array();
    $result_objects = array();

    if ($this->getCreatedDate()) {
      $this->addCreatedDateToQuery();
    }
    else {
      $this->addStartDateToQuery();
      $this->addEndDateToQuery();
    }

    $this->addUidToQuery();
    $this->addCityToQuery();
    $this->addStateToQuery();
    $this->addZipToQuery();
    $this->addCountryToQuery();
    $this->addPetitionIdsToQuery();
    $this->addSignatureIdsToQuery();

    $fields = array(
      'uid',
      'user.first_name',
      'user.last_name',
      'user.city',
      'user.state',
      'user.zip',
      'timestamp',
      'petition_id',
      // number,
      // abuse_flags,
      // comment,
      // ip_address,
      // user_agent,
    );

    $query = $this->getQuery();
    $mongo_results = $this->getCollection()
      ->find($query, $fields)
      ->limit($this->getLimit())
      ->skip($this->getOffset())
      ->sort(array('timestamp' => -1));

    foreach ($mongo_results as $doc) {
      $signature = new SignatureItem();
      $signature
        ->setLegacyPetitionId($doc['petition_id'])
        ->setUid($doc['uid'])
        ->setId($this->formatReturnId($doc['_id']))
        ->setCreated($doc['timestamp']);

      // The follow user properties are optional.
      if (!empty($doc['user']['first_name'])) {
        $signature->setFirstName($doc['user']['first_name']);
      }
      if (!empty($doc['user']['last_name'])) {
        $signature->setLastName($doc['user']['last_name']);
      }
      if (!empty($doc['user']['city'])) {
        $signature->setCity($doc['user']['city']);
      }
      if (!empty($doc['user']['state'])) {
        $signature->setState($doc['user']['state']);
      }
      if (!empty($doc['user']['zip'])) {
        $signature->setZip($doc['user']['zip']);
      }

      $result_objects[] = $signature;
      $result_arrays[] = $signature->toRestResponseItemArray();
    }
    $this->setCount($mongo_results->count());
    $this->setResult($result_arrays);
    $this->setResultObjects($result_objects);

    return $this;
  }

  /**
   * Helper function to add $this->city as a query parameter.
   */
  protected function addCityToQuery() {
    if ($this->getCity()) {
      $query = $this->getQuery();
      $query += array(
        'user.city' => new MongoRegex("/^" . trim($this->getCity()) . "$/i"),
      );
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->state as a query parameter.
   */
  protected function addStateToQuery() {
    if ($this->getState()) {
      $query = $this->getQuery();
      $query += array(
        'user.state' => new MongoRegex("/" . $this->getState() . "/i"),
      );
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->uid as a query parameter.
   */
  protected function addUidToQuery() {
    if ($this->getUid()) {
      $query = $this->getQuery();
      $query += array('uid' => $this->getUid());
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->zipcode as a query parameter.
   */
  protected function addZipToQuery() {
    if ($this->getZipCode()) {
      $query = $this->getQuery();
      $query += array(
        'user.zip' => "{$this->getZipCode()}",
      );
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to add $this->country as a query parameter.
   */
  protected function addCountryToQuery() {
    if ($this->getCountry()) {
      $query = $this->getQuery();
      $query += array(
        'user.country' => new MongoRegex("/" . $this->getCountry() . "/i"),
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
        $query['timestamp'] = array();
      }
      $query['timestamp'] += array('$gt' => $this->getStartDate());

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
        $query['timestamp'] = array();
      }
      $query['timestamp'] += array('$lt' => $this->getEndDate());
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
        'timestamp' => $this->getCreatedDate(),
      );
      $this->setQuery($query);
    }
  }

  /**
   * Helper function filter results by petition ids.
   */
  protected function addPetitionIdsToQuery() {
    if ($this->getPetitionIds()) {
      $query = $this->getQuery();
      $query += array(
        'petition_id' => array('$in' => $this->getPetitionIds()),
      );
      $this->setQuery($query);
    }
  }

  /**
   * Helper function filter results by signature ids.
   */
  protected function addSignatureIdsToQuery() {
    if ($this->getSignatureIds()) {
      $sids = $this->getSignatureIds();
      $mongo_ids = array();
      foreach ($sids as $sid) {
        $mongo_ids[] = new MongoId($sid);
      }
      $query = $this->getQuery();
      $query += array(
        '_id' => array('$in' => $mongo_ids),
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
