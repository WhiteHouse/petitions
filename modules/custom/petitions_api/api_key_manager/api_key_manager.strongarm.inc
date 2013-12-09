<?php
/**
 * @file
 * api_key_manager.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function api_key_manager_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_key';
  $strongarm->value = array();
  $export['menu_options_key'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_key';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_key'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_key';
  $strongarm->value = array(
    0 => 'status',
    1 => 'revision',
  );
  $export['node_options_key'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_key';
  $strongarm->value = '0';
  $export['node_preview_key'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_key';
  $strongarm->value = 0;
  $export['node_submitted_key'] = $strongarm;

  return $export;
}
