<?php

function wh_petitions_drush_command() {
  $items = array();

  $items['petitions-generate'] = array(
    'callback' => 'wh_petitions_generate',
    'description' => "Generate random petitions for testing.",
    'arguments' => array(
      'number' => 'The number of petitions to generate.',
      'uid' => 'The user ID to create the petitions with.',
      'status' => 'The petition status to generate these petitions with.',
    ),
    'aliases' => array('petgen'),
  );

  $items['petitions-check-compliance-review-threshold'] = array(
    'callback' => 'wh_petitions_compliance_review_threshold_check_drush',
    'description' => "Check for petitions with enough signatures to pass the compliance review threshold.",
    'arguments' => array(
    ),
    'aliases' => array('petcrt'),
  );


  return $items;
}


function wh_petitions_generate($number, $uid = NULL, $status = NULL) {
  module_load_include('inc', 'wh_petitions', 'wh_petitions.generate');
  create_petitions($number, $uid, $status);
}

function wh_petitions_compliance_review_threshold_check_drush() {
  $email = variable_get('wh_petitions_compliance_reviewer_emails', FALSE);
  $threshold = (int)variable_get('wh_petitions_compliance_review_threshold', FALSE);

  if ($email && $threshold) {
    wh_petitions_compliance_review_threshold_check($threshold, $email);
  }  
}
