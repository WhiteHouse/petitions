<?php

/*********************** PETITION LIST ****************************/
/**
 * Callback to display the list of petitions on the Open Petitions ( /petitions ) page.
 */
function wh_petitions_show_petitions() {
  // Get the arguments from the URL.  These will be sanitized later in the helper function ( wh_petitions_get_petitions )
  $sort = arg(1);
  $page = arg(2);
  $cols = arg(3);
  $issues = arg(4);
  $search = arg(5);

  // Variables that are passed by reference to the helper function.
  $count = 0;
  $total = 0;
  
  // Call the helper function to retrieve the HTML for the petitions to display on this page ( page 1 ).  Also passes the other variables by reference so they can be 
  // sanitized and updated if need be.
  $html = wh_petitions_get_petitions($sort, $page, $cols, $issues, $search, $count, $total);  
  
  // Display the 'More' link if we had petitions on this page, plus the total count is more than the total count we've displayed so far.
  $next_page = '';
  if ($count > 0 && ($total > ($page * WH_PETITION_PETITIONS_PER_PAGE))) {
    $next_page = 'petitions/' . $sort . '/' . ($page + 1) . '/' . $cols . '/' . implode('+', $issues) . '/' . $search;
  }

  // Load the issues and search forms
  module_load_include('inc', 'wh_petitions', 'wh_petitions.form');
  $issues_form = drupal_get_form('wh_petitions_list_page_issues_form');
  $issues_form = drupal_render($issues_form);
  $search_form = drupal_get_form('wh_petitions_list_page_search_form');
  $search_form = drupal_render($search_form);
  
  // Theme the petitions page
  return theme('wh_petitions_display_petition_page', array(
    'petitions' => $html, 
    'sort' => check_plain($sort), 
    'page' => intval($page), 
    'cols' => intval($cols), 
    'issues_form' => $issues_form, 
    'issues' => implode(' ', $issues), 
    'search_form' => $search_form, 
    'search' => urlencode($search), 
    'next_page' => urlencode($next_page), 
    'count' => intval($count + (WH_RESPONSE_RESPONSES_PER_PAGE * ($page - 1))),
    'total' => wh_petitions_format_number($total),
  ));
}

/**
 * Function to retrieve the HTML for the petitions that should show on a given page.  Also sanitizes and modifies the page arguments to make sure they're valid.
 *
 * Arguments:
 * 1) sort
 *   -all: by date they became public desc
 *   -trending: most signatures in past 24 hours desc
 *   -popular: total signatures desc
 * 2) page number ( 8 per page, default = 0 )
 * 3) columns ( 1 or 2, default = 2 )
 * 4) Issues ( issue term tid's separated by spaces )
 * 5) Search terms ( separated by spaces )
 */
