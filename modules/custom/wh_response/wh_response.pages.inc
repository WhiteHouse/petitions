<?php

/**
 * @file
 * Display code for Responses.
 */

/**
 * Callback to display the list of responses on the Responses ( /responses ) page.
 */
function wh_response_show_responses() {
  if (variable_get('wh_petitions_use_no_responses_page', FALSE)) {
    return theme('wh_response_display_responses_page', array(
      'no_response_text' => variable_get('wh_petitions_no_responses_page_text', ''),
      'no_responses' => TRUE,
    ));
  }

  // Get the arguments from the URL.  These will be sanitized later in the helper function ( wh_response_get_responses )
  $sort = arg(1);
  $page = arg(2);
  $cols = arg(3);
  $issues = arg(4);
  $search = arg(5);

  // Variables that are passed by reference to the helper function.
  $count = 0;
  $total = 0;

  // Call the helper function to retrieve the HTML for the petitions to
  // display on this page ( page 1 ).  Also passes the other variables by
  // reference so they can be sanitized and updated if need be.
  $html = wh_response_get_responses($sort, $page, $cols, $issues, $search, $count, $total);

  // Display the 'More' link if we had responses on this page, plus the total
  // count is more than the total count we've displayed so far.
  $next_page = '';
  if ($count > 0 && ($total > ($page * WH_RESPONSE_RESPONSES_PER_PAGE))) {
    $next_page = 'responses/' . $sort . '/' . ($page + 1) . '/' . $cols . '/' . implode('+', $issues) . '/' . $search;
  }

  // Load the issues and search forms.
  module_load_include('inc', 'wh_response', 'wh_response.form');
  $issues_form = drupal_get_form('wh_response_list_page_issues_form');
  $issues_form = drupal_render($issues_form);
  $search_form = drupal_get_form('wh_response_list_page_search_form');
  $search_form = drupal_render($search_form);

  // Theme the responses page.
  return theme('wh_response_display_responses_page', array(
    'responses' => $html,
    'sort' => check_plain($sort),
    'page' => intval($page),
    'cols' => intval($cols),
    'issues_form' => $issues_form,
    'issues' => implode(' ', $issues),
    'search_form' => $search_form,
    'search' => urlencode($search),
    'next_page' => urlencode($next_page),
    'count' => intval($count + (WH_RESPONSE_RESPONSES_PER_PAGE * ($page - 1))),
    'total' => intval($total),
    'no_responses' => FALSE,
  ));
}

/**
 * Function to retrieve the HTML for the responses that should show on a given page.  Also sanitizes and modifies the page arguments to make sure they're valid.
 *
 * Arguments:
 * 1) sort
 *   -featured: whether it is featured ( checkbox )
 *   -popular: petitions applied to a response desc  ( currently removed )
 * 2) page number ( default = 1 )
 * 3) columns ( 1 or 2, default = 2 )
 * 4) Issues ( issue term tid's separated by spaces )
 * 5) Search terms ( separated by spaces )
 */
