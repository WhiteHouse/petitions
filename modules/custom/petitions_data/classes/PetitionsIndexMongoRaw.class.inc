<?php
/**
 * @file
 * File containing class that accesses data using mongodb queries.
 */

class PetitionsIndexMongoRaw extends PetitionsIndexBase implements IPetitionsIndexRaw {

  /**
   * Constructor.
   */
  public function __construct() {
    $this->setCollection(mongodb_collection('petitions'));
    $this->setMaxReturnLimit();
  }

  /**
   * Primary processing function of class. Makes query to MongoDB.
   *
   * @return array
   *   Results of query to MongoDB.
   */
  public function load() {
    $results = array();

    if ($this->getCreatedDate()) {
      $this->addCreatedDateToQuery();
    }
    else {
      $this->addStartDateToQuery();
      $this->addEndDateToQuery();
    }

    if ($this->getSignatureThreshold()) {
      $this->addSignatureThresholdToQuery();
    }
    else {
      $this->addSignatureThresholdCeilingToQuery();
      $this->addSignatureThresholdFloorToQuery();
    }

    if ($this->getSignatureCount()) {
      $this->addSignatureCountToQuery();
    }
    else {
      $this->addSignatureCountCeilingToQuery();
      $this->addSignatureCountFloorToQuery();
    }

    $this->addTitleToQuery();
    $this->addBodyToQuery();
    $this->addURLToQuery();
    $this->addStatusToQuery();
    $this->addResponseIDToQuery();

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
    $mongoResults = $this->getCollection()
      ->find($query, $fields)
      ->limit($this->getLimit())
      ->skip($this->getOffset())
      ->sort(array('created' => -1));

    if (!$mongoResults->hasNext()) {
      return $results;
    }

    foreach ($mongoResults as $doc) {

      $petition = array(
        'id' => $this->formatReturnId($doc['_id']),
        'type' => 'petition',
        'title' => petitions_data_sanitize_output($doc['title']),
        'body' => petitions_data_sanitize_output($doc['body']),
        'issues' => $this->formatReturnIssues($doc['issues']),
        'signatureThreshold' => $doc['response_signatures'],
        'signatureCount' => $doc['signature_count'],
        'signaturesNeeded' => $this->calcSignaturesNeeded($doc['response_signatures'], $doc['signature_count']),
        'url' => $this->formatReturnUrl($doc['nice_url']),
        'deadline' => $this->calcDeadline($doc['created'], $doc['review_timeframe']),
        'status' => $this->formatReturnPetitionStatus($doc['petition_status']),
        'response' => isset($doc['response_id']) ? $this->formatReturnResponse($doc['response_id']) : NULL,
        'created' => $doc['created'],
      );

      $results[] = $petition;
    }

    $this->setCount($mongoResults->count());
    $this->setResult($results);
    return $results;
  }
}
