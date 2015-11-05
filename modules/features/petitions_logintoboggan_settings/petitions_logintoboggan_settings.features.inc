<?php
/**
 * @file
 * petitions_logintoboggan_settings.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function petitions_logintoboggan_settings_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
