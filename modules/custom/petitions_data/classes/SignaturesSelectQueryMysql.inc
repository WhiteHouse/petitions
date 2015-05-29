<?php

/**
 * @file
 * Contains SignaturesSelectQueryMysql.
 */

/**
 * Class SignaturesSelectQueryMysql.
 *
 * This should contain MySQL-specific logic for querying signatures.
 */
class SignaturesSelectQueryMysql extends SignaturesSelectQuery {

  /**
   * The query object for this query.
   *
   * @var EntityFieldQueryExtraFields
   */
  protected $query;

  /**
   * OrderBy ASC/DESC mapping
   *
   * @var array
   */
  protected $orderByDirMap = array(
    self::SELECT_QUERY_ORDER_BY_ASC => 'ASC',
    self::SELECT_QUERY_ORDER_BY_DESC => 'DESC',
  );

  /**
   * OrderBy MySQL Field mapping
   *
   * Maps Order By constants to appropriate database column name.
   *
   * @var array
   *   An array containing 'field' and 'column' elements. For node columns, only
   *   'field' is required.
   */
  protected $orderByFieldsMap = array(
    self::SELECT_QUERY_ORDER_BY_FIELD_ID => array('field' => 'id'),
    self::SELECT_QUERY_ORDER_BY_FIELD_TIMESTAMP => array('field' => 'timestamp'),
  );

  /**
   * {@inheritdoc}
   */
  public function execute() {
    try {
      $this->buildQuery();
      $entities = $this->executeQuery();
      $this->buildResult($entities);
    }
    catch (Exception $e) {
      watchdog('petitions_data', 'Exception in SignaturesSelectQueryMysql::execute(): !e', array(
        '!e' => petitionslog_format_for_watchdog($e),
      ));
    }
    return $this;
  }

  /**
   * Builds the query.
   */
  protected function buildQuery() {
    $this->initializeQuery();
    $this->addArgumentsToQuery();
  }

  /**
   * Initializes the basic query.
   */
  protected function initializeQuery() {
    $this->query = new EntityFieldQueryExtraFields();
    $this->query
      ->entityCondition('entity_type', 'signature_mail')
      ->addExtraField('', 'legacy_id', 'legacy_id', 'signature_mail')
      ->addExtraField('', 'timestamp', 'timestamp', 'signature_mail')
      ->addExtraField('', 'petition_id', 'petition_id', 'signature_mail')
      ->addExtraField('', 'legacy_petition_id', 'legacy_petition_id', 'signature_mail')
      ->addExtraField('', 'uid', 'uid', 'signature_mail')
      ->addExtraField('', 'user_username', 'user_username', 'signature_mail')
      ->addExtraField('', 'user_first_name', 'user_first_name', 'signature_mail')
      ->addExtraField('', 'user_last_name', 'user_last_name', 'signature_mail')
      ->addExtraField('', 'user_city', 'user_city', 'signature_mail')
      ->addExtraField('', 'user_state', 'user_state', 'signature_mail')
      ->addExtraField('', 'user_zip', 'user_zip', 'signature_mail')
      ->addExtraField('', 'user_country', 'user_country', 'signature_mail');
  }

  /**
   * Adds the orderBy arguments to the query.
   */
  protected function addOrderByToQuery() {
    if (!$this->getOrderBy()) {
      return;
    }

    // Order by on signature_mail shouldn't be done in MySQL, throw exception.
    throw new Exception('addOrderByToQuery - attempting to use setOrderBy with MySQL on SignatureSelectQuery');

    // Uncomment below and remove exception to restore order by ability.

    /*
    foreach ($this->getOrderBy() as $order_by) {
      $field = $order_by->field;
      $order_by_dir = $order_by->direction;
      if (!$this->isValidOrderByField($field)) {
        throw new Exception('addOrderByToQuery - Not a valid field: ' . $field);
      }
      if (!$this->isValidOrderByDirection($order_by_dir)) {
        throw new Exception('addOrderByToQuery - Not a valid direction: ' . $order_by_dir);
      }

      $mapped_field = $this->orderByFieldsMap[$field]['field'];
      $mapped_dir = $this->orderByDirMap[$order_by_dir];
      $this->query->propertyOrderBy($mapped_field, $mapped_dir);
    }
    */
  }

