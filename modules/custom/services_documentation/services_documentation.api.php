<?php

/**
 * @file
 * Example implementation of Services Documentation API.
 */

/**
 * Implements hook_services_resources().
 *
 * This is similar to a standard Services resource definition, with two
 * additional array keys: 'documentation callback' and 'documentation versions'.
 *
 * @see services/services.services.api.php
 */
function api_resource_users_services_resources() {
  $resources = array();

  // Standard Services resource definition.
  $resources['users'] = array(
    'index' => array(
      'callback' => '_api_resource_users_resource_index',
      'args' => array(
        array(
          'name' => 'page',
          'optional' => TRUE,
          'type' => 'int',
          'description' => 'The zero-based index of the page to get, defaults to 0.',
          'default value' => 0,
          'source' => array('param' => 'page'),
        ),
      ),
      'access arguments' => array('access user profiles'),
      'access arguments append' => FALSE,
      // New documentation array keys made available by services_documentation.
      'documentation callback' => '_api_resource_users_index_doc',
      'documentation versions' => array(1000),
    ),
  );

  return $resources;
}

/**
 * Documentation callback for index operation of users resource.
 */
function _api_resource_users_index_doc() {
  $element = array(
    '#name' => t('name'),
    '#description' => t('desc'),
    // Example request. E.g., a request URL, headers, and a JSON array.
    '#request' => t('request'),
    // Example response. E.g., a JSON array.
    '#response' => t('response'),
  );

  return $element;
}


/**
 * Implements services_documentation_versions_alter().
 *
 * This allows you to define versioning information for your services
 * documentation. This information will be used to generate the documentation
 * overview page.
 */
function services_documentation_versions_alter($info) {
  $info = array(
    'default_version' => 1000,
    'versions' => array(1000),
  );
  $info['current_version'] = max($info['versions']);
  drupal_alter('services_documentation_versions')

  return $info;
}
