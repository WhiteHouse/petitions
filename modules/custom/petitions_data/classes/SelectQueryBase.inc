<?php

/**
 * @file
 * Contains SelectQueryBase.
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
  protected $resultObjects;
  protected $count;
  protected $orderBy = array();

  const SELECT_QUERY_ORDER_BY_ASC = 'ASC';
  const SELECT_QUERY_ORDER_BY_DESC = 'DESC';

  /**
   * These constants are used by the setOrderBy function.
   * Mapping is done in *SelectQueryMongo.inc and
   * *SelectQueryMysql.inc classes to determine the column names
   * for each one of these constants.
   */

  const SELECT_QUERY_ORDER_BY_FIELD_ID = 'ID';
  const SELECT_QUERY_ORDER_BY_FIELD_TITLE = 'TITLE';
  const SELECT_QUERY_ORDER_BY_FIELD_DATE_CREATED = 'DATE_CREATED';

  protected $validOrderByDirections = array(
    self::SELECT_QUERY_ORDER_BY_ASC,
    self::SELECT_QUERY_ORDER_BY_DESC,
  );


  /**
   * Valid common fields for order by
   *
   * NOTE: Adding any additional fields requires adding of indexes to database.
   * Use of a new field without indexes will cause ADVERSE performance issues.
   *
   * @var array
   */
  protected $validOrderByFields = array(
    self::SELECT_QUERY_ORDER_BY_FIELD_ID,
    self::SELECT_QUERY_ORDER_BY_FIELD_TITLE,
    self::SELECT_QUERY_ORDER_BY_FIELD_DATE_CREATED,
  );

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
   * Gets the result of the query as an array of objects.
   *
   * @return PetitionItem[] | SignatureItem[]
   *   An array of PetitionItem or SignatureItem objects.
   */
  public function getResultObjects() {
    return $this->resultObjects;
  }

  /**
   * Sets the result of the query as an array of objects.
   *
   * @param PetitionItem[] $result_objects
   *   An array of PetitionItem objects.
   *
   * @return $this
   */
  protected function setResultObjects(array $result_objects) {
    $this->resultObjects = $result_objects;
    return $this;
  }

  /**
   * Get accessor for $this->orderBy.
   *
   * @return array
   *   The array of orderBy stdClass objects.
   *   orderBy objects contain properties field and direction
   */
  public function getOrderBy() {
    return $this->orderBy;
  }

  /**
   * Set accessor for $this->orderBy.
   *
   * @param string $order_by_field
   *   Field to order by, such as SelectQueryBase::SELECT_QUERY_ORDER_BY_FIELD_TITLE
   *
   * @param string $order_by_dir
   *   Order by direction, such as SelectQueryBase::SELECT_QUERY_ORDER_BY_DESC
   *
   * @param bool $append
   *   If TRUE, will append to orderBy array, otherwise will replace
   *
   * @return SelectQueryBase
   *   Returns current instance of object.
   */
  public function setOrderBy($order_by_field, $order_by_dir, $append = FALSE) {
    $order_by_obj = new stdClass();
    $order_by_obj->field = $order_by_field;
    $order_by_obj->direction = $order_by_dir;

    if ($append) {
      $this->orderBy = array_merge($this->orderBy, array($order_by_obj));
    }
    else {
      $this->orderBy = array($order_by_obj);
    }
    return $this;
  }

  /**
   * Getter accessor for $this->validOrderByFields.
   *
   * @return array
   *   The array of validOrderByFields
   */
  public function getValidOrderByFields() {
    return $this->validOrderByFields;
  }

  /**
   * Set accessor for $this->validOrderByFields.
   *
   * @param array $order_by_fields
   *   Array of valid fields.
   *
   * @return SelectQueryBase
   *   Returns current instance of object.
   */
  protected function setValidOrderByFields($order_by_fields) {
    $this->validOrderByFields = $order_by_fields;
    return $this;
  }

  /**
   * Getter accessor for $this->validOrderByDirections.
   *
   * @return array
   *   The array of validOrderByFields
   */
  public function getValidOrderByDirections() {
    return $this->validOrderByDirections;
  }

  /**
   * Set accessor for $this->validOrderByDirections.
   *
   * @param array $order_by_dirs
   *   Array of valid directions.
   *
   * @return SelectQueryBase
   *   Returns current instance of object.
   */
  protected function setValidOrderByDirections($order_by_dirs) {
    $this->validOrderByDirections = $order_by_dirs;
    return $this;
  }

  /**
   * Checks if field is a valid order by field.
   *
   * @param string $field
   *   Field to validate
   *
   * @return bool
   *   TRUE if field is valid
   */
  protected function isValidOrderByField($field) {
    return in_array($field, $this->validOrderByFields);
  }

  /**
   * Checks if direction is a valid order by ASC/DESC.
   *
   * @param string $direction
   *   Direction to validate
   *
   * @return bool
   *   TRUE if direction is valid
   */
  protected function isValidOrderByDirection($direction) {
    return in_array($direction, $this->validOrderByDirections);
  }

  /**
   * Executes the query and sets $this->result and $this->count.
   *
   * @return object
   *   Returns current instance of object, required for chaining.
   */
  abstract public function execute();
}
