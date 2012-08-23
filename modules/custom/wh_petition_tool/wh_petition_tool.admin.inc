<?php
/**
 * @file
 */
 
/**
 * Config options for Web Services integration with the OpenSaaS Hub.
 */
function wh_petition_tool_hub_settings($form, &$form_state) {

  $form['wh_petition_tool_hub_key'] = array(
    '#title' => t('API Key'),
    '#type' => 'textfield',
    '#description' => t('The authentication key as configured in the services module of the OpenSaaS Hub'),
    '#default_value' => variable_get('wh_petition_tool_hub_key'),
  );
  
  $form['wh_petition_tool_hub_url'] = array(
    '#title' => t('Services URL'),
    '#type' => 'textfield',
    '#description' => t('The Services URL of the OpenSaaS Hub.  Example: http://server.com/services/xmlrpc'),
    '#default_value' => variable_get('wh_petition_tool_hub_url', ''),
  );

  $form['wh_petition_tool_request_domain'] = array(
    '#title' => t('Request Domain'),
    '#type' => 'textfield',
    '#description' => t('The domain as configured in the Services API Key Authentication. Example: server.com'),
    '#default_value' => variable_get('wh_petition_tool_request_domain'),
  );

  return system_settings_form($form);
}