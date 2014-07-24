<?php
/**
 * @file
 * Contains non-hooks functions that are used within the Petition module.
 */

/**
 * Find a node from a mongodb id.
 * 
 * @param string $legacy_id
 *   The legacy ID which may have originiated in mongo or is the node id.
 *
 * @return NULL|object
 *   NULL : If there is no petition to load.
 *   object : A fully populated node object.
 */
function petition_load_from_legacy_id($legacy_id) {
  if ($nid = petition_get_nid($legacy_id)) {
    $node = node_load($nid);
  }
  return (isset($node) && is_object($node)) ? $node : NULL;
}


/**
 * Helper function to delete all petition nodes and output a message with count.
 */
function petition_delete_all() {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'petition');
  $results = $query->execute();
  if (!empty($results)) {
    foreach ($results['node'] as $nid => $info) {
      node_delete($nid);
    }

    $petition_quantity = count($results['node']);
    drupal_set_message($message = t('There were @petition_quantity petitions deleted.', array('@petition_quantity' => $petition_quantity)), $type = 'status');
  }
  else {
    drupal_set_message($message = t('Petitions node deletion attempted: There were no Petition nodes to delete.'), $type = 'status');
  }
}

/**
 * Insert a renderable array at a certain position in a build array.
 *
 * @param string $key
 *   The key in the array where we should insert our renderable array
 * @param string $value_key
 *   The key that we want to add to the build array
 * @param array $value
 *   The renderable array that we want to insert
 * @param array $array
 *   The original build array where we are inserting things
 *
 * @return array
 *   The new build array
 */
function _petition_array_pre_insert($key, $value_key, $value, $array) {
  $key_position = array_search($key, array_keys($array));
  $value['#weight'] = $key_position;

  $res = array_slice($array, 0, $key_position, TRUE) +
    array($value_key => $value) +
    array_slice($array, $key_position, count($array) - $key_position, TRUE);

  // Let's reorder elements.
  $counter = 0;
  foreach ($res as $key => $value) {
    if (is_array($value)) {
      $value['#weight'] = $counter;
      $res[$key] = $value;
    }
    $counter++;
  }

  return $res;
}
