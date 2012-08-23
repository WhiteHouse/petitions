<?php

/**
 * Implementation of hook_strongarm().
 */
function petitions_user_registration_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'login_destination_immediate_redirect';
  $strongarm->value = 1;
  $export['login_destination_immediate_redirect'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'login_destination_preserve_destination';
  $strongarm->value = 1;
  $export['login_destination_preserve_destination'] = $strongarm;

  return $export;
}
