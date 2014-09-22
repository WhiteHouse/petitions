<?php

/**
 * @file
 * Non-hook implementations for the API Validations module.
 */

/**
 * Callback for Services validations index method.
 */
function _api_validations_resource_index($api_key, $petition_id, $mock, $offset, $limit) {
  global $base_url;

  $website_url = variable_get('petitions_data_petitions_website_url', $base_url);

  // Return mock response.
  if ($mock) {
    $response = drupal_json_decode(_api_validations_mock_response('index'));
    return $response;
  }

  // Check for the required key.
  if (empty($api_key) || !api_key_manager_valid_key($api_key)) {
    $status_code = 401;
    $developer_message = t("Invalid key.");
    $user_message = t("The key provided is not valid.");
    $error_code = 85;
    $more_info = t('See the documentation: !url/developers#validations-index-error-401', array('!url' => $website_url));
    return api_errors_throw_error($status_code, $developer_message, $user_message, $more_info, $error_code);
  }

  $query = db_select('signature_validations', 'v')
    ->fields('v', array('vid', 'email', 'signature_id', 'petition_id'))
    // Only get records for the provided key.
    ->condition('signature_source_api_key', $api_key);

  // Filter by optional parameters.
  !empty($petition_id) AND $query = $query->condition('petition_id', $petition_id);

  // Find the total count available.
  $count = $query->countQuery()->execute()->fetchField();

  // Set the limit to the smallest of: the argument, maximum, and result count.
  $limit = min(array(
    $limit,
    variable_get('api_validations_index_max_limit', API_VALIDATIONS_MAX_LIMIT_DEFAULT),
  ));

  // Limit and offset the query as specified maximum.
  $query->range($offset, $limit);

  $response_params = array(
    'results' => $query->execute()->fetchAll(),
    'count' => $count,
    'limit' => $limit,
    'offset' => $offset,
  );

  return api_services_responses_format($response_params);
}

/**
 * Returns a mock response for validations methods.
 */
function _api_validations_mock_response($method) {
  switch ($method) {
    case 'index':
      $file = 'api_validations_index.response.json';
      break;

    case 'index401':
      $file = 'api_validations_index.response401.json';
      break;

  }
  $response = file_get_contents(__DIR__ . '/example_validations/' . $file);
  return $response;
}
