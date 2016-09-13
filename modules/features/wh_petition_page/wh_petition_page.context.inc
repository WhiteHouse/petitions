<?php
/**
 * @file
 * wh_petition_page.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function wh_petition_page_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'anchor_links';
  $context->description = 'Adds the anchor links menu to pages';
  $context->tag = '';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'wh_petitions_page' => 'wh_petitions_page',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-anchor_links-block_1' => array(
          'module' => 'views',
          'delta' => 'anchor_links-block_1',
          'region' => 'sidebar_top',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Adds the anchor links menu to pages');
  $export['anchor_links'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'full_width_page';
  $context->description = 'Sets a "full-width" class on the body';
  $context->tag = '';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '<front>' => '<front>',
        'petitions' => 'petitions',
        'responses' => 'responses',
        404 => '404',
      ),
    ),
  );
  $context->reactions = array(
    'theme_html' => array(
      'class' => 'full-width',
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Sets a "full-width" class on the body');
  $export['full_width_page'] = $context;

  return $export;
}
