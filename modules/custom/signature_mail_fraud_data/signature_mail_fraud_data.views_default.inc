<?php
/**
 * @file
 * signature_mail_fraud_data.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function signature_mail_fraud_data_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'signature_mail_fraud_data';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'signature_mail_fraud_data';
  $view->human_name = 'Signature Mail Fraud Data';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Signature Mail Fraud Data';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'unsub_email',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No Results Found';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  /* Field: Signature Mail Fraud Data: Unsub_email */
  $handler->display->display_options['fields']['unsub_email']['id'] = 'unsub_email';
  $handler->display->display_options['fields']['unsub_email']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['fields']['unsub_email']['field'] = 'unsub_email';
  $handler->display->display_options['fields']['unsub_email']['group_type'] = 'count';
  /* Contextual filter: Signature Mail Fraud Data: Petition_id */
  $handler->display->display_options['arguments']['petition_id']['id'] = 'petition_id';
  $handler->display->display_options['arguments']['petition_id']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['arguments']['petition_id']['field'] = 'petition_id';
  $handler->display->display_options['arguments']['petition_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['petition_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['petition_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['petition_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['petition_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['petition_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['petition_id']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['petition_id']['validate_options']['types'] = array(
    'petition' => 'petition',
  );
  $handler->display->display_options['arguments']['petition_id']['validate']['fail'] = 'empty';

  /* Display: Subaddressed Signature Emails */
  $handler = $view->new_display('block', 'Subaddressed Signature Emails', 'subaddr_signature_emails_block');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Subaddressed Email Signatures';
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'unsub_email' => 'unsub_email',
    'unsub_email_1' => 'unsub_email_1',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'unsub_email' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'unsub_email_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<h2>Subaddressed Signature Emails</h2>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Signature Mail Fraud Data: Unsub_email */
  $handler->display->display_options['fields']['unsub_email']['id'] = 'unsub_email';
  $handler->display->display_options['fields']['unsub_email']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['fields']['unsub_email']['field'] = 'unsub_email';
  $handler->display->display_options['fields']['unsub_email']['label'] = 'Email';
  $handler->display->display_options['fields']['unsub_email']['element_label_colon'] = FALSE;
  /* Field: Signature Mail Fraud Data: Unsub_email */
  $handler->display->display_options['fields']['unsub_email_1']['id'] = 'unsub_email_1';
  $handler->display->display_options['fields']['unsub_email_1']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['fields']['unsub_email_1']['field'] = 'unsub_email';
  $handler->display->display_options['fields']['unsub_email_1']['group_type'] = 'count';
  $handler->display->display_options['fields']['unsub_email_1']['label'] = '# of subaddressed emails';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Signature Mail Fraud Data: Unsub_email */
  $handler->display->display_options['sorts']['unsub_email']['id'] = 'unsub_email';
  $handler->display->display_options['sorts']['unsub_email']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['sorts']['unsub_email']['field'] = 'unsub_email';
  $handler->display->display_options['sorts']['unsub_email']['group_type'] = 'count';
  $handler->display->display_options['sorts']['unsub_email']['order'] = 'DESC';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Signature Mail Fraud Data: Unsub_email */
  $handler->display->display_options['filters']['unsub_email']['id'] = 'unsub_email';
  $handler->display->display_options['filters']['unsub_email']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['filters']['unsub_email']['field'] = 'unsub_email';
  $handler->display->display_options['filters']['unsub_email']['group_type'] = 'count';
  $handler->display->display_options['filters']['unsub_email']['operator'] = '>';
  $handler->display->display_options['filters']['unsub_email']['value']['value'] = '1';
  $handler->display->display_options['block_description'] = 'Subaddressed Signature Emails';

  /* Display: Duplicate IP Address Signatures */
  $handler = $view->new_display('block', 'Duplicate IP Address Signatures', 'duplicate_ip_addr_sig_block');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Duplicate IP Address Signatures';
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'unsub_email' => 'unsub_email',
    'unsub_email_1' => 'unsub_email_1',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'unsub_email' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'unsub_email_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<h2>Duplicate IP Address Signature Emails</h2>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Signature Mail Fraud Data: Ip_address */
  $handler->display->display_options['fields']['ip_address']['id'] = 'ip_address';
  $handler->display->display_options['fields']['ip_address']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['fields']['ip_address']['field'] = 'ip_address';
  $handler->display->display_options['fields']['ip_address']['label'] = 'IP Address';
  /* Field: Signature Mail Fraud Data: Ip_address */
  $handler->display->display_options['fields']['ip_address_1']['id'] = 'ip_address_1';
  $handler->display->display_options['fields']['ip_address_1']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['fields']['ip_address_1']['field'] = 'ip_address';
  $handler->display->display_options['fields']['ip_address_1']['group_type'] = 'count';
  $handler->display->display_options['fields']['ip_address_1']['label'] = '# of duplicate IP addresses';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Signature Mail Fraud Data: Ip_address */
  $handler->display->display_options['sorts']['ip_address']['id'] = 'ip_address';
  $handler->display->display_options['sorts']['ip_address']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['sorts']['ip_address']['field'] = 'ip_address';
  $handler->display->display_options['sorts']['ip_address']['group_type'] = 'count';
  $handler->display->display_options['sorts']['ip_address']['order'] = 'DESC';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Signature Mail Fraud Data: Ip_address */
  $handler->display->display_options['filters']['ip_address']['id'] = 'ip_address';
  $handler->display->display_options['filters']['ip_address']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['filters']['ip_address']['field'] = 'ip_address';
  $handler->display->display_options['filters']['ip_address']['group_type'] = 'count';
  $handler->display->display_options['filters']['ip_address']['operator'] = '>';
  $handler->display->display_options['filters']['ip_address']['value']['value'] = '1';
  /* Filter criterion: Signature Mail Fraud Data: Ip_address */
  $handler->display->display_options['filters']['ip_address_1']['id'] = 'ip_address_1';
  $handler->display->display_options['filters']['ip_address_1']['table'] = 'signature_mail_fraud_data';
  $handler->display->display_options['filters']['ip_address_1']['field'] = 'ip_address';
  $handler->display->display_options['filters']['ip_address_1']['operator'] = '!=';
  $handler->display->display_options['filters']['ip_address_1']['value'] = '0';
  $handler->display->display_options['block_description'] = 'Duplicate IP Address Signatures';
  $export['signature_mail_fraud_data'] = $view;

  return $export;
}
