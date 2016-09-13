<?php
/**
 * @file
 * petitionadmin.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function petitionadmin_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'petitionadmin';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Petition Administration';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Petitions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'petition access administrator';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'views_bulk_operations' => 'views_bulk_operations',
    'nid' => 'nid',
    'field_legacy_id' => 'field_legacy_id',
    'field_response_status' => 'field_response_status',
    'field_petition_review_timeframe' => 'field_petition_review_timeframe',
    'field_timestamp_published' => 'field_timestamp_published',
    'field_timestamp_reached_public' => 'field_timestamp_reached_public',
    'field_timestamp_reached_ready' => 'field_timestamp_reached_ready',
    'field_timestamp_responded' => 'field_timestamp_responded',
    'field_short_url' => 'field_short_url',
    'title' => 'title',
    'nothing_1' => 'nothing_1',
    'field_petition_public_signatures' => 'field_petition_public_signatures',
    'field_petition_signature_count' => 'field_petition_signature_count',
    'promote' => 'promote',
    'field_petition_status' => 'field_petition_status',
    'field_petition_issues' => 'field_petition_issues',
    'status' => 'status',
    'created' => 'created',
    'changed_1' => 'changed_1',
    'edit_node' => 'edit_node',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'views_bulk_operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_legacy_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_response_status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_petition_review_timeframe' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_timestamp_published' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_timestamp_reached_public' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_timestamp_reached_ready' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_timestamp_responded' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_short_url' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_petition_public_signatures' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_petition_signature_count' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'promote' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_petition_status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_petition_issues' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  /* Header: Global: Unfiltered text */
  $handler->display->display_options['header']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['header']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['header']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['header']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['header']['area_text_custom']['content'] = '<ul class="action-links">
<li>
<a href="/petition/create">Create new petition</a>
</li>
</ul>
<hr />
<br />
<h4><strong>Filters</strong></h4><br />';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'It would appear that no petitions exist or you need to apply the filter options.<br />
Please submit the filters to get your first result set.';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::petitionevents_hide_petition_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::petitionevents_show_petition_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  $handler->display->display_options['fields']['edit_node']['exclude'] = TRUE;
  $handler->display->display_options['fields']['edit_node']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['edit_node']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['edit_node']['element_label_colon'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Legacy ID */
  $handler->display->display_options['fields']['field_legacy_id']['id'] = 'field_legacy_id';
  $handler->display->display_options['fields']['field_legacy_id']['table'] = 'field_data_field_legacy_id';
  $handler->display->display_options['fields']['field_legacy_id']['field'] = 'field_legacy_id';
  $handler->display->display_options['fields']['field_legacy_id']['exclude'] = TRUE;
  /* Field: Content: Response Status */
  $handler->display->display_options['fields']['field_response_status']['id'] = 'field_response_status';
  $handler->display->display_options['fields']['field_response_status']['table'] = 'field_data_field_response_status';
  $handler->display->display_options['fields']['field_response_status']['field'] = 'field_response_status';
  $handler->display->display_options['fields']['field_response_status']['exclude'] = TRUE;
  /* Field: Content: Review timeframe */
  $handler->display->display_options['fields']['field_petition_review_timeframe']['id'] = 'field_petition_review_timeframe';
  $handler->display->display_options['fields']['field_petition_review_timeframe']['table'] = 'field_data_field_petition_review_timeframe';
  $handler->display->display_options['fields']['field_petition_review_timeframe']['field'] = 'field_petition_review_timeframe';
  $handler->display->display_options['fields']['field_petition_review_timeframe']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_petition_review_timeframe']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_petition_review_timeframe']['alter']['text'] = '[field_petition_review_timeframe] days';
  $handler->display->display_options['fields']['field_petition_review_timeframe']['type'] = 'number_unformatted';
  $handler->display->display_options['fields']['field_petition_review_timeframe']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Timestamp Published */
  $handler->display->display_options['fields']['field_timestamp_published']['id'] = 'field_timestamp_published';
  $handler->display->display_options['fields']['field_timestamp_published']['table'] = 'field_data_field_timestamp_published';
  $handler->display->display_options['fields']['field_timestamp_published']['field'] = 'field_timestamp_published';
  $handler->display->display_options['fields']['field_timestamp_published']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_timestamp_published']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Content: Timestamp Reached Public */
  $handler->display->display_options['fields']['field_timestamp_reached_public']['id'] = 'field_timestamp_reached_public';
  $handler->display->display_options['fields']['field_timestamp_reached_public']['table'] = 'field_data_field_timestamp_reached_public';
  $handler->display->display_options['fields']['field_timestamp_reached_public']['field'] = 'field_timestamp_reached_public';
  $handler->display->display_options['fields']['field_timestamp_reached_public']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_timestamp_reached_public']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Content: Timestamp Reached Ready */
  $handler->display->display_options['fields']['field_timestamp_reached_ready']['id'] = 'field_timestamp_reached_ready';
  $handler->display->display_options['fields']['field_timestamp_reached_ready']['table'] = 'field_data_field_timestamp_reached_ready';
  $handler->display->display_options['fields']['field_timestamp_reached_ready']['field'] = 'field_timestamp_reached_ready';
  $handler->display->display_options['fields']['field_timestamp_reached_ready']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_timestamp_reached_ready']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Content: Timestamp Responded */
  $handler->display->display_options['fields']['field_timestamp_responded']['id'] = 'field_timestamp_responded';
  $handler->display->display_options['fields']['field_timestamp_responded']['table'] = 'field_data_field_timestamp_responded';
  $handler->display->display_options['fields']['field_timestamp_responded']['field'] = 'field_timestamp_responded';
  $handler->display->display_options['fields']['field_timestamp_responded']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_timestamp_responded']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Content: Short URL */
  $handler->display->display_options['fields']['field_short_url']['id'] = 'field_short_url';
  $handler->display->display_options['fields']['field_short_url']['table'] = 'field_data_field_short_url';
  $handler->display->display_options['fields']['field_short_url']['field'] = 'field_short_url';
  $handler->display->display_options['fields']['field_short_url']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['text'] = '<strong>[title]</strong>';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Petition';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '[title]<br />
