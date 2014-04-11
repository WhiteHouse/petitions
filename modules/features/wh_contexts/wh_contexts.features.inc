<?php
/**
 * @file
 * wh_contexts.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function wh_contexts_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
}
