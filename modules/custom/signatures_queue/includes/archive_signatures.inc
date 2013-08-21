<?php

/**
 * @file
 * The "archive signatures" workflow.
 *
 * Retrieve and delete batches of signatures from signatures_submitted_queue,
 * send a validation email, then store signature info in
 * signatures_pending_validation_queue.
 */

/**
 * Initiates archive of signatures.
 *
 * This function should not be invoked directly. Use
 * signatures_queue_invoke_workflow() instead.
 *
 * @return bool
 *   Returns TRUE if the workflow executes successfully or FALSE if not.
 *
 * @see signatures_queue_invoke_workflow()
 */
function signatures_queue_archive_signatures() {
  // Define a variable for signatures_queue_archive_unvalidated_signatures.
  $archive_signatures = variable_get('signatures_queue_archive_unvalidated_signatures', 1);

  // Set the active database to the signatures_processing db.
  signatures_queue_set_db();

  if ($archive_signatures == 1) {
    // Query to copy signatures from pending_validation to not_valid table.
    $query = db_select('signatures_pending_validation', 's');
    $query
      ->fields('s', array(
        'id',
        'secret_validation_key',
        'signature_source_api_key',
        'timestamp_petition_close',
        'timestamp_validation_close',
        'petition_id',
        'first_name',
        'last_name',
        'zip',
        'email',
        'signup',
        'timestamp_validation_email_sent',
        'timestamp_submitted'))
      ->condition('timestamp_validation_close', $time, '<');

    db_insert('signatures_not_valid')
      ->from($query)
      ->execute();
  }
  // Delete signatures that are no longer valid from the pending table.
  db_delete('signatures_pending_validation')
    ->condition('timestamp_validation_close', $time, '<')
    ->execute();

  // Set the active database back to default, in case queue backend is using
  // Drupal's database.
  db_set_active();

  return TRUE;
}