function wh_response_get_responses(&$sort, &$page, &$cols, &$issues, &$search, &$count, &$total) {
  $html = '';
  $count = 0;
  $responses = array();

  // Make sure the sort is a valid option.
  if (!in_array($sort, array('featured'))) {
    $sort = 'featured';
    drupal_set_title(t('Featured Responses'));
  }

  // Validate the page and calculate the offset for the query.
  $page = intval($page);
  if ($page <= 0) {
    $page = 1;
  }
  $offset = ($page - 1) * WH_RESPONSE_RESPONSES_PER_PAGE;

  // 1 Col or 2
  if ($cols != 1 && $cols != 2) {
    $cols = 2;
  }

  // Issues.
  if (!empty($issues)) {
    $issues = explode(" ", $issues);
    for ($i = 0; $i < count($issues); $i++) {
      $issues[$i] = intval($issues[$i]);
    }
  }

  // Search.
  $search = trim(strip_tags(urldecode($search)));
  $keywords = explode(' ', $search);

  // Query.
  $query = db_select('node', 'n', array('target' => 'slave'));
  $query->innerJoin('field_data_field_petition_count', 'c', 'n.nid = c.entity_id');
  $query->addField('c', 'field_petition_count_value', 'field_petition_count_value');
  $query->fields('n', array('nid', 'title'))
    ->condition('n.type', 'wh_response')
    ->condition('n.status', 0, '<>');

  if ($sort == 'featured') {
    $query->innerJoin('field_data_field_is_featured', 'f', 'n.nid = f.entity_id');
    $query->addField('f', 'field_is_featured_value', 'field_is_featured_value');
    $query->orderBy('c.field_petition_count_value', 'DESC')
      ->orderBy('n.title', 'ASC')
      ->condition('f.field_is_featured_value', 1);
  }
  else {
    $query->orderBy('c.field_petition_count_value', 'DESC')
      ->orderBy('n.title', 'ASC');
  }

  if (!empty($search)) {
    $query->where("upper(n.title) LIKE upper(:search)", array(':search' => '%' . $search . '%'));
  }

  if (!empty($issues)) {
    $query->innerJoin('field_data_field_issues', 'i', 'n.nid = i.entity_id');
    $query->condition('i.field_issues_tid', $issues, 'IN');
    $query->distinct(TRUE);
  }
  $result = $query->range($offset, WH_RESPONSE_RESPONSES_PER_PAGE)->execute();

  // Display the results.
  while ($res = $result->fetchAssoc()) {
    $html .= theme('wh_response_display_list_response', array(
      'entry_num' => check_plain($count),
      'cols' => check_plain($cols),
      'nid' => check_plain($res['nid']),
      'title' => l($res['title'], 'node/' . $res['nid'], array('attributes' => array('rel' => 'nofollow'))),
      'response_link' => l(t('Read Full Response'), 'node/' . $res['nid'])
    ));
    $count++;
  }

  // Retrieve the count if we had results on this page.
  if ($count) {
    $query->range();
    $total = $query->countQuery()->execute()->fetchField();
  }

  // We want something to occupy the place in the url in case there are no issues.
  if (empty($issues)) {
    $issues = array(0);
  }

  if (!empty($issues) && $count == 0 && $issues[0] != 0) {
    if (count($issues) == 1) {
      $html = '<p class="no-results">' . t('There are no responses that match this issue.') . '</p>';
    }
    else {
      $html = '<p class="no-results">' . t('There are no responses that match these issues.') . '</p>';
    }
  }

  if ($count == 0 && !empty($search)) {
    return '<p class="no-results">' . t('There are no responses that match this search.') . '</p>';
  }

  return $html;
}

/**
 * Callback to display more responses from an ajax request.  Take the variables we got from the URL and pass them to the function to retrive more responses.
 */
