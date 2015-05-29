<?php
/**
 * @file
 * petition_pages.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function petition_pages_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'petitions_listing';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Petitions Listing';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Petitions Listing';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Public signatures */
  $handler->display->display_options['fields']['field_petition_public_signatures']['id'] = 'field_petition_public_signatures';
  $handler->display->display_options['fields']['field_petition_public_signatures']['table'] = 'field_data_field_petition_public_signatures';
  $handler->display->display_options['fields']['field_petition_public_signatures']['field'] = 'field_petition_public_signatures';
  $handler->display->display_options['fields']['field_petition_public_signatures']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1,
  );
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '0';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'petition' => 'petition',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'petitions-list';
  $export['petitions_listing'] = $view;

  $view = new view();
  $view->name = 'signature_mail_listings';
  $view->description = 'A handful of views that display signatures in different ways';
  $view->tag = 'default';
  $view->base_table = 'signature_mail';
  $view->human_name = 'Signature Mail Listings';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Signature Mail List';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'infinite_scroll';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'id' => 'id',
    'legacy_id' => 'legacy_id',
    'legacy_petition_id' => 'legacy_petition_id',
    'petition_id' => 'petition_id',
    'uid' => 'uid',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'legacy_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'legacy_petition_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'petition_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'uid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Signature Mail: Signature mail ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  /* Field: Signature Mail: Legacy_id */
  $handler->display->display_options['fields']['legacy_id']['id'] = 'legacy_id';
  $handler->display->display_options['fields']['legacy_id']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['legacy_id']['field'] = 'legacy_id';
  /* Field: Signature Mail: Legacy_petition_id */
  $handler->display->display_options['fields']['legacy_petition_id']['id'] = 'legacy_petition_id';
  $handler->display->display_options['fields']['legacy_petition_id']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['legacy_petition_id']['field'] = 'legacy_petition_id';
  /* Field: Signature Mail: Petition_id */
  $handler->display->display_options['fields']['petition_id']['id'] = 'petition_id';
  $handler->display->display_options['fields']['petition_id']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['petition_id']['field'] = 'petition_id';
  /* Field: Signature Mail: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'signature-mail-list';

  /* Display: EVA Field */
  $handler = $view->new_display('entity_view', 'EVA Field', 'entity_view_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Signatures';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Signature Mail: Petition_id */
  $handler->display->display_options['relationships']['petition_id']['id'] = 'petition_id';
  $handler->display->display_options['relationships']['petition_id']['table'] = 'signature_mail';
  $handler->display->display_options['relationships']['petition_id']['field'] = 'petition_id';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Signature Mail: Signature mail ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  /* Field: Signature Mail: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  /* Field: Signature Mail: User_username */
  $handler->display->display_options['fields']['user_username']['id'] = 'user_username';
  $handler->display->display_options['fields']['user_username']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['user_username']['field'] = 'user_username';
  /* Field: Signature Mail: User_first_name */
  $handler->display->display_options['fields']['user_first_name']['id'] = 'user_first_name';
  $handler->display->display_options['fields']['user_first_name']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['user_first_name']['field'] = 'user_first_name';
  /* Field: Signature Mail: User_last_name */
  $handler->display->display_options['fields']['user_last_name']['id'] = 'user_last_name';
  $handler->display->display_options['fields']['user_last_name']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['user_last_name']['field'] = 'user_last_name';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'petition_id';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['entity_type'] = 'node';
  $handler->display->display_options['bundles'] = array(
    0 => 'petition',
  );
  $handler->display->display_options['show_title'] = 1;
  $export['signature_mail_listings'] = $view;

  return $export;
}
