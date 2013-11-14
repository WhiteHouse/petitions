<?php
/**
 * @file
 * api_key_manager.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function api_key_manager_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function api_key_manager_node_info() {
  $items = array(
    'key' => array(
      'name' => t('API Key'),
      'base' => 'node_content',
      'description' => t('API Key nodes are created by users to request access to the API.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
