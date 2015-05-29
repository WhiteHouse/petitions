<?php
/**
 * @file
 * Contains all the comparison functions
 * The functions in this file are used by the hook_menu for petition and
 * signature comparison.
 */

/**
 * Page callback for admin/reports/mongo-mysql-petition/compare/%.
 *
 * @param string $petition_id
 *   A mongo peition id that corresponds to mongo petition id or mysql
 * legacy_id.
 *
 * @return string
 *   Returns the html to build the page that compares the mongo and mysql data
 *   for a single petition.
 */
function _migrate_mongo2mysql_petsig_page($petition_id = 0) {

  $page_output_html = '';
  $petition_id = check_plain($petition_id);
  $o_petition = new stdClass();
  // Validate $petition_id
  if (!empty($petition_id)) {
    $page_output_html .= t('I have an id of @petitionid', array('@petitionid' => $petition_id));
    // Check for mongo support function.
    if (function_exists('wh_petitions_mongo_petition_signatures_connection') && function_exists('wh_petitions_load_petition') && (strlen($petition_id) == 24)) {
      // Load the mongo petition data.
      $collection = wh_petitions_mongo_petition_connection();

      $o_petition->mongo = wh_petitions_load_petition($collection, $petition_id);
    }
    else {
      // Mongo methods not available.
      $o_petition->mongo = 0;
    }

    // Load the MySQL petition data.
    $o_petition->mysql = migrate_mongo2mysql_petsig_petition_node_load($petition_id);

  }
  else {
    // Non-useable $petition_id.
    $page_output_html = t('I have no usuable Petition ID to work with.  Please provide a petition ID after the url.');
  }

  // Make comparison array.
  $a_compare = array();
  $language = $o_petition->mysql->language;
  $lang = "'$language'";
  $a_compare[] = array(
    'name' => 'User_ID',
    'mongo' => 'uid',
    'mysql' => 'uid',
  );
  $a_compare[] = array(
    'name' => 'title',
    'mongo' => 'title',
    'mysql' => 'title',
  );
  $a_compare[] = array(
    'name' => 'Legacy_ID',
    'mongo' => "_id",
    'mysql' => "field_legacy_id",
  );
  $a_compare[] = array(
    'name' => 'nice_url',
    'mongo' => 'nice_url',
    'mysql' => 'nice_url',
  );

  $a_compare[] = array(
    'name' => 'body',
    'mongo' => 'body',
    'mysql' => "body",
  );
  $a_compare[] = array(
    'name' => 'Response_ID',
    'mongo' => 'response_id',
    'mysql' => "field_response_id",
  );
  $a_compare[] = array(
    'name' => 'Petition_Status',
    'mongo' => 'petition_status',
    'mysql' => "field_petition_status",
  );
  $a_compare[] = array(
    'name' => 'Response_Status',
    'mongo' => 'response_status',
    'mysql' => "field_response_status",
  );
  $a_compare[] = array(
    'name' => 'Timestamp_Published',
    'mongo' => 'published',
    'mysql' => "field_timestamp_published",
  );
  $a_compare[] = array(
    'name' => 'Timestamp_Reached_Public',
    'mongo' => 'reached_public',
    'mysql' => "field_timestamp_reached_public",
  );
  $a_compare[] = array(
    'name' => 'Timestamp_Reached_Ready',
    'mongo' => 'reached_ready',
    'mysql' => "field_timestamp_reached_ready",
  );
  $a_compare[] = array(
    'name' => 'Timestamp_Responded',
    'mongo' => 'closed',
    'mysql' => "field_timestamp_responded",
  );
  $a_compare[] = array(
    'name' => 'Signature_Count',
    'mongo' => 'signature_count',
    'mysql' => "field_petition_signature_count",
  );
  $a_compare[] = array(
    'name' => 'Abuse_Count',
    'mongo' => 'abuse_count',
    'mysql' => "field_abuse_count",
  );
  $a_compare[] = array(
    'name' => 'Review_Timeframe',
    'mongo' => 'review_timeframe',
    'mysql' => "field_petition_review_timeframe",
  );
  $a_compare[] = array(
    'name' => 'Signatures_to_reach_response',
    'mongo' => 'response_signatures',
    'mysql' => "field_petition_response_sign",
  );
  $a_compare[] = array(
    'name' => 'Signatures_to_reach_public',
    'mongo' => 'public_signatures',
    'mysql' => "field_petition_public_signatures",
  );
  $a_compare[] = array(
    'name' => 'Short_URL',
    'mongo' => 'short_url',
    'mysql' => "field_short_url",
  );
  $a_compare[] = array(
    'name' => 'Legacy_URL',
    'mongo' => 'nice_url',
    'mysql' => "field_legacy_path",
  );
  $a_compare[] = array(
    'name' => 'Timestamp_Created',
    'mongo' => 'created',
    'mysql' => 'created',
  );

  // let's flatten all of the fields in mysql.
  $fields = array(
    "field_short_url", "field_petition_public_signatures",
    'field_petition_response_sign', 'field_petition_review_timeframe',
    'field_abuse_count', 'field_petition_signature_count', 'field_timestamp_responded',
    'field_legacy_id', 'body', 'field_response_id', 'field_petition_status',
    'field_response_status', 'field_timestamp_published',
    'field_timestamp_reached_public', 'field_timestamp_reached_ready',
    "field_response_status");

  $node = $o_petition->mysql;
  foreach ($fields as $field_name) {
    $items = field_get_items("node", $node, $field_name);
    if ($items) {
      foreach ($items as $item) {
        $o_petition->mysql->{$field_name} = ($field_name == 'field_response_id') ?
                $item['target_id'] : $item['value'];
      }
    }
    else {
      unset($o_petition->mysql->{$field_name});
    }
  }

  // Special handling for Issues:
  // This needs to be compared as a flat array of term ids.
  $o_petition = _migrate_mogo_2mysql_petsig_compare_mysql_field_process($o_petition,
          "field_petition_issues", "tid", $language);

  $a_compare[] = array(
    'name' => 'Issues',
    'mongo' => 'issues',
    'mysql' => "field_petition_issues_processed",
  );

  // Special handling for Related Petitions:
  // This needs to be compared as a flat array of node ids.
  $o_petition = _migrate_mogo_2mysql_petsig_compare_mysql_field_process($o_petition,
          "field_petition_related_petitions", "target_id", $language);

  $a_compare[] = array(
    'name' => 'Related_Petitions',
    'mongo' => 'related_petitions',
    'mysql' => "field_petition_related_petitions_processed",
  );

  // Special handling for Abuse Flags:
  // This needs to be compared as a flat array of user ids.
  $o_petition = _migrate_mogo_2mysql_petsig_compare_mysql_field_process($o_petition,
          "field_abuse_flags", "target_id", $language);

  $a_compare[] = array(
    'name' => 'Abuse_Flags',
    'mongo' => 'abuse_flags',
    'mysql' => "field_abuse_flags_processed",
  );

  // Load mysql signatures for this petition.
  $a_signatures = migrate_mongo2mysql_petsig_signature_list_mysql($petition_id);

  $o_petition->mysql->signatures_list_processed = $a_signatures['list'];
  $o_petition->mysql->signatures_loaded_count = $a_signatures['sig_loaded_count'];
  $o_petition->mysql->signatures_loaded = $a_signatures['signatures'];

  // Load mongo signatures for this petition.
  $a_signatures = migrate_mongo2mysql_petsig_signature_list($petition_id);
  $o_petition->mongo['signatures_loaded'] = $a_signatures['signatures'];
  $o_petition->mongo['signatures_loaded_count'] = $a_signatures['sig_loaded_count'];
  $o_petition->mongo['signatures_list_processed'] = $a_signatures['list'];

  $a_compare[] = array(
    'name' => 'Signatures_Loaded_Count',
    'mongo' => 'signatures_loaded_count',
    'mysql' => 'signatures_loaded_count',
  );
  $a_compare[] = array(
    'name' => 'Signatures',
    'mongo' => 'signatures_list_processed',
    'mysql' => 'signatures_list_processed',
  );

  $o_petition->compare = $a_compare;

  $page_output_html .= theme('migrate_mongo2mysql_petsig_compare_table',
          array('o_petition' => $o_petition));

  return $page_output_html;
}

