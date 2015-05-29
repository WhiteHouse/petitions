<?php

/**
 * @file
 * Contains SignaturesSelectQuerySolr.
 */

/**
 * Apache Solr signatures select query class.
 */
class SignaturesSelectQuerySolr extends SignaturesSelectQuery {

  /**
   * An array of Solr search parameters.
   *
   * @var array
   */
  protected $params = array();

  /**
   * The Solr query object.
   *
   * @var DrupalSolrQueryInterface
   */
  protected $query;

  /**
   * OrderBy ASC/DESC mapping
   *
   * @var array
   */
  protected $orderByDirMap = array(
    self::SELECT_QUERY_ORDER_BY_ASC => 'asc',
    self::SELECT_QUERY_ORDER_BY_DESC => 'desc',
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
    self::SELECT_QUERY_ORDER_BY_FIELD_ID => array('field' => SIGNATURE_SOLR_FIELD_ID),
    self::SELECT_QUERY_ORDER_BY_FIELD_TIMESTAMP => array('field' => SIGNATURE_SOLR_FIELD_TIMESTAMP),
  );

  /**
   * The Solr sort parameter.
   *
   * @var string
   */
  protected $sortString = '';

  /**
   * {@inheritdoc}
   */
  public function execute() {
    try {
      $this->buildQuery();
      $response = $this->executeQuery();
      $this->buildResults($response);
    }
    catch (Exception $e) {
      watchdog('petitions_data', 'Exception in SignaturesSelectQuerySolr::execute(): !e', array(
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
    $this->addQueryParam('bundle', 'signature_mail');
  }

  /**
   * Adds User ID to the query.
   */
  protected function addUidToQuery() {
    $uid = $this->getUid();
    if (!empty($uid)) {
      $this->addQueryParam(SIGNATURE_SOLR_FIELD_UID, $uid);
    }
  }

  /**
   * Adds User City to the query.
   */
  protected function addCityToQuery() {
    $city = $this->getCity();
    if (!empty($city)) {
      $this->addQueryParam(SIGNATURE_SOLR_FIELD_USER_CITY, $city);
    }
  }

  /**
   * Adds User State to the query.
   */
  protected function addStateToQuery() {
    $state = $this->getState();
    if (!empty($state)) {
      $this->addQueryParam(SIGNATURE_SOLR_FIELD_USER_STATE, $state);
    }
  }

  /**
   * Adds Zipcode to the query.
   */
  protected function addZipcodeToQuery() {
    $zip = $this->getZipCode();
    if (!empty($zip)) {
      $this->addQueryParam(SIGNATURE_SOLR_FIELD_USER_ZIP, $zip);
    }
  }

  /**
   * Adds User Country the query.
   */
  protected function addCountryToQuery() {
    $country = $this->getCountry();
    if (!empty($country)) {
      $this->addQueryParam(SIGNATURE_SOLR_FIELD_USER_COUNTRY, $country);
    }
  }


  /**
   * Adds petition IDs to the query.
   */
  protected function addPetitionIdsToQuery() {
    $petition_ids = $this->getPetitionIds();
    if (!empty($petition_ids)) {
      $expressions = array();

      $nids = petition_extract_nids_from_petition_ids($petition_ids);
      if (!empty($nids)) {
        $nid_expression = '(' . implode(' OR ', $nids) . ')';
        $expressions[] = SIGNATURE_SOLR_FIELD_PETITION_ID . ':' . $nid_expression;
      }

      $legacy_ids = petition_extract_legacy_ids_from_petition_ids($petition_ids);
      if (!empty($legacy_ids)) {
        $legacy_id_expression = '(' . implode(' OR ', $legacy_ids) . ')';
        $expressions[] = SIGNATURE_SOLR_FIELD_LEGACY_PETITION_ID . ':' . $legacy_id_expression;
      }

      if ($expressions) {
        $this->params['fq'][] = '(' . implode(' OR ', $expressions) . ')';
      }
      // If no expressions could be generated, none of the given petition IDs
      // were valid.
      else {
        throw new PetitionNotFoundException();
      }
    }
  }

  /**
   * Adds the offset and limit arguments to the query.
   */
  protected function addOffsetAndLimitToQuery() {
    $this->params['start'] = $this->getOffset();
    $this->params['rows'] = $this->getLimit();
  }

  /**
   * Adds the sort argument to the query.
   */
  protected function addSortToQuery() {
    $order_bys = $this->getOrderBy();
    if (empty($order_bys)) {
      $this->sortString = SIGNATURE_SOLR_FIELD_TIMESTAMP . ' asc';
    }
    else {
      foreach ($order_bys as $order_by) {
        $field = $order_by->field;
        $order_by_dir = $order_by->direction;
        if (!$this->isValidOrderByField($field)) {
          throw new Exception('addSortToQuery - Not a valid field: ' . $field);
        }
        if (!$this->isValidOrderByDirection($order_by_dir)) {
          throw new Exception('addSortToQuery - Not a valid direction: ' . $order_by_dir);
        }
        $mapped_field = $this->orderByFieldsMap[$field]['field'];
        $mapped_dir = $this->orderByDirMap[$order_by_dir];
        $this->sortString = $mapped_field . ' ' . $mapped_dir . ',';

      }
      // Remove comma from end of sortString.
      $this->sortString = substr($this->sortString, 0, -1);
    }
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
    $this->addOffsetAndLimitToQuery();
    $this->addSortToQuery();
  }

  /**
   * Adds a Solr search parameter.
   *
   * @param string $field
   *   The field to query.
   * @param string $value
   *   The value to query for.
   * @param bool $exact
   *   Whether to require an exact match (a filter query) or not (an ordinary
   *   query). A filter query can be cached, so an exact search is more
   *   performant. Defaults to TRUE.
   */
  private function addQueryParam($field, $value, $exact = TRUE) {
    $query_type = $exact ? 'fq' : 'q';
    $this->params[$query_type][] = "{$field}:{$value}";
  }

  /**
   * Executes the query.
   *
   * @return object
   *   A search result response object.
   */
  protected function executeQuery() {
    $query = new SolrBaseQuery('petitions_data', apachesolr_get_solr(), $this->params, $this->sortString);
    // Make Solr aware of sorting fields that are available.
    foreach ($this->orderByFieldsMap as $order_by) {
      $query->setAvailableSort($order_by['field'], array(
        'title' => t($order_by['field']),
        'default' => 'asc',
      ));
    }
    $result = $query->search();
    // @todo Handle a non-200 response code.
    return $result->response;
  }


  /**
   * Builds the results array from the query results.
   *
   * @param object $response
   *   A search result response object, from the return value from
   *   DrupalSolrQueryInterface::search().
   */
  protected function buildResults($response) {
    $result_arrays = array();
    $result_objects = array();

    foreach ($response->docs as $doc) {
      $signature = new SignatureItem();
      $signature
        ->setId($doc->{SIGNATURE_SOLR_FIELD_ID})
        ->setUid($doc->{SIGNATURE_SOLR_FIELD_UID})
        ->setPetitionId($doc->{SIGNATURE_SOLR_FIELD_PETITION_ID})
        ->setFirstName($doc->{SIGNATURE_SOLR_FIELD_USER_FIRST_NAME})
        ->setLastName($doc->{SIGNATURE_SOLR_FIELD_USER_LAST_NAME})
        ->setLegacyId($doc->{SIGNATURE_SOLR_FIELD_LEGACY_ID})
        ->setLegacyPetitionId($doc->{SIGNATURE_SOLR_FIELD_LEGACY_PETITION_ID})
        ->setCreated($doc->{SIGNATURE_SOLR_FIELD_TIMESTAMP})
        ->setUserAgent($doc->{SIGNATURE_SOLR_FIELD_USER_AGENT})
        ->setCity($doc->{SIGNATURE_SOLR_FIELD_USER_CITY})
        ->setState($doc->{SIGNATURE_SOLR_FIELD_USER_STATE})
        ->setZip($doc->{SIGNATURE_SOLR_FIELD_USER_ZIP})
        ->setUserCountry($doc->{SIGNATURE_SOLR_FIELD_USER_COUNTRY});

      if (filter_var($doc->{SIGNATURE_SOLR_FIELD_IP_ADDRESS}, FILTER_VALIDATE_IP)) {
        // setIpAddress throws exception if IP not formatted correctly, breaking method chaining.
        $signature->setIpAddress($doc->{SIGNATURE_SOLR_FIELD_IP_ADDRESS});
      }

      $result_objects[] = $signature;
      $result_arrays[] = $signature->toRestResponseItemArray();
    }

    $this->setResultObjects($result_objects);
    $this->setResult($result_arrays);
    $this->setCount($response->numFound);
  }
}
