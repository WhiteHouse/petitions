<?php
/**
 * @file
 * File containing class that accesses Petition data using mongodb queries.
 */

class PetitionsRetrieveMongoRaw extends PetitionsRetrieveBase implements IPetitionsRetrieveRaw {

  /**
   * Constructor.
   */
  public function __construct() {
    $this->setCollection(mongodb_collection('petitions'));
    $this->setExecutionTime(microtime());
  }

  /**
   * Primary processing function of class. Makes query to MongoDB.
   *
   * @param string $id
   *   Unique identifier of the requested petition.
   *
   * @return array
   *   Results of query to MongoDB.
   */
  public function load($id) {

    $response['metadata'] = array('responseInfo' => array());
    $response['results'] = array();

    $doc = $this->getCollection()
      ->findOne(array('_id' => new MongoId($id)));

    if (!empty($doc['_id'])) {
      $petition = array(
        'id' => $this->formatReturnId($doc['_id']),
        'type' => 'petition',
        'title' => $doc['title'],
        'body' => $doc['body'],
        'issues' => $this->formatReturnIssues($doc['issues']),
        'signatureThreshold' => $doc['response_signatures'],
        'signatureCount' => $doc['signature_count'],
        'signaturesNeeded' => $this->calcSignaturesNeeded($doc['response_signatures'], $doc['signature_count']),
        'url' => $this->formatReturnUrl($doc['nice_url']),
        'deadline' => $this->calcDeadline($doc['created'], $doc['review_timeframe']),
        'status' => $this->formatReturnPetitionStatus($doc['petition_status']),
        'response' => $doc['response_id'] ? $this->formatReturnResponse($doc['response_id']) : NULL,
        'created' => $doc['created'],
      );

      $response['results'][] = $petition;
    }

    $response['metadata']['executionTime'] = $this->calcExecutionTime();
    $response['metadata']['responseInfo']['status'] = 200;
    $response['metadata']['responseInfo']['developerMessage'] = 'OK';

    $this->setResult($response);
    return $response;
  }
}
