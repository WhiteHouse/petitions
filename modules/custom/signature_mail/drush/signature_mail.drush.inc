<?php

function signature_mail_drush_command() {
  $items = array();

  $items['signatures-export'] = array(
    'description' => "Export signatures for a petition in CSV format to stdout.",
    'arguments' => array(
      'node_id' => 'Petition Node ID',
      'export_fields' => 'base - for all fields, email - only email addresses',
    ),
    'examples' => array(
      'drush signatures-export 143 base > petition_143_signatures.csv' => 'Export base fields of Petition Node 143',
    ),
    'required-arguments' => TRUE,
    'aliases' => array('sigexp'),
  );
  return $items;
}

/**
 * Export petition signatures to stdout.
 *
 * @see petitionadmin_export_signatures()
 */
function drush_signature_mail_signatures_export($node_id, $export_fields) {
  petitionadmin_export_signatures($node_id, $export_fields, 'stdout');
}
