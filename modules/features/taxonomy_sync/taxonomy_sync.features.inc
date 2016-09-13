<?php
/**
 * @file
 * taxonomy_sync.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function taxonomy_sync_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
}
