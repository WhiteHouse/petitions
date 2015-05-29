<?php

/**
 * @file
 * Contains PetitionsSelectQueryMysql.
 */

/**
 * MySQL petitions select query class.
 */
class PetitionsSelectQueryMysql extends PetitionsSelectQuery {

  /**
   * The query object for this query.
   *
   * @var EntityFieldQueryExtraFields
   */
  protected $query;

  /**
   * The unlimited (COUNT) query object for this query.
   *
   * @var EntityFieldQueryExtraFields
   */
  protected $countQuery;

  /**
   * The size of the unlimited resultset.
   *
   * @var int
   */
  protected $resultsetCount = 0;

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
   *   'field' is required. For petition fields, 'field' and 'column' name
   *   are required.
   */
  protected $orderByFieldsMap = array(
    self::SELECT_QUERY_ORDER_BY_FIELD_ID => array('field' => 'nid'),
    self::SELECT_QUERY_ORDER_BY_FIELD_TITLE => array('field' => 'title'),
    self::SELECT_QUERY_ORDER_BY_FIELD_DATE_CREATED => array('field' => 'created'),
    self::SELECT_QUERY_ORDER_BY_FIELD_PUBLISHED => array('field' => 'field_timestamp_published', 'column' => 'value'),
    self::SELECT_QUERY_ORDER_BY_FIELD_SIGNATURE_COUNT => array('field' => 'field_petition_signature_count', 'column' => 'value'),
    self::SELECT_QUERY_ORDER_BY_FIELD_DATE_REACHED_PUBLIC => array('field' => 'field_timestamp_reached_public', 'column' => 'value'),
  );


  /**
   * Constructor.
   */
  public function __construct() {
    parent::__construct();
    $this->setOrderBy(self::SELECT_QUERY_ORDER_BY_FIELD_DATE_CREATED, self::SELECT_QUERY_ORDER_BY_DESC);
  }

