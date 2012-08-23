<?php

/**
 * Implementation of hook_strongarm().
 */
function wh_petition_page_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_wh_petitions_page';
  $strongarm->value = 0;
  $export['comment_anonymous_wh_petitions_page'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_wh_petitions_page';
  $strongarm->value = 1;
  $export['comment_default_mode_wh_petitions_page'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_wh_petitions_page';
  $strongarm->value = '50';
  $export['comment_default_per_page_wh_petitions_page'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_wh_petitions_page';
  $strongarm->value = 1;
  $export['comment_form_location_wh_petitions_page'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_wh_petitions_page';
  $strongarm->value = '1';
  $export['comment_preview_wh_petitions_page'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_wh_petitions_page';
  $strongarm->value = 1;
  $export['comment_subject_field_wh_petitions_page'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_wh_petitions_page';
  $strongarm->value = '1';
  $export['comment_wh_petitions_page'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_wh_petitions_page';
  $strongarm->value = array();
  $export['node_options_wh_petitions_page'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_wh_petitions_page';
  $strongarm->value = '1';
  $export['node_preview_wh_petitions_page'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_wh_petitions_page';
  $strongarm->value = 0;
  $export['node_submitted_wh_petitions_page'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_wh_petitions_page_pattern';
  $strongarm->value = 'how-why/[node:title]';
  $export['pathauto_node_wh_petitions_page_pattern'] = $strongarm;

  return $export;
}
