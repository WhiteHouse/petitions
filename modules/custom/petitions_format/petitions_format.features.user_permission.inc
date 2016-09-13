<?php
/**
 * @file
 * petitions_format.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function petitions_format_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'use text format petitions_filtered'.
  $permissions['use text format petitions_filtered'] = array(
    'name' => 'use text format petitions_filtered',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'filter',
  );

  return $permissions;
}
