<?php
/**
 * @file
 * File containing class that accesses data using mongodb queries.
 */

class SignaturesMongoRaw extends SignaturesBase implements ISignaturesRaw {

  /**
   * Constructor.
   */
  public function __construct() {
    $this->setCollection(mongodb_collection('petition_signatures'));
    $this->setExecutionTime(microtime());
    $this->setMaxReturnLimit();
  }

  /**
   * Primary processing function of class. Makes query to MongoDB.
   *
   * @return array
   *   Results of query to MongoDB.
   */
  public function load($petition_id) {
    $response = array();
    $response['metadata'] = array(
      'executionTime' => '',
      'resultset' => array(),
      'responseInfo' => array(
        'status' => 200,
        'developerMessage' => t('OK'),
      ),
    );
    $response['results'] = array();

    if ($this->getCreatedDate()) {
      $this->addCreatedDateToQuery();
    }
    else {
      $this->addStartDateToQuery();
      $this->addEndDateToQuery();
    }

    $this->addCityToQuery();
    $this->addStateToQuery();
    $this->addZipToQuery();
    $this->addCountryToQuery();

    $fields = array(
      'user.first_name',
      'user.last_name',
      'user.city',
      'user.state',
      'user.zip',
      'timestamp',
    );

    $query = $this->getQuery();
    $query += array(
      'petition_id' => $petition_id,
    );

    $results = $this->getCollection()
      ->find($query, $fields)
      ->limit($this->getLimit())
      ->skip($this->getOffset())
      ->sort(array('timestamp' => -1));

    // Set the result set information.
    $response['metadata']['resultset'] = array(
      'count' => $results->count(),
      'offset' => $this->getOffset(),
      'limit' => $this->getLimit(),
    );

    foreach ($results as $doc) {

      $location = $this->formatReturnLocation($doc['user']['zip'], $doc['user']['city'], $doc['user']['state']);

      $signature = array();
      $signature = array(
        'id' => $this->formatReturnId($doc['_id']),
        'type' => 'signature',
        'name' => $this->formatReturnName($doc['user']['username'], $doc['user']['first_name'], $doc['user']['last_name']),
        'city' => $this->filterUTF16($location['city']),
        'state' => $this->filterUTF16($location['state']),
        'zip' => $this->filterUTF16($location['zipcode']),
        'created' => $doc['timestamp'],
      );

      $response['results'][] = $signature;
    }

    $response['metadata']['executionTime'] = $this->calcExecutionTime();

    $this->setResult($response);
    return $response;
  }
}
