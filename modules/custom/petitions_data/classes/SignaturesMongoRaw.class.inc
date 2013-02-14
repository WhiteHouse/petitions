<?php
/**
 * @file
 * File containing class that accesses data using mongodb queries.
 */

class SignaturesMongoRaw implements iSignaturesRaw {

  protected $zipcode;
  protected $country;
  protected $startDate;
  protected $endDate;
  protected $createdDate;
  protected $limit = 100;
  protected $offset;
  protected $query = array();
  protected $collection;
  protected $executionTime;
  protected $result;
  protected $baseURL;

  /**
   * Set accessor for $this->zipcode.
   *
   * @param int $zipcode
   *   Postal code to identify locations within the United States
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setZipCode($zipcode) {
    if (is_numeric($zipcode)) {
      $this->zipcode = $zipcode;
    }
    return $this;
  }

  /**
   * Set accessor for $this->country.
   *
   * @return string
   *   Country name for signature.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setCountry($country) {
    $this->country = $country;

    return $this;
  }

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
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setLimit($limit) {
    if (is_numeric($limit)) {
      if ($limit > 100) {
        $limit = 100;
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
   * Set accessor for $this->executionTime.
   *
   * @param int $time
   *   Epoch (UNIX style) time stamp.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  protected function setExecutionTime($time) {
    $this->executionTime = $time;

    return $this;
  }

  /**
   * Set accessor for $this->result.
   *
   * @param array $result
   *   Processed results of query to MongoDB.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  protected function setResult($result) {
    $this->result = $result;

    return $this;
  }

  /**
   * Set accessor for $this->base_url.
   *
   * @param array $base_url
   *   The base url for the site that absolute URLs will be constructed with.
   *
   * @return PetitionsMongoRaw
   *   Returns current instance of object.
   */
  public function setBaseURL($base_url) {
    $this->baseURL = $base_url;

    return $this;
  }

  /**
   * Get accessor for $this->zipcode.
   *
   * @return int
   *   Postal code to identify locations within the United States
   */
  protected function getZipCode() {
    return intval($this->zipcode);
  }

  /**
   * Get accessor for $this->country.
   *
   * @return string
   *   Country name for signature.
   */
  protected function getCountry() {
    return $this->country;
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
   * Get accessor for $this->collection.
   *
   * @return array
   *   MongoCollection resource for querying against a collection.
   */
  protected function getCollection() {
    return $this->collection;
  }

  /**
   * Get accessor for $this->executionTime.
   *
   * @return int
   *   Epoch (UNIX style) time stamp.
   */
  protected function getExecutionTime() {
    return $this->executionTime;
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
   * Get accessor for $this->result.
   *
   * @return array
   *   Results of query to MongoDB.
   */
  public function result() {
    return $this->result;
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
   * Constructor.
   */
  public function __construct() {
    $this->setCollection(mongodb_collection('petition_signatures'));
    $this->setExecutionTime(microtime());

    // @TODO: This really should be added as a publicly accessible method.
    $this->setQuery(
      array(
        'petition_status' => array(
          '$in' => array(
            WH_PETITION_STATUS_PUBLIC,
            WH_PETITION_STATUS_UNDER_REVIEW,
            WH_PETITION_STATUS_REVIEWED,
          ),
        ),
      )
    );
  }

  /**
   * Primary processing function of class. Makes query to MongoDB.
   *
   * @return array
   *   Results of query to MongoDB.
   */
  public function load($petition_id) {
    $response = array();
    $response['metadata'] = array();
    $response['results'] = array();

    if ($this->getCreatedDate()) {
      $this->addCreatedDateToQuery();
    }
    else {
      $this->addStartDateToQuery();
      $this->addEndDateToQuery();
    }

    $this->addZipToQuery();
    $this->addCountryToQuery();

    $fields = array(
      'user.first_name',
      'user.last_name',
      'user.city',
      'user.state',
      'user.zip',
      'timestamp',
    );

    $query = $this->getQuery();
    $query += array(
      'petition_id' => $petition_id,
    );

    $results = $this->getCollection()
      ->find($query, $fields)
      ->limit($this->getLimit())
      ->skip($this->getOffset())
      ->sort(array('timestamp' => -1));

    foreach ($results as $doc) {

      $signature = array();
      $signature = array(
        'id' => $this->formatReturnId($doc['_id']),
        'type' => 'signature',
        'name' => $this->formatReturnName($doc['user.first_name'], $doc['user.last_name']),
        'city' => $doc['user.city'],
        'state' => $doc['user.state'],
        'zip' => $doc['user.zip'],
        'created' => $doc['timestamp'],
      );

      $response['results'][] = $signature;
    }

    $response['results'] = "load method is working";

    $response['metadata']['execution time'] = $this->calcExecutionTime();

    $this->setResult($response);
    return $response;
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
   * Helper function to add $this->zipcode as a query parameter.
   */
  protected function addZipToQuery() {
    if ($this->getZipCode()) {
      $query = $this->getQuery();
      $query += array(
        'user.zip' => $this->getZipCode(),
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
        'user.country' => $this->getQuery(),
      );
      $this->setQuery($query);
    }
  }

  /**
   * Helper function to calculate the execution time of the load function.
   */
  protected function calcExecutionTime() {
    return microtime() - $this->getExecutionTime();
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
