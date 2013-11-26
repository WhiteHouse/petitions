<?php
/**
 * @file
 * Contains drush hook implementations for Petitions Data module.
 */

/**
 * Implements hook_drush_cache_clear().
 */
function petitions_data_drush_cache_clear(&$types) {
  $types['petitions'] = 'petitions_data_clear_caches';
}
