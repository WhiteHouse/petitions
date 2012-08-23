<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function wh_petition_page_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function wh_petition_page_node_info() {
  $items = array(
    'wh_petitions_page' => array(
      'name' => t('Page'),
      'base' => 'node_content',
      'description' => t('Pages that appear in the How & Why Section'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
