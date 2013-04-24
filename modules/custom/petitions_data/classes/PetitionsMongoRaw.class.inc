<?php
/**
 * @file
 * File containing class that accesses data using mongodb queries.
 */

class PetitionsMongoRaw implements IPetitionsRaw {

  protected $startDate;
  protected $endDate;
  protected $createdDate;
  protected $max_return_limit = 1000;
  protected $limit;
  protected $offset;
  protected $query = array();
  protected $collection;
  protected $executionTime;
  protected $result;
  protected $baseURL;

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
      if ($limit > $this->max_return_limit) {
        $limit = $this->max_return_limit;
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
    $this->baseURL = variable_get('petitions_data_petitions_website_url', $base_url);

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

    if(!intval($this->limit)){
      return $this->max_return_limit;
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
    $this->setCollection(mongodb_collection('petitions'));
    $this->setExecutionTime(microtime());

    // @TODO: This really should be added as a publicly accessible method.
    $this->setQuery(
      array(
        'signature_count' => array(
          '$gt' => new MongoInt64(150)
        ),
        'petition_status' => array('$ne' => new MongoInt64(WH_PETITION_STATUS_FLAGGED)) 
      )
    );
  }

  /**
   * Primary processing function of class. Makes query to MongoDB.
   *
   * @return array
   *   Results of query to MongoDB.
   */
  public function load() {
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
    $results = $this->getCollection()
      ->find($query, $fields)
      ->limit($this->getLimit())
      ->skip($this->getOffset())
      ->sort(array('created' => -1));

    if (!$results->hasNext()) {
      $response['noresults'] = 1;
      $response['metadata']['execution time'] = $this->calcExecutionTime();
      return $response;
    }

    foreach ($results as $doc) {

      $petition = array(
        'id' => $this->formatReturnId($doc['_id']),
        'type' => 'petition',
        'title' => $doc['title'],
        'body' => $doc['body'],
        'issues' => $this->formatReturnIssues($doc['issues']),
        'signature threshold' => $doc['response_signatures'],
        'signature count' => $doc['signature_count'],
        'signatures needed' => $this->calcSignaturesNeeded($doc['response_signatures'], $doc['signature_count']),
        'url' => $this->formatReturnUrl($doc['nice_url']),
        'deadline' => $this->calcDeadline($doc['created'], $doc['review_timeframe']),
        'status' => $this->formatReturnPetitionStatus($doc['petition_status']),
        'response' => $doc['response_id'] ? $this->formatReturnResponse($doc['response_id']) : NULL,
        'created' => $doc['created'],
      );

      $response['results'][] = $petition;
    }
    
        // Set the result set information.
    $response['metadata']['resultset'] = array(
      'count' => $results->count(),
      'offset' => $this->getOffset(),
      'limit' => $this->getLimit(),
    );

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
      $row = array();
      $term = taxonomy_term_load($issue_id);
      $row = array(
        'id' => $term->tid,
        'name' => $term->name,
      );
      $terms[] = $row;
    }
    return $terms;
  }

  /**
   * Helper function to format URL.
   */
  protected function formatReturnURL($uri) {
    return $this->getBaseURL() . '/' . $uri;
  }

  /**
   * Helper function to calculate needed signatures.
   *
   * @return int
   *   Signatures remaining to required to reach response threshold.
   */
  protected function calcSignaturesNeeded($total, $current) {
    return ($current >= $total) ? 0 : $total - $current;
  }

  /**
   * Helper function to calculate deadline.
   */
  protected function calcDeadline($created, $days) {
    return strtotime("+" . $days . " days", $created);
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
  protected function formatReturnResponse($response_id = '') {

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
      'association time' => $created_time,
    );

    return $response;
  }

  /**
   * Helper function to translate status codes into readable string formats.
   *
   * @param int $status_code
   *   Status code as stored in mongodb.
   *
   * @return string
   *   String representing status code.
   */
  protected function formatReturnPetitionStatus($status_code) {

    switch ($status_code) {

      // Has the required number of signatures to become public,
      // visible everywhere on site.
      case WH_PETITION_STATUS_PUBLIC:
        return "open";
      break;

      // Has received the required number of signatures, visible on site.
      case WH_PETITION_STATUS_UNDER_REVIEW:
        return "pending response";
      break;

      // Has received a response.
      case WH_PETITION_STATUS_REVIEWED:
        return "responded";
      break;
      
      default:
        return "closed";
    }
  }
}
