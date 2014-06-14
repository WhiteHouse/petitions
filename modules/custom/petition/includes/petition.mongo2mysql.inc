<?php
/**
 * @file
 * Functions specific to the mongo implementation of the Petiton module.
 *
 * @todo This will likely be removed once mongo support is no longer needed.
 *   Be sure to strip out shunt default setting in petition.install
 */


/**
 * Take data as it is saved to mongodb, transform it, and save it as a node too.
 *
 * @param array $mongodb_petition
 *   The mongo petition array containing all the values loaded from mongoDB
 *   petition.
 */
function petition_mongo2mysql_save($mongodb_petition) {
  // Lets make the _id plain to make our life easier.
  $mongodb_petition['id'] = $mongodb_petition['_id']->__toString();

  // If petition already exists, load it.
  if ($nid = petition_get_nid($mongodb_petition['id'])) {
    $node = node_load($nid);
  }
  // Otherwise, create a new one.
  else {
    $node_array = array();
    // The type is always the same.
    $node_array['type'] = "petition";
    $node = entity_create('node', $node_array);
  }

  $node->status = (petition_mongo2mysql_is_mongo_published($mongodb_petition)) ? 1 : 0;

  // The node properties we are interested in are: type, title, uid, status,
  // created, and changed.
  $properties = array('title', 'uid', 'created');

  // Changed is always whatever time it is right now.
  $node->changed = time();

  // The rest of the properties must be set, so let's check.
  foreach ($properties as $property) {
    if (array_key_exists($property, $mongodb_petition)) {
      $node->{$property} = $mongodb_petition[$property];
    }
  }

  // The rest of the data is organized in fields:
  // body:Long text and summary.
  $info = array('body' => "body");
  $key = 'value';
  _petition_set_field($node, $mongodb_petition, $info, $key);

  // field_petition_signature_count:Integer.
  // field_petition_public_signatures:Integer.
  // field_petition_review_timeframe:Integer.
  // field_abuse_count:Integer. @todo What do we do with this?
  // field_petition_response_sign:Integer.
  // field_response_status:List (integer).
  // field_petition_status:List (integer).
  $info = array(
    'field_petition_signature_count' => "signature_count",
    'field_petition_public_signatures' => "public_signatures",
    'field_petition_review_timeframe' => "review_timeframe",
    'field_petition_response_sign' => 'response_signatures',
    'field_response_status' => "response_status",
    'field_petition_status' => "petition_status");
  $key = 'value';
  _petition_set_field($node, $mongodb_petition, $info, $key);

  // field_legacy_id:Text.
  // field_short_url:Text.
  $info = array('field_legacy_id' => 'id', 'field_short_url' => 'short_url');
  $key = 'value';
  _petition_set_field($node, $mongodb_petition, $info, $key);

  // field_petition_issues:Term reference.
  $info = array('field_petition_issues' => 'issues');
  $key = 'tid';
  _petition_set_field($node, $mongodb_petition, $info, $key);

  // field_petition_related_petitions:Entity Reference.
  // field_abuse_flags:Entity Reference.
  // field_response_id:Entity Reference. @todo This does not exist in mongo.
  $info = array(
    'field_petition_related_petitions' => 'related_petitions',
    'field_abuse_flags' => 'abuse_flags',
    // 'field_response_id' => 'related_petitions',
  );
  $key = 'target_id';
  _petition_set_field($node, $mongodb_petition, $info, $key);

  // field_timestamp_reached_public:Date.
  // field_timestamp_reached_ready:Date.
  // field_timestamp_published:Date.
  // field_timestamp_responded:Date.
  $info = array(
    'field_timestamp_reached_public' => 'reached_public',
    'field_timestamp_reached_ready' => 'reached_ready',
    'field_timestamp_published' => 'published',
    'field_timestamp_responded' => 'closed',
  );
  $key = 'value';
  _petition_set_field($node, $mongodb_petition, $info, $key);

  node_save($node);
}

/**
 * Set a field in a node.
 *
 * @param object $node
 *   A node object.
 * @param array $raw_data
 *   An array with data as it is loaded from MongoDB.
 * @param array $info
 *   A simple map in which the key is the name of a field on the node, and
 *   the value is the key of a piece of data from the $raw_data array.
 * @param string $key
 *   We use the key to get around the ambiguity of multiple field types and how
 *   they store the data, ex. if we are mapping to numeric or text fields, the
 *   key param should be "value", if we are mapping entityreference fields it
 *   should be target_id.
 */