function wh_petitions_get_petitions(&$sort, &$page, &$cols, &$issues, &$search, &$count, &$total) {
  $html = '';
  $count = 0;
  $petitions = array();
  $conn = wh_petitions_mongo_petition_connection();

  // Make sure the sort is a valid option
  if (!in_array($sort, array('all', 'trending', 'popular'))) {
    $sort = 'all';
  }
  
  // Validate the page and calculate the offset for the query
  $page = intval($page);
  if ($page <= 0) {
    $page = 1;
  }
  $offset = ($page - 1) * WH_PETITION_PETITIONS_PER_PAGE;
  
  // 1 Col or 2
  if ($cols != 1 && $cols != 2) {
    $cols = 2;
  }
  
  // Issues
  if (!empty($issues)) {
    $issues = explode(" ", urldecode($issues));
    for ($i=0; $i<sizeof($issues); $i++) {
      $issues[$i] = intval($issues[$i]);
    }
  }

  // Search
  $search = strtolower(strip_tags(trim(urldecode($search)))); 
  $keywords = explode(' ', $search);

  // Retrieve only public and under_review petitions that aren't hidden from lists by admins
  $query = array(
    'petition_status' => array(
      '$in' => array((int)WH_PETITION_STATUS_PUBLIC, (int)WH_PETITION_STATUS_UNDER_REVIEW)
    ),
    'hidden' => 0,
  );

  // Retrieve petitions with at least one of the issue tid's
  if (!empty($issues)) {
    $query['issues'] = array('$in' => $issues);
  }
  
  // Retrieve petitions that have at least one of the same keywords from the search in their titles.
  if (!empty($search)) {
    $query['title_keywords'] = array('$in' => $keywords);
  }  

  // Compose the sort option
  $sort_query = array();
  if ($sort == 'all') {
    $sort_query = array(
      'reached_public' => -1,
    );
    drupal_set_title(t('All Petitions'));
  }
  elseif ($sort == 'popular') {
    $sort_query = array(
      'signature_count' => -1,
    );
    drupal_set_title(t('Popular Petitions'));
  }
  elseif ($sort == 'trending') {
    drupal_set_title(t('Trending Petitions'));
  }

  // Fields to retrieve
  $retrieve_fields = array('title', 'signature_count', 'reached_public', 'nice_url');
  
  // If we're searching by text, we should be displaying results that match the title first, then results that match the body.
  // To accomplish this, we do two queries, one matching title and one matching description
  // Based on the counts from each and the page we're on, we can figure out which query(s) to run and which results to get.
  if (!empty($search)) {
    // Find how many match titles
    $title_total = $conn->find($query)->sort($sort_query)->count();
    // Find how many match descriptions
    $body_query = $query;
    $body_query['body_keywords'] = $body_query['title_keywords'];
    $body_query['title_keywords'] = array('$nin' => $keywords);
    $body_total = $conn->find($body_query)->sort($sort_query)->count();

    // Combine the results
    $total = $title_total + $body_total;
  }
  
  // If we ran a search and had results, we need to figure out what to display.
  if (!empty($search) && $total > 0) {    
    // If we're not past the count of titles, query petitions based on title.
    if ($offset < $title_total) {
      $result = $conn->find($query, $retrieve_fields)->sort($sort_query)->skip($offset)->limit(WH_PETITION_PETITIONS_PER_PAGE);
      
      foreach ($result as $res) {
        array_push($petitions, $res);
        $count++;
      }      
    }
    
    // Query descriptions if we didn't find 8 (WH_PETITION_PETITIONS_PER_PAGE) results from title search ( either ran out of results or the offset is past the 
    // result count for titles )
    if ($count < WH_PETITION_PETITIONS_PER_PAGE) {
      $limit = WH_PETITION_PETITIONS_PER_PAGE;
      if ($count < WH_PETITION_PETITIONS_PER_PAGE) { 
        $limit = WH_PETITION_PETITIONS_PER_PAGE - $count;
      }
      $body_offset = $offset - $title_total;
      if ($body_offset < 0) { 
        $body_offset = 0;
      }
      $body_result = $conn->find($body_query, $retrieve_fields)->sort($sort_query)->skip($body_offset)->limit($limit);
      
      foreach ($body_result as $res) {
        array_push($petitions, $res);
        $count++;
      }        
    }
  }
  else { 
    // We had a search but found no results, keep the page the same.
    if (!empty($search)) {
      return '<p class="no-results">' . t('There are no petitions that match this search.') . '</p>';
    }
    
    // For Trending sort, we have a helper function to do this for us since this logic is used in multiple places.
    if ($sort == 'trending') {
      $total = 0;
      $result = wh_petitions_trending_results($conn, $offset, WH_PETITION_PETITIONS_PER_PAGE, $total);
    }
    // Otherwise, proceed normally
    else {
      $result = $conn->find($query, $retrieve_fields)->sort($sort_query)->skip($offset)->limit(WH_PETITION_PETITIONS_PER_PAGE);
      $total = $conn->find($query)->sort($sort_query)->count();
    }

    foreach ($result as $res) {
      array_push($petitions, $res);
      $count++;
    }
  }

  // We want something to occupy the place in the url in case there are no issues.
  if (empty($issues)) { 
    $issues = array(0);
  }
  
  // Theme the petitions
  $i = 0;
  foreach ($petitions as $res) {
    $id = $res['_id']->__toString();
    $html .= theme('wh_petitions_display_list_petition', array(
      'entry_num' => $i, 
      'title' => check_plain($res['title']), 
      'signature_count' => wh_petitions_format_number($res['signature_count']), 
      'petition_link' => l(t('Find out more'), $res['nice_url']), 
      'cols' => intval($cols), 
      'petition_id' => check_plain($id),
      'nice_url' => $res['nice_url'],
    ));
    $i++;
  }
  
  if (!empty($issues) && empty($html) && $issues[0] != 0) {
    if (sizeof($issues) == 1) {
      $html = '<p class="no-results">' . t('There are no petitions that match this issue.') . '</p>';
    }
    else {
      $html = '<p class="no-results">' . t('There are no petitions that match these issues.') . '</p>';
    }
  }
  
  return $html;  
}

/**
 * Callback to display more petitions from an ajax request.  Take the variables we got from the URL and pass them to the function to retrive more petitions.
 */
