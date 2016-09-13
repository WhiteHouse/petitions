<?php
/**
 * @file
 * wh_petition_page.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function wh_petition_page_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function wh_petition_page_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function wh_petition_page_node_info() {
  $items = array(
    'wh_petitions_page' => array(
      'name' => t('Page'),
      'base' => 'node_content',
      'description' => t('Pages to hold non-petition, non-response content'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
