<?php

/**
 * @file
 * Contains PetitionsSelectQuerySolr.
 */

/**
 * Apache Solr petitions select query class.
 */
class PetitionsSelectQuerySolr extends PetitionsSelectQuery {

  /**
   * An array of Solr search parameters.
   *
   * @var array
   */
  protected $params = array();

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
      watchdog('petitions_data', 'Exception in PetitionsSelectQuerySolr::execute(): !e', array(
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
   * Adds the supplied arguments to the query.
   */
  protected function addArgumentsToQuery() {
    $this->addBodyToQuery();
    $this->addCreatedArgumentsToQuery();
    $this->addOffsetAndLimitToQuery();
    $this->addPetitionIdsToQuery();
    $this->addResponseIdToQuery();
    $this->addSignatureCountArgumentsToQuery();
    $this->addSignatureThresholdArgumentsToQuery();
    $this->addSortToQuery();
    $this->addStatusToQuery();
    $this->addTitleToQuery();
    $this->addIssuesToQuery();
    $this->addUidToQuery();
    $this->addUrlToQuery();
  }

  /**
   * Initializes the basic query.
   */
  protected function initializeQuery() {
    $this->addQueryParam('bundle', 'petition');
  }

  /**
   * Adds the body argument to the query.
   */
  protected function addBodyToQuery() {
    if ($this->getBody()) {
      $this->addQueryParam(PETITION_SOLR_FIELD_BODY, $this->getBody(), FALSE);
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
    $this->addQueryParam(PETITION_SOLR_FIELD_TIMESTAMP_PUBLISHED, $this->getCreatedDate());
  }

  /**
   * Adds the createdAfter argument to the query.
   */
  protected function addCreatedAfterToQuery() {
    if ($this->getStartDate()) {
      $time = $this->getStartDate() + 1;
      $this->addQueryParam(PETITION_SOLR_FIELD_TIMESTAMP_PUBLISHED, "[{$time} TO *]");
    }
  }

  /**
   * Adds the createdBefore argument to the query.
   */
  protected function addCreatedBeforeToQuery() {
    if ($this->getEndDate()) {
      $time = $this->getEndDate() - 1;
      $this->addQueryParam(PETITION_SOLR_FIELD_TIMESTAMP_PUBLISHED, "[* TO {$time}]");
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
   * Adds petition IDs to the query.
   */
  protected function addPetitionIdsToQuery() {
    $petition_ids = $this->getPetitionIds();
    if (!empty($petition_ids)) {
      $expressions = array();

      $nids = petition_extract_nids_from_petition_ids($petition_ids);
      if (!empty($nids)) {
        $nid_expression = '(' . implode(' OR ', $nids) . ')';
        $expressions[] = PETITION_SOLR_FIELD_NODE_ID . ':' . $nid_expression;
      }

      $legacy_ids = petition_extract_legacy_ids_from_petition_ids($petition_ids);
      if (!empty($legacy_ids)) {
        $legacy_id_expression = '(' . implode(' OR ', $legacy_ids) . ')';
        $expressions[] = PETITION_SOLR_FIELD_LEGACY_ID . ':' . $legacy_id_expression;
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
   * Adds the responseId argument to the query.
   */
  protected function addResponseIdToQuery() {
    if ($this->getResponseID()) {
      $this->addQueryParam(PETITION_SOLR_FIELD_RESPONSE_ID, $this->getResponseID());
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
    $this->addQueryParam(PETITION_SOLR_FIELD_SIGNATURE_COUNT, $this->getSignatureCount());
  }

  /**
   * Adds the signatureCountCeiling argument to the query.
   */
  protected function addSignatureCountCeilingToQuery() {
    if ($this->getSignatureCountCeiling()) {
      $count = $this->getSignatureCountCeiling();
      $this->addQueryParam(PETITION_SOLR_FIELD_SIGNATURE_COUNT, "[* TO {$count}]");
    }
  }

  /**
   * Adds the signatureCountFloor argument to the query.
   */
  protected function addSignatureCountFloorToQuery() {
    if ($this->getSignatureCountFloor()) {
      $count = $this->getSignatureCountFloor();
      $this->addQueryParam(PETITION_SOLR_FIELD_SIGNATURE_COUNT, "[{$count} TO *]");
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
    $this->addQueryParam(PETITION_SOLR_FIELD_SIGNATURE_RESPONSE_THRESHOLD, $this->getSignatureThreshold());
  }

  /**
   * Adds the signatureThresholdCeiling argument to the query.
   */
  protected function addSignatureThresholdCeilingToQuery() {
    if ($this->getSignatureThresholdCeiling()) {
      $threshold = $this->getSignatureThresholdCeiling();
      $this->addQueryParam(PETITION_SOLR_FIELD_SIGNATURE_RESPONSE_THRESHOLD, "[* TO {$threshold}]");
    }
  }

  /**
   * Adds the signatureThresholdFloor argument to the query.
   */
  protected function addSignatureThresholdFloorToQuery() {
    if ($this->getSignatureThresholdFloor()) {
      $threshold = $this->getSignatureThresholdFloor();
      $this->addQueryParam(PETITION_SOLR_FIELD_SIGNATURE_RESPONSE_THRESHOLD, "[{$threshold} TO *]");
    }
  }

  /**
   * Adds the sort argument to the query.
   */
  protected function addSortToQuery() {
    $this->sortString = PETITION_SOLR_FIELD_TIMESTAMP_CREATED . ' desc';
  }

  /**
   * Adds the status argument to the query.
   */
  protected function addStatusToQuery() {
    $include_statuses = static::parseStatus();

    if ($include_statuses) {
      $status_expression = implode(' OR ', $include_statuses);
      $this->addQueryParam(PETITION_SOLR_FIELD_PETITION_STATUS, "({$status_expression})");
    }
    // All possible results are excluded.
    else {
      throw new PetitionNotFoundException();
    }
  }

  /**
   * Adds the title argument to the query.
   */
  protected function addTitleToQuery() {
    if ($this->getTitle()) {
      $this->addQueryParam(PETITION_SOLR_FIELD_TITLE, $this->getTitle(), FALSE);
    }
  }

  /**
   * Adds the issues argument to the query.
   */
  protected function addIssuesToQuery() {
    if ($this->getIssueIDs()) {
      $tid_expression = implode(' OR ', $this->getIssueIDs());
      $this->params['fq'][] = PETITION_SOLR_FIELD_ISSUE_TIDS . ":({$tid_expression})";
    }
  }

  /**
   * Adds User ID to the query.
   */
  protected function addUidToQuery() {
    $uid = $this->getUid();
    if (!empty($uid)) {
      $this->addQueryParam(PETITION_SOLR_FIELD_UID, $uid);
    }
  }

  /**
   * Adds the url argument to the query.
   */
  protected function addUrlToQuery() {
    $url = $this->getURL();
    if ($url) {
      $path = ltrim(parse_url($url, PHP_URL_PATH), '/');

      // Invalid URL or no path.
      if ($path === FALSE) {
        throw new PetitionNotFoundException();
      }

      if (petition_is_legacy_path($path)) {
        $this->addQueryParam(PETITION_SOLR_FIELD_LEGACY_PATH, $path);;
      }
      else {
        $this->addQueryParam(PETITION_SOLR_FIELD_PATH, $path);;
      }
    }
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
    $result = $query->search();
    // @todo Handle a non-200 response code.
    return $result->response;
  }

  /**
   * Builds the results arrays from the query results.
   *
   * @param object $response
   *   A search result response object, from the return value from
   *   DrupalSolrQueryInterface::search().
   */
  protected function buildResults($response) {
    $result_objects = array();
    $result_arrays = array();
    foreach ($response->docs as $doc) {
      $issues = array();
      foreach ($doc->{PETITION_SOLR_FIELD_ISSUE_TIDS} as $delta => $tid) {
        $issues[] = array(
          'id' => $tid,
          'name' => petitions_data_sanitize_output($doc->{PETITION_SOLR_FIELD_ISSUE_NAMES}[$delta]),
        );
      }

      $petition_response = array();
      if (!empty($doc->{PETITION_SOLR_FIELD_RESPONSE_ID})) {
        $petition_response = array(
          'id' => $doc->{PETITION_SOLR_FIELD_RESPONSE_ID},
          'url' => petitions_data_url("node/{$doc->{PETITION_SOLR_FIELD_RESPONSE_ID}}"),
          'associationTime' => $doc->{PETITION_SOLR_FIELD_TIMESTAMP_RESPONSE_ASSOCIATED},
        );
      }

      $timestamp_published = NULL;
      if (!empty($doc->{PETITION_SOLR_FIELD_TIMESTAMP_PUBLISHED})) {
        $timestamp_published = $doc->{PETITION_SOLR_FIELD_TIMESTAMP_PUBLISHED};
      }

      $timestamp_reached_public = NULL;
      if (!empty($doc->{PETITION_SOLR_FIELD_TIMESTAMP_REACHED_PUBLIC})) {
        $timestamp_reached_public = $doc->{PETITION_SOLR_FIELD_TIMESTAMP_REACHED_PUBLIC};
      }

      $petition = new PetitionItem();
      $petition
        ->setEntityId($doc->{PETITION_SOLR_FIELD_NODE_ID})
        ->setLegacyId($doc->{PETITION_SOLR_FIELD_LEGACY_ID})
        ->setTitle(static::sanitizeSolrString($doc->{PETITION_SOLR_FIELD_TITLE}))
        ->setBody(static::sanitizeSolrString($doc->{PETITION_SOLR_FIELD_BODY}))
        ->setIssues($issues)
        ->setNiceUrl($doc->{PETITION_SOLR_FIELD_PATH})
        ->setReviewTimeframe($doc->{PETITION_SOLR_FIELD_REVIEW_TIMEFRAME})
        ->setSignatureCount($doc->{PETITION_SOLR_FIELD_SIGNATURE_COUNT})
        ->setSignatureThreshold($doc->{PETITION_SOLR_FIELD_SIGNATURE_RESPONSE_THRESHOLD})
        ->setStatus($doc->{PETITION_SOLR_FIELD_PETITION_STATUS})
        ->setResponse($petition_response)
        ->setCreated(strtotime($doc->{PETITION_SOLR_FIELD_TIMESTAMP_CREATED}))
        ->setPublished($timestamp_published)
        ->setUid($doc->{PETITION_SOLR_FIELD_UID})
        ->setReachedPublic($timestamp_reached_public)
        ->setSignaturePublicThreshold($doc->{PETITION_SOLR_FIELD_SIGNATURE_PUBLIC_THRESHOLD})
        ->setResponseStatus($doc->{PETITION_SOLR_FIELD_RESPONSE_STATUS});

      $result_objects[] = $petition;
      $result_arrays[] = $petition->toRestResponseItemArray();
    }

    $this->setResultObjects($result_objects);
    $this->setResult($result_arrays);
    $this->setCount($response->numFound);
  }

  /**
   * Sanitizes string values retrieved from the Solr index.
   *
   * @param string $value
   *   The value to sanitize.
   *
   * @return string
   *   The sanitized string.
   */
  public static function sanitizeSolrString($value) {
    // @todo Don't store HTML entities encoded in the first place.
    $decoded_value = html_entity_decode($value, ENT_QUOTES);

    return petitions_data_sanitize_output($decoded_value);
  }

}
