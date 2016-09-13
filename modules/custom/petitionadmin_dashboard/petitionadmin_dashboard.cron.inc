<?php

/**
 * Functions that should run on cron.
 *
 * -Remove events in the event_log table logged longer ago than the time
 * specified in the petitions_event_lifetime variable.
 */
function petitionadmin_dashboard_clean_event_log() {
  $event_lifetime = variable_get('petitions_event_lifetime', '3 days');
  $max_event_created_date = strtotime('-' . $event_lifetime);

  // If new descriptions are logged to the event_log table that should be
  // deleted as part of the event cleanup, they should be added here.
  $descriptions = db_or()
    ->condition('description', db_like(PetitionsStatistics::PETITION_FRAUD_METRIC_BASE) . '%', 'LIKE')
    ->condition('description', PetitionsStatistics::PETITION_SIGNATURE_RECEIVED_TO_INITIATED_VALIDATION);

  // Only delete 200k rows at a time to reduce overhead on db.
  $event_ids = db_select('event_log', 'el')
    ->fields('el', array('lid'))
    ->condition('created', $max_event_created_date, '<')
    ->condition($descriptions)
    ->range(0, 200000)
    ->execute()->fetchCol();

  $event_count = count($event_ids);
  if ($event_count > 0) {
    db_delete('event_log')
      ->condition('lid', $event_ids, 'IN')
      ->execute();
  }
}
