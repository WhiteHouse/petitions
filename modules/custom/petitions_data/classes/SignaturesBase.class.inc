<?php
/**
 * @file
 * File containing class that accesses data using mongodb queries.
 */

class SignaturesBase {

  protected $city;
  protected $state;
  protected $zipcode;
  protected $country;
  protected $startDate;
  protected $endDate;
  protected $createdDate;
  protected $limit;
  protected $maxReturnLimit;
  protected $offset;
  protected $nameFormat;
  protected $locationFormat;
  protected $query = array();
  protected $collection;
  protected $count;
  protected $result;

  /**
   * Set accessor for $this->city.
   *
   * @param string $city
   *   City name for signature.
   *
   * @return SignaturesMongoRaw
   *   Returns current instance of object.
   */
  public function setCity($city) {
    $this->city = $city;

    return $this;
  }

  /**
   * Set accessor for $this->state.
   *
   * @param string $state
   *   State name for signature.
   *
   * @return SignaturesMongoRaw
   *   Returns current instance of object.
   */
  public function setState($state) {
    $this->state = $state;

    return $this;
  }

  /**
   * Set accessor for $this->zipcode.
   *
   * @param int $zipcode
   *   Postal code to identify locations within the United States.
   *
   * @return SignaturesMongoRaw
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
   * @param string $country
   *   Country name for signature.
   *
   * @return SignaturesMongoRaw
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
   * @return SignaturesMongoRaw
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
   * @return SignaturesMongoRaw
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
   * @return SignaturesMongoRaw
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
   *   Number of results to return.
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
   * Set accessor for $this->maxReturnLimit.
   */
  public function setMaxReturnLimit($max_return_limit) {
    if (is_numeric($max_return_limit)) {
      $this->maxReturnLimit = $max_return_limit;
    }
    return $this;
  }

  /**
   * Set accessor for $this->offset.
   *
   * @param int $offset
   *   Number of results to skip before return.
   *
   * @return SignaturesMongoRaw
   *   Returns current instance of object.
   */
  public function setOffset($offset) {
    if (is_numeric($offset)) {
      $this->offset = $offset;
    }
    return $this;
  }

  /**
   * Set accessor for $this->nameFormat.
   *
   * @param string $name_format
   *   Function used to format name values.
   *
   * @return SignaturesMongoRaw
   *   Returns current instance of object.
   */
  public function setNameFormat($name_format) {
    $this->nameFormat = $name_format;
    return $this;
  }
  /**
   * Set accessor for $this->locationFormat.
   *
   * @param string $location_format
   *   Function used to format location values.
   *
   * @return SignaturesMongoRaw
   *   Returns current instance of object.
   */
  public function setLocationFormat($location_format) {
    $this->locationFormat = $location_format;
    return $this;
  }

  /**
   * Set accessor for $this->query.
   *
   * @param array $query
   *   Array of query parameters to get passed to mongodb.
   *
   * @return SignaturesMongoRaw
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
   * @return SignaturesMongoRaw
   *   Returns current instance of object.
   */
  protected function setCollection($collection) {
    $this->collection = $collection;
    return $this;
  }

  /**
   * Set accessor for $this->result.
   *
   * @param array $result
   *   Processed results of query to MongoDB.
   *
   * @return SignaturesMongoRaw
   *   Returns current instance of object.
   */
  protected function setResult($result) {
    $this->result = $result;

    return $this;
  }

  /**
   * Set accessor for $this->result.
   *
   * @param int $count
   *   Number of results of query to MongoDB.
   *
   * @return SignaturesMongoRaw
   *   Returns current instance of object.
   */
  protected function setCount($count) {
    $this->count = $count;
    return $this;
  }

  /**
   * Get accessor for $this->city.
   *
   * @return string
   *   City name for signature.
   */
  protected function getCity() {
    return $this->city;
  }

  /**
   * Get accessor for $this->state.
   *
   * @return string
   *   State name for signature.
   */
  protected function getState() {
    return $this->state;
  }

  /**
   * Get accessor for $this->zipcode.
   *
   * @return int
   *   Postal code to identify locations within the United States.
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
   *   Number of results to return.
   */
  protected function getLimit() {

    if (!intval($this->limit)) {
      return $this->maxReturnLimit;
    }

    return intval($this->limit);
  }

  /**
   * Get accessor for $this->maxReturnLimit.
   *
   * @return int
   *   Max number of results to return
   */
  protected function getMaxReturnLimit() {
    return intval($this->maxReturnLimit);
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
   * Get accessor for $this->nameFormat.
   *
   * @return string
   *   Function used to format name values.
   */
  protected function getNameFormat() {
    return $this->nameFormat;
  }

  /**
   * Get accessor for $this->locationFormat.
   *
   * @return string
   *   Function used to format location values.
   */
  protected function getLocationFormat() {
    return $this->locationFormat;
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
   * Get accessor for $this->collection.
   *
   * @return array
   *   MongoCollection resource for querying against a collection.
   */
  protected function getCollection() {
    return $this->collection;
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
   * Get accessor for $this->count.
   *
   * @return int
   *   Number of results of query to MongoDB.
   */
  public function getcount() {
    return $this->count;
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
   * Helper function to format first and last name.
   *
   * @param string $username
   *   Username of petition signer.
   * @param string $first_name
   *   First name of petition signer.
   * @param string $last_name
   *   Last name of petition signer.
   *
   * @return string
   *   Formatted name.
   */
  protected function formatReturnName($username, $first_name, $last_name) {
    if ($this->getNameFormat()) {
      $function = $this->getNameFormat();
      $name = petitions_data_sanitize_output($function($username, $first_name, $last_name));
    }
    else {
      $first_name = petitions_data_sanitize_output($first_name);
      $last_name = petitions_data_sanitize_output($last_name);

      // Grab first letter of first and last name.
      $name = substr($first_name, 0, 1) . substr($last_name, 0, 1);

      // Set default empty value for name.
      if (empty($name)) {
        $name = ' ';
      }
    }

    return $name;
  }

  /**
   * Helper function to format locations.
   *
   * @param int $zipcode
   *   Zip code of petition signer.
   * @param string $city
   *   City of petition signer.
   * @param string $state
   *   State of petition signer.
   *
   * @return array
   *   Array of location values.
   */
  protected function formatReturnLocation($zipcode, $city, $state) {
    if ($this->getLocationFormat()) {
      $function = $this->getLocationFormat();
      $location = $function($zipcode, $city, $state, 'api');
    }
    else {
      $location = array(
        'city' => $city,
        'state' => $state,
        'zipcode' => $zipcode,
      );
    }

    return $location;
  }
}