function _petition_set_field($node, $raw_data, $info, $key) {
  foreach ($info as $field => $raw) {
    if (array_key_exists($raw, $raw_data)) {
      $value = $raw_data[$raw];
      if (is_array($value)) {
        $counter = 0;
        foreach ($value as $v) {
          $node->{$field}[LANGUAGE_NONE][$counter][$key] = $v;
          $counter++;
        }
      }
      else {
        $node->{$field}[LANGUAGE_NONE][0][$key] = $value;
      }
    }
  }
}

/**
 * Implements hook_wh_petitions_petition_save().
 */
function petition_wh_petitions_petition_save($petition) {

  if (shunt_is_enabled('petition_mysql_save')) {
    return;
  }

  petition_mongo2mysql_save($petition);
}

/**
 * Implements hook_wh_petitions_petition_close().
 */
function petition_wh_petitions_petition_close($petition) {

  if (shunt_is_enabled('petition_mysql_save')) {
    return;
  }

  $node = petition_load_from_legacy_id($petition['_id']);
  if ($node) {
    $node->field_petition_status[LANGUAGE_NONE][0]['value'] = WH_PETITION_STATUS_CLOSED;
    // We are not saving the time of closing.
    node_save($node);
  }
}

/**
 * Implements hook_wh_petitions_petition_body_update().
 */
function petition_wh_petitions_petition_body_update($petition_id, $body) {

  if (shunt_is_enabled('petition_mysql_save')) {
    return;
  }

  $node = petition_load_from_legacy_id($petition_id);
  if ($node) {
    $node->body[LANGUAGE_NONE][0]['value'] = $body;
    node_save($node);
  }
}

/**
 * Implements hook_wh_petitions_petition_inappropriate().
 */
function petition_wh_petitions_petition_inappropriate($petition_id) {

  if (shunt_is_enabled('petition_mysql_save')) {
    return;
  }

  $node = petition_load_from_legacy_id($petition_id);

  if ($node) {
    $items = field_get_items("node", $node, "field_abuse_count");
    if ($items) {
      $node->field_abuse_count[LANGUAGE_NONE][0]['value']++;
    }
    else {
      $node->field_abuse_count[LANGUAGE_NONE][0]['value'] = 0;
    }

    $items = field_get_items("node", $node, "field_abuse_flags");

    if ($items) {
      $node->field_abuse_flags[LANGUAGE_NONE][]['target_id'] = $user->uid;
    }
    else {
      $node->field_abuse_flags[LANGUAGE_NONE][0]['target_id'] = $user->uid;
    }

    node_save($node);
  }
}

/**
 * Implements hook_shunt().
 */
function petition_shunt() {
  return array(
    'petition_mysql_save' => t('Disable petition writes to MySQL.'),
  );
}

/**
 * Checks to see if a mongo_petition is published.
 *
 * Determining if the published/unpublished state of a mongo petition is
 * surprisingly complex.
 *
 * @param array $mongo_petition
 *   A Mongo petition--NOT a MySQL petition.
 *
 * @return bool
 *   Returns TRUE if the mongo petition published. FALSE otherwise.
 */
function petition_mongo2mysql_is_mongo_published(array $mongo_petition) {
  $was_published = isset($mongo_petition['published']);
  $is_published = !in_array($mongo_petition['petition_status'], wh_petitions_unpublished_statuses());

  if ($was_published && !$is_published) {
    return FALSE;
  }
  elseif ($was_published && $is_published) {
    return TRUE;
  }
  else {
    // $was_published is FALSE, $is_published is TRUE. This shouldn’t be
    // possible.
    watchdog("Mongo2Mysql", "Encountered a petition with an indeterminate published state.");
  }
}


/**
 * Function to load an individual mongo petition from a URL.
 *
 * @param string $url
 *   URL for the current petition.
 */
function petition_load_mongo2mysql_from_url($url) {
  $conn = wh_petitions_mongo_petition_connection();
  $petition = $conn->findOne(array('nice_url' => $url), array('title'));
  return $petition;
}
