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
<a href="/node/add/petition">Create new petition</a>
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
    'action::node_assign_owner_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 0,
      'postpone_processing' => 1,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_make_sticky_action' => array(
      'selected' => 0,
      'postpone_processing' => 1,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_make_unsticky_action' => array(
      'selected' => 0,
      'postpone_processing' => 1,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          '_all_' => '_all_',
        ),
      ),
    ),
    'action::views_bulk_operations_argument_selector_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'url' => '',
      ),
    ),
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
    'action::node_promote_action' => array(
      'selected' => 0,
      'postpone_processing' => 1,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_publish_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpromote_action' => array(
      'selected' => 0,
      'postpone_processing' => 1,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_save_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpublish_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpublish_by_keyword_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::pathauto_node_update_action' => array(
      'selected' => 0,
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
  $handler->display->display_options['path'] = 'admin/content/petitions';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Petitions';
  $handler->display->display_options['menu']['description'] = 'Petitions management index';
  $handler->display->display_options['menu']['weight'] = '5';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
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
  $handler->display->display_options['header']['area']['content'] = '<strong>Download All Signatures</strong><br />
CSV | Email<br />
<hr />
<br />
<h4><strong>Filters</strong></h4><br />';
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
  /* Field: Signature Mail: Signature mail ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['id']['separator'] = '';
  /* Field: Signature Mail: Legacy_id */
  $handler->display->display_options['fields']['legacy_id']['id'] = 'legacy_id';
  $handler->display->display_options['fields']['legacy_id']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['legacy_id']['field'] = 'legacy_id';
  $handler->display->display_options['fields']['legacy_id']['label'] = 'Legacy ID';
  $handler->display->display_options['fields']['legacy_id']['exclude'] = TRUE;
  /* Field: Signature Mail: Petition_id */
  $handler->display->display_options['fields']['petition_id']['id'] = 'petition_id';
  $handler->display->display_options['fields']['petition_id']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['petition_id']['field'] = 'petition_id';
  $handler->display->display_options['fields']['petition_id']['label'] = 'Petition ID';
  $handler->display->display_options['fields']['petition_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['petition_id']['separator'] = '';
  /* Field: Signature Mail: Legacy_petition_id */
  $handler->display->display_options['fields']['legacy_petition_id']['id'] = 'legacy_petition_id';
  $handler->display->display_options['fields']['legacy_petition_id']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['legacy_petition_id']['field'] = 'legacy_petition_id';
  $handler->display->display_options['fields']['legacy_petition_id']['label'] = 'Legacy Petition ID';
  $handler->display->display_options['fields']['legacy_petition_id']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Unique Identifiers';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<strong>ID:</strong> [id]<br />
<strong>Legacy ID:</strong> [legacy_id]<br />
<strong>Petition ID:</strong> [petition_id]<br />
<strong>Legacy Petition ID:</strong> [legacy_petition_id]<br />';
  /* Field: Signature Mail: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['label'] = 'UID';
  $handler->display->display_options['fields']['uid']['separator'] = '';
  /* Field: User: Data */
  $handler->display->display_options['fields']['data']['id'] = 'data';
  $handler->display->display_options['fields']['data']['table'] = 'users';
  $handler->display->display_options['fields']['data']['field'] = 'data';
  $handler->display->display_options['fields']['data']['relationship'] = 'uid';
  $handler->display->display_options['fields']['data']['exclude'] = TRUE;
  $handler->display->display_options['fields']['data']['format'] = 'serialized';
  /* Field: User: Link */
  $handler->display->display_options['fields']['view_user']['id'] = 'view_user';
  $handler->display->display_options['fields']['view_user']['table'] = 'users';
  $handler->display->display_options['fields']['view_user']['field'] = 'view_user';
  $handler->display->display_options['fields']['view_user']['relationship'] = 'uid';
  $handler->display->display_options['fields']['view_user']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['view_user']['text'] = 'User';
  /* Field: Signature Mail: User_username */
  $handler->display->display_options['fields']['user_username']['id'] = 'user_username';
  $handler->display->display_options['fields']['user_username']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['user_username']['field'] = 'user_username';
  $handler->display->display_options['fields']['user_username']['label'] = 'Username';
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
  /* Field: Signature Mail: Ip_address */
  $handler->display->display_options['fields']['ip_address']['id'] = 'ip_address';
  $handler->display->display_options['fields']['ip_address']['table'] = 'signature_mail';
  $handler->display->display_options['fields']['ip_address']['field'] = 'ip_address';
  $handler->display->display_options['fields']['ip_address']['label'] = 'IP Address';
  /* Contextual filter: Signature Mail: Petition_id */
  $handler->display->display_options['arguments']['petition_id']['id'] = 'petition_id';
  $handler->display->display_options['arguments']['petition_id']['table'] = 'signature_mail';
  $handler->display->display_options['arguments']['petition_id']['field'] = 'petition_id';
  $handler->display->display_options['arguments']['petition_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['petition_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['petition_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['petition_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['petition_id']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Signature Mail: Legacy_id */
  $handler->display->display_options['filters']['legacy_id']['id'] = 'legacy_id';
  $handler->display->display_options['filters']['legacy_id']['table'] = 'signature_mail';
  $handler->display->display_options['filters']['legacy_id']['field'] = 'legacy_id';
  $handler->display->display_options['filters']['legacy_id']['group'] = 1;
  $handler->display->display_options['filters']['legacy_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['legacy_id']['expose']['operator_id'] = 'legacy_id_op';
  $handler->display->display_options['filters']['legacy_id']['expose']['label'] = 'Legacy ID';
  $handler->display->display_options['filters']['legacy_id']['expose']['operator'] = 'legacy_id_op';
  $handler->display->display_options['filters']['legacy_id']['expose']['identifier'] = 'legacy_id';
  $handler->display->display_options['filters']['legacy_id']['expose']['remember_roles'] = array(
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
  /* Filter criterion: Signature Mail: Legacy_petition_id */
  $handler->display->display_options['filters']['legacy_petition_id']['id'] = 'legacy_petition_id';
  $handler->display->display_options['filters']['legacy_petition_id']['table'] = 'signature_mail';
  $handler->display->display_options['filters']['legacy_petition_id']['field'] = 'legacy_petition_id';
  $handler->display->display_options['filters']['legacy_petition_id']['group'] = 1;
  $handler->display->display_options['filters']['legacy_petition_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['legacy_petition_id']['expose']['operator_id'] = 'legacy_petition_id_op';
  $handler->display->display_options['filters']['legacy_petition_id']['expose']['label'] = 'Legacy Petition ID';
  $handler->display->display_options['filters']['legacy_petition_id']['expose']['operator'] = 'legacy_petition_id_op';
  $handler->display->display_options['filters']['legacy_petition_id']['expose']['identifier'] = 'legacy_petition_id';
  $handler->display->display_options['filters']['legacy_petition_id']['expose']['remember_roles'] = array(
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

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<strong>Download All Signatures</strong><br />
<a href="/node/[petition_id]/signatures/export/base/download">CSV</a> | <a href="/node/[petition_id]/signatures/export/email/download">Email</a><br />
<hr />
<br />
<h4><strong>Filters</strong></h4><br />';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  $handler->display->display_options['path'] = 'node/%/signature';
  $handler->display->display_options['menu']['title'] = 'Signatures';
  $handler->display->display_options['menu']['description'] = 'Manage signatures for the Petition.';
  $handler->display->display_options['menu']['weight'] = '15';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['petitionadmin_signatures'] = $view;

  $view = new view();
  $view->name = 'petitions';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Petitions Moderation';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Petitions Moderation';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer petition tool hub settings';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Content revision: User */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node_revision';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['label'] = 'Last Petition Editor';
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['relationships']['uid_1']['table'] = 'node';
  $handler->display->display_options['relationships']['uid_1']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid_1']['label'] = 'Author Relationship';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::node_assign_owner_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_make_sticky_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_make_unsticky_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          '_all_' => '_all_',
        ),
      ),
    ),
    'action::views_bulk_operations_argument_selector_action' => array(
      'selected' => 0,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Unpublish',
      'settings' => array(
        'url' => 'unpublish-petition',
      ),
    ),
    'action::node_promote_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_publish_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpromote_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
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
    'action::node_save_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpublish_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpublish_by_keyword_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::pathauto_node_update_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Content: Signature count */
  $handler->display->display_options['fields']['field_petition_signature_count']['id'] = 'field_petition_signature_count';
  $handler->display->display_options['fields']['field_petition_signature_count']['table'] = 'field_data_field_petition_signature_count';
  $handler->display->display_options['fields']['field_petition_signature_count']['field'] = 'field_petition_signature_count';
  $handler->display->display_options['fields']['field_petition_signature_count']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Petition Status */
  $handler->display->display_options['fields']['field_petition_status']['id'] = 'field_petition_status';
  $handler->display->display_options['fields']['field_petition_status']['table'] = 'field_data_field_petition_status';
  $handler->display->display_options['fields']['field_petition_status']['field'] = 'field_petition_status';
  /* Field: Content: Abuse Flags */
  $handler->display->display_options['fields']['field_abuse_flags']['id'] = 'field_abuse_flags';
  $handler->display->display_options['fields']['field_abuse_flags']['table'] = 'field_data_field_abuse_flags';
  $handler->display->display_options['fields']['field_abuse_flags']['field'] = 'field_abuse_flags';
  $handler->display->display_options['fields']['field_abuse_flags']['settings'] = array(
    'link' => 0,
  );
  $handler->display->display_options['fields']['field_abuse_flags']['delta_offset'] = '0';
  /* Field: Content revision: Updated date */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'node_revision';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'long';
  /* Field: Content: Abuse Count */
  $handler->display->display_options['fields']['field_abuse_count']['id'] = 'field_abuse_count';
  $handler->display->display_options['fields']['field_abuse_count']['table'] = 'field_data_field_abuse_count';
  $handler->display->display_options['fields']['field_abuse_count']['field'] = 'field_abuse_count';
  $handler->display->display_options['fields']['field_abuse_count']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1,
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href = "/unpublish_petition/[nid]&destination=/admin/structure">Unpublish</a>';
  /* Field: User: Rendered User */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_user';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['relationship'] = 'uid';
  $handler->display->display_options['fields']['rendered_entity']['label'] = 'Last Edited By';
  $handler->display->display_options['fields']['rendered_entity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'full';
  $handler->display->display_options['fields']['rendered_entity']['bypass_access'] = 0;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid_1';
  $handler->display->display_options['fields']['name']['label'] = 'Author';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
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
  /* Filter criterion: Content: Short URL (field_short_url) */
  $handler->display->display_options['filters']['field_short_url_value']['id'] = 'field_short_url_value';
  $handler->display->display_options['filters']['field_short_url_value']['table'] = 'field_data_field_short_url';
  $handler->display->display_options['filters']['field_short_url_value']['field'] = 'field_short_url_value';
  $handler->display->display_options['filters']['field_short_url_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_short_url_value']['expose']['operator_id'] = 'field_short_url_value_op';
  $handler->display->display_options['filters']['field_short_url_value']['expose']['label'] = 'Short URL';
  $handler->display->display_options['filters']['field_short_url_value']['expose']['operator'] = 'field_short_url_value_op';
  $handler->display->display_options['filters']['field_short_url_value']['expose']['identifier'] = 'field_short_url_value';
  $handler->display->display_options['filters']['field_short_url_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Nid */
  $handler->display->display_options['filters']['nid']['id'] = 'nid';
  $handler->display->display_options['filters']['nid']['table'] = 'node';
  $handler->display->display_options['filters']['nid']['field'] = 'nid';
  $handler->display->display_options['filters']['nid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['nid']['expose']['operator_id'] = 'nid_op';
  $handler->display->display_options['filters']['nid']['expose']['label'] = 'Petition ID';
  $handler->display->display_options['filters']['nid']['expose']['operator'] = 'nid_op';
  $handler->display->display_options['filters']['nid']['expose']['identifier'] = 'nid';
  $handler->display->display_options['filters']['nid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'moderation_tools');
  $handler->display->display_options['path'] = 'admin/moderation-tools';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Petitions Moderation';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['petitions'] = $view;

  return $export;
}
