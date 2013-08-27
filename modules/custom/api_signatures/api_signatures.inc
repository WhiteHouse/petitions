<?php

/**
 * @file
 * Helper functions for the Signatures Resource module.
 */

/**
 * Adds a new signature to a petition.
 *
 * @param array $signature
 *   The signature argument received from the service request.
 */
function _api_signatures_resource_create(array $signature) {
  $options = array('signature' => $signature);
  // @todo Test for success and add error handling here.
  signatures_queue_invoke_workflow('receive_new_signatures', SIGNATURES_QUEUE_SERVER_NAME_DEFAULT, SIGNATURES_QUEUE_WORKER_NAME_DEFAULT, $options);
}