<strong>Nid:</strong> [nid]<br />
<strong>Legacy Id:</strong> [field_legacy_id]<br />
<strong>Published:</strong> [field_timestamp_published]<br />
<strong>Reached Ready:</strong> [field_timestamp_reached_ready]<br />
<strong>Reached Public:</strong> [field_timestamp_reached_public]<br />
<strong>Responded:</strong> [field_timestamp_responded] <br />
<strong>Review Timeframe:</strong> [field_petition_review_timeframe]<br />
<strong>Response Status:</strong> [field_response_status]<br />
<strong>Short URL:</strong> [field_short_url]<br />';
  /* Field: Content: Public signatures */
  $handler->display->display_options['fields']['field_petition_public_signatures']['id'] = 'field_petition_public_signatures';
  $handler->display->display_options['fields']['field_petition_public_signatures']['table'] = 'field_data_field_petition_public_signatures';
  $handler->display->display_options['fields']['field_petition_public_signatures']['field'] = 'field_petition_public_signatures';
  $handler->display->display_options['fields']['field_petition_public_signatures']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Signature count */
  $handler->display->display_options['fields']['field_petition_signature_count']['id'] = 'field_petition_signature_count';
  $handler->display->display_options['fields']['field_petition_signature_count']['table'] = 'field_data_field_petition_signature_count';
  $handler->display->display_options['fields']['field_petition_signature_count']['field'] = 'field_petition_signature_count';
  $handler->display->display_options['fields']['field_petition_signature_count']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Promoted to front page */
  $handler->display->display_options['fields']['promote']['id'] = 'promote';
  $handler->display->display_options['fields']['promote']['table'] = 'node';
  $handler->display->display_options['fields']['promote']['field'] = 'promote';
  $handler->display->display_options['fields']['promote']['not'] = 0;
  /* Field: Content: Petition Status */
  $handler->display->display_options['fields']['field_petition_status']['id'] = 'field_petition_status';
  $handler->display->display_options['fields']['field_petition_status']['table'] = 'field_data_field_petition_status';
  $handler->display->display_options['fields']['field_petition_status']['field'] = 'field_petition_status';
  /* Field: Content: Issues */
  $handler->display->display_options['fields']['field_petition_issues']['id'] = 'field_petition_issues';
  $handler->display->display_options['fields']['field_petition_issues']['table'] = 'field_data_field_petition_issues';
  $handler->display->display_options['fields']['field_petition_issues']['field'] = 'field_petition_issues';
  $handler->display->display_options['fields']['field_petition_issues']['delta_offset'] = '0';
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed_1']['id'] = 'changed_1';
  $handler->display->display_options['fields']['changed_1']['table'] = 'node';
  $handler->display->display_options['fields']['changed_1']['field'] = 'changed';
  $handler->display->display_options['fields']['changed_1']['date_format'] = 'short';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Options';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<strong>Petition:</strong><br />