/**
 * Page callback for admin/reports/mongo-mysql-signature/compare/%.
 *
 * @param string $signature_id
 *   The signature id of the signature being compared
 *
 * @return string
 *   The html of the page comparing a mongo signature to a mysql signature.
 */
function _migrate_mongo2mysql_petsig_signature_page($signature_id = 0) {
  $signature_id = check_plain($signature_id);
  $o_signature = new stdClass();

  // Load the MySQL signature.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'signature_mail')
    ->propertyCondition('legacy_id', $signature_id, '=')
    // Run the query as user 1.
    ->addMetaData('account', user_load(1));

  $a_signature_entity_ids = $query->execute();

  $ids = array_keys((is_array($a_signature_entity_ids['signature_mail'])) ? $a_signature_entity_ids['signature_mail'] : array());
  if (!empty($ids)) {
    $entities = entity_load('signature_mail', $ids);
    $o_signature->mysql = array_pop($entities);
  }
  else {
    $o_signature->mysql = '';
  }

  // Load the Mongo signature.
  $sig_connection = wh_petitions_mongo_petition_signatures_connection();
  $query = array(
    '_id' => new MongoId($signature_id),
  );

  $o_signature->mongo = $sig_connection->findOne($query);

  // Process links to petition.
  $path = "/admin/reports/mongo-mysql-petition/compare";
  $o_signature->mongo['petition_link'] = "<a href=\"$path/" . $o_signature->mongo['petition_id'] . "\">" . $o_signature->mongo['petition_id'] . "</a>";
  $o_signature->mysql->petition_link = "<a href=\"$path/" . $o_signature->mysql->legacy_petition_id . "\">" . $o_signature->mysql->legacy_petition_id . "</a>";

  // Set the compares.
  $a_compare = array();
  $a_compare[] = array(
    'name' => 'petition_id',
    'mongo' => "['petition_link']",
    'mysql' => 'petition_link',
  );

  $a_compare[] = array(
    'name' => 'timestamp',
    'mongo' => "['timestamp']",
    'mysql' => 'timestamp',
  );

  $a_compare[] = array(
    'name' => 'ip_address',
    'mongo' => "['ip_address']",
    'mysql' => 'ip_address',
  );

  $a_compare[] = array(
    'name' => 'user_agent',
    'mongo' => "['user_agent']",
    'mysql' => 'user_agent',
  );

  $a_compare[] = array(
    'name' => 'uid',
    'mongo' => "['uid']",
    'mysql' => 'uid',
  );

  $a_compare[] = array(
    'name' => 'user_first_name',
    'mongo' => "['user']['first_name']",
    'mysql' => 'user_first_name',
  );

  $a_compare[] = array(
    'name' => 'user_last_name',
    'mongo' => "['user']['last_name']",
    'mysql' => 'user_last_name',
  );

  $a_compare[] = array(
    'name' => 'user_city',
    'mongo' => "['user']['city']",
    'mysql' => 'user_city',
  );

  $a_compare[] = array(
    'name' => 'user_state',
    'mongo' => "['user']['state']",
    'mysql' => 'user_state',
  );

  $a_compare[] = array(
    'name' => 'user_zip',
    'mongo' => "['user']['zip']",
    'mysql' => 'user_zip',
  );

  $a_compare[] = array(
    'name' => 'username',
    'mongo' => "['user']['username']",
    'mysql' => 'user_username',
  );

  $o_signature->compare = $a_compare;

  $output_html = theme('migrate_mongo2mysql_petsig_sig_compare',
          array('o_signature' => $o_signature, 'signature_id' => $signature_id));

  return $output_html;
}

