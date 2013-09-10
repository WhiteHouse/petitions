<?php

/**
 * @file
 * Private functions for api_validation module.
 */

/**
 * Callback for Services validations index method.
 */
function _api_validations_resource_index($key, $petition_id, $mock) {
  global $base_url;

  // Return mock response.
  if ($mock) {
    $response = json_decode(_api_validations_mock_response('index'));
    return $response;
  }

  // Check for the required key.
  // @todo: include a check for valid API key after PT-1044 is implemented,
  // and there is a content type to validate against.
  if (empty($key)) {
    $status_code = 400;
    $developer_message = t("Invalid key.");
    $user_message = t("The key provided is not valid.");
    api_errors_throw_error($status_code, $developer_message, $user_message);
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

  // Error out if there aren't any results.
  if (empty($results)) {
    $status_code = 400;
    $developer_message = t("Not found.");
    $user_message = t("There were no results for this request.");
    $more_info = "http://github.com/whitehouse/petitions/issue/[PLACEHOLDER]";
    $error_code = "[GitHub issue number]";
    api_errors_throw_error($status_code, $developer_message, $user_message, $more_info, $error_code);
  }

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
 * Documentation callback for index method.
 */
function _api_validations_index_doc() {
  $element = array(
    '#name' => t('Index'),
    '#path' => '/validations',
    '#weight' => 1,
    '#description' => t('Returns a list of validations matching passed parameters.'),
    '#request_url' => url(variable_get('petitions_data_petitions_api_url', '') . '/v1/validations.json?key=asY1k9uCQY7Hg8MCBaa', array('absolute' => TRUE)),
    '#response' => _api_validations_mock_response('index'),
    '#auth' => TRUE,
  );

  return $element;
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
