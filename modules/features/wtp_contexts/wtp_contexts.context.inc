<?php
/**
 * @file
 * wtp_contexts.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function wtp_contexts_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'api_key_request_form';
  $context->description = '';
  $context->tag = '';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'developers/apply' => 'developers/apply',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'petitions_api_keys_submission-api_keys_submit' => array(
          'module' => 'petitions_api_keys_submission',
          'delta' => 'api_keys_submit',
          'region' => 'content',
          'weight' => '10',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;
  $export['api_key_request_form'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'homepage';
  $context->description = 'Elements on homepage';
  $context->tag = 'layout';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '<front>' => '<front>',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'petitions_blocks-how_petitions_work_block' => array(
          'module' => 'petitions_blocks',
          'delta' => 'how_petitions_work_block',
          'region' => 'content_bottom',
          'weight' => '-10',
        ),
        'views-0760e70c35322fc3c653f411c63f2660' => array(
          'module' => 'views',
          'delta' => '0760e70c35322fc3c653f411c63f2660',
          'region' => 'content_bottom',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Elements on homepage');
  t('layout');
  $export['homepage'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'petition_signable';
  $context->description = 'Elements on a petition node page';
  $context->tag = 'layout';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'petition' => 'petition',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
    'petition_context_condition_signable' => array(
      'values' => array(
        'signable' => 'signable',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'petition-signature_count' => array(
          'module' => 'petition',
          'delta' => 'signature_count',
          'region' => 'sidebar_top',
          'weight' => '-10',
        ),
        'system-main' => array(
          'module' => 'system',
          'delta' => 'main',
          'region' => 'content',
          'weight' => '-10',
        ),
        'petitionssignatureform-petitionssignatureform_form' => array(
          'module' => 'petitionssignatureform',
          'delta' => 'petitionssignatureform_form',
          'region' => 'sidebar_bottom',
          'weight' => '-10',
        ),
        'petitions_blocks-how_petitions_work_block' => array(
          'module' => 'petitions_blocks',
          'delta' => 'how_petitions_work_block',
          'region' => 'content_bottom',
          'weight' => '-10',
        ),
        'views-751ef66fe65c3cf71c599d3c6c5c7cd4' => array(
          'module' => 'views',
          'delta' => '751ef66fe65c3cf71c599d3c6c5c7cd4',
          'region' => 'content_bottom',
          'weight' => '-9',
        ),
      ),
    ),
    'region' => array(
      'petitions_responsive' => array(
        'disable' => array(
          'preface' => 0,
          'header' => 0,
          'help' => 0,
          'content_top' => 0,
          'sidebar_top' => 0,
          'content' => 0,
          'sidebar_bottom' => 0,
          'content_bottom' => 0,
          'footer' => 0,
          'appendix' => 0,
        ),
      ),
      'seven' => array(
        'disable' => array(
          'content' => 0,
          'help' => 0,
          'page_top' => 0,
          'page_bottom' => 0,
          'sidebar_first' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Elements on a petition node page');
  t('layout');
  $export['petition_signable'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'petition_unsignable';
  $context->description = 'Elements on a petition node page';
  $context->tag = 'layout';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'petition' => 'petition',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'petition-signature_count' => array(
          'module' => 'petition',
          'delta' => 'signature_count',
          'region' => 'sidebar_top',
          'weight' => '-10',
        ),
        'system-main' => array(
          'module' => 'system',
          'delta' => 'main',
          'region' => 'content',
          'weight' => '-10',
        ),
        'petitionssignatureform-petitionssignatureform_form' => array(
          'module' => 'petitionssignatureform',
          'delta' => 'petitionssignatureform_form',
          'region' => 'sidebar_bottom',
          'weight' => '-10',
        ),
        'petitions_blocks-how_petitions_work_block' => array(
          'module' => 'petitions_blocks',
          'delta' => 'how_petitions_work_block',
          'region' => 'content_bottom',
          'weight' => '-10',
        ),
        'views-751ef66fe65c3cf71c599d3c6c5c7cd4' => array(
          'module' => 'views',
          'delta' => '751ef66fe65c3cf71c599d3c6c5c7cd4',
          'region' => 'content_bottom',
          'weight' => '-9',
        ),
      ),
    ),
    'region' => array(
      'petitions_responsive' => array(
        'disable' => array(
          'preface' => 0,
          'header' => 0,
          'help' => 0,
          'content_top' => 0,
          'sidebar_top' => 0,
          'content' => 0,
          'sidebar_bottom' => 0,
          'content_bottom' => 0,
          'footer' => 0,
          'appendix' => 0,
        ),
      ),
      'seven' => array(
        'disable' => array(
          'content' => 0,
          'help' => 0,
          'page_top' => 0,
          'page_bottom' => 0,
          'sidebar_first' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Elements on a petition node page');
  t('layout');
  $export['petition_unsignable'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'responses';
  $context->description = 'Responses list page';
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
        'petitions_blocks-how_petitions_work_block' => array(
          'module' => 'petitions_blocks',
          'delta' => 'how_petitions_work_block',
          'region' => 'content_bottom',
          'weight' => '-10',
        ),
        'views-76cfb65e9cbafcc874505ff1abba6928' => array(
          'module' => 'views',
          'delta' => '76cfb65e9cbafcc874505ff1abba6928',
          'region' => 'content_bottom',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Responses list page');
  t('list');
  $export['responses'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'dashboard_link_edit_account';
  $context->description = 'Set the breadcrumb to active for the "Edit Account" menu link';
  $context->tag = 'Menu';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'user/edit' => 'user/edit',
        'user/*/edit' => 'user/*/edit',
      ),
    ),
  );
  $context->reactions = array(
    'menu' => array(
      0 => 'menu-petitions-dashboard:user/edit',
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Menu');
  t('Set the breadcrumb to active for the "Edit Account" menu link');
  $export['dashboard_link_edit_account'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'dashboard_link_my_account';
  $context->description = 'Set the breadcrumb to active for the "My Account" menu link';
  $context->tag = 'Menu';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'user' => 'user',
        'user/*' => 'user/*',
        '~user/*/edit' => '~user/*/edit',
      ),
    ),
  );
  $context->reactions = array(
    'menu' => array(
      0 => 'menu-petitions-dashboard:user',
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Menu');
  t('Set the breadcrumb to active for the "My Account" menu link');
  $export['dashboard_link_my_account'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'sitewide_layout';
  $context->description = '';
  $context->tag = 'Sitewide';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '*' => '*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-footer-menu' => array(
          'module' => 'menu',
          'delta' => 'menu-footer-menu',
          'region' => 'footer',
          'weight' => '-10',
        ),
        'petitions_blocks-wh_footer_logo' => array(
          'module' => 'petitions_blocks',
          'delta' => 'wh_footer_logo',
          'region' => 'footer',
          'weight' => '-9',
        ),
        'menu-menu-corner-links' => array(
          'module' => 'menu',
          'delta' => 'menu-corner-links',
          'region' => 'footer',
          'weight' => '-8',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Sitewide');
  $export['sitewide_layout'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'terms_of_service_page';
  $context->description = 'Layout rules for the configured Terms of Service page';
  $context->tag = 'layout';
  $context->conditions = array(
    'petitions_tos_violation_context_condition_tos_page' => array(
      'values' => array(
        'tos_page' => 'tos_page',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'petitions_blocks-how_petitions_work_block' => array(
          'module' => 'petitions_blocks',
          'delta' => 'how_petitions_work_block',
          'region' => 'content_bottom',
          'weight' => '-10',
        ),
        'views-751ef66fe65c3cf71c599d3c6c5c7cd4' => array(
          'module' => 'views',
          'delta' => '751ef66fe65c3cf71c599d3c6c5c7cd4',
          'region' => 'content_bottom',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Layout rules for the configured Terms of Service page');
  t('layout');
  $export['terms_of_service_page'] = $context;

  return $export;
}
