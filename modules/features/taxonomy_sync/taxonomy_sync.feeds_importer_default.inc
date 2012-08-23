<?php

/**
 * Implementation of hook_feeds_importer_default().
 */
function taxonomy_sync_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'issues_taxonomy';
  $feeds_importer->config = array(
    'name' => 'Issues Taxonomy',
    'description' => 'Syncs the Issues vocabulary with the Issue Tag vocabulary on the main site.',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsSyndicationParser',
      'config' => array(),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsTermProcessor',
      'config' => array(
        'vocabulary' => 'issues',
        'mappings' => array(
          0 => array(
            'source' => 'title',
            'target' => 'name',
            'unique' => 0,
          ),
          1 => array(
            'source' => 'description',
            'target' => 'description',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'tags',
            'target' => 'parent',
            'unique' => 0,
          ),
          3 => array(
            'source' => 'guid',
            'target' => 'guid',
            'unique' => 1,
          ),
          4 => array(
            'source' => 'author_name',
            'target' => 'weight',
            'unique' => 0,
          ),
        ),
        'update_existing' => '2',
        'input_format' => 'plain_text',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['issues_taxonomy'] = $feeds_importer;

  return $export;
}
