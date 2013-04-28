<?php
/**
 * @file
 * File containing class that accesses Petitions data.
 */

class PetitionsBase {

  protected $collection;
  protected $baseURL;
  protected $executionTime;
  protected $result;
  protected $query = array();

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
   * Get accessor for $this->collection.
   *
   * @return array
   *   MongoCollection resource for querying against a collection.
   */
  protected function getCollection() {
    return $this->collection;
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
   * Get accessor for $this->executionTime.
   *
   * @return int
   *   Epoch (UNIX style) time stamp.
   */
  protected function getExecutionTime() {
    return $this->executionTime;
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
   * Get accessor for $this->query.
   *
   * @return array
   *   Results of query to MongoDB.
   */
  protected function getQuery() {
    return $this->query;
  }

  /**
   * Helper function to calculate the execution time of the load function.
   *
   * @return string
   *   Unique string identifier for a petition.
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
        'name' => $this->filterUTF16($term->name),
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
      'associationTime' => $created_time,
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

  /**
   * Helper function to format characters in a string.
   *
   * @param string $string
   *   The raw input string.
   *
   * @return string
   *   Formatted output string.
   */
  protected function filterUTF16($string) {
    // Replace non-english characters with x's.
    $string = preg_replace('/[^\00-\255]+/u', 'x', $string);

    return $string;
  }
}
