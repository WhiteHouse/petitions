<?php
/**
 * @file
 * Functions that should run on cron.
 */

/**
 * Close petitions that have exceeded the duration to be signed.
 */
function wh_petitions_close_petitions() {
  // Set the variables we need for both cases.
  $petition_status_array = array(
    (int) WH_PETITION_STATUS_PRIVATE,
    (int) WH_PETITION_STATUS_PUBLIC,
  );
  $today = time();

  // Close petitions in MySQL.
  $query = new EntityFieldQueryExtraFields();
  $result = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'petition')
    ->fieldCondition('field_petition_status', 'value', $petition_status_array, 'IN')
    ->addExtraField('field_timestamp_published', 'value', 'value')
    ->addExtraField('field_petition_review_timeframe', 'value', 'value')
    ->execute();

  if (!empty($result['node'])) {
    foreach ($result['node'] as $nid => $node) {
      $fields = $node->extraFields;
      // Identify if this petition has exceeded the review timeframe.
      $end_date = strtotime('+ ' . $fields->field_petition_review_timeframe_value . ' days', $fields->field_timestamp_published_value);
      if ($today > $end_date) {
        // Petition has been open too long, close it.
        $full_node = node_load($nid);
        $node_wrapper = entity_metadata_wrapper('node', $full_node);
        $previous_status = $node_wrapper->field_petition_status->raw();
        $node_wrapper->field_petition_status->set(WH_PETITION_STATUS_CLOSED);
        $node_wrapper->save();

        module_invoke_all('petition_status_updated', $nid, $previous_status, WH_PETITION_STATUS_CLOSED);
      }
    }
  }
}

/**
 * Update the counts of how many petitions/responses are related to each issue.
 */
function wh_petitions_update_issue_counts() {
  // Get the issues taxonomy terms for issue count caching.
  $issues_list = array();
  $vocab = taxonomy_vocabulary_machine_name_load('issues');
  if (!empty($vocab)) {
    $issues_list = taxonomy_get_tree($vocab->vid);
  }

  // Petitions Issues.
  $issues = array();
  $petition_open_statuses = wh_petitions_open_statuses();

  foreach ($issues_list as $issue) {
    $issues[$issue->tid] = check_plain($issue->name);

    $query = db_select('node', 'n', array('target' => 'slave'));
    $query->fields('n', array('nid'))
      ->condition('n.type', 'petition');
    $query->innerJoin('field_data_field_petition_status', 's', 'n.nid=s.entity_id');
    $query->condition('s.field_petition_status_value', $petition_open_statuses, 'IN');
    $query->innerJoin('field_data_field_petition_issues', 'i', 'n.nid = i.entity_id');
    $query->condition('i.field_petition_issues_tid', $issue->tid);

    $total = $query->countQuery()->execute()->fetchField();
    $issues[$issue->tid] = $total;
  }

  variable_set('wh_petitions_petition_issue_counts', $issues);

  // Response Counts - Issues.
  $issues = array();
  if (!empty($issues_list)) {
    foreach ($issues_list as $issue) {
      $issues[$issue->tid] = check_plain($issue->name);

      $query = db_select('node', 'n', array('target' => 'slave'));
      $query->fields('n', array('nid'))
        ->condition('n.type', 'wh_response')
        ->condition('n.status', 0, '<>');
      $query->innerJoin('field_data_field_issues', 'i', 'n.nid = i.entity_id');
      $query->condition('i.field_issues_tid', $issue->tid);

      $total = $query->countQuery()->execute()->fetchField();
      $issues[$issue->tid] = $total;
    }
  }
  variable_set('wh_petitions_response_issue_counts', $issues);
}