/**
 * Process multivalue fields to match MongoDB structures.
 *
 * @param object $petitions
 *   An object containing versions of the mongodb and mysql petitions.
 * @param string $field_name
 *   Which module is being processed.
 * @param string $key
 *   The key where the actual value in this field is stored.
 * @param string $language
 *   The language that needs processing.
 */
function _migrate_mogo_2mysql_petsig_compare_mysql_field_process($petitions,
        $field_name, $key, $language = LANGUAGE_NONE) {
  $processed_field_name = "{$field_name}_processed";
  $petitions->mysql->{$processed_field_name} = array();

  $items = field_get_items("node", $petitions->mysql, $field_name, $language);

  if ($items) {
    foreach ($items as $result) {
      $petitions->mysql->{$processed_field_name}[] = $result[$key];
    }
  }

  return $petitions;
}

/**
 * Loads the petition node based on a petition id.
 *
 * @param int $petition_id
 *   The petition id to be loaded
 *
 * @return object|bool
 *   - object of the fully loaded petition.
 *   - FALSE if there is no petition node.
 */
function migrate_mongo2mysql_petsig_petition_node_load($petition_id) {
  $o_petition = FALSE;

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'petition')
    ->fieldCondition('field_legacy_id', 'value', $petition_id, '=')
    ->range(0, 1)
    // Run the query as user 1.
    ->addMetaData('account', user_load(1));

  $a_petition_nodes = $query->execute();

  $ids = array_keys($a_petition_nodes['node']);

  if (!empty($ids)) {
    $o_petition = entity_load('node', $ids);
    $o_petition = array_pop($o_petition);
  }

  // Include path of node.
  if (!empty($o_petition->nid)) {
    // Trim leading slash to match mongos method.
    $o_petition->nice_url = ltrim(url('node/' . $o_petition->nid), '/');
  }

  return $o_petition;
}


