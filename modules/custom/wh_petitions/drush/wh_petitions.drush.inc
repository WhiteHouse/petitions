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
  
  $items['petitions-signatures-report'] = array(
    'callback' => 'wh_petitions_signatures_report',
    'description' => 'Create a list of signatures for a specific petition.',
    'arguments' => array(
      'petition_id' => "The long string unique ID of the petition. Can be found in mongoDB"
    ),
    'options' => array(
      'report_type' => "Define the appropriate report to run.",
      'filepath' => "Set the file path that the report will be written to"
    ),
    'aliases' => array('sig-report'),
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

function wh_petitions_signatures_report($arguments) {
  
  //Set up the report type
  $report_type = '';
  if($report_type_options = drush_get_option('report_type')) {
    $report_type = $report_type_options[0];
  }
  
  $collection = mongodb_collection('petition_signatures');
 
  $query = array('petition_id' => $petition_id);
  $fields = array('uid', 'timestamp');
  
  $mongo_signatures = $signatures_collection->find($query, $fields)->skip($context['sandbox']['progress'])->limit($limit);
  
  $id_list = array();
  $timestamps = array();
  foreach ($mongo_signatures as $signature) {
    $context['sandbox']['progress']++;
    $id_list[$signature['uid']] = array(
      'timestamp' => $signature['timestamp'],
    );
  }
 
  $mysql_query = db_select('users', 'u');
  $mysql_query->join('field_data_field_first_name', 'fn', 'u.uid = fn.entity_id');
  $mysql_query->join('field_data_field_last_name', 'ln', 'u.uid = ln.entity_id');
  $mysql_query->join('field_data_field_city', 'fc', 'u.uid = fc.entity_id');
  $mysql_query->join('field_data_field_state', 'fs', 'u.uid = fs.entity_id');
  $mysql_query->join('field_data_field_zip', 'fz', 'u.uid = fz.entity_id');
  $mysql_query->fields('u', array('uid', 'mail'));
  $mysql_query->fields('fn', array('field_first_name_value'));
  $mysql_query->fields('ln', array('field_last_name_value'));
  $mysql_query->fields('fc', array('field_city_value'));
  $mysql_query->fields('fs', array('field_state_value'));
  $mysql_query->fields('fz', array('field_zip_value'));
  $mysql_query->condition('u.uid', array(array_keys($id_list)), 'IN');
  $result = $mysql_query->execute();
  
  $rows = array();
  
  while ($record = $result->fetchAssoc()) {
    
    switch($report_type) {
      case 'email_csv':
        fwrite($file_handle, $record['mail'] . PHP_EOL);
        break;
      default:
        $row = array(
          'uid' => $record['uid'],
          'mail' => $record['mail'],
          'firstname' => csv_sanitize($record['field_first_name_value']),
          'lastname' => csv_sanitize($record['field_last_name_value']),
          'city' => csv_sanitize($record['field_city_value']),
          'state' => csv_sanitize($record['field_state_value']),
          'zip' => $record['field_zip_value'],
          'signature_timestamp' => $timestamps[$record['uid']]
        );
        $rows[] = $row;
        break;  
    }

    $filepath = './subscriber_export_' . $petition_id . '_' . $time_stamp . '.csv';
    
    // If a filepath is specified, override the default
    if($filepath_options = drush_get_option('filepath')) {
      $filepath = $filepath_options[0];
    }

    if($handle = fopen($filepath, 'w')) {
      fwrite($handle, implode(PHP_EOL, $rows));
    }
    
    fclose($handle);
  }
}
