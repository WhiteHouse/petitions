<?php

/**
 * @file
 * Defines SelectQueryBase.
 */

/**
 * Class SelectQueryBase
 *
 * This should contain methods for querying petitions OR signatures. It should
 * not contain anything specific to a particular object class or backend.
 */
abstract class SelectQueryBase {

  protected $limit;
  protected $maxReturnLimit;
  protected $offset;
  protected $result;
  protected $count;

  /**
   * Constructor. This must be explicitly called by subclasses.
   */
  public function __construct() {
    $this->setMaxReturnLimit();
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
   * Set accessor for $this->limit.
   *
   * @param int $limit
   *   Number of results to return
   *
   * @return SelectQueryBase
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
   * Get accessor for $this->maxReturnLimit.
   *
   * @return int
   *   Max number of results to return
   */
  protected function getMaxReturnLimit() {
    return intval($this->maxReturnLimit);
  }

  /**
   * Set accessor for $this->maxReturnLimit.
   *
   * @param int $max_return_limit
   *   The maximum number of results that may be returned, regardless of limit.
   *
   * @return SelectQueryBase
   *   Returns current instance of object.
   */
  public function setMaxReturnLimit($max_return_limit = 100) {
    if (is_numeric($max_return_limit)) {
      $this->maxReturnLimit = $max_return_limit;
    }
    return $this;
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
   * Set accessor for $this->offset.
   *
   * @param int $offset
   *   Number of results to skip before return.
   *
   * @return SelectQueryBase
   *   Returns current instance of object.
   */
  public function setOffset($offset) {
    if (is_numeric($offset)) {
      $this->offset = $offset;
    }
    return $this;
  }


  /**
   * Get accessor for $this->count.
   *
   * @return int
   *   Number of results of query.
   */
  public function getCount() {
    return $this->count;
  }

  /**
   * Set accessor for $this->count.
   *
   * @param int $count
   *   Number of results of query.
   *
   * @return SelectQueryBase
   *   Returns current instance of object.
   */
  protected function setCount($count) {
    $this->count = $count;
    return $this;
  }

  /**
   * Get accessor for $this->result.
   *
   * @return array
   *   Results of query.
   */
  public function getResult() {
    return $this->result;
  }

  /**
   * Set accessor for $this->result.
   *
   * @param array $result
   *   Processed results of query.
   *
   * @return SelectQueryBase
   *   Returns current instance of object.
   */
  protected function setResult($result) {
    $this->result = $result;
    return $this;
  }

  /**
   * Executes the query and sets $this->result and $this->count.
   *
   * @return object
   *   Returns current instance of object, required for chaining.
   */
  abstract public function execute();
}
