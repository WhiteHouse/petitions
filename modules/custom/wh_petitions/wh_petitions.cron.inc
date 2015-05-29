<?php

/**
 * Functions that should run on cron.
 *
 * -Close published petitions that have exceeded the amount of days they should be open.
 * -Get and cache the number of petitions and responses associated with each issue term
 * -Get a list of the top trending petitions
 *
 * @todo: Note: Trending used to be a link on the /petitions page. This has been commented out, but the code is still here because we're concerned about breaking something else by removing it. Please revisit.
 */
function wh_petitions_close_petitions() {
  // Set the variables we need for both cases.
  $petition_status_array = array((int)WH_PETITION_STATUS_PRIVATE, (int)WH_PETITION_STATUS_PUBLIC, (int)WH_PETITION_STATUS_FLAGGED);
  $today = time();

  if (petitions_data_mongo_writes_are_enabled()) {
    $conn = wh_petitions_mongo_petition_connection();

    // Retrieve public, private and flagged petitions
    $query = array(
      'petition_status' => array(
        '$in' => $petition_status_array
      ),
    );
    $retrieve_fields = array('published', 'review_timeframe');
    $results = $conn->find($query, $retrieve_fields);

    foreach ($results as $res) {
      // Identify if this petition has exceeded the review timeframe
      $end_date = strtotime('+ ' . $res['review_timeframe'] . ' days', $res['published']);

      // Petition has been open too long, close it.
      if ($today > $end_date) {
        $conn->update(
          array('_id' => $res['_id']),
          array('$set' => array('petition_status' => (int)WH_PETITION_STATUS_CLOSED, 'closed' => (int)time()))
        );

      }
    }
  }

  // Close petitions in MySQL
  if (petitions_data_mysql_writes_are_enabled()) {
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
        // Identify if this petition has exceeded the review timeframe
        $end_date = strtotime('+ ' . $fields->field_petition_review_timeframe_value . ' days', $fields->field_timestamp_published_value);
        if ($today > $end_date) {
          // Petition has been open too long, close it.
          $full_node = node_load($nid);
          $node_wrapper = entity_metadata_wrapper('node', $full_node);
          $node_wrapper->field_petition_status->set(WH_PETITION_STATUS_CLOSED);
          $node_wrapper->save();
        }
      }
    }
  }

  // Get the issues taxonomy terms for issue count caching
  $issues_list = array();
  $vocab = taxonomy_vocabulary_machine_name_load('issues');
  if (!empty($vocab)) {
    $issues_list = taxonomy_get_tree($vocab->vid);
  }

  // Petitions Issues
  $issues = array();
  $petition_open_statuses = wh_petitions_open_statuses();

  // If we are reading from mongo, get the counts from there, otherwise get them from MySQL.
  if (petitions_data_mongo_reads_are_enabled()) {
    foreach ($issues_list as $issue) {      
      // Need to get the number of petitions in a given issue.
      $query = array(
        'petition_status' => array(
          '$in' => array((int)WH_PETITION_STATUS_PUBLIC, (int)WH_PETITION_STATUS_UNDER_REVIEW)
        ),
        'hidden' => 0,
      );
      $query['issues'] = array('$in' => array((int)$issue->tid));
      $total = $conn->find($query)->count();
      $issues[$issue->tid] = $total;
    }
  }
  else {
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
   }
  variable_set('wh_petitions_petition_issue_counts', $issues);
  
  // Response Counts - Issues
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
      // Featured
      $query->innerJoin('field_data_field_is_featured', 'f', 'n.nid = f.entity_id');
      $query->addField('f', 'field_is_featured_value', 'field_is_featured_value');
      $query->condition('f.field_is_featured_value', 1);          
      
      $total = $query->countQuery()->execute()->fetchField();
      $issues[$issue->tid] = $total;
    }
  } 
  variable_set('wh_petitions_response_issue_counts', $issues);

  // Trending sort
  // This does not have a mysql equivalent as the trending link has been removed from the petitions listing page.
  if (petitions_data_mongo_reads_are_enabled()) {
    $timestamp = strtotime("-1 day");
    $sig_conn = wh_petitions_mongo_petition_signatures_connection();

    // Retrieve all the petitions that have been signed within the past 24 hours, with the count of how many signatures are associated with each.
    $reduce = "function (obj, prev) { prev.count++; }";
    $result = $sig_conn->group(array('petition_id' => 1), array('count' => 0), $reduce, array('timestamp' => array('$gt' => $timestamp)));

    // Put the results in an array so we can sort
    $trending = array();
    foreach ($result['retval'] as $key => $res) {
      $trending[$res['petition_id']] = $res['count'];
    }
    arsort($trending);

    // Retrieve the top 100 and put the ids in an array ( of Mongo Ids that we can feed to a query )
    $trending = array_slice($trending, 0, 100);
    $ids = array();
    foreach ($trending as $id => $val) {
      array_push($ids, new MongoId($id));
    }

    // Out of those 100, retrieve all that are open to the public and show in lists
    $query = array(
      'petition_status' => array(
        '$in' => array((int)WH_PETITION_STATUS_PUBLIC, (int)WH_PETITION_STATUS_UNDER_REVIEW)
      ),
      'hidden' => 0,
      '_id' => array('$in' => $ids),
    );
    $results = $conn->find($query, array('title'));

    // Put the results of the previous query into an array of mongo ids to use in the trending sort function.  At this point, $ids contains an array of the ids of petitions
    // that have been signed the most in the past 24 hours, ordered by the count descending.
    $ids = array();
    foreach ($results as $res) {
      array_push($ids, $res['_id']);
    }
    variable_set('wh_petitions_trending_ids', $ids);
  }
}
