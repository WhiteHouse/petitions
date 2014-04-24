<?php
/**
 * @file
 * wh_contexts.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function wh_contexts_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'create_petition';
  $context->description = 'Create A Petition page';
  $context->tag = 'misc';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'petition/create*' => 'petition/create*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'wh_petitions-wh_petitions_recent_petitions' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_recent_petitions',
          'region' => 'right_rail',
          'weight' => '-10',
        ),
        'wh_response-wh_response_recent_responses' => array(
          'module' => 'wh_response',
          'delta' => 'wh_response_recent_responses',
          'region' => 'right_rail',
          'weight' => '-9',
        ),
        'wh_petitions-wh_petitions_helpful_hints' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_helpful_hints',
          'region' => 'right_rail',
          'weight' => '-8',
        ),
        'wh_petitions-wh_petitions_history' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_history',
          'region' => 'right_rail',
          'weight' => '-7',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Create A Petition page');
  t('misc');
  $export['create_petition'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'dashboard';
  $context->description = 'User Dashboard';
  $context->tag = 'list';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'dashboard' => 'dashboard',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'wh_petitions-wh_petitions_recent_petitions' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_recent_petitions',
          'region' => 'right_rail',
          'weight' => '-10',
        ),
        'wh_response-wh_response_recent_responses' => array(
          'module' => 'wh_response',
          'delta' => 'wh_response_recent_responses',
          'region' => 'right_rail',
          'weight' => '-9',
        ),
        'wh_petitions-wh_petitions_history' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_history',
          'region' => 'right_rail',
          'weight' => '-8',
        ),
        'wh_petitions-wh_petitions_helpful_hints' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_helpful_hints',
          'region' => 'right_rail',
          'weight' => '-7',
        ),
        'wh_petitions-wh_petitions_create_petition' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_create_petition',
          'region' => 'right_rail',
          'weight' => '-6',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('User Dashboard');
  t('list');
  $export['dashboard'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'petition';
  $context->description = 'Petition Detail page';
  $context->tag = 'detail';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'petition/detail/*' => 'petition/detail/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'petitionssignatureform-petitionssignatureform_form' => array(
          'module' => 'petitionssignatureform',
          'delta' => 'petitionssignatureform_form',
          'region' => 'right_rail',
          'weight' => '-10',
        ),
        'wh_petitions-wh_petitions_similar_petitions' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_similar_petitions',
          'region' => 'right_rail',
          'weight' => '-9',
        ),
        'wh_petitions-wh_petitions_helpful_hints' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_helpful_hints',
          'region' => 'right_rail',
          'weight' => '-8',
        ),
        'wh_petitions-wh_petitions_recent_petitions' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_recent_petitions',
          'region' => 'right_rail',
          'weight' => '-7',
        ),
        'wh_response-wh_response_recent_responses' => array(
          'module' => 'wh_response',
          'delta' => 'wh_response_recent_responses',
          'region' => 'right_rail',
          'weight' => '-6',
        ),
        'wh_petitions-wh_petitions_history' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_history',
          'region' => 'right_rail',
          'weight' => '-5',
        ),
        'wh_petitions-wh_petitions_create_petition' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_create_petition',
          'region' => 'right_rail',
          'weight' => '-4',
        ),
        'wh_petitions-wh_petitions_back_to_top' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_back_to_top',
          'region' => 'right_rail',
          'weight' => '-3',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Petition Detail page');
  t('detail');
  $export['petition'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'petitions';
  $context->description = 'Open Petitions list page';
  $context->tag = 'list';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'petitions*' => 'petitions*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'wh_petitions-wh_petitions_recent_petitions' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_recent_petitions',
          'region' => 'right_rail',
          'weight' => '-10',
        ),
        'wh_response-wh_response_recent_responses' => array(
          'module' => 'wh_response',
          'delta' => 'wh_response_recent_responses',
          'region' => 'right_rail',
          'weight' => '-9',
        ),
        'wh_petitions-wh_petitions_history' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_history',
          'region' => 'right_rail',
          'weight' => '-8',
        ),
        'wh_petitions-wh_petitions_helpful_hints' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_helpful_hints',
          'region' => 'right_rail',
          'weight' => '-7',
        ),
        'wh_petitions-wh_petitions_create_petition' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_create_petition',
          'region' => 'right_rail',
          'weight' => '-6',
        ),
        'wh_petitions-wh_petitions_back_to_top' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_back_to_top',
          'region' => 'right_rail',
          'weight' => '-5',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Open Petitions list page');
  t('list');
  $export['petitions'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'response';
  $context->description = 'Response Detail page';
  $context->tag = 'detail';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'wh_response' => 'wh_response',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'path' => array(
      'values' => array(
        'response/email/*' => 'response/email/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'wh_petitions-wh_petitions_helpful_hints' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_helpful_hints',
          'region' => 'right_rail',
          'weight' => '-10',
        ),
        'wh_petitions-wh_petitions_recent_petitions' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_recent_petitions',
          'region' => 'right_rail',
          'weight' => '-9',
        ),
        'wh_response-wh_response_recent_responses' => array(
          'module' => 'wh_response',
          'delta' => 'wh_response_recent_responses',
          'region' => 'right_rail',
          'weight' => '-7',
        ),
        'wh_petitions-wh_petitions_create_petition' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_create_petition',
          'region' => 'right_rail',
          'weight' => '-6',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Response Detail page');
  t('detail');
  $export['response'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'responses';
  $context->description = 'Reponses list page';
  $context->tag = 'list';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'responses*' => 'responses*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'wh_petitions-wh_petitions_recent_petitions' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_recent_petitions',
          'region' => 'right_rail',
          'weight' => '-10',
        ),
        'wh_response-wh_response_recent_responses' => array(
          'module' => 'wh_response',
          'delta' => 'wh_response_recent_responses',
          'region' => 'right_rail',
          'weight' => '-9',
        ),
        'wh_petitions-wh_petitions_helpful_hints' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_helpful_hints',
          'region' => 'right_rail',
          'weight' => '-8',
        ),
        'wh_petitions-wh_petitions_create_petition' => array(
          'module' => 'wh_petitions',
          'delta' => 'wh_petitions_create_petition',
          'region' => 'right_rail',
          'weight' => '-7',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Reponses list page');
  t('list');
  $export['responses'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'thankyou_signing';
  $context->description = 'Thank you for signing page';
  $context->tag = 'misc';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'thank-you/*' => 'thank-you/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'wh_response-wh_response_recent_responses' => array(
          'module' => 'wh_response',
          'delta' => 'wh_response_recent_responses',
          'region' => 'right_rail',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Thank you for signing page');
  t('misc');
  $export['thankyou_signing'] = $context;

  return $export;
}
