<?php

/**
 * @file
 * Separate anything related to our MongoDB to MySQL transition.
 *
 * @todo This will be removed once mongo support is no longer needed.
 *   Remove shunt enabling from signature_mail.install when Mongo is removed.
 */

/**
 * Get the input from what is saved to mongodb transform it and save it.
 *
 * @param array $mongodb_signature
 *   An array of data as it is structure when loaded from MongoDB.
 */
function signature_mail_mongo2mysql_save($mongodb_signature) {
  // Turn signature_id into the legacy_id.
  if (array_key_exists('signature_id', $mongodb_signature)) {
    $_id = $mongodb_signature['signature_id'];
    unset($mongodb_signature['signature_id']);
    $mongodb_signature['legacy_id'] = $_id;
  }

  // Turn the mongodb petition id, into the legacy_petition_id.
  $petition_id = $mongodb_signature['petition_id'];
  unset($mongodb_signature['petition_id']);
  $mongodb_signature['legacy_petition_id'] = $petition_id;

  // Let's get the node id of the petition.
  $node = petition_load_from_legacy_id($petition_id);
  if ($node) {
    $mongodb_signature['petition_id'] = $node->nid;
  }

  $property_map = array(
    "first_name" => "user_first_name",
    "last_name" => "user_last_name",
    "zip" => "user_zip",
    "timestamp_submitted" => "timestamp",
    "client_ip" => "ip_address",
  );

  foreach ($property_map as $current => $needed) {
    $temp = $mongodb_signature[$current];
    unset($mongodb_signature[$current]);
    $mongodb_signature[$needed] = $temp;
  }

  // Let's double check that we have default values for all properties.
  $schema = drupal_get_schema("signature_mail");
  foreach ($schema['fields'] as $property => $info) {
    if (empty($mongodb_signature[$property])) {
      if ($property != 'id' && ($info['type'] == 'text' ||
        $info['type'] == "varchar")) {
        $mongodb_signature[$property] = "";
      }
      elseif ($property != 'id' && $info['type'] == 'int') {
        $mongodb_signature[$property] = 0;
      }
    }
  }

  $signature = entity_create("signature_mail", $mongodb_signature);
  $signature->save();
}

/**
 * Implements hook_shunt().
 */
function signature_mail_shunt() {
  return array(
    'signature_mail_mysql_save' => t('Disable signature writes to MySQL.'),
  );
}

/**
 * Disable mysql. (This uses a shunt trip that turns off access to mysql.)
 */
function signature_mail_disable_mysql() {
  // Set the initial state of MySQL shunt to disable mysql writes.
  // @todo This default setting should be removed when mongo is fully removed.
  shunt_enable_shunt('signature_mail_mysql_save');
}
