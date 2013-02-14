<?php

/**
 * @file
 * api_authentication.inc
 *
 * Include file for api_authentication module.
 */

/**
 * Authenticates a call using a key to verify the request.
 */
function _api_authentication_authenticate_call() {
  $request = getallheaders();
  $key = $request['Key'];
  $authkey = variable_get('api_authentication_key', NULL);
  if ($key != $authkey) {
    return services_error(t('You do not have permission to access this resource.'), 401);
  }
}
