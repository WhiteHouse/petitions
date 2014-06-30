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
      'petition_id' => "The long string unique ID of the petition. Can be found in mongoDB",
      'name' => "Human readable name to be used in filename like this: signatures-report.petition_id.name.txt",
    ),
    'options' => array(
      'batch_size' => "Define the number of signatures that should be queried in each batch. Defaults to 1000.",
      'report_type' => "Define the appropriate report to run (e.g. email_csv).",
      'filepath' => "Set the file path that the report will be written to",
    ),
    'aliases' => array('sig-report'),
  );

  $items['petitions-signatures-report-get-signatures-with-ips'] = array(
    'callback' => 'wh_petitions_get_signatures_uids_with_ips',
    'description' => 'Writes a list of signatures uids and IPs for a petition',
    'arguments' => array(
      'petition_id' => "The long string unique ID of the petition. Can be found in mongoDB",
    ),
    'options' => array(
      'filepath' => "Set the file path that the report will be written to",
      'name' => 'Name to use for file',
    ),
    'examples' => array(
      'drush sig-uids 509a9ec76ce61c3449000005' => 'Get signature uids for this petition',
      'drush sig-uids 509a9ec76ce61c3449000005 --name=Louisiana --filepath=/home/bryanhirsch' => 'Write results to a file called /home/bryanhirsch/signatures-uids.509a9ec76ce61c3449000005.Louisiana.txt',
    ),
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

/**
 * Write a given petition's signatures to a file.
 *
 * @param string $petition_id
 *
 * @param string $name
 *  The name appended to the file being written.
 */
function wh_petitions_signatures_report($petition_id, $name) {
  // Operate on the filepath option.
  if(!$filepath = drush_get_option('filepath')) {
    drush_print('Please specify the --filepath option.');
    return;
  }
  else {
    // Define the name and path of the file being written.
    $filename = sprintf('%s/signatures-report.%s.%s.txt', $filepath, $petition_id, $name);
    // Confirm that the file does not already exist before proceeding.
    if (file_exists($filename)) {
      drush_print("File exists:\n$filename");
      return;
    }
  }

  // Define the report type.
  if(!$report_type = drush_get_option('report_type')) {
    $report_type = '';
  }

  // Retrieve an array of uids.
  $uids = _wh_petitions_get_signature_uids($petition_id);

  // Print the count of a petition's signatures.
  $signatures_count = count($uids);

  // Create a file containing uids associated to a petition's signatures.
  $uids_file = _wh_petitions_write_uids_to_file($petition_id, $filepath, $name, $uids);

  // Define the number of signatures to write in each batch.
  if (!$batch_size = drush_get_option('batch_size')) {
    $batch_size = 1000;
  }

  // Write records in batches.
  drush_print("Processing...");
  while ($uids_batch = _wh_petitions_get_uids_batch($uids_file, $batch_size)) {
    $uids_write = _wh_petitions_signatures_report_rows($uids_batch, $report_type);
    if ($report_type != 'email_csv') {
      $fp = fopen($filename, a);
      foreach ($uids_write as $uid_write) {
        fputcsv($fp, $uid_write);
      }
      fclose($fp);
    }
    else {
      file_put_contents($filename, implode(PHP_EOL, $uids_write), FILE_APPEND);
      file_put_contents($filename, PHP_EOL, FILE_APPEND);
    }
  }

  // Delete the file containing uids associated to a petition's signatures.
  unlink($uids_file);

  // Compare the petition's signature count to the number of records written.
  $records_count = count(explode(PHP_EOL, file_get_contents($filename))) - 1 ;
  if ($signatures_count == $records_count) {
    drush_print("Success! $signatures_count expected and $records_count written.");
  }
  else {
    drush_print("Error. $signatures_count expected and $records_count written.");
  }

  // The process is complete.
  drush_print("All uids have been processed and stored here:\n$filename");
}

/**
 * Return an array of values to be written.
 *
 * @param array $uids
 *
 * @param string $report_type
 *
 * @return array
 *  Rows to be appended to a petitions signatures report.
 */
function _wh_petitions_signatures_report_rows($uids, $report_type) {
  // Set up the query.
  $mysql_query = db_select('users', 'u');
  $mysql_query->fields('u', array('uid', 'mail'));
  // Skip joins if the report type is email_csv.
  // @todo Fix this. It doesn't work.
  if ($report_type != 'email_csv') {
    $mysql_query->join('field_data_field_first_name', 'fn', 'u.uid = fn.entity_id');
    $mysql_query->join('field_data_field_last_name', 'ln', 'u.uid = ln.entity_id');
    $mysql_query->join('field_data_field_city', 'fc', 'u.uid = fc.entity_id');
    $mysql_query->join('field_data_field_state', 'fs', 'u.uid = fs.entity_id');
    $mysql_query->join('field_data_field_zip', 'fz', 'u.uid = fz.entity_id');
    $mysql_query->fields('fn', array('field_first_name_value'));
    $mysql_query->fields('ln', array('field_last_name_value'));
    $mysql_query->fields('fc', array('field_city_value'));
    $mysql_query->fields('fs', array('field_state_value'));
    $mysql_query->fields('fz', array('field_zip_value'));
  }
  $mysql_query->condition('u.uid', $uids, 'IN');
  // Run the query.
  $result = db_query("SELECT uid, mail FROM {users} WHERE uid IN (:uids) ", array(':uids' => $uids));
  $records = $result->fetchAllAssoc('uid', PDO::FETCH_ASSOC);

  $rows = array();

  foreach ($records as $record) {

    switch($report_type) {
      case 'email_csv':
        $rows[] = $record['mail'];
        break;

      default:
        $row = array(
          // Fun fact: this doesn't work.
          // @todo Fix this. It doesn't work.
          'uid' => $record['uid'],
          'mail' => $record['mail'],
          'firstname' => _wh_petitions_csv_sanitize($record['field_first_name_value']),
          'lastname' => _wh_petitions_csv_sanitize($record['field_last_name_value']),
          'city' => _wh_petitions_csv_sanitize($record['field_city_value']),
          'state' => _wh_petitions_csv_sanitize($record['field_state_value']),
          'zip' => $record['field_zip_value'],
          'signature_timestamp' => $timestamps[$record['uid']]
        );
        $rows[] = $row;
        break;
    }
  }

  return $rows;
}

/**
 * Return an array of uids associated to a petition's signatures.
 *
 * @param string $petition_id
 *
 * @return array
 */
function _wh_petitions_get_signature_uids($petition_id) {
  $collection = mongodb_collection('petition_signatures');
  $query = array('petition_id' => $petition_id);
  $fields = array('uid', 'timestamp', 'ip_address', 'user');
  $mongo_signatures = $collection->find($query, $fields);

  $uids = array();
  foreach ($mongo_signatures as $signature) {
    $uid = $signature['uid'];
    $uids[$uid] = array(
      'uid' => $uid,
      'username' => $signature['user']['username'],
      'timestamp' => $signature['timestamp'],
      'ip_address' => $signature['ip_address'],
    );
  }
  ksort($uids);

  return $uids;
}

/**
 * Write uids associated to a petition's signatures to a file.
 *
 * @param string $petition_id
 *
 * @param string $filepath
 *
 * @param string $name
 *  The name appended to the file being written.
 *
 * @param array $uids
 *
 * @return string $filename
 */
function _wh_petitions_write_uids_to_file($petition_id, $filepath, $name, $uids) {
  $id_list = array();
  foreach ($uids as $uid => $values) {
    $id_list[] = $uid;
  }

  $filename = sprintf('%s/signatures-uids.%s.%s.txt', $filepath, $petition_id, $name);
  if (file_exists($filename)) {
    unlink($filename);
  }
  file_put_contents($filename, implode(PHP_EOL, array_unique($id_list)));

  return $filename;
}

/**
 * Return an array of a batch of uids and remove them from the queue file.
 *
 * @param string $uids_file
 *
 * @param string $batch_size
 *
 * @return array $uids_batch
 */
function _wh_petitions_get_uids_batch($uids_file, $batch_size) {
  $uids = explode(PHP_EOL, file_get_contents($uids_file));
  // Alternatively, confirm that the file is empty.
  if (!empty($uids[0])) {
    $count = count($uids);
    if ($count < $batch_size) {
      $batch_size = $count;
    }
    // Slice a batch of uids off $uids to be processed.
    $uids_batch = array_splice($uids, 0, $batch_size);
    // Write remaining uids to $uids_file for the next batch.
    $remainder = count($uids);
    file_put_contents($uids_file, implode(PHP_EOL, $uids));
    return $uids_batch;
  }
}

/**
 * Get list of uids and ips of for a petition's signatures.
 * Write results to file.
 *
 * @param string $petition_id
 */
function wh_petitions_get_signatures_uids_with_ips($petition_id) {
  $filepath = drush_get_option('filepath');
  $name = drush_get_option('name');
  if (!$filepath || !$name) {
    drush_print('Please use --name and --filepath options');
    return;
  }

  $uids = _wh_petitions_get_signature_uids($petition_id);

  $id_list = "uid, username, ip_address, timestamp\n";
  foreach ($uids as $uid => $values) {
    $id_list .= sprintf("%s, %s, %s, %s\n",
      $values['uid'],
      $values['username'],
      $values['ip_address'],
      $values['timestamp']);
  }

  $file_name = sprintf('%s/signatures-uids-with-ips.%s.%s.txt', $filepath, $petition_id, $name);
  drush_print('Writing results to file: ' . $file_name);
  file_put_contents($file_name, $id_list);

}

function _wh_petitions_csv_sanitize($data) {
  $data = check_plain($data);
  $data = str_replace(',', '&#44;', $data);
  return $data;
}
