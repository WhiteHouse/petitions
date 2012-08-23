<?php

/**
 * Callback for the standalone page that has the issues filter form.  
 */
function wh_response_filter_issue_standalone() {
  $form = drupal_get_form('wh_response_list_page_issues_form');
  $form = drupal_render($form);
  drupal_set_title(t('Filter By Issues'));
  
  return '<div id="filter-page"><h3>' . t('Filter By Issues') . '</h3><div class="drop-down">' . $form . '</div></div>';
}

/**
 * Callback for the standalone page that has the search filter form.  
 */
function wh_response_filter_search_standalone() {
  $form = drupal_get_form('wh_response_list_page_search_form');
  $form = drupal_render($form);
  drupal_set_title(t('Response Search'));
  
  return '<div id="filter-page"><h3>' . t('Search') . '</h3><div class="drop-down">' . $form . '</div></div>';
}

/**
 * Callback for the standalone pages that have the search and filter forms.
 */
function wh_response_filter_results() {
  $issues = array();
  if (!empty($_POST['filter_issues'])) {
    $tmp = check_plain($_POST['filter_issues']);
    foreach ($tmp as $key => $val) {
      if ($val > 0) { 
        array_push($issues, intval($key));
      }
    }
  }

  // Stick something in the url if they didn't select any.
  if (empty($issues)) {
    $issues = array(0);
  }
  
  $search = 0;
  if (!empty($_POST['filter_search'])) {
    $search = urlencode(strip_tags($_POST['filter_search']));
  }
  
  $arr[0] = 'responses';
  $arr[1] = 'featured';
  $arr[2] = 0;
  $arr[3] = 2;
  $url = implode('/', $arr) . '/' . implode('+', $issues) . '/' . $search;
  
  // Redirect to the main site or the petition tool site, depending on where this request came from
  drupal_goto($url);
}

/**
 * Filter by issues form on responses list page
 */
function wh_response_list_page_issues_form($form, &$form_state) {
  global $base_url;
  $form['#attributes'] = array('id' => 'form-item-issues-filter', 'class' => array('response-issue-form'));
  $form['#action'] = $base_url . '/filter/results/responses';
  
  // Issues
  $issues = array();
  $vocab = taxonomy_vocabulary_machine_name_load('issues');
  $counts = variable_get('wh_petitions_response_issue_counts', array());
  if (!empty($vocab)) {
    $issues_list = taxonomy_get_tree($vocab->vid);
    foreach ($issues_list as $issue) {
      $issues[$issue->tid] = check_plain($issue->name);
      $total = 0;
      if (!empty($counts[$issue->tid])) {
        $total = $counts[$issue->tid];
      }
      $issues[$issue->tid] .= '<span class="num"> - ' . $total . '</span>';
    }
  }  
  
  // If there are issues in the url, pre-populate the values in the form
  $arg_issues = arg(4);
  if (!empty($arg_issues)) {
    $arg_issues = explode(' ', $arg_issues);
    foreach ($arg_issues as $key => $val) {
      $form_state['values']['filter_issues'][$val] = $val;
    }    
  }
  
  // Issues checkboxes
  $form['filter_issues'] = array(
    '#type' => 'checkboxes',
    '#title' => '',
    '#prefix' => '<div class="clearfix">',
    '#suffix' => '</div>',    
    '#required' => FALSE,
    '#options' => $issues,
    '#after_build' => array('wh_response_issue_column_checks'),
    '#default_value' => !empty($form_state['values']['filter_issues']) ? $form_state['values']['filter_issues'] : array(),
  );
  
  // Submit Button
  $form['report'] = array(
    '#type' => 'submit',
    '#value' => t('Filter Responses'),
    '#attributes' => array('id' => 'button-filter-responses', 'class' => array('blue-button')),    
  );
   
  return $form;
}

/**
 * Places the checkboxes into 3 (mostly-equal) columms.
 */
function wh_response_issue_column_checks($form_element, &$form_state) {
  $size = sizeof($form_element['#options']);
 
  if ($size > 3) {
    $in_each = ceil($size / 3);
    $i = 1;
    foreach ($form_element['#options'] as $key => $val) {
      if ($i == 1) {
        $form_element[$key]['#prefix'] = '<div class="group">';
      }
      elseif ($i == ($in_each + 1)) {
        $form_element[$key]['#prefix'] = '</div><div class="group">';
      }
      elseif ($i == (($in_each * 2) + 1)) {
        $form_element[$key]['#prefix'] = '</div><div class="group">';
      }
      elseif ($i == $size) {
        $form_element[($key)]['#suffix'] = '</div>';
      }     
      
      $form_element[$key]['#attributes']['id'] = $form_element['#id']. '-' . $key;
      $i++;
    }
    
    return $form_element;
  }
}

/**
 * Filter by search form on responses list page
 */
function wh_response_list_page_search_form($form, &$form_state) {
  global $base_url;
  $form['#attributes'] = array('id' => 'form-item-search-drop', 'class' => array('response-search-form'));
  $form['#action'] = $base_url . '/filter/results/responses';

  // Prepopulate if there was something in the url
  $search = arg(5);
  if (!empty($search)) {
    $search = strip_tags(urldecode($search));
    $form_state['values']['filter_search'] = check_plain($search);    
  }

  $form['filter_search'] = array(
    '#type' => 'textfield',
    '#title' => t('Search Terms'),
    '#required' => FALSE,
    '#attributes' => array('onfocus' => array("if(this.value=='" . t('Enter your search terms') . "'){this.value=''};")),    
    '#default_value' => !empty($form_state['values']['filter_search']) ? $form_state['values']['filter_search'] : t('Enter your search terms'),
  );
  
  // Submit Button
  $form['go'] = array(
    '#type' => 'submit',
    '#value' => t('Go'),
    '#attributes' => array('id' => 'search-go-button', 'class' => array('button')),      
  );
  
  if (arg(0) == 'filter-search') {
    $form['go']['#attributes'] = array('id' => 'search-go-button', 'class' => array('blue-button'));
  }
    
  return $form;
}