function wh_petitions_more_petitions($sort, $page, $cols, $issues) {
  $count = 0;
  $total = 0;
  $search = arg(6);
  $html = wh_petitions_get_petitions($sort, $page, $cols, $issues, $search, $count, $total);

  // Display the 'More' link if we had petitions on this page, plus the total count is more than the total count we've displayed so far.  Otherwise, display the count ( last page )
  if ($total > 0) {
    $next_page = '';
    if ($count > 0 && ($total > ($page * WH_PETITION_PETITIONS_PER_PAGE))) {
      $count = $count + (WH_PETITION_PETITIONS_PER_PAGE * ($page - 1));
      $next_page = 'petitions/' . $sort . '/' . ($page + 1) . '/' . $cols . '/' . implode('+', $issues) . '/' . urlencode($search);
      $html .= '<div id="petition-bars">';
      $html .= l(t('Viewing !count of !total: Show More Petitions', array('!count' => '<span id="petition-count">' . $count . '</span>', '!total' => wh_petitions_format_number($total))), $next_page, array("html" => TRUE, "attributes" => array("class" => "clear show-more-petitions-bar no-follow")));
      $html .= '<div class="clear loading-more-petitions-bar display-none">' . t('loading more petitions...') . '</div>';
      $html .= '<div class="display-none" id="page-num">' . $page . '</div></div>';
    }
    else {
      $count = $count + (WH_PETITION_PETITIONS_PER_PAGE * ($page - 1));
      $html .= '<div id="petition-bars"><div class="clear loading-more-petitions-bar">' . t('Viewing !count of !total', array('!count' => '<span id="petition-count">' . $count . '</span>', '!total' => wh_petitions_format_number($total))) . '</div><div class="display-none" id="page-num">' . $page . '</div></div>';
    }
  }
 
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
 * Function to retrieve the petition set for a trending query.  Logic is to retrieve results that have the most signatures within previous 24-hours.
 * To accomplish this, we need to get the results into an array and sort the result array based on signature count from the timeframe.
 * 
 * @todo: Note: Trending used to be a link on the /petitions page. This has been commented out, but the code is still here because we're concerned about breaking something else by removing it. Please revisit.
 */
function wh_petitions_trending_results($conn, $offset, $per_page, &$total) {
  $ids = variable_get('wh_petitions_trending_ids', array()); 
  if (!empty($ids)) {
    $id_slice = array_slice($ids, $offset, $per_page);
    $query = array(
      '_id' => array('$in' => $id_slice),
    );
    $retrieve_fields = array('title', 'signature_count', 'published', 'nice_url');
    $result = $conn->find($query, $retrieve_fields);
  }
  $arr = array();
  foreach ($id_slice as $key => $val) {
    foreach ($result as $res) {
      if ($res['_id'] == $val) {
        array_push($arr, $res);
      }
    } 
  }
  $total = sizeof($ids);
  
  return $arr;
}

/*********************** PETITION DETAIL ****************************/

/**
 * Function to display an individual petition page.
 */
function wh_petitions_petition_detail($petition_id) {
  global $user;
  global $base_url;
  
  $logged_in = ($user->uid > 0) ? TRUE : FALSE;

  // Make sure we got a petition id
  if (empty($petition_id)) {
    return '';
  }
    
  // Load the petition
  $conn = wh_petitions_mongo_petition_connection(); 
  $petition = wh_petitions_load_petition($conn, $petition_id);

  // If it wasn't a valid petition id, exit
  if (empty($petition)) {
    return '';
  }
  
  // Check for the status of this. No one should be able to see draft petitions through this callback, not even admins.
  if ($petition['petition_status'] == WH_PETITION_STATUS_DRAFT) {
    return '';
  }
  
  // Archived petitions should return a generic message page for all but admins
  if ($petition['petition_status'] == WH_PETITION_STATUS_CLOSED && !user_access('administer petitions')) {
    return theme('wh_petitions_page_archived', array('archived_text' => variable_get('wh_petitions_archived_page_text', '')));
  }  
  
  // Set a context variable for petition status we can use later.
  context_set('petition', 'status', $petition['petition_status']);
  drupal_set_title(filter_xss($petition['title'], array()), PASS_THROUGH);
  
  // Clear some variables we'll set later for some petition statuses.
  $archived = FALSE;
  $published = TRUE;
  $responded = FALSE;
  $flagged = FALSE;
  $under_review = FALSE;
  $response_title = '';
  $response_body = '';
  $response_audio = '';
  $response_video = '';
  $agency_seal = '';
  $agency_name = '';
  $response_intro = '';
  
  // Get petition details  
  $timestamp = $petition['_id']->getTimestamp();
  $created = date("M d, Y", $timestamp);
  $issues = array();
  if (!empty($petition['issues'])) {
    for ($i=0;$i<sizeof($petition['issues']);$i++) {
      $term = taxonomy_term_load($petition['issues'][$i]);
      if (!empty($term)) {
        array_push($issues, l($term->name, 'petitions/all/0/2/' . intval($term->tid)));
      }
    }
  }
  $issues = implode(", ", $issues); 
  $public_needed = $petition['public_signatures'];
  $total_needed = $petition['response_signatures'];
  $signatures_needed = $total_needed - $petition['signature_count'];
  if ($signatures_needed < 0) { 
    $signatures_needed = 0;
  }
  $signatures = $petition['signature_count'];
  $time = $petition['published'];
  $end_date = strtotime('+' . $petition['review_timeframe'] . ' days', $time);
  $end_date = date("F d, Y", $end_date);  
  
  // Set a flag for archived petitions
  if ($petition['petition_status'] == WH_PETITION_STATUS_CLOSED) {
    $archived = TRUE;
  }  
  // Petitions that have been reviewed and have a response attached ( should always be there together, but check just in case )
  elseif ($petition['petition_status'] == WH_PETITION_STATUS_REVIEWED) {    
    if ($petition['response_status'] == WH_PETITION_RESPONSE_STATUS_ANSWERED) {
      $responded = TRUE;    
    
      // Get response details.  node_load might be a bit much in this case, but for now we'll stick with it.
      $node = node_load($petition['response_id']);
      if (!empty($node) && $node->status > 0) {  
        $response_title = check_plain($node->title);
        $response_body = $node->body[$node->language][0]['safe_value'];
      
        $agency_name = '';
        if (!empty($node->field_agency_name[$node->language][0]['value'])) {
          $agency_name = $node->field_agency_name[$node->language][0]['value'];
        }
        $agency_seal = '';
        if (!empty($node->field_agency_seal[$node->language][0]['uri'])) {
          $image_vars = array(
           'style_name' => 'agency_seal',
           'path' => $node->field_agency_seal[$node->language][0]['uri'],
           'alt' => (!empty($node->field_agency_seal[$node->language][0]['alt'])) ? check_plain($node->field_agency_seal[$node->language][0]['alt']) : '',
           'title' => (!empty($node->field_agency_seal[$node->language][0]['title'])) ? check_plain($node->field_agency_seal[$node->language][0]['title']) : '',
           'attributes' => array('class' => array('graphic')),
           'getsize' => TRUE,
         );
         $agency_seal = theme('image_style', $image_vars);
       }        
      
       // Retrieve the appropiate audio/video field info for the response.
       if (!empty($node->field_response_media[$node->language][0])) {
         $media_tid = $node->field_response_media[$node->language][0]['tid'];
         $media = taxonomy_term_load($media_tid);
         $media_name = $media->name;
  
         switch ($media_name) {
           case 'Audio':
             if (!empty($node->field_audio_embed[$node->language][0])) {
               $response_audio = check_plain($node->field_audio_embed[$node->language][0]['value']);
             }
             break;
           case 'Video':
             if (!empty($node->field_video_embed[$node->language][0])) {
               $response_video = check_plain($node->field_video_embed[$node->language][0]['value']);
             }
             break;                 
          }
        }
      }
      
      // Get the intro text
      $res_conn = wh_response_mongo_response_connection();
      $obj = wh_response_load_petition_response($res_conn, $node->nid, $petition_id);   
      $response_intro = $obj['intro'];
    }
  }
  // Petitions that have been flagged for review.
  elseif ($petition['petition_status'] == WH_PETITION_STATUS_FLAGGED) {
    $flagged = TRUE;
  }
  
  // Admin Functionality
  $admin = FALSE;
  $admin_link = '';
  $private_tags = '';
  $user_tags = '';
  $reached_ready = '';
  if (user_access('administer petitions')) {
    $admin = TRUE;
    $private_tags = implode(', ', $petition['private_tags']);
    $user_tags = implode(', ', $petition['user_tags']);
    if ($petition['reached_ready']) {
      $reached_ready = date('M d, Y', $petition['reached_ready']);
    }
    $admin_link = l(t('Administer This Petition'), 'admin/petitions/' . check_plain($petition_id), array('attributes' => array('classes' => array('no-follow'))));
  }
  
  $flagged_text = '';
  $flagged_text_owner = '';
  if ($flagged) {
    $flagged_text = variable_get('wh_petitions_flagged_page_text', '');
    if ($petition['uid'] == $user->uid) {
      $flagged_text_owner = variable_get('wh_petitions_flagged_page_creator_text', '');
    }
  }
  $read_rules_link = variable_get('wh_petitions_read_rules_link', '');
  $petition_goals_link = variable_get('wh_petitions_learn_more_link', '');

  $govdelivery_topic = '';
  if (!empty($petition['govdelivery_id'])) {
    $govdelivery_topic = $petition['govdelivery_id'];
  }

  // Theme the top petition detail part
  $petition_html = theme('wh_petitions_display_petition_info', array(
    'title' => check_plain($petition['title']), 
    'body' => wh_petitions_format_description($petition['body']), 
    'created' => check_plain($created), 
    'issues' => $issues, 
    'published' => check_plain($published), 
    'total_needed' => wh_petitions_format_number($total_needed), 
    'signatures_needed' => wh_petitions_format_number($signatures_needed), 
    'signatures' => wh_petitions_format_number($signatures), 
    'end_date' => check_plain($end_date), 
    'archived' => $archived, 
    'responded' => $responded, 
    'flagged' => $flagged,
    'private_tags' => check_plain($private_tags),
    'user_tags' => check_plain($user_tags),
    'reached_ready' => check_plain($reached_ready),
    'admin' => $admin,
    'admin_link' => $admin_link,
    'read_rules_link' => $read_rules_link,
    'petition_goals_link' => $petition_goals_link,
    'flagged_text' => $flagged_text,
    'flagged_text_owner' => $flagged_text_owner,
    'govdelivery' => $govdelivery_topic,
  )); 
  
  // See if this user already signed the petition
  $already_signed = FALSE;
  if (!empty($user->uid)) {
    $sig_conn = wh_petitions_mongo_petition_signatures_connection();
    $sig_conn->setSlaveOkay(FALSE);
    $query = array(
      'uid' => (int)$user->uid,
      'petition_id' => (string)$petition_id,
    );
    $result = $sig_conn->findOne($query);
    if (!empty($result)) {
      $already_signed = TRUE;
    }  
  }
  
  // Show the signature form for petitions that haven't been signed by this user and are available to be signed.
  $comment_form = '';
  $signature_url = '';
  $why_text = '';
  if ($already_signed) {
    $signature_form = '';
  }
  elseif (empty($user->uid)) {
    $signature_form = l('Sign This Petition', 'user/login', array('attributes' => array('id' => 'button-sign-this-petition')));
    $why_text = variable_get('wh_petitions_tooltip_why', '');
  }
  elseif (in_array($petition['petition_status'], array(WH_PETITION_STATUS_PRIVATE, WH_PETITION_STATUS_PUBLIC, WH_PETITION_STATUS_UNDER_REVIEW))) {
    module_load_include('inc', 'wh_petitions', 'wh_petitions.form');
    $signature_form = drupal_get_form('wh_petitions_sign_petition_form', $petition_id);
    $signature_form = drupal_render($signature_form);
    $comment_form = drupal_get_form('wh_petitions_comment_petition_form', $petition_id);
    $comment_form = drupal_render($comment_form); 
    $signature_url = '';
    $signature_url = $base_url . "/petition/form/sign/" . check_plain($petition_id);
  }
  else {
    $signature_form = '';
  }
  
  // Set the 'Report as Inappropriate' link if it should appear.  Should appear on public/private/under review
  $inappropriate_form = '';
  $has_reported = FALSE;
  if (in_array($petition['petition_status'], array(WH_PETITION_STATUS_DRAFT, WH_PETITION_STATUS_CLOSED, WH_PETITION_STATUS_REVIEWED, WH_PETITION_STATUS_FLAGGED)) || empty($user->uid)) {
    $inappropriate_link = FALSE;
  }
  elseif (in_array($user->uid, $petition['abuse_flags'])) {
    $inappropriate_link = FALSE;
    $has_reported = TRUE;
  }
  else {
    $inappropriate_link = TRUE; 
    module_load_include('inc', 'wh_petitions', 'wh_petitions.form');
    $inappropriate_form = drupal_get_form('wh_petitions_report_inappropriate_form', $petition_id);
    $inappropriate_form = drupal_render($inappropriate_form);
  }

  // Whether to include the share functionality.
  $has_share_bar = TRUE;
  if (in_array($petition['petition_status'], array(WH_PETITION_STATUS_DRAFT, WH_PETITION_STATUS_REVIEWED, WH_PETITION_STATUS_FLAGGED))) {
    $has_share_bar = FALSE;
  }   
  
  // Get the signatures to display
  $has_more = TRUE;
  $signature_html = '';
  $page = 1;
  $sigs_per_page = 20;
  $last_id = '';
  if (!empty($_GET['page'])) {
    $get_page = preg_replace('/[^0-9]/', '', $_GET['page']);
    if (is_int((int)$get_page)) {
      $page = intval($get_page);
      $sigs_per_page = 19;
      $last_id = preg_replace('/[^a-zA-Z0-9]/', '', $_GET['last']);
    }
  }
  $signatures = wh_petitions_get_signatures($petition_id, $petition['uid'], $page, $has_more, FALSE, $petition['signature_count'], $last_id);
  $last_id = '';
  if (!empty($signatures)) {
    $last_id = $signatures[(sizeof($signatures) -1)]['_id']->__toString();
  }
  
  $signature_html = theme('wh_petitions_display_signatures', array(
    'signatures' => $signatures,
    'petition_id' => check_plain($petition_id), 
    'page' => $page, 
    'has_more' => $has_more, 
    'sigs_per_page' => $sigs_per_page,
    'last_id' => $last_id,
    'nice_url' => $petition['nice_url'],
  ));

  // Load the issues and search forms
  module_load_include('inc', 'wh_petitions', 'wh_petitions.form');
  $issues_form = drupal_get_form('wh_petitions_list_page_issues_form');
  $issues_form = drupal_render($issues_form);
  $search_form = drupal_get_form('wh_petitions_list_page_search_form');
  $search_form = drupal_render($search_form);    

  $reported_link = variable_get('wh_petitions_reported_as_inappropriate_link', '');
  $short_url_link = '';
  if (!empty($petition['short_url'])) {
    $short_url_link = l($petition['short_url'], $petition['short_url'], array('attributes' => array('class' => 'no-follow')));
  }
  else {
    $short_url_link = $petition['nice_url'];
  }

  // Theme the page
  $signatures_thanks = $petition['signature_count'] + 1;
  $thanks_needed = 0;
  if ($signatures_needed > 0) {
    $thanks_needed = $signatures_needed - 1;
  }

  $return_destination = drupal_get_destination();
  $return_destination = preg_replace('/\?.*/', '', $return_destination);

  return theme('wh_petitions_page_petition', array(
    'petition_id' => check_plain($petition_id),
    'response_title' => $response_title, 
    'response_body' => $response_body, 
    'response_audio' => $response_audio, 
    'response_video' => $response_video, 
    'petition' => $petition_html, 
    'petition_title' => check_plain($petition['title']),
    'already_signed' => $already_signed, 
    'signature_form' => $signature_form,
    'comment_form' => $comment_form,
    'signature_html' => $signature_html,
    'inappropriate_link' => $inappropriate_link, 
    'inappropriate_form' => $inappropriate_form,
    'has_share_bar' => $has_share_bar, 
    'issues_form' => $issues_form, 
    'search_form' => $search_form,
    'responded' => $responded,
    'archived' => $archived,
    'agency_name' => check_plain($agency_name),
    'agency_seal' => $agency_seal,
    'response_intro' => check_plain($response_intro),
    'flagged' => $flagged,
    'signature_count' => sizeof($signatures), 
    'signature_total' => wh_petitions_format_number($petition['signature_count']),
    'signature_url' => $signature_url,
    'has_reported' => $has_reported,
    'logged_in' => $logged_in,
    'reported_link' => $reported_link,
    'why_text' => $why_text,
    'return_destination' => $return_destination,
    'total_needed' => wh_petitions_format_number($total_needed), 
    'signatures_needed' => wh_petitions_format_number($thanks_needed), 
    'signatures' => wh_petitions_format_number($signatures_thanks), 
    'end_date' => check_plain($end_date),     
    'twitter_link' => wh_petition_tool_twitter_link(check_plain($petition_id), check_plain($petition['title']), 'petition', '', $petition['short_url'], $petition['nice_url']),
    'facebook_link' => wh_petition_tool_facebook_link(check_plain($petition_id), check_plain($petition['title']), 'petition', '', $petition['nice_url']),
    'short_url' => $short_url_link,
  ));
}

/**
 * Function to display an individual petition page by nice url
 */
function wh_petitions_petition_detail_niceurl($url) {
  // Load the petition
  $conn = wh_petitions_mongo_petition_connection(); 
  $nice_url = check_plain($_GET['q']);
  $petition = $conn->findOne(array('nice_url' => $nice_url), array('title'));

  if ($petition) {
    // These nice URLs can't be cleanly captured so force the Context.
    $context = context_load('petition');
    context_set('context', 'petition', $context);
    return wh_petitions_petition_detail($petition['_id']);
  }
  
  drupal_not_found();
}

/*********************** SIGNATURES ****************************/

/**
 * Retrieve the array of signatures for rendering based on the offset.  First signature on first page is the creator, second signature is the current user, if they've signed.
 *
 * petition_signatures = array of petition signatures
 * petition_uid = uid of creator
 * page = page of results
 * has_more = whether to display the 'more' box
 * whether we're coming from an ajax request.  if so, retrieve 20.  otherwise retrieve 19.
 * last id that we retrieved. use this for faster pagination.  ( range query instead of skip is faster according to 10gen )
 */
function wh_petitions_get_signatures($petition_id, $petition_uid, $page, &$has_more, $from_ajax = FALSE, $signature_count = 0, $last_id) {
  global $user;
  $has_user_signed = FALSE;
  $user_signature = array();
  $signatures = array();
  if (empty($page)) {
    $page = 1;
  }
  if ($from_ajax && $page != 1) {
    $per_page = 20;
  }
  else {
    $per_page = 19;
  }  
  $sig_conn = wh_petitions_mongo_petition_signatures_connection();
  
  // Find out if a user has signed the petition and capture their signature if they have
  if ($user->uid != $petition_uid) {
    $query = array(
      'uid' => (int)$user->uid,
      'petition_id' => (string)$petition_id,
    );
    $result = $sig_conn->findOne($query);
    if (!empty($result)) {
      $has_user_signed = TRUE;
      $user_signature = $result;
    }
  }
  
  // Get the signatures we should be retrieving
  if ($page == 1) { 
    $skip = 0;
    $max = 18;    
  }
  else {
    $skip = 18 + (($page - 2) * $per_page);
    $max = 18 + (($page - 1) * $per_page);
  }

  $query = array(
    'petition_id' => (string)$petition_id,
  );
  
  // If this current user has signed this petition, remove them from the possible signatures we retrieve ( they'll always be second on the first page )
  if (!empty($has_user_signed)) {
    $query['uid'] = array('$nin' => array((int)$user->uid));
    if ($page == 1) { 
      $per_page = $per_page - 1;
    }
    else {
      $skip--;
    }
  }
  
  // If we're on the first page, we want to get the creator's signature to display first.
  if ($page == 1) {
    $creator_signature = $sig_conn->findOne(array('petition_id' => (string)$petition_id, 'uid' => (int)$petition_uid));
  }
  
  // We don't want to retrieve the creator's signature in any of the queries either.
  if (empty($query['uid']) && !empty($petition_uid)) {
    $query['uid'] = array('$nin' => array((int)$petition_uid));
  }
  else {
    array_push($query['uid']['$nin'], (int)$petition_uid);
  }  

  // Get the first page of results
  if ($page == 1) {
    $results = $sig_conn->find($query)->sort(array('_id' => -1))->limit(($per_page - 1));
  }
  // If we got the last id from the previous page, use this to query by as it will be faster
  else if (!empty($last_id)) {
    $query['_id'] = array('$lt' => new MongoId($last_id));
    $results = $sig_conn->find($query)->sort(array('_id' => -1))->limit($per_page);
  }
  // If we just got a page number, not a last id, do the query anyway using skip
  else {
    $results = $sig_conn->find($query)->sort(array('_id' => -1))->skip($skip)->limit($per_page);
  }
  
  // Put the results into an array
  foreach ($results as $res) {
    array_push($signatures, $res);
  }

  // Determine if there are more signatures to display after these
  if ($max >= $signature_count) {
    $has_more = FALSE;
  }
  
  // If we have a creator's signature, make it the first one.
  if (!empty($creator_signature)) {
    if (empty($signatures)) {
      array_push($signatures, $creator_signature);
    }
    else {
      array_splice($signatures, 0, 1, array_merge(array($creator_signature), array($signatures[0])));
    }
  }  
  
  // If this user has signed it and we need to display their signature ( page = 1 ), add their signature
  if ($has_user_signed && $page == 1) {
    array_splice($signatures, 0, 1, array_merge(array($signatures[0]), array($user_signature)));
  }
  
  // Set some themeing variables that will be available to the template for displaying the results.
  for ($i=0; $i<sizeof($signatures); $i++) {
    if ($signatures[$i]['uid'] == $petition_uid) {
      $signatures[$i]['is_creator'] = TRUE;
    }
    if ($signatures[$i]['uid'] == $user->uid) {
      $signatures[$i]['is_user'] = TRUE;
    }    
  
    $signatures[$i]['comment'] = check_plain($signatures[$i]['comment']);  
    $signatures[$i]['number'] = wh_petitions_format_number($signatures[$i]['number']);
      
    $name = '';
    if (!empty($signatures[$i]['user']['first_name'])) {
      $name = $signatures[$i]['user']['first_name'];
    }
    if (!empty($signatures[$i]['user']['last_name'])) {
      $last_name = trim($signatures[$i]['user']['last_name']);
      $last_initial = mb_substr($last_name, 0, 1);
      if (!empty($name)) {
        $name .= ' ' . $last_initial;
      }
      else {
        $name = $last_initial;
      }
    }
    if (empty($name)) {
      $name = $signatures[$i]['user']['username'];
    }
      
    $location = '';
    if (!empty($signatures[$i]['user']['city'])) {
      $location = $signatures[$i]['user']['city'];
    }
    if (!empty($signatures[$i]['user']['state'])) {
      if (!empty($name)) {
        $location .= ', ' . $signatures[$i]['user']['state'];
      }
      else {
        $location = $signatures[$i]['user']['state'];
      }
    }
    $signatures[$i]['location'] = check_plain($location);
    $signatures[$i]['full_name'] = check_plain($name);  
  }

  return $signatures;
}

/**
 * Function to display more signatures from an ajax request
 */
function wh_petitions_more_signatures($petition_id, $page, $last_id) {
  // Make sure we got a petition id
  if (empty($petition_id)) {
    print '';
  }
  
  // Determine what page we're on
  if (empty($page)) {
    $page = 1;
  }
  else {
    $page = intval($page);
  }
  
  // Load the petition
  $conn = wh_petitions_mongo_petition_connection();
  $petition = array();
  $petition = wh_petitions_load_slim_petition($conn, $petition_id, array('uid', 'signature_count'));
  
  // If it wasn't a valid petition id, exit
  if (empty($petition)) {
    print '';
  }

  $has_more = TRUE;
  $signatures = wh_petitions_get_signatures($petition_id, $petition['uid'], $page, $has_more, TRUE, $petition['signature_count'], $last_id);
  
  // Set the last id that we found in the signature list for this page so we can pass that on to the next pagination query
  $last_id = '';
  if (!empty($signatures)) {
    $last_id = $signatures[(sizeof($signatures) -1)]['_id']->__toString();
  }  
  
  $signature_html = theme('wh_petitions_display_signatures', array(
    'signatures' => $signatures, 
    'petition_id' => check_plain($petition_id), 
    'page' => $page + 1, 
    'has_more' => $has_more, 
    'count' => sizeof($signatures), 
    'total' => wh_petitions_format_number($petition['signature_count']),
    'from_ajax' => TRUE,
    'sigs_per_page' => 20,
    'last_id' => $last_id,
    'nice_url' => $petition['nice_url'],
  ));
  
  $data = array('markup' => $signature_html);
  if (!empty($_GET['callback'])) {
    $callback = check_plain($_GET['callback']);
    print $callback . '(' . json_encode($data) . ')';
  }
  else {
    print json_encode($data);
  }
}

/*********************** MISC PAGES ****************************/

/**
 * Function to display the user dashboard
 *
 * Displays:
 * -Your saved petitions (all drafts)
 * -Petitions you've created
 * -Petitions you've signed ( in which you're not the creator also )
 */
function wh_petitions_user_dashboard() {
  global $user;

  // Make sure only the current logged-in user can see their own dashboard
  if (empty($user->uid)) {
    drupal_goto('user/login');
  }
  
  drupal_set_title(t('My Petitions'));
  
  // Get this user's draft petitions, sort by the id ( descending in order created )
  $conn = wh_petitions_mongo_petition_connection(); 
  $query = array(
    'uid' => (int)$user->uid,
    'petition_status' => (int)WH_PETITION_STATUS_DRAFT,
  );
  $result = $conn->find($query, array('title', 'signature_count', 'nice_url'));
  
  // Delete Modal
  $draft_html = '';
  $count = 0;
  module_load_include('inc', 'wh_petitions', 'wh_petitions.form');
  $delete_text = variable_get('wh_petitions_delete_modal_text', '');
  foreach ($result as $res) {  
    $petition_status = WH_PETITION_STATUS_DRAFT; 
    $delete_form = drupal_get_form('wh_petitions_delete_petition_form', $res['_id']->__toString());
    $delete_form = drupal_render($delete_form);    
    
    $draft_html .= theme('wh_petitions_display_list_petition', array(
      'title' => check_plain($res['title']), 
      'signature_count' => wh_petitions_format_number($res['signature_count']), 
      'nice_url' => '',
      'petition_link' => l(t('Finish'), 'petition/create/' . check_plain($res['_id']->__toString()), array('attributes' => array('class' => 'finish'))), 
      'cols' => 2, 
      'status' => $petition_status, 
      'delete_link' => TRUE, 
      'petition_id' => check_plain($res['_id']->__toString()),
      'entry_num' => $count,
      'dashboard' => TRUE,
      'draft' => TRUE,
      'delete_form' => $delete_form,
      'delete_text' => $delete_text,
    ));
    $count++;
  }
  
  // Get the petitions this user has created
  $created_html = '';
  $query = array(
    'uid' => (int)$user->uid,
    'petition_status' => array('$gt' => (int)WH_PETITION_STATUS_DRAFT),
  );
  $sort = array(
    'published' => -1,
  );
  $result = $conn->find($query, array('petition_status', 'title', 'signature_count', 'nice_url'))->sort($sort);

  $count = 0;
  foreach ($result as $res) {
    $petition_status = '';
    if (in_array($res['petition_status'], array(WH_PETITION_STATUS_FLAGGED, WH_PETITION_STATUS_REVIEWED, WH_PETITION_STATUS_CLOSED))) {
      switch ($res['petition_status']) {
        case WH_PETITION_STATUS_FLAGGED:
          $petition_status = 'removed';
          break;
        case WH_PETITION_STATUS_REVIEWED:
          $petition_status = 'response';
          break; 
        case WH_PETITION_STATUS_CLOSED:
          $petition_status = 'archived';
          break;                
      }
    }

    $created_html .= theme('wh_petitions_display_list_petition', array(
      'title' => check_plain($res['title']), 
      'signature_count' => wh_petitions_format_number($res['signature_count']), 
      'nice_url' => $res['nice_url'],
      'petition_link' => l(t('View'), $res['nice_url'], array('attributes' => array('class' => 'view'))), 
      'cols' => 2, 
      'petition_status' => $petition_status, 
      'petition_id' => check_plain($res['_id']->__toString()),
      'entry_num' => $count,
      'dashboard' => TRUE,
    ));
    $count++;
  }
  
  // Get the petitions this user has signed ( but ones they're not a creator for, since creators automatically sign their petitions )
  $sig_conn = wh_petitions_mongo_petition_signatures_connection();
  $mongo_array = array();
  $result = $sig_conn->find(array('uid' => (int)$user->uid), array('petition_id'));
  foreach ($result as $res) {
    array_push($mongo_array, new MongoId($res['petition_id']));
  }

  $signed_html = '';  
  $query = array(
    'uid' => array('$ne' => (int)$user->uid),
    '_id' => array('$in' => $mongo_array),
    'petition_status' => array('$gt' => (int)WH_PETITION_STATUS_DRAFT),
  );
  $result = $conn->find($query, array('petition_status', 'title', 'signature_count', 'nice_url'));
  
  $count = 0;
  foreach ($result as $res) {
    $petition_status = '';
    if (in_array($res['petition_status'], array(WH_PETITION_STATUS_FLAGGED, WH_PETITION_STATUS_REVIEWED, WH_PETITION_STATUS_CLOSED))) {
      switch ($res['petition_status']) {
        case WH_PETITION_STATUS_FLAGGED:
          $petition_status = 'removed';
          break;
        case WH_PETITION_STATUS_REVIEWED:
          $petition_status = 'response';
          break; 
        case WH_PETITION_STATUS_CLOSED:
          $petition_status = 'archived';
          break;                
      }
    }

    $signed_html .= theme('wh_petitions_display_list_petition', array(
      'title' => check_plain($res['title']), 
      'signature_count' => wh_petitions_format_number($res['signature_count']), 
      'nice_url' => $res['nice_url'],
      'petition_link' => l(t('View'), $res['nice_url'], array('attributes' => array('class' => 'view'))), 
      'cols' => 2, 
      'petition_status' => $petition_status, 
      'petition_id' => check_plain($res['_id']->__toString()),
      'entry_num' => $count,
      'dashboard' => TRUE,
    ));
    $count++;
  }  
  
  $username = '';
  $user_load = user_load($user->uid);
  if (!empty($user_load->profile_first_name)) {
    $username = $user_load->profile_first_name;
  }
  if (empty($username)) {
    $username = $user->name;
  } 

  return theme('wh_petitions_user_dashboard', array(
    'username' => check_plain($username), 
    'created' => $created_html, 
    'signed' => $signed_html, 
    'draft' => $draft_html
  ));
}

/**
 * Callback to display the deleted petition page.  Users only get sent here when they delete their petition on step 4 of the create a petition process.
 */
function wh_petitions_deleted_page() {
  global $user;
  if (!empty($user->uid)) {
    drupal_set_title(t('Deleted Petition'));
    return theme('wh_petitions_page_deleted', array('delete_text' => variable_get('wh_petitions_delete_page_text', ''), 'uid' => $user->uid));
  }
  else {
    return '';
  }
}

/**
 * Callback to display the email a petition iframe.  Needs to be in an iframe otherwise the forward form and captcha fail if we bring this form over via the normal json.
 */
function wh_petitions_email_petition($petition_id) {
  $conn = wh_petitions_mongo_petition_connection(); 
  $petition = wh_petitions_load_slim_petition($conn, $petition_id);
  if (!empty($petition)) {
    global $base_url;
    drupal_set_title(t('Email A Petition')); 
    /**
     * @todo - Ben - We could probably switch this to use url() function now that the hostname isn't being modified.
     */
    $url = '/petition-tool/petition/email_frame/' . check_plain($petition_id);
    return '<iframe src="' . $url . '?t=' . microtime() . '" width="687" height="1000" frameborder="0"></iframe>';
  } 
  return '';
}
 
/**
 * Callback to display the email response form in the iframe from above.
 */
function wh_petitions_email_petition_frame($petition_id) {
  $styles = drupal_get_css();
  $scripts = drupal_get_js();
  $conn = wh_petitions_mongo_petition_connection(); 
  $petition = wh_petitions_load_slim_petition($conn, $petition_id);
  if (!empty($petition)) {
    $form_html = drupal_render(drupal_get_form('forward_form', 'petition/detail/' . check_plain($petition_id), $petition['title'], TRUE));
    $messages = theme('status_messages');    
  
    $html = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
    <html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" dir="ltr">
    <head><title>' . t('Email A Petition') . '</title>
    ' . $styles . $scripts . '
    <style type="text/css">
      #petition-inner { padding:0px; }
    </style>
    </head>
    <body><div id="petition-inner">
    ' . $messages . '
    <div id="email-page"><div class="email-petition-page"><h2 class="page-title">' . t('Email This Petition') . '</h2>' . $form_html . '</div></div></div></body>
    </html>';
    print $html;
    exit;
  }
  print '';
  exit;
}

/**
 * Callback to display the email response thank you page in the iframe and link out of the iframe.
 *
 * @todo - Ben - This feature doesn't exist. Need to test the nice url if it's implemented.
 */
function wh_petitions_email_petition_thank_you($petition_id) {
  $styles = drupal_get_css();
  $scripts = drupal_get_js();
  $conn = wh_petitions_mongo_petition_connection(); 
  $petition = wh_petitions_load_slim_petition($conn, $petition_id);  
  if (!empty($petition)) {
    $messages = theme('status_messages');    
    $nice_url = $petition['nice_url'];
    global $base_url;
    $html = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
    <html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" dir="ltr">
    <head><title>' . t('Email A Petition') . '</title>
    ' . $styles . $scripts . '
    <style type="text/css">
      #petition-inner { padding:0px; }
    </style>
    </head>
    <body><div id="petition-inner">
    ' . $messages . '
    <div id="email-page"><h2 class="page-title">' . t('Email This Petition') . '</h2>' . l(t('Click here to return to the petition'), $nice_url, array('attributes' => array('target' => '_top', 'class' => array('no-follow')))) . '</div></body>
    </html>';
    print $html;
    exit;
  }
  print '';
  exit;
}