  /**
   * Adds the supplied arguments to the query.
   */
  protected function addArgumentsToQuery() {
    $this->addUidToQuery();
    $this->addCityToQuery();
    $this->addStateToQuery();
    $this->addZipcodeToQuery();
    $this->addCountryToQuery();
    $this->addPetitionIdsToQuery();
    $this->addOrderByToQuery();
    $this->addOffsetAndLimitToQuery();
  }

  /**
   * Adds the uid argument to the query.
   */
  protected function addUidToQuery() {
    if ($this->getUid()) {
      $this->query->propertyCondition('uid', $this->getUid());
    }
  }

  /**
   * Adds the city argument to the query.
   */
  protected function addCityToQuery() {
    if ($this->getCity()) {
      $this->query->propertyCondition('user_city', $this->getCity());
    }
  }

  /**
   * Adds the state argument to the query.
   */
  protected function addStateToQuery() {
    if ($this->getState()) {
      $this->query->propertyCondition('user_state', $this->getState());
    }
  }

  /**
   * Adds the zipcode argument to the query.
   */
  protected function addZipcodeToQuery() {
    if ($this->getZipCode()) {
      $this->query->propertyCondition('user_zip', $this->getZipCode());
    }
  }

  /**
   * Adds the country argument to the query.
   */
  protected function addCountryToQuery() {
    if ($this->getCountry()) {
      $this->query->propertyCondition('user_country', $this->getCountry());
    }
  }

  /**
   * Adds the petition_id argument to the query.
   */
  protected function addPetitionIdsToQuery() {
    $petition_ids = $this->getPetitionIds();
    if (!empty($petition_ids)) {
      $entity_ids = petition_extract_nids_from_petition_ids($petition_ids);
      if (!empty($entity_ids)) {
        $this->query->propertyCondition('petition_id', $entity_ids, 'IN');
      }

      $legacy_ids = petition_extract_legacy_ids_from_petition_ids($petition_ids);
      if (!empty($legacy_ids)) {
        $this->query->propertyCondition('legacy_petition_id', $legacy_ids, 'IN');
      }
    }
  }

  /**
   * Adds the offset and limit argument to the query.
   */
  protected function addOffsetAndLimitToQuery() {
    $this->query->range($this->getOffset(), $this->getLimit());
  }

  /**
   * Executes the query.
   *
   * @return array
   *   An array of nodes with field data, or an empty array if there are no
   *   results.
   */
  protected function executeQuery() {
    $result = $this->query->addTag('petitions_data_debug')->execute();

    // Return early if no results.
    if (empty($result['signature_mail'])) {
      return array();
    }

    $entities = $result['signature_mail'];
    return $entities;
  }

  /**
   * Builds the results array from the query results.
   *
   * @param array $entities
   *   An array of signature data, as returned by
   *   SignaturesSelectQueryMysql::executeQuery.
   */
  protected function buildResult(array $entities) {
    $result_arrays = array();
    $result_objects = array();
    foreach ($entities as $entity) {
      $fields = $entity->extraFields;

      $signature = new SignatureItem();
      $signature
        ->setPetitionId($fields->petition_id)
        ->setUid($fields->uid)
        ->setId($fields->entity_id)
        ->setCreated($fields->timestamp)
        ->setFirstName($fields->user_first_name)
        ->setLastName($fields->user_last_name)
        ->setCity($fields->user_city)
        ->setState($fields->user_state)
        ->setZip($fields->user_zip);
      if (!empty($fields->legacy_id)) {
        $signature->setLegacyId($fields->legacy_id);
      }
      if (!empty($fields->legacy_petition_id)) {
        $signature->setLegacyPetitionId($fields->legacy_petition_id);
      }

      $result_objects[] = $signature;
      $result_arrays[] = $signature->toRestResponseItemArray();
    }

    $this->setResultObjects($result_objects);
    $this->setResult($result_arrays);

  }

}