[edit_node]<br />
<br />
<strong>Signatures:</strong><br />
<a href="/node/[nid]/signatures">View</a><br />
<i>Download</i><br />
<a href="/node/[nid]/signatures/download/base">CSV</a> | <a href="/node/[nid]/signatures/download/email">Email</a>';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'petition' => 'petition',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Legacy ID (field_legacy_id) */
  $handler->display->display_options['filters']['field_legacy_id_value']['id'] = 'field_legacy_id_value';
  $handler->display->display_options['filters']['field_legacy_id_value']['table'] = 'field_data_field_legacy_id';
  $handler->display->display_options['filters']['field_legacy_id_value']['field'] = 'field_legacy_id_value';
  $handler->display->display_options['filters']['field_legacy_id_value']['group'] = 1;
  $handler->display->display_options['filters']['field_legacy_id_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_legacy_id_value']['expose']['operator_id'] = 'field_legacy_id_value_op';
  $handler->display->display_options['filters']['field_legacy_id_value']['expose']['label'] = 'Legacy ID';
  $handler->display->display_options['filters']['field_legacy_id_value']['expose']['operator'] = 'field_legacy_id_value_op';
  $handler->display->display_options['filters']['field_legacy_id_value']['expose']['identifier'] = 'field_legacy_id_value';
  $handler->display->display_options['filters']['field_legacy_id_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Nid */
  $handler->display->display_options['filters']['nid']['id'] = 'nid';
  $handler->display->display_options['filters']['nid']['table'] = 'node';
  $handler->display->display_options['filters']['nid']['field'] = 'nid';
  $handler->display->display_options['filters']['nid']['group'] = 1;
  $handler->display->display_options['filters']['nid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['nid']['expose']['operator_id'] = 'nid_op';
  $handler->display->display_options['filters']['nid']['expose']['label'] = 'Nid';
  $handler->display->display_options['filters']['nid']['expose']['operator'] = 'nid_op';
  $handler->display->display_options['filters']['nid']['expose']['identifier'] = 'nid';
  $handler->display->display_options['filters']['nid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Petition Status (field_petition_status) */
  $handler->display->display_options['filters']['field_petition_status_value']['id'] = 'field_petition_status_value';
  $handler->display->display_options['filters']['field_petition_status_value']['table'] = 'field_data_field_petition_status';
  $handler->display->display_options['filters']['field_petition_status_value']['field'] = 'field_petition_status_value';
  $handler->display->display_options['filters']['field_petition_status_value']['group'] = 1;
  $handler->display->display_options['filters']['field_petition_status_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_petition_status_value']['expose']['operator_id'] = 'field_petition_status_value_op';
  $handler->display->display_options['filters']['field_petition_status_value']['expose']['label'] = 'Petition Status';
  $handler->display->display_options['filters']['field_petition_status_value']['expose']['operator'] = 'field_petition_status_value_op';
  $handler->display->display_options['filters']['field_petition_status_value']['expose']['identifier'] = 'field_petition_status_value';
  $handler->display->display_options['filters']['field_petition_status_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Response Status (field_response_status) */
  $handler->display->display_options['filters']['field_response_status_value']['id'] = 'field_response_status_value';
  $handler->display->display_options['filters']['field_response_status_value']['table'] = 'field_data_field_response_status';
  $handler->display->display_options['filters']['field_response_status_value']['field'] = 'field_response_status_value';
  $handler->display->display_options['filters']['field_response_status_value']['group'] = 1;
  $handler->display->display_options['filters']['field_response_status_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_response_status_value']['expose']['operator_id'] = 'field_response_status_value_op';
  $handler->display->display_options['filters']['field_response_status_value']['expose']['label'] = 'Response Status';
  $handler->display->display_options['filters']['field_response_status_value']['expose']['operator'] = 'field_response_status_value_op';
  $handler->display->display_options['filters']['field_response_status_value']['expose']['identifier'] = 'field_response_status_value';
  $handler->display->display_options['filters']['field_response_status_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Issues (field_petition_issues) */
  $handler->display->display_options['filters']['field_petition_issues_tid']['id'] = 'field_petition_issues_tid';
  $handler->display->display_options['filters']['field_petition_issues_tid']['table'] = 'field_data_field_petition_issues';
  $handler->display->display_options['filters']['field_petition_issues_tid']['field'] = 'field_petition_issues_tid';
  $handler->display->display_options['filters']['field_petition_issues_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_petition_issues_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_petition_issues_tid']['expose']['operator_id'] = 'field_petition_issues_tid_op';
  $handler->display->display_options['filters']['field_petition_issues_tid']['expose']['label'] = 'Issues';
  $handler->display->display_options['filters']['field_petition_issues_tid']['expose']['operator'] = 'field_petition_issues_tid_op';
  $handler->display->display_options['filters']['field_petition_issues_tid']['expose']['identifier'] = 'field_petition_issues_tid';
  $handler->display->display_options['filters']['field_petition_issues_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_petition_issues_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_petition_issues_tid']['vocabulary'] = 'issues';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 'All';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Published';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Promoted to front page */
  $handler->display->display_options['filters']['promote']['id'] = 'promote';
  $handler->display->display_options['filters']['promote']['table'] = 'node';
  $handler->display->display_options['filters']['promote']['field'] = 'promote';
  $handler->display->display_options['filters']['promote']['group'] = 1;
  $handler->display->display_options['filters']['promote']['exposed'] = TRUE;
  $handler->display->display_options['filters']['promote']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['promote']['expose']['label'] = 'Featured';
  $handler->display->display_options['filters']['promote']['expose']['operator'] = 'promote_op';
  $handler->display->display_options['filters']['promote']['expose']['identifier'] = 'promote';
  $handler->display->display_options['filters']['promote']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Timestamp Published (field_timestamp_published) */
  $handler->display->display_options['filters']['field_timestamp_published_value']['id'] = 'field_timestamp_published_value';
  $handler->display->display_options['filters']['field_timestamp_published_value']['table'] = 'field_data_field_timestamp_published';
  $handler->display->display_options['filters']['field_timestamp_published_value']['field'] = 'field_timestamp_published_value';
  $handler->display->display_options['filters']['field_timestamp_published_value']['operator'] = '<=';
  $handler->display->display_options['filters']['field_timestamp_published_value']['group'] = 1;
  $handler->display->display_options['filters']['field_timestamp_published_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_timestamp_published_value']['expose']['operator_id'] = 'field_timestamp_published_value_op';
  $handler->display->display_options['filters']['field_timestamp_published_value']['expose']['label'] = 'Published Date';
  $handler->display->display_options['filters']['field_timestamp_published_value']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_timestamp_published_value']['expose']['operator'] = 'field_timestamp_published_value_op';
  $handler->display->display_options['filters']['field_timestamp_published_value']['expose']['identifier'] = 'field_timestamp_published_value';
  $handler->display->display_options['filters']['field_timestamp_published_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_timestamp_published_value']['group_info']['label'] = 'Timestamp Published (field_timestamp_published)';
  $handler->display->display_options['filters']['field_timestamp_published_value']['group_info']['identifier'] = 'field_timestamp_published_value';
  $handler->display->display_options['filters']['field_timestamp_published_value']['group_info']['remember'] = FALSE;
  $handler->display->display_options['filters']['field_timestamp_published_value']['group_info']['group_items'] = array(
    1 => array(),
    2 => array(),
    3 => array(),
  );
  $handler->display->display_options['filters']['field_timestamp_published_value']['granularity'] = 'minute';
  $handler->display->display_options['filters']['field_timestamp_published_value']['default_date'] = 'now';
  $handler->display->display_options['filters']['field_timestamp_published_value']['year_range'] = '-10:+10';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/petitions';
  $handler->display->display_options['menu']['title'] = 'Petitions';
  $handler->display->display_options['menu']['description'] = 'Petitions management index';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['title'] = 'Petitions';
  $handler->display->display_options['tab_options']['description'] = 'Petitions Management';
  $handler->display->display_options['tab_options']['weight'] = '1';
  $handler->display->display_options['tab_options']['name'] = 'petitions-menu';
  $export['petitionadmin'] = $view;

  $view = new view();
  $view->name = 'petitionadmin_signatures';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'signature_mail';
  $view->human_name = 'Petition Administration - Signatures';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Petitions - Signatures';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'id' => 'id',
    'legacy_id' => 'legacy_id',
    'petition_id' => 'petition_id',
    'legacy_petition_id' => 'legacy_petition_id',
    'ip_address' => 'ip_address',
    'timestamp' => 'timestamp',
    'uid' => 'uid',
    'user_username' => 'user_username',
    'user_first_name' => 'user_first_name',
    'user_last_name' => 'user_last_name',
    'user_city' => 'user_city',
    'user_state' => 'user_state',
    'user_zip' => 'user_zip',
    'user_country' => 'user_country',
    'user_agent' => 'user_agent',
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
    'petition_id' => array(
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
    'ip_address' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp' => array(
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
    'user_username' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'user_first_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'user_last_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'user_city' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'user_state' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'user_zip' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'user_country' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'user_agent' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<h4><strong>Filters</strong></h4><br />';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  /* Relationship: Signature Mail: Uid */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'signature_mail';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Relationship: Signature Mail: Petition_id */
  $handler->display->display_options['relationships']['petition_id']['id'] = 'petition_id';
  $handler->display->display_options['relationships']['petition_id']['table'] = 'signature_mail';
  $handler->display->display_options['relationships']['petition_id']['field'] = 'petition_id';
  $handler->display->display_options['relationships']['petition_id']['required'] = TRUE;
  /* Relationship: Signature Mail: Signature Mail Fraud Data */
  $handler->display->display_options['relationships']['signature_mail_fraud_data']['id'] = 'signature_mail_fraud_data';
  $handler->display->display_options['relationships']['signature_mail_fraud_data']['table'] = 'signature_mail';
  $handler->display->display_options['relationships']['signature_mail_fraud_data']['field'] = 'signature_mail_fraud_data';
  /* Field: Bulk operations: Signature Mail */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['row_clickable'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '1000';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::petitionadmin_remove_signature' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Signature Mail: Signature mail ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['id']['separator'] = '';
  /* Field: Signature Mail: Petition_id */
  $handler->display->display_options['fields']['petition_id']['id'] = 'petition_id';
  $handler->display->display_options['fields']['petition_id']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['petition_id']['field'] = 'petition_id';
  $handler->display->display_options['fields']['petition_id']['label'] = 'Petition ID';
  $handler->display->display_options['fields']['petition_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['petition_id']['separator'] = '';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Unique Identifiers';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<strong>ID:</strong> [id]<br />
<strong>Petition ID:</strong> [petition_id]<br />';
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['relationship'] = 'uid';
  /* Field: User: Data */
  $handler->display->display_options['fields']['data']['id'] = 'data';
  $handler->display->display_options['fields']['data']['table'] = 'users';
  $handler->display->display_options['fields']['data']['field'] = 'data';
  $handler->display->display_options['fields']['data']['relationship'] = 'uid';
  $handler->display->display_options['fields']['data']['exclude'] = TRUE;
  $handler->display->display_options['fields']['data']['format'] = 'serialized';
  /* Field: Signature Mail Fraud Data: Email */
  $handler->display->display_options['fields']['email']['id'] = 'email';
  $handler->display->display_options['fields']['email']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['fields']['email']['field'] = 'email';
  $handler->display->display_options['fields']['email']['relationship'] = 'signature_mail_fraud_data';
  /* Field: Signature Mail: User_first_name */
  $handler->display->display_options['fields']['user_first_name']['id'] = 'user_first_name';
  $handler->display->display_options['fields']['user_first_name']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['user_first_name']['field'] = 'user_first_name';
  $handler->display->display_options['fields']['user_first_name']['label'] = 'First Name';
  /* Field: Signature Mail: User_last_name */
  $handler->display->display_options['fields']['user_last_name']['id'] = 'user_last_name';
  $handler->display->display_options['fields']['user_last_name']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['user_last_name']['field'] = 'user_last_name';
  $handler->display->display_options['fields']['user_last_name']['label'] = 'Last Name';
  /* Field: Signature Mail: User_city */
  $handler->display->display_options['fields']['user_city']['id'] = 'user_city';
  $handler->display->display_options['fields']['user_city']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['user_city']['field'] = 'user_city';
  $handler->display->display_options['fields']['user_city']['label'] = 'City';
  /* Field: Signature Mail: User_state */
  $handler->display->display_options['fields']['user_state']['id'] = 'user_state';
  $handler->display->display_options['fields']['user_state']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['user_state']['field'] = 'user_state';
  $handler->display->display_options['fields']['user_state']['label'] = 'State';
  /* Field: Signature Mail: User_zip */
  $handler->display->display_options['fields']['user_zip']['id'] = 'user_zip';
  $handler->display->display_options['fields']['user_zip']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['user_zip']['field'] = 'user_zip';
  $handler->display->display_options['fields']['user_zip']['label'] = 'Zip Code';
  /* Field: Signature Mail: User_country */
  $handler->display->display_options['fields']['user_country']['id'] = 'user_country';
  $handler->display->display_options['fields']['user_country']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['user_country']['field'] = 'user_country';
  $handler->display->display_options['fields']['user_country']['label'] = 'Country';
  /* Field: Signature Mail: Timestamp */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
  /* Field: Signature Mail: User_agent */
  $handler->display->display_options['fields']['user_agent']['id'] = 'user_agent';
  $handler->display->display_options['fields']['user_agent']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['user_agent']['field'] = 'user_agent';
  $handler->display->display_options['fields']['user_agent']['label'] = 'User Agent';
  /* Field: Signature Mail Fraud Data: Ip_address */
  $handler->display->display_options['fields']['ip_address_1']['id'] = 'ip_address_1';
  $handler->display->display_options['fields']['ip_address_1']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['fields']['ip_address_1']['field'] = 'ip_address';
  $handler->display->display_options['fields']['ip_address_1']['relationship'] = 'signature_mail_fraud_data';
  $handler->display->display_options['fields']['ip_address_1']['label'] = 'IP Address';
  /* Contextual filter: Signature Mail: Petition_id */
  $handler->display->display_options['arguments']['petition_id']['id'] = 'petition_id';
  $handler->display->display_options['arguments']['petition_id']['table'] = 'signature_mail';
  $handler->display->display_options['arguments']['petition_id']['field'] = 'petition_id';
  $handler->display->display_options['arguments']['petition_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['petition_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['petition_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['petition_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['petition_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['petition_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['petition_id']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['petition_id']['validate_options']['types'] = array(
    'petition' => 'petition',
  );
  /* Filter criterion: Signature Mail: Signature mail ID */
  $handler->display->display_options['filters']['id']['id'] = 'id';
  $handler->display->display_options['filters']['id']['table'] = 'signature_mail';
  $handler->display->display_options['filters']['id']['field'] = 'id';
  $handler->display->display_options['filters']['id']['group'] = 1;
  $handler->display->display_options['filters']['id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['id']['expose']['operator_id'] = 'id_op';
  $handler->display->display_options['filters']['id']['expose']['label'] = 'Signature mail ID';
  $handler->display->display_options['filters']['id']['expose']['operator'] = 'id_op';
  $handler->display->display_options['filters']['id']['expose']['identifier'] = 'id';
  $handler->display->display_options['filters']['id']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    3 => 0,
  );
  /* Filter criterion: Signature Mail: Uid */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'signature_mail';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['group'] = 1;
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'User ID';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    3 => 0,
  );
  /* Filter criterion: Signature Mail: User_username */
  $handler->display->display_options['filters']['user_username']['id'] = 'user_username';
  $handler->display->display_options['filters']['user_username']['table'] = 'signature_mail';
  $handler->display->display_options['filters']['user_username']['field'] = 'user_username';
  $handler->display->display_options['filters']['user_username']['operator'] = 'contains';
  $handler->display->display_options['filters']['user_username']['group'] = 1;
  $handler->display->display_options['filters']['user_username']['exposed'] = TRUE;
  $handler->display->display_options['filters']['user_username']['expose']['operator_id'] = 'user_username_op';
  $handler->display->display_options['filters']['user_username']['expose']['label'] = 'Username';
  $handler->display->display_options['filters']['user_username']['expose']['operator'] = 'user_username_op';
  $handler->display->display_options['filters']['user_username']['expose']['identifier'] = 'user_username';
  $handler->display->display_options['filters']['user_username']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    3 => 0,
  );
  /* Filter criterion: Signature Mail: User_first_name */
  $handler->display->display_options['filters']['user_first_name']['id'] = 'user_first_name';
  $handler->display->display_options['filters']['user_first_name']['table'] = 'signature_mail';
  $handler->display->display_options['filters']['user_first_name']['field'] = 'user_first_name';
  $handler->display->display_options['filters']['user_first_name']['operator'] = 'contains';
  $handler->display->display_options['filters']['user_first_name']['group'] = 1;
  $handler->display->display_options['filters']['user_first_name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['user_first_name']['expose']['operator_id'] = 'user_first_name_op';
  $handler->display->display_options['filters']['user_first_name']['expose']['label'] = 'First Name';
  $handler->display->display_options['filters']['user_first_name']['expose']['operator'] = 'user_first_name_op';
  $handler->display->display_options['filters']['user_first_name']['expose']['identifier'] = 'user_first_name';
  $handler->display->display_options['filters']['user_first_name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    3 => 0,
  );
  /* Filter criterion: Signature Mail: User_last_name */
  $handler->display->display_options['filters']['user_last_name']['id'] = 'user_last_name';
  $handler->display->display_options['filters']['user_last_name']['table'] = 'signature_mail';
  $handler->display->display_options['filters']['user_last_name']['field'] = 'user_last_name';
  $handler->display->display_options['filters']['user_last_name']['operator'] = 'contains';
  $handler->display->display_options['filters']['user_last_name']['group'] = 1;
  $handler->display->display_options['filters']['user_last_name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['user_last_name']['expose']['operator_id'] = 'user_last_name_op';
  $handler->display->display_options['filters']['user_last_name']['expose']['label'] = 'Last Name';
  $handler->display->display_options['filters']['user_last_name']['expose']['operator'] = 'user_last_name_op';
  $handler->display->display_options['filters']['user_last_name']['expose']['identifier'] = 'user_last_name';
  $handler->display->display_options['filters']['user_last_name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    3 => 0,
  );
  /* Filter criterion: Signature Mail: User_city */
  $handler->display->display_options['filters']['user_city']['id'] = 'user_city';
  $handler->display->display_options['filters']['user_city']['table'] = 'signature_mail';
  $handler->display->display_options['filters']['user_city']['field'] = 'user_city';
  $handler->display->display_options['filters']['user_city']['operator'] = 'contains';
  $handler->display->display_options['filters']['user_city']['group'] = 1;
  $handler->display->display_options['filters']['user_city']['exposed'] = TRUE;
  $handler->display->display_options['filters']['user_city']['expose']['operator_id'] = 'user_city_op';
  $handler->display->display_options['filters']['user_city']['expose']['label'] = 'City';
  $handler->display->display_options['filters']['user_city']['expose']['operator'] = 'user_city_op';
  $handler->display->display_options['filters']['user_city']['expose']['identifier'] = 'user_city';
  $handler->display->display_options['filters']['user_city']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    3 => 0,
  );
  /* Filter criterion: Signature Mail: User_state */
  $handler->display->display_options['filters']['user_state']['id'] = 'user_state';
  $handler->display->display_options['filters']['user_state']['table'] = 'signature_mail';
  $handler->display->display_options['filters']['user_state']['field'] = 'user_state';
  $handler->display->display_options['filters']['user_state']['operator'] = 'contains';
  $handler->display->display_options['filters']['user_state']['group'] = 1;
  $handler->display->display_options['filters']['user_state']['exposed'] = TRUE;
  $handler->display->display_options['filters']['user_state']['expose']['operator_id'] = 'user_state_op';
  $handler->display->display_options['filters']['user_state']['expose']['label'] = 'State';
  $handler->display->display_options['filters']['user_state']['expose']['operator'] = 'user_state_op';
  $handler->display->display_options['filters']['user_state']['expose']['identifier'] = 'user_state';
  $handler->display->display_options['filters']['user_state']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    3 => 0,
  );
  /* Filter criterion: Signature Mail: User_zip */
  $handler->display->display_options['filters']['user_zip']['id'] = 'user_zip';
  $handler->display->display_options['filters']['user_zip']['table'] = 'signature_mail';
  $handler->display->display_options['filters']['user_zip']['field'] = 'user_zip';
  $handler->display->display_options['filters']['user_zip']['operator'] = 'contains';
  $handler->display->display_options['filters']['user_zip']['group'] = 1;
  $handler->display->display_options['filters']['user_zip']['exposed'] = TRUE;
  $handler->display->display_options['filters']['user_zip']['expose']['operator_id'] = 'user_zip_op';
  $handler->display->display_options['filters']['user_zip']['expose']['label'] = 'Zip Code';
  $handler->display->display_options['filters']['user_zip']['expose']['operator'] = 'user_zip_op';
  $handler->display->display_options['filters']['user_zip']['expose']['identifier'] = 'user_zip';
  $handler->display->display_options['filters']['user_zip']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    3 => 0,
  );
  /* Filter criterion: Signature Mail Fraud Data: Ip_address */
  $handler->display->display_options['filters']['ip_address']['id'] = 'ip_address';
  $handler->display->display_options['filters']['ip_address']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['filters']['ip_address']['field'] = 'ip_address';
  $handler->display->display_options['filters']['ip_address']['relationship'] = 'signature_mail_fraud_data';
  $handler->display->display_options['filters']['ip_address']['group'] = 1;
  $handler->display->display_options['filters']['ip_address']['exposed'] = TRUE;
  $handler->display->display_options['filters']['ip_address']['expose']['operator_id'] = 'ip_address_op';
  $handler->display->display_options['filters']['ip_address']['expose']['label'] = 'IP Address (v4 or v6)';
  $handler->display->display_options['filters']['ip_address']['expose']['operator'] = 'ip_address_op';
  $handler->display->display_options['filters']['ip_address']['expose']['identifier'] = 'ip_address';
  $handler->display->display_options['filters']['ip_address']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    3 => 0,
  );
  /* Filter criterion: Signature Mail Fraud Data: Unsub_email */
  $handler->display->display_options['filters']['unsub_email']['id'] = 'unsub_email';
  $handler->display->display_options['filters']['unsub_email']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['filters']['unsub_email']['field'] = 'unsub_email';
  $handler->display->display_options['filters']['unsub_email']['relationship'] = 'signature_mail_fraud_data';
  $handler->display->display_options['filters']['unsub_email']['group'] = 1;
  $handler->display->display_options['filters']['unsub_email']['exposed'] = TRUE;
  $handler->display->display_options['filters']['unsub_email']['expose']['operator_id'] = 'unsub_email_op';
  $handler->display->display_options['filters']['unsub_email']['expose']['label'] = 'Unsubaddressed Email';
  $handler->display->display_options['filters']['unsub_email']['expose']['description'] = 'Email address without user+tag@gmail.com ie. user@gmail.com';
  $handler->display->display_options['filters']['unsub_email']['expose']['operator'] = 'unsub_email_op';
  $handler->display->display_options['filters']['unsub_email']['expose']['identifier'] = 'unsub_email';
  $handler->display->display_options['filters']['unsub_email']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    3 => 0,
  );
  /* Filter criterion: Signature Mail Fraud Data: Email_domain */
  $handler->display->display_options['filters']['email_domain']['id'] = 'email_domain';
  $handler->display->display_options['filters']['email_domain']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['filters']['email_domain']['field'] = 'email_domain';
  $handler->display->display_options['filters']['email_domain']['relationship'] = 'signature_mail_fraud_data';
  $handler->display->display_options['filters']['email_domain']['group'] = 1;
  $handler->display->display_options['filters']['email_domain']['exposed'] = TRUE;
  $handler->display->display_options['filters']['email_domain']['expose']['operator_id'] = 'email_domain_op';
  $handler->display->display_options['filters']['email_domain']['expose']['label'] = 'Email domain';
  $handler->display->display_options['filters']['email_domain']['expose']['operator'] = 'email_domain_op';
  $handler->display->display_options['filters']['email_domain']['expose']['identifier'] = 'email_domain';
  $handler->display->display_options['filters']['email_domain']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['email_domain']['group_info']['label'] = 'Email_domain';
  $handler->display->display_options['filters']['email_domain']['group_info']['identifier'] = 'email_domain';
  $handler->display->display_options['filters']['email_domain']['group_info']['remember'] = FALSE;
  $handler->display->display_options['filters']['email_domain']['group_info']['group_items'] = array(
    1 => array(),
    2 => array(),
    3 => array(),
  );
  /* Filter criterion: Signature Mail Fraud Data: Is_forwarding_email */
  $handler->display->display_options['filters']['is_forwarding_email']['id'] = 'is_forwarding_email';
  $handler->display->display_options['filters']['is_forwarding_email']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['filters']['is_forwarding_email']['field'] = 'is_forwarding_email';
  $handler->display->display_options['filters']['is_forwarding_email']['relationship'] = 'signature_mail_fraud_data';
  $handler->display->display_options['filters']['is_forwarding_email']['group'] = 1;
  $handler->display->display_options['filters']['is_forwarding_email']['exposed'] = TRUE;
  $handler->display->display_options['filters']['is_forwarding_email']['expose']['operator_id'] = 'is_forwarding_email_op';
  $handler->display->display_options['filters']['is_forwarding_email']['expose']['label'] = 'Is_forwarding_email';
  $handler->display->display_options['filters']['is_forwarding_email']['expose']['operator'] = 'is_forwarding_email_op';
  $handler->display->display_options['filters']['is_forwarding_email']['expose']['identifier'] = 'is_forwarding_email';
  $handler->display->display_options['filters']['is_forwarding_email']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['is_forwarding_email']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['is_forwarding_email']['group_info']['label'] = 'Is Forwarding Email';
  $handler->display->display_options['filters']['is_forwarding_email']['group_info']['identifier'] = 'is_forwarding_email';
  $handler->display->display_options['filters']['is_forwarding_email']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Yes',
      'operator' => '=',
      'value' => array(
        'value' => '1',
        'min' => '',
        'max' => '',
      ),
    ),
    2 => array(
      'title' => '',
      'operator' => '=',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
    ),
    3 => array(
      'title' => '',
      'operator' => '=',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
    ),
  );
  /* Filter criterion: Signature Mail Fraud Data: Is_free_email */
  $handler->display->display_options['filters']['is_free_email']['id'] = 'is_free_email';
  $handler->display->display_options['filters']['is_free_email']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['filters']['is_free_email']['field'] = 'is_free_email';
  $handler->display->display_options['filters']['is_free_email']['relationship'] = 'signature_mail_fraud_data';
  $handler->display->display_options['filters']['is_free_email']['group'] = 1;
  $handler->display->display_options['filters']['is_free_email']['exposed'] = TRUE;
  $handler->display->display_options['filters']['is_free_email']['expose']['operator_id'] = 'is_free_email_op';
  $handler->display->display_options['filters']['is_free_email']['expose']['label'] = 'Is_free_email';
  $handler->display->display_options['filters']['is_free_email']['expose']['operator'] = 'is_free_email_op';
  $handler->display->display_options['filters']['is_free_email']['expose']['identifier'] = 'is_free_email';
  $handler->display->display_options['filters']['is_free_email']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['is_free_email']['group_info']['label'] = 'Is Free Email';
  $handler->display->display_options['filters']['is_free_email']['group_info']['identifier'] = 'is_free_email';
  $handler->display->display_options['filters']['is_free_email']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Yes',
      'operator' => '=',
      'value' => array(
        'value' => '1',
        'min' => '',
        'max' => '',
      ),
    ),
    2 => array(
      'title' => '',
      'operator' => '=',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
    ),
    3 => array(
      'title' => '',
      'operator' => '=',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
    ),
  );
  /* Filter criterion: Signature Mail Fraud Data: Is_open_email */
  $handler->display->display_options['filters']['is_open_email']['id'] = 'is_open_email';
  $handler->display->display_options['filters']['is_open_email']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['filters']['is_open_email']['field'] = 'is_open_email';
  $handler->display->display_options['filters']['is_open_email']['relationship'] = 'signature_mail_fraud_data';
  $handler->display->display_options['filters']['is_open_email']['group'] = 1;
  $handler->display->display_options['filters']['is_open_email']['exposed'] = TRUE;
  $handler->display->display_options['filters']['is_open_email']['expose']['operator_id'] = 'is_open_email_op';
  $handler->display->display_options['filters']['is_open_email']['expose']['label'] = 'Is_open_email';
  $handler->display->display_options['filters']['is_open_email']['expose']['operator'] = 'is_open_email_op';
  $handler->display->display_options['filters']['is_open_email']['expose']['identifier'] = 'is_open_email';
  $handler->display->display_options['filters']['is_open_email']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['is_open_email']['group_info']['label'] = 'Is Open Email';
  $handler->display->display_options['filters']['is_open_email']['group_info']['identifier'] = 'is_open_email';
  $handler->display->display_options['filters']['is_open_email']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Yes',
      'operator' => '=',
      'value' => array(
        'value' => '1',
        'min' => '',
        'max' => '',
      ),
    ),
    2 => array(
      'title' => '',
      'operator' => '=',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
    ),
    3 => array(
      'title' => '',
      'operator' => '=',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
    ),
  );
  /* Filter criterion: Signature Mail Fraud Data: Is_shredder_email */
  $handler->display->display_options['filters']['is_shredder_email']['id'] = 'is_shredder_email';
  $handler->display->display_options['filters']['is_shredder_email']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['filters']['is_shredder_email']['field'] = 'is_shredder_email';
  $handler->display->display_options['filters']['is_shredder_email']['relationship'] = 'signature_mail_fraud_data';
  $handler->display->display_options['filters']['is_shredder_email']['group'] = 1;
  $handler->display->display_options['filters']['is_shredder_email']['exposed'] = TRUE;
  $handler->display->display_options['filters']['is_shredder_email']['expose']['operator_id'] = 'is_shredder_email_op';
  $handler->display->display_options['filters']['is_shredder_email']['expose']['label'] = 'Is_shredder_email';
  $handler->display->display_options['filters']['is_shredder_email']['expose']['operator'] = 'is_shredder_email_op';
  $handler->display->display_options['filters']['is_shredder_email']['expose']['identifier'] = 'is_shredder_email';
  $handler->display->display_options['filters']['is_shredder_email']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['is_shredder_email']['group_info']['label'] = 'Is Shredder Email';
  $handler->display->display_options['filters']['is_shredder_email']['group_info']['identifier'] = 'is_shredder_email';
  $handler->display->display_options['filters']['is_shredder_email']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Yes',
      'operator' => '=',
      'value' => array(
        'value' => '1',
        'min' => '',
        'max' => '',
      ),
    ),
    2 => array(
      'title' => '',
      'operator' => '=',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
    ),
    3 => array(
      'title' => '',
      'operator' => '=',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
    ),
  );
  /* Filter criterion: Signature Mail Fraud Data: Is_time_bound_email */
  $handler->display->display_options['filters']['is_time_bound_email']['id'] = 'is_time_bound_email';
  $handler->display->display_options['filters']['is_time_bound_email']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['filters']['is_time_bound_email']['field'] = 'is_time_bound_email';
  $handler->display->display_options['filters']['is_time_bound_email']['relationship'] = 'signature_mail_fraud_data';
  $handler->display->display_options['filters']['is_time_bound_email']['group'] = 1;
  $handler->display->display_options['filters']['is_time_bound_email']['exposed'] = TRUE;
  $handler->display->display_options['filters']['is_time_bound_email']['expose']['operator_id'] = 'is_time_bound_email_op';
  $handler->display->display_options['filters']['is_time_bound_email']['expose']['label'] = 'Is_time_bound_email';
  $handler->display->display_options['filters']['is_time_bound_email']['expose']['operator'] = 'is_time_bound_email_op';
  $handler->display->display_options['filters']['is_time_bound_email']['expose']['identifier'] = 'is_time_bound_email';
  $handler->display->display_options['filters']['is_time_bound_email']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['is_time_bound_email']['group_info']['label'] = 'Is Time Bound Email';
  $handler->display->display_options['filters']['is_time_bound_email']['group_info']['identifier'] = 'is_time_bound_email';
  $handler->display->display_options['filters']['is_time_bound_email']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Yes',
      'operator' => '=',
      'value' => array(
        'value' => '1',
        'min' => '',
        'max' => '',
      ),
    ),
    2 => array(
      'title' => '',
      'operator' => '=',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
    ),
    3 => array(
      'title' => '',
      'operator' => '=',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
    ),
  );
  /* Filter criterion: Signature Mail Fraud Data: Is_trash_email */
  $handler->display->display_options['filters']['is_trash_email']['id'] = 'is_trash_email';
  $handler->display->display_options['filters']['is_trash_email']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['filters']['is_trash_email']['field'] = 'is_trash_email';
  $handler->display->display_options['filters']['is_trash_email']['relationship'] = 'signature_mail_fraud_data';
  $handler->display->display_options['filters']['is_trash_email']['group'] = 1;
  $handler->display->display_options['filters']['is_trash_email']['exposed'] = TRUE;
  $handler->display->display_options['filters']['is_trash_email']['expose']['operator_id'] = 'is_trash_email_op';
  $handler->display->display_options['filters']['is_trash_email']['expose']['label'] = 'Is_trash_email';
  $handler->display->display_options['filters']['is_trash_email']['expose']['operator'] = 'is_trash_email_op';
  $handler->display->display_options['filters']['is_trash_email']['expose']['identifier'] = 'is_trash_email';
  $handler->display->display_options['filters']['is_trash_email']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['is_trash_email']['group_info']['label'] = 'Is Trash Email';
  $handler->display->display_options['filters']['is_trash_email']['group_info']['identifier'] = 'is_trash_email';
  $handler->display->display_options['filters']['is_trash_email']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Yes',
      'operator' => '=',
      'value' => array(
        'value' => '1',
        'min' => '',
        'max' => '',
      ),
    ),
    2 => array(
      'title' => '',
      'operator' => '=',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
    ),
    3 => array(
      'title' => '',
      'operator' => '=',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
    ),
  );

  /* Display: Petition Signatures Admin */
  $handler = $view->new_display('block', 'Petition Signatures Admin', 'block_1');

  $export['petitionadmin_signatures'] = $view;

  return $export;
}
