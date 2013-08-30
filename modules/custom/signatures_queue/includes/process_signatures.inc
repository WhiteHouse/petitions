<?php
/**
 * @file
 * The "process signatures" step.
 *
 * Signature processing includes these steps: 
 *  - Match records from validations to signatures_pending_validation tables.
 *  - If signature (email address) does not have a corresponding user, create one.
 *  - If a signature record with this email does not exist in petition_signatures
 *    create one, and get the signature id.
 */


/**
 * Run the process signatures tasks.
 */
function _signatures_queue_process_signatures($server_name, $worker_name) {

  $limit = signatures_queue_get_queue_batch_size('process_signatures');

  // Set the active database to the signatures_processing db.
  signatures_queue_set_db();

  $query = db_select('signatures_pending_validation', 'p')->fields('p');
  $query->join('validations', 'v', 'p.secret_validation_key=v.secret_validation_key');
  $query->range(0, $limit);

  $result = $query->execute();

echo "\n\n" . $result->rowCount() . "\n\n";

  // Set the active database back to default, in case queue backend wasn't using
  // Drupal's database.
  db_set_active();
}

