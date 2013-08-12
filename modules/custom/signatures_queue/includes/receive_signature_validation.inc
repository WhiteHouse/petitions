<?php
/**
 * @file
 * The "receive signature validation" step.
 *
 * Compares the second parameter in the request to the MD5 hash of the first
 * parameter (the unique signature validation key) and stores timestamp, client
 * IP, secret validation, and petition ID in the signatures validated queue.
 */

/**
 * Receives valid signatures.
 *
 * @param string $server_name
 *   The name of the active server.
 * @param string $worker_name
 *   An arbitrary worker identifier.
 *
 * @return bool
 *   Returns TRUE if the function completes or FALSE if not.
 */
function signatures_queue_receive_signature_validation($server_name, $worker_name) {

}