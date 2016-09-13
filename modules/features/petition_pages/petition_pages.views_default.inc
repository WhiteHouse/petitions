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
  $handler->display->display_options['pager']['type'] = 'history_pager';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['tags']['previous'] = 'Load Previous';
  $handler->display->display_options['pager']['options']['tags']['next'] = 'Load More';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'entity';
  $handler->display->display_options['row_options']['view_mode'] = 'teaser';
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
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'petition' => 'petition',
  );

  /* Display: Open Petitions */
  $handler = $view->new_display('block', 'Open Petitions', 'open_petitions');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Sign a Petition';
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<p class="large">Add your name to these petitions and help them reach their goal.</p>
<div class="responses-link"><a class="responses-link" href="/responses#petitionswithupdates">View Petitions With Updates</a></div>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Signature count (field_petition_signature_count) */
  $handler->display->display_options['sorts']['field_petition_signature_count_value']['id'] = 'field_petition_signature_count_value';
  $handler->display->display_options['sorts']['field_petition_signature_count_value']['table'] = 'field_data_field_petition_signature_count';
  $handler->display->display_options['sorts']['field_petition_signature_count_value']['field'] = 'field_petition_signature_count_value';
  $handler->display->display_options['sorts']['field_petition_signature_count_value']['order'] = 'DESC';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'petition' => 'petition',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Filter criterion: Content: Petition Status (field_petition_status) */
  $handler->display->display_options['filters']['field_petition_status_value']['id'] = 'field_petition_status_value';
  $handler->display->display_options['filters']['field_petition_status_value']['table'] = 'field_data_field_petition_status';
  $handler->display->display_options['filters']['field_petition_status_value']['field'] = 'field_petition_status_value';
  $handler->display->display_options['filters']['field_petition_status_value']['value'] = array(
    2 => '2',
    4 => '4',
  );
  $handler->display->display_options['block_description'] = 'Open Petitions';

  /* Display: Petitions With Updates */
  $handler = $view->new_display('block', 'Petitions With Updates', 'petitions_with_updates');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Petitions With Updates';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<div class="openpetitions-link"><a class="openpetitions-link" href="/#signapetition">View Petitions You Can Sign</a></div>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_response_id_target_id']['id'] = 'field_response_id_target_id';
  $handler->display->display_options['relationships']['field_response_id_target_id']['table'] = 'field_data_field_response_id';
  $handler->display->display_options['relationships']['field_response_id_target_id']['field'] = 'field_response_id_target_id';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['relationship'] = 'field_response_id_target_id';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'petition' => 'petition',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Filter criterion: Content: Petition Status (field_petition_status) */
  $handler->display->display_options['filters']['field_petition_status_value']['id'] = 'field_petition_status_value';
  $handler->display->display_options['filters']['field_petition_status_value']['table'] = 'field_data_field_petition_status';
  $handler->display->display_options['filters']['field_petition_status_value']['field'] = 'field_petition_status_value';
  $handler->display->display_options['filters']['field_petition_status_value']['value'] = array(
    5 => '5',
  );
  $handler->display->display_options['block_description'] = 'Petitions With Updates';

  /* Display: Petitions Created By User */
  $handler = $view->new_display('page', 'Petitions Created By User', 'petitions_created_by_user');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'My Petitions';
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    2 => '2',
  );
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Timestamp Published (field_timestamp_published) */
  $handler->display->display_options['sorts']['field_timestamp_published_value']['id'] = 'field_timestamp_published_value';
  $handler->display->display_options['sorts']['field_timestamp_published_value']['table'] = 'field_data_field_timestamp_published';
  $handler->display->display_options['sorts']['field_timestamp_published_value']['field'] = 'field_timestamp_published_value';
  $handler->display->display_options['sorts']['field_timestamp_published_value']['order'] = 'DESC';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'petition' => 'petition',
  );
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid_current']['value'] = '1';
  $handler->display->display_options['path'] = 'dashboard';

  /* Display: Popular Petitions */
  $handler = $view->new_display('block', 'Popular Petitions', 'popular_petitions');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Popular Petitions';
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = TRUE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'View more popular petitions';
  $handler->display->display_options['defaults']['link_display'] = FALSE;
  $handler->display->display_options['link_display'] = 'custom_url';
  $handler->display->display_options['link_url'] = '/petitions';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '4';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Signature count (field_petition_signature_count) */
  $handler->display->display_options['sorts']['field_petition_signature_count_value']['id'] = 'field_petition_signature_count_value';
  $handler->display->display_options['sorts']['field_petition_signature_count_value']['table'] = 'field_data_field_petition_signature_count';
  $handler->display->display_options['sorts']['field_petition_signature_count_value']['field'] = 'field_petition_signature_count_value';
  $handler->display->display_options['sorts']['field_petition_signature_count_value']['order'] = 'DESC';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'petition' => 'petition',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Filter criterion: Content: Petition Status (field_petition_status) */
  $handler->display->display_options['filters']['field_petition_status_value']['id'] = 'field_petition_status_value';
  $handler->display->display_options['filters']['field_petition_status_value']['table'] = 'field_data_field_petition_status';
  $handler->display->display_options['filters']['field_petition_status_value']['field'] = 'field_petition_status_value';
  $handler->display->display_options['filters']['field_petition_status_value']['value'] = array(
    2 => '2',
    4 => '4',
  );
  $handler->display->display_options['block_description'] = 'Popular Petitions';
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
  $export['signature_mail_listings'] = $view;

  return $export;
}
