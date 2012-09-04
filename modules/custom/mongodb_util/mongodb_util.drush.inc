<?php
/**
 * Implements hook_drush_help().
 */
function mongodb_util_drush_help($section) {
  switch ($section) {
    case 'drush:ensure-indexes':
      return dt("usage: drush ensure-indexes [master|<server-name>]");
    case 'drush:check-indexes':
      return dt("usage: drush check-indexes [master|<server-name>] [json-output:0]");
  }
}

/**
 * Implements hook_drush_command().
 */
function mongodb_util_drush_command() {
  $items = array();
  $items['ensure-indexes'] = array(
    'callback' => 'mongodb_util_drush_ensure_indexes',
    'description' => dt('Ensure mongodb indexes, this can take a long time, use check-indexes first.'),
    'options' => array(
      '--background' => "Optional. Run ensureIndexes with background:true is default, use background=0 to run in foreground",
    ),
  );
  $items['check-indexes'] = array(
    'callback' => 'mongodb_util_drush_ensure_indexes',
    'callback arguments' => array(TRUE),
    'description' => dt('Check mongodb indexes, but do not actually update them.'),
  );
  return $items;
}

/**
 * Callback function to ensure all mongodb indexes.
 *
 * @param $check
 *   When set to TRUE, only check indexes, don't change them.
 * @param $server
 *   The server on which the indexes are being check or created, defaults
 *   to 'master'.
 * @param $json_output
 *   If TRUE, output will be full json, which can be used in the
 *  mongo shell.
 */
function mongodb_util_drush_ensure_indexes($check = FALSE, $server = '', $json_output = FALSE) {
  $background = drush_get_option('background', TRUE);
  if (strtolower($background) == 'false') {
    $background = FALSE;
  }
  mongodb_util_ensure_all_indexes($check ? TRUE : FALSE, $server, TRUE, $background);
}
