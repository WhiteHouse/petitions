<?php

/**
 * @file
 * Mongo-to-MySQL-specific functionality for the Petitions Data module.
 *
 * @mongo2mysql
 */

/**
 * Implements hook_shunt().
 */
function petitions_data_shunt() {
  return array(
    'petitions_data_solr' => t('Disables Petitions Data Solr Petitions functionality, causing all petition queries to go to MySQL instead.'),
    'signatures_data_solr' => t('Disables Petitions Data Solr Signatures functionality, causing all signatures queries to go to MySQL instead.'),
    'petitions_solr_index' => t('Disables Solr indexing of petitions.'),
    'signatures_solr_index' => t('Disables Solr indexing of signatures.'),
  );
}

/**
 * Determines whether reads from MongoDB are enabled or not.
 *
 * @return bool
 *   Returns TRUE if reads from MongoDB are enabled or FALSE if not.
 */
function petitions_data_mongo_reads_are_enabled() {
  return FALSE;
}

/**
 * Determines whether writes to MongoDB are enabled or not.
 *
 * @return bool
 *   Returns TRUE if writes to MongoDB are enabled or FALSE if not.
 */
function petitions_data_mongo_writes_are_enabled() {
  return FALSE;
}

/**
 * Determines whether MongoDB access of any kind is enabled or not.
 *
 * @return bool
 *   Returns TRUE if MongoDB access is enabled or FALSE if not.
 */
function petitions_data_mongo_is_on() {
  return petitions_data_mongo_reads_are_enabled() || petitions_data_mongo_writes_are_enabled();
}

/**
 * Determines whether writes to MySQL are enabled or not.
 *
 * @return bool
 *   Returns TRUE if writes to MySQL are enabled or FALSE if not.
 */
function petitions_data_mysql_writes_are_enabled() {
  return TRUE;
}

/**
 * Determines whether signature queries to Solr are enabled or not.
 *
 * @return bool
 *   Returns TRUE if queries to Solr are enabled or FALSE if not.
 */
function petitions_data_solr_signature_queries_are_enabled() {
  return !shunt_is_enabled('signatures_data_solr');
}