function wh_response_more_responses($sort, $page, $cols, $issues) {
  $count = 0;
  $total = 0;
  $search = arg(6);
  $html = wh_response_get_responses($sort, $page, $cols, $issues, $search, $count, $total);

  // Display the 'More' link if we had responses on this page, plus the total count is more than the total count we've displayed so far.  Otherwise, display a bar with the total count of results.
  if ($total > 0) {
    $next_page = '';
    if ($count > 0 && ($total > ($page * WH_RESPONSE_RESPONSES_PER_PAGE))) {
      $count = $count + (WH_RESPONSE_RESPONSES_PER_PAGE * ($page - 1));
      $next_page = 'responses/' . $sort . '/' . ($page + 1) . '/' . $cols . '/' . implode('+', $issues) . '/' . urlencode($search);
      $html .= '<div id="petition-bars">';
      $html .= l(t('Viewing !count of !total: Show More Responses', array(
        '!count' => '<span id="response-count">' . $count . '</span>',
        '!total' => $total
      )), $next_page, array(
        "html" => TRUE,
        "attributes" => array("class" => "clear show-more-responses-bar no-follow")
      ));
      $html .= '<div class="clear loading-more-petitions-bar display-none">' . t('loading more responses...') . '</div>';
      $html .= '<div class="display-none" id="page-num">' . $page . '</div></div>';
    }
    else {
      $count = $count + (WH_RESPONSE_RESPONSES_PER_PAGE * ($page - 1));
      $html .= '<div id="petition-bars">';
      $html .= '<div class="clear loading-more-petitions-bar">' . t('Viewing !count of !total', array(
          '!count' => '<span id="response-count">' . $count . '</span>',
          '!total' => $total
        )) . '</div>';
      $html .= '<div class="display-none" id="page-num">' . $page . '</div></div>';
    }
  }

  // If this was a jsonp request with a callback, output the callback.  Otherwise just return the json-encoded data.
  $data = array('markup' => $html);
  if (!empty($_GET['callback'])) {
    $callback = check_plain($_GET['callback']);
    print $callback . '(' . json_encode($data) . ')';
  }
  else {
    print json_encode($data);
  }
  exit;
  print $html;
}

/**
 * Callback to display the email response iframe.  Needs to be in an iframe otherwise the forward form and captcha fail if we bring this form over via the normal json.
 */
function wh_response_email_response($nid) {
  global $base_url;

  if (!empty($nid)) {
    $node = node_load($nid);
    if (!empty($node)) {
      global $base_url;
      drupal_set_title(t('Email A Response'));
      $url = $base_url . '/response/email_frame/' . check_plain($nid) . '/';
      return '<iframe src="' . $url . '" width="687" height="1000" frameborder="0"></iframe>';
    }
  }
  return '';
}

/**
 * Callback to display the email response form in the iframe from above.
 */
function wh_response_email_response_frame($nid) {
  $html = '';
  if (!empty($nid)) {
    $nid = intval((int) $nid);
  }
  if (!empty($nid)) {
    $node = node_load($nid);
    if (!empty($node)) {
      global $base_url;
      $styles = drupal_get_css();
      $scripts = drupal_get_js();
      $url = 'node/' . $nid;
      $form_html = drupal_render(drupal_get_form('forward_form', $url, check_plain($node->title), TRUE));
      $messages = theme('status_messages');

      $html = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
      <html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" dir="ltr">
      <head><title>' . t('Email A Response') . '</title>
        ' . $styles . $scripts . '
        <style type="text/css">
          #petition-inner { padding:0px; }
        </style>
      </head>
      <body><div id="petition-inner">
        ' . $messages . '
        <div id="email-page"><div class="email-response"><h2 class="page-title">' . t('Email This Response') . '</h2>' . $form_html . '</div></div>
      </div></body>
      </html>';
    }
  }
  print $html;
  exit;
}

/**
 * Callback to display the email response thank you page in the iframe and link out of the iframe.
 *
 * @todo - Ben - This feature isn't implemented in prod. Should verify the $url param when it is.
 */
function wh_response_email_response_thank_you($nid) {
  $html = '';
  if (!empty($nid)) {
    $node = node_load($nid);
    if (!empty($node)) {
      global $base_url;
      $styles = drupal_get_css();
      $scripts = drupal_get_js();
      $messages = theme('status_messages');
      $url = url('node/' . $node->nid);

      $html = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
      <html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" dir="ltr">
      <head><title>' . t('Email A Response') . '</title>
      ' . $styles . $scripts . '
      <style type="text/css">
        #petition-inner { padding:0px; }
      </style>
      </head>
      <body><div id="petition-inner">
      ' . $messages . '
      <div id="email-page"><div class="email-response"><h2 class="page-title">' . t('Email This Response') . '</h2>' . l(t('Click here to return to the response'), $url, array(
          'attributes' => array(
            'target' => '_top',
            'class' => array('no-follow')
          )
        )) . '</div></div></div></body>
      </html>';
    }
  }
  print $html;
  exit;
}
