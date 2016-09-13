<?php
/**
 * @file
 * wh_petition_page.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function wh_petition_page_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'anchor_links';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Anchor Links';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['style_options']['class'] = 'menu';
  $handler->display->display_options['style_options']['wrapper_class'] = '';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  /* Field: Content: Anchor Links */
  $handler->display->display_options['fields']['field_anchor_links']['id'] = 'field_anchor_links';
  $handler->display->display_options['fields']['field_anchor_links']['table'] = 'field_data_field_anchor_links';
  $handler->display->display_options['fields']['field_anchor_links']['field'] = 'field_anchor_links';
  $handler->display->display_options['fields']['field_anchor_links']['label'] = '';
  $handler->display->display_options['fields']['field_anchor_links']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_anchor_links']['alter']['text'] = '<a href="[field_anchor_links-url]">[field_anchor_links-title]</a>';
  $handler->display->display_options['fields']['field_anchor_links']['alter']['path'] = '#[field_anchor_links-url]';
  $handler->display->display_options['fields']['field_anchor_links']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_anchor_links']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_anchor_links']['group_rows'] = FALSE;
  $handler->display->display_options['fields']['field_anchor_links']['delta_offset'] = '0';
  /* Sort criterion: Content: Anchor Links (field_anchor_links:delta) */
  $handler->display->display_options['sorts']['delta']['id'] = 'delta';
  $handler->display->display_options['sorts']['delta']['table'] = 'field_data_field_anchor_links';
  $handler->display->display_options['sorts']['delta']['field'] = 'delta';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Anchor Links (field_anchor_links:delta) */
  $handler->display->display_options['filters']['delta']['id'] = 'delta';
  $handler->display->display_options['filters']['delta']['table'] = 'field_data_field_anchor_links';
  $handler->display->display_options['filters']['delta']['field'] = 'delta';
  $handler->display->display_options['filters']['delta']['operator'] = 'not empty';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $export['anchor_links'] = $view;

  return $export;
}
