<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function wh_user_profile_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_default_profile2_type().
 */
function wh_user_profile_default_profile2_type() {
  $items = array();
  $items['main'] = entity_import('profile2_type', '{
    "userCategory" : true,
    "userView" : true,
    "type" : "main",
    "label" : "Main profile",
    "weight" : "0",
    "data" : { "registration" : true, "use_page" : true },
    "rdf_mapping" : []
  }');
  return $items;
}
