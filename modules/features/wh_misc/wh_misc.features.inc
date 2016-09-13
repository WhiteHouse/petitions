<?php
/**
 * @file
 * wh_misc.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function wh_misc_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
