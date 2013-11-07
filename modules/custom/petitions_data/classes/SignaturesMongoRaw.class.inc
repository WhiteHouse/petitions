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
  }

  /**
   * Primary processing function of class. Makes query to MongoDB.
   *
   * @return array
   *   Results of query to MongoDB.
   */
  public function load($petition_id) {
    $results = array();

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

    $mongoResults = $this->getCollection()
      ->find($query, $fields)
      ->limit($this->getLimit())
      ->skip($this->getOffset())
      ->sort(array('timestamp' => -1));

    foreach ($mongoResults as $doc) {

      $location = $this->formatReturnLocation($doc['user']['zip'], $doc['user']['city'], $doc['user']['state']);

      $signature = array(
        'id' => $this->formatReturnId($doc['_id']),
        'type' => 'signature',
        'name' => $this->formatReturnName($doc['user']['username'], $doc['user']['first_name'], $doc['user']['last_name']),
        'city' => petitions_data_sanitize_output($location['city']),
        'state' => petitions_data_sanitize_output($location['state']),
        'zip' => petitions_data_sanitize_output($location['zipcode']),
        'created' => $doc['timestamp'],
      );

      $results[] = $signature;
    }

    $this->setCount($mongoResults->count());
    $this->setResult($results);
    return $results;
  }
}