/**
 * When passed an array , returns friendly html, all else pass through.
 *
 * @param mixed $input
 *   A variable to be processed
 *
 * @return string
 *   Suitable for output.
 */
function migrate_mongo2mysql_petsig_arrayout($input = '') {
  if (is_array($input)) {
    $readable = "<pre>\n";
    $readable .= print_r($input, TRUE);
    $readable .= "</pre>\n";
    return $readable;
  }
  else {
    return $input;
  }
}


/**
 * Returns a list of MONGO linked signatures from a petition legacy id.
 *
 * @param string $petition_id
 *   A 24 character petition_id or legacy_id.
 *
 * @return array
 *   list = Linked list of MONGO petition ids that link to the signature
 *   comparison.
 *   signatures = The array of signatures associated to the petition_id
 */
function migrate_mongo2mysql_petsig_signature_list($petition_id = '') {
  $a_return = array();

  $sig_connection = wh_petitions_mongo_petition_signatures_connection();

  $query = array(
    'petition_id' => (string) $petition_id,
  );
  $sort = array('_id' => -1);

  $a_return['signatures'] = $sig_connection->find($query)->sort($sort);
  $a_return['sig_loaded_count'] = $a_return['signatures']->count();

  $a_return['list'] = theme('migrate_mongo2mysql_petsig_sig_list', array('o_signatures' => $a_return['signatures']));
  return $a_return;
}


/**
 * Returns a list of MYSQL linked signatures from a petition legacy id.
 *
 * @param string $petition_id
 *   A 24 character petition_id or legacy_id.
 *
 * @return array
 *   list = Linked list of MYSQL petition ids that link to the signature
 *   comparison.
 *   signatures = The array of MYSQL signatures associated to the petition_id
 */
function migrate_mongo2mysql_petsig_signature_list_mysql($petition_id = '') {
  $a_return = array();

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'signature_mail')
    ->propertyCondition('legacy_petition_id', $petition_id, '=')
    // Run the query as user 1.
    ->addMetaData('account', user_load(1));

  $a_signature_entity_ids = $query->execute();

  $ids = array_keys((is_array($a_signature_entity_ids['signature_mail'])) ? $a_signature_entity_ids['signature_mail'] : array());

  if (!empty($ids)) {
    $a_return['signatures'] = entity_load('signature_mail', $ids);
  }
  else {
    $a_return['signatures'] = '';
  }

  $a_return['sig_loaded_count'] = count($a_return['signatures']);
  $a_return['list'] = theme('migrate_mongo2mysql_petsig_sig_list_mysql', array('a_signatures' => $a_return['signatures']));

  return $a_return;
}
