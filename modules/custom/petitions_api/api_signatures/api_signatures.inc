<?php

/**
 * @file
 * Helper functions for the Signatures Resource module.
 */

/**
 * Adds a new signature to a petition.
 *
 * @param string $api_key
 *   The API key.
 * @param array $signature
 *   The signature argument received from the service request.
 *
 * @return array
 *   A REST response array as formatted by api_services_responses_format().
 */
function _api_signatures_resource_create($api_key, array $signature) {

  // Verbose logging, if debugging is enabled.
  if ($debug = variable_get('signatures_queue_debug', FALSE)) {
    watchdog('api_signatures', 'DEBUG ENABLED: _api_signatures_resource_create() param $api_key: @api_key', array(
      '@api_key' => $api_key,
    ));
    watchdog('api_signatures', 'DEBUG ENABLED: _api_signatures_resource_create() param $signature: !signature', array(
       '!signature' => petition_format_for_watchdog($signature),
    ));
  }

  // Check for the required key.
  if (empty($api_key) || !api_key_manager_valid_key($api_key)) {
    $error_code = 85;
    list($status_code, $developer_message, $user_message, $more_info) = api_errors_error_code_information($error_code, $signature['petition_id']);
    return api_errors_throw_error($status_code, $developer_message, $user_message, $more_info, $error_code);
  }

  // Add the API Key to the signature.
  $signature['signature_source_api_key'] = $api_key;

  // Submit the signature.
  $options = array('signature' => $signature);
  $returned_code = signatures_queue_invoke_workflow('receive_new_signatures', $options);

  // Deliver response.
  if ($returned_code == SIGNATURES_QUEUE_STATUS_OK) {
    return api_services_responses_format();
  }
  else {
    list($status_code, $developer_message, $user_message, $more_info) = api_errors_error_code_information($returned_code, $signature['petition_id']);
    return api_errors_throw_error($status_code, $developer_message, $user_message, $more_info, $returned_code);
  }
}
