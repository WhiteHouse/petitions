<?php

/**
 * @file
 * Private functions for api_validation module.
 */

/**
 * Callback for Services validations index method.
 */
function _api_validations_resource_index($key, $petition_id) {
  global $base_url;

  // Return mock response. @todo implement this parameter.
  if (!empty($mock)) {
    $response = json_decode(_api_validations_mock_response('index'));
    return $response;
  }

  // Check for the required key.
  if (empty($key)) {
    $status = 500;
    $developer_message = $user_message = t('You must provide your API key.');
    $more_info = 'https://github.com/WhiteHouse/petitions/issues/75';
    $error_code = 75;
    // @todo We have to do this manually because Services module completely
    //   borks on a 5xx error code. Since others will need this too, factor it
    //   out into a helper function somewhere.
    return array(
      'status' => $status,
      'developerMessage' => "{$status}:{$developer_message}",
      'errorCode' => $error_code,
      'userMessage' => $user_message,
      'moreInfo' => $more_info,
    );
  }

  $query = db_select('signature_validations', 'v')
    ->fields('v', array('vid', 'email', 'signature_id', 'petition_id'))
    // Only get records for the provided key.
    ->condition('signature_source_api_key', $key);

  // Filter by optional parameters.
  !empty($petition_id) AND $query = $query->condition('petition_id', $petition_id);

  // Limit the query to some maximum.
  $query->range(0, 10000);

  // Get all results in an array.
  $results = $query->execute()->fetchAllAssoc('vid', PDO::FETCH_ASSOC);

  $response = array(
    'metadata' => array(
      'count' => count($results),
      // The query can be useful during development.
      // 'query' => $query->execute()->getQueryString(),
    ),
    'results' => $results,
  );

  return $response;
}

/**
 * Returns a mock response for validations methods.
 */
function _api_validations_mock_response($method) {
  switch ($method) {
    case 'index':
      $file = 'api_validations_index.response.json';
      break;

  }
  $response = file_get_contents(__DIR__ . '/example_validations/' . $file);
  return $response;
}