  /**
   * {@inheritdoc}
   */
  public function execute() {
    try {
      $this->buildQuery();
      $nodes = $this->executeQuery();
      $this->buildResults($nodes);
    }
    catch (PetitionNotFoundException $e) {
      $this->setResult(array());
    }
    catch (Exception $e) {
      watchdog('petitions_data', 'Exception in PetitionsSelectQueryMySQL::execute(): !e', array(
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
      ->entityCondition('entity_type', 'node')
      ->propertyCondition('type', array('petition'))
      ->addExtraField('', 'title', 'title', 'node')
      ->addExtraField('', 'created', 'created', 'node')
      ->addExtraField('', 'uid', 'uid', 'node');
  }

  /**
   * Adds the orderBy arguments to the query.
   */
  protected function addOrderByToQuery() {
    if (!$this->getOrderBy()) {
      return;
    }
    foreach ($this->getOrderBy() as $order_by) {
      $field = $order_by->field;
      $order_by_dir = $order_by->direction;
      if (!$this->isValidOrderByField($field)) {
        throw new Exception('addOrderByToQuery - Not a valid field: ' . $field);
      }
      if (!$this->isValidOrderByDirection($order_by_dir)) {
        throw new Exception('addOrderByToQuery - Not a valid direction: ' . $order_by_dir);
      }

      if (in_array($field, $this->getValidPetitionsOrderByFields())) {
        $mapped_field = $this->orderByFieldsMap[$field]['field'];
        $mapped_column = $this->orderByFieldsMap[$field]['column'];
        $mapped_dir = $this->orderByDirMap[$order_by_dir];

        $this->query->fieldCondition($mapped_field);
        $this->query->fieldOrderBy($mapped_field, $mapped_column, $mapped_dir);

        // Adding propertyOrderBy is necessary to ensure node table is joined.
        $this->query->propertyOrderBy('created', 'DESC');
      }
      else {
        $mapped_field = $this->orderByFieldsMap[$field]['field'];
        $mapped_dir = $this->orderByDirMap[$order_by_dir];
        $this->query->propertyOrderBy($mapped_field, $mapped_dir);
      }
    }
  }

  /**
   * Adds the supplied arguments to the query.
   */
  protected function addArgumentsToQuery() {
    // Start with the arguments that fail cheapest in case of bad input.
    $this->addUrlToQuery();

    $this->addBodyToQuery();
    $this->addCreatedArgumentsToQuery();
    $this->addStatusToQuery();
    $this->addPetitionIdsToQuery();
    $this->addResponseIdToQuery();
    $this->addSignatureCountArgumentsToQuery();
    $this->addSignatureThresholdArgumentsToQuery();
    $this->addTitleToQuery();
    $this->addIssuesToQuery();
    $this->addUidToQuery();

    // Clone the query before limiting its range.
    $this->countQuery = clone $this->query;

    // Add order by after clone, since order isn't necessary on count.
    $this->addOrderByToQuery();

    // This should be the only addition to the query after cloning it.
    $this->addOffsetAndLimitToQuery();
  }

  /**
   * Adds the url argument to the query.
   */
  protected function addUrlToQuery() {
    $url = $this->getURL();
    if ($url) {
      $nid = static::getPetitionIdFromUrl($url);
      if ($nid) {
        $this->query->propertyCondition('nid', $nid);
      }
      else {
        $path = petitions_data_get_path_from_full_url($url);
        $this->query->fieldCondition('field_legacy_path', 'value', $path);
      }
    }
  }

  /**
   * Adds the body argument to the query.
   */
  protected function addBodyToQuery() {
    if ($this->getBody()) {
      $this->query->fieldCondition('body', 'value', '%' . $this->getBody() . '%', 'LIKE');
    }
  }

  /**
   * Adds the created time-related arguments to the query.
   */
  protected function addCreatedArgumentsToQuery() {
    if ($this->getCreatedDate()) {
      $this->addCreatedAtToQuery();
    }
    else {
      $this->addCreatedAfterToQuery();
      $this->addCreatedBeforeToQuery();
    }
  }

  /**
   * Adds the createdAt argument to the query.
   */
  protected function addCreatedAtToQuery() {
    $this->query->fieldCondition('field_timestamp_published', 'value', $this->getCreatedDate());
  }

  /**
   * Adds the createdAfter argument to the query.
   */
  protected function addCreatedAfterToQuery() {
    if ($this->getStartDate()) {
      $this->query->fieldCondition('field_timestamp_published', 'value', $this->getStartDate(), '>');
    }
  }

  /**
   * Adds the createdBefore argument to the query.
   */
  protected function addCreatedBeforeToQuery() {
    if ($this->getEndDate()) {
      $this->query->fieldCondition('field_timestamp_published', 'value', $this->getEndDate(), '<');
    }
  }

  /**
   * Adds the offset and limit arguments to the query.
   */
  protected function addOffsetAndLimitToQuery() {
    $this->query->range($this->getOffset(), $this->getLimit());
  }

  /**
   * Adds the responseId argument to the query.
   */
  protected function addResponseIdToQuery() {
    if ($this->getResponseID()) {
      $this->query->fieldCondition('field_response_id', 'target_id', $this->getResponseID());
    }
  }

  /**
   * Adds the signature count-related arguments to the query.
   */
  protected function addSignatureCountArgumentsToQuery() {
    if ($this->getSignatureCount()) {
      $this->addSignatureCountToQuery();
    }
    else {
      $this->addSignatureCountCeilingToQuery();
      $this->addSignatureCountFloorToQuery();
    }
  }

  /**
   * Adds the signatureCount argument to the Query.
   */
  protected function addSignatureCountToQuery() {
    $this->query->fieldCondition('field_petition_signature_count', 'value', $this->getSignatureCount());
  }

  /**
   * Adds the signatureCountCeiling argument to the query.
   */
  protected function addSignatureCountCeilingToQuery() {
    if ($this->getSignatureCountCeiling()) {
      $this->query->fieldCondition('field_petition_signature_count', 'value', $this->getSignatureCountCeiling(), '<=');
    }
  }

  /**
   * Adds the signatureCountFloor argument to the query.
   */
  protected function addSignatureCountFloorToQuery() {
    if ($this->getSignatureCountFloor()) {
      $this->query->fieldCondition('field_petition_signature_count', 'value', $this->getSignatureCountFloor(), '>=');
    }
  }

  /**
   * Adds the signature threshold-related arguments to the query.
   */
  protected function addSignatureThresholdArgumentsToQuery() {
    if ($this->getSignatureThreshold()) {
      $this->addSignatureThresholdToQuery();
    }
    else {
      $this->addSignatureThresholdCeilingToQuery();
      $this->addSignatureThresholdFloorToQuery();
    }
  }

  /**
   * Adds the signatureThreshold argument to the Query.
   */
  protected function addSignatureThresholdToQuery() {
    $this->query->fieldCondition('field_petition_response_sign', 'value', $this->getSignatureThreshold());
  }

  /**
   * Adds the signatureThresholdCeiling argument to the query.
   */
  protected function addSignatureThresholdCeilingToQuery() {
    if ($this->getSignatureThresholdCeiling()) {
      $this->query->fieldCondition('field_petition_response_sign', 'value', $this->getSignatureThresholdCeiling(), '<=');
    }
  }

  /**
   * Adds the signatureThresholdFloor argument to the query.
   */
  protected function addSignatureThresholdFloorToQuery() {
    if ($this->getSignatureThresholdFloor()) {
      $this->query->fieldCondition('field_petition_response_sign', 'value', $this->getSignatureThresholdFloor(), '>=');
    }
  }

  /**
   * Adds the title argument to the query.
   */
  protected function addTitleToQuery() {
    if ($this->getTitle()) {
      $this->query->propertyCondition('title', '%' . $this->getTitle() . '%', 'LIKE');
    }
  }

  /**
   * Adds the issues argument to the query.
   */
  protected function addIssuesToQuery() {
    if ($this->getIssueIDs()) {
      $this->query->fieldCondition('field_petition_issues', 'tid', $this->getIssueIDs(), 'IN');
    }
  }

  /**
   * Adds petition IDs to the query.
   */
  protected function addPetitionIdsToQuery() {
    $petition_ids = $this->getPetitionIds();
    if (!empty($petition_ids)) {
      $nids = petition_extract_nids_from_petition_ids($petition_ids);
      if (!empty($nids)) {
        $this->query->propertyCondition('nid', $nids, 'IN');
      }

      $legacy_ids = petition_extract_legacy_ids_from_petition_ids($petition_ids);
      if (!empty($legacy_ids)) {
        $this->query->fieldCondition('field_legacy_id', 'value', $legacy_ids, 'IN');
      }

      // None of the given petition IDs were valid.
      if (empty($nids) && empty($legacy_ids)) {
        throw new PetitionNotFoundException();
      }
    }
  }

  /**
   * Adds User ID to the query.
   */
  protected function addUidToQuery() {
    $uid = $this->getUid();
    if (!empty($uid)) {
      $this->query->propertyCondition('uid', $uid);
    }
  }

  /**
   * Adds the status argument to the query.
   */
  protected function addStatusToQuery() {
    $include_statuses = static::parseStatus();

    if ($include_statuses) {
      $this->query->fieldCondition('field_petition_status', 'value', $include_statuses, 'IN');
    }
    else {
      // There are no statuses so don't bother executing any query because there
      // aren't any results.
      throw new PetitionNotFoundException();
    }
  }

  /**
   * Gets a petition ID from a given petition URL.
   *
   * @param $url
   *   The URL to get a petition ID from.
   *
   * @return mixed|false
   *   A petition ID corresponding to the given URL, or FALSE if there is no
   *   corresponding petition.
   */
  public static function getPetitionIdFromUrl($url) {
    $url_parts = parse_url($url);
    $path = substr($url_parts['path'], 1);
    $internal_path = drupal_get_normal_path($path);

    // Find a node ID, if present.
    $matches = array();
    preg_match('@^node/([1-9][0-9]*)$@', $internal_path, $matches);
    if (!isset($matches[1])) {
      return FALSE;
    }
    $nid = (int) $matches[1];

    return $nid;
  }

  /**
   * Executes the query.
   *
   * @return array
   *   An array of nodes with field data, or an empty array if there are no
   *   results.
   */
  protected function executeQuery() {
    $this->executeCountQuery();
    return $this->executeFullQuery();
  }

  /**
   * Executes the COUNT query to get the unlimited resultset size.
   */
  protected function executeCountQuery() {
    $this->resultsetCount = $this->countQuery->addTag('petitions_data_debug')->count()->execute();
  }

  /**
   * Executes the full query to get the resultset data.
   *
   * @return array
   *   An array of node data.
   */
  protected function executeFullQuery() {
    // If the COUNT query found no results, there's no reason to execute the
    // full query.
    if ($this->resultsetCount === 0) {
      return array();
    }

    $result = $this->query->addTag('petitions_data_debug')->execute();

    // Return early if no results.
    if (empty($result['node'])) {
      return array();
    }

    $nodes = $result['node'];
    field_attach_load('node', $nodes);
    return $nodes;
  }

  /**
   * Builds the results arrays from the query results.
   *
   * @param array $nodes
   *   An array of node data, as returned by
   *   PetitionsSelectQueryMysql::executeQuery.
   */
  protected function buildResults(array $nodes) {
    $result_objects = array();
    $result_arrays = array();
    foreach ($nodes as $node) {
      $response = isset($node->field_response_id[LANGUAGE_NONE][0]['target_id']) ? static::formatReturnResponse($node->field_response_id[LANGUAGE_NONE][0]['target_id']) : array();
      $response_status = isset($node->field_response_status[LANGUAGE_NONE][0]['value']) ? (int) $node->field_response_status[LANGUAGE_NONE][0]['value'] : NULL;

      // This is the pre-PHP 5.5.0 equivalent of array_column().
      // @see http://php.net/manual/en/function.array-column.php
      $issue_tids = array();
      if (!empty($node->field_petition_issues[LANGUAGE_NONE])) {
        $issue_tids = array_map(function ($element) {
          return $element['tid'];
        }, $node->field_petition_issues[LANGUAGE_NONE]);
      }

      $petition = new PetitionItem();
      // Make sure we have a legacy ID before we assign it.
      $legacy_id = (is_array($node->field_legacy_id) && isset($node->field_legacy_id[LANGUAGE_NONE])) ? (string) $node->field_legacy_id[LANGUAGE_NONE][0]['value'] : NULL;
      $timestamp_published = isset($node->field_timestamp_published[LANGUAGE_NONE][0]['value']) ? $node->field_timestamp_published[LANGUAGE_NONE][0]['value'] : NULL;
      $timestamp_reached_public = isset($node->field_timestamp_reached_public[LANGUAGE_NONE][0]['value']) ? $node->field_timestamp_reached_public[LANGUAGE_NONE][0]['value'] : NULL;
      $timestamp_reached_ready = isset($node->field_timestamp_reached_ready[LANGUAGE_NONE][0]['value']) ? $node->field_timestamp_reached_ready[LANGUAGE_NONE][0]['value'] : NULL;
      $timestamp_review_threshold_mail_sent = isset($node->field_review_threshold_mail_sent[LANGUAGE_NONE][0]['value']) ? $node->field_review_threshold_mail_sent[LANGUAGE_NONE][0]['value'] : NULL;
      $timestamp_responded = isset($node->field_timestamp_responded[LANGUAGE_NONE][0]['value']) ? $node->field_timestamp_responded[LANGUAGE_NONE][0]['value'] : NULL;
      $short_url = isset($node->field_short_url[LANGUAGE_NONE][0]['value']) ? $node->field_short_url[LANGUAGE_NONE][0]['value'] : NULL;
      $body = isset($node->body[LANGUAGE_NONE][0]['value']) ? $node->body[LANGUAGE_NONE][0]['value'] : '';

      $petition
        ->setEntityId($node->nid)
        ->setLegacyId($legacy_id)
        ->setTitle($node->extraFields->title)
        ->setBody($body)
        ->setIssues($this->formatReturnIssues($issue_tids))

        // Since path aliases are cached, using url() for this value here will
        // ultimately be much more performant (not to mention less complex) than
        // adding a JOIN to the database query for it.
        ->setNiceUrl(drupal_get_path_alias("node/{$node->nid}"))

        ->setShortUrl($short_url)
        ->setReviewTimeframe($node->field_petition_review_timeframe[LANGUAGE_NONE][0]['value'])
        ->setSignatureCount((int) $node->field_petition_signature_count[LANGUAGE_NONE][0]['value'])
        ->setSignatureThreshold((int) $node->field_petition_response_sign[LANGUAGE_NONE][0]['value'])
        ->setStatus($node->field_petition_status[LANGUAGE_NONE][0]['value'])
        ->setResponse($response)
        ->setCreated($node->extraFields->created)
        ->setPublished((int) $timestamp_published)
        ->setUid($node->extraFields->uid)
        ->setReachedPublic((int) $timestamp_reached_public)
        ->setReachedReady((int) $timestamp_reached_ready)
        ->setSignaturePublicThreshold((int) $node->field_petition_public_signatures[LANGUAGE_NONE][0]['value'])
        ->setResponseStatus($response_status)
        ->setReviewThresholdMailSent($timestamp_review_threshold_mail_sent)
        ->setClosed($timestamp_responded);
      if (!empty($node->field_legacy_path[LANGUAGE_NONE][0]['value'])) {
        $petition->setLegacyPath($node->field_legacy_path[LANGUAGE_NONE][0]['value']);
      }

      $result_objects[] = $petition;
      $result_arrays[] = $petition->toRestResponseItemArray();
    }

    $this->setResultObjects($result_objects);
    $this->setResult($result_arrays);
    $this->setCount($this->resultsetCount);
  }

}
