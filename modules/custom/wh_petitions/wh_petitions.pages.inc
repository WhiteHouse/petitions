<?php
/**
 * @file
 * This file contains functionality for displaying petitions.
 */

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

  // Make sure the sort is a valid option.
  if (!in_array($sort, array('all', 'trending', 'popular'))) {
    $sort = 'all';
  }

  // Validate the page and calculate the offset for the query.
  $page = intval($page);
  if ($page <= 0) {
    $page = 1;
  }
  $offset = ($page - 1) * WH_PETITION_PETITIONS_PER_PAGE;

  // 1 Col or 2
  if ($cols != 1 && $cols != 2) {
    $cols = 2;
  }

  // Issues.
  if (!empty($issues)) {
    $issues = explode(" ", urldecode($issues));
    for ($i = 0; $i < count($issues); $i++) {
      $issues[$i] = intval($issues[$i]);
    }
  }

  // Search.
  $search = strtolower(strip_tags(trim(urldecode($search))));

  // Retrieve only public, under_review petitions not hidden by admins.
  // $realtime boolean of FALSE sets up the query for Solr, instead of MySQL.
  // IMPORTANT: $realtime is NOT tied to an empty $search. This is only
  // coincidental and avoids confusing negative-negatives.
  $realtime = empty($search);
  $petitions_query = PetitionsSelectQueryFactory::create($realtime);
  $petitions_query->setIsOpen(TRUE);

  // Retrieve petitions with at least one of the issue tid's
  if (!empty($issues)) {
    $petitions_query->setIssueIDs($issues);
  }

  // Compose the sort options.
  if ($sort == 'all') {
    $petitions_query->setOrderBy(PetitionsSelectQuery::SELECT_QUERY_ORDER_BY_FIELD_DATE_REACHED_PUBLIC, PetitionsSelectQuery::SELECT_QUERY_ORDER_BY_DESC);
    drupal_set_title(t('All Petitions'));
  }
  elseif ($sort == 'popular') {
    $petitions_query->setOrderBy(PetitionsSelectQuery::SELECT_QUERY_ORDER_BY_FIELD_SIGNATURE_COUNT, PetitionsSelectQuery::SELECT_QUERY_ORDER_BY_DESC);
    drupal_set_title(t('Popular Petitions'));
  }
  elseif ($sort == 'trending') {
    drupal_set_title(t('Trending Petitions'));
  }

  // If we're searching by text, we should be displaying results that match the title first, then results that match the body.
  // To accomplish this, we do two queries, one matching title and one matching description
  // Based on the counts from each and the page we're on, we can figure out which query(s) to run and which results to get.

  if (!empty($search)) {
    if (petitions_data_mongo_reads_are_enabled()) {
      $conn = wh_petitions_mongo_petition_connection();
      $keywords = explode(' ', $search);

      // Fields to retrieve.
      $retrieve_fields = array(
        'title',
        'signature_count',
        'reached_public',
        'nice_url',
      );

      // Retrieve petitions that have at least one of the same keywords from the search in their titles.
      $query = array(
        'petition_status' => array(
          '$in' => array((int) WH_PETITION_STATUS_PUBLIC, (int) WH_PETITION_STATUS_UNDER_REVIEW),
        ),
        'hidden' => 0,
        'title_keywords' => array('$in' => $keywords),
      );

      // Retrieve petitions with at least one of the issue tid's
      if (!empty($issues)) {
        $query['issues'] = array('$in' => $issues);
      }

      $sort_query = array();
      if ($sort == 'all') {
        $sort_query = array(
          'reached_public' => -1,
        );
      }
      elseif ($sort == 'popular') {
        $sort_query = array(
          'signature_count' => -1,
        );
      }

      // Find how many match titles.
      $title_total = $conn->find($query)->sort($sort_query)->count();

      // Find how many match descriptions.
      $body_query = $query;
      $body_query['body_keywords'] = $body_query['title_keywords'];
      $body_query['title_keywords'] = array('$nin' => $keywords);
      $body_total = $conn->find($body_query)->sort($sort_query)->count();

      // Combine the results.
      $total = $title_total + $body_total;

      if ($total > 0) {
        // If we're not past the count of titles, query petitions based on title.
        if ($offset < $title_total) {
          $result = $conn->find($query, $retrieve_fields)
            ->sort($sort_query)
            ->skip($offset)
            ->limit(WH_PETITION_PETITIONS_PER_PAGE);

          foreach ($result as $res) {
            $res['id'] = $res['_id']->__toString();
            $res['url'] = $res['nice_url'];
            $res['signatureCount'] = $res['signature_count'];
            array_push($petitions, $res);
            $count++;
          }
        }

        // Query descriptions if we didn't find 8 (WH_PETITION_PETITIONS_PER_PAGE) results from title search ( either ran out of results or the offset is past the
        // result count for titles )
        if ($count < WH_PETITION_PETITIONS_PER_PAGE) {
          $limit = WH_PETITION_PETITIONS_PER_PAGE - $count;
          $body_offset = $offset - $title_total;
          if ($body_offset < 0) {
            $body_offset = 0;
          }
          $body_result = $conn->find($body_query, $retrieve_fields)->sort($sort_query)->skip($body_offset)->limit($limit);

          foreach ($body_result as $res) {
            $res['id'] = $res['_id']->__toString();
            $res['url'] = $res['nice_url'];
            $res['signatureCount'] = $res['signature_count'];
            array_push($petitions, $res);
            $count++;
          }
        }
      }
    }
    else {
      // Perform MySQL Title search only.
      if (strlen($search) < 3) {
        return '<p class="no-results">' . t('Please enter at least 3 characters to search by.') . '</p>';
      }

      $petitions_query->setTitle($search);

      // Find how many match titles.
      $petitions_execute = $petitions_query->setLimit(WH_PETITION_PETITIONS_PER_PAGE)->setOffset($offset)->execute();
      $title_total_results = $petitions_execute->getCount();
      $total = $title_total_results;

      if ($offset < $title_total_results) {
        $result = $petitions_execute->getResult();

        foreach ($result as $res) {
          array_push($petitions, $res);
          $count++;
        }
      }
    }

    if (!$total) {
      return '<p class="no-results">' . t('There are no petitions that match this search.') . '</p>';
    }
  }
  else {
    $result = NULL;
    $total = 0;

    // For Trending sort, we have a helper function to do this for us since this logic is used in multiple places.
    if ($sort == 'trending') {
      if (petitions_data_mongo_reads_are_enabled()) {
        $conn = wh_petitions_mongo_petition_connection();
        $result = wh_petitions_trending_results($conn, $offset, WH_PETITION_PETITIONS_PER_PAGE, $total);
      }
    }
    // Otherwise, proceed normally.
    else {
      $petitions_execute = $petitions_query->setLimit(WH_PETITION_PETITIONS_PER_PAGE)->setOffset($offset)->execute();
      $result = $petitions_execute->getResult();
      $total = $petitions_execute->getCount();
    }

    if ($result) {
      foreach ($result as $res) {
        array_push($petitions, $res);
        $count++;
      }
    }
  }

  // We want something to occupy the place in the url in case there are no issues.
  if (empty($issues)) {
    $issues = array(0);
  }

  // Theme the petitions.
  $i = 0;
  foreach ($petitions as $res) {
    $html .= theme('wh_petitions_display_list_petition', array(
      'entry_num' => $i,
      'title' => filter_xss($res['title']),
      'signature_count' => wh_petitions_format_number($res['signatureCount']),
      'petition_link' => l(t('Find out more'), $res['url']),
      'cols' => intval($cols),
      'petition_id' => check_plain($res['id']),
      'nice_url' => $res['url'],
    ));
    $i++;
  }

  if (!empty($issues) && empty($html) && $issues[0] != 0) {
    if (count($issues) == 1) {
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

    if (petitions_data_mongo_reads_are_enabled()) {
      $query = array(
        '_id' => array('$in' => $id_slice),
      );
      $retrieve_fields = array('title', 'signature_count', 'published', 'nice_url');
      $result = $conn->find($query, $retrieve_fields);
      $arr = array();
      foreach ($id_slice as $key => $val) {
        foreach ($result as $res) {
          if ($res['_id'] == $val) {
            array_push($arr, $res);
          }
        }
      }

    }
    else {
      $query = db_select('node', 'n');
      $query->leftJoin('field_data_field_petition_signature_count', 'psc', 'psc.entity_id = n.nid');
      $query->leftJoin('field_data_field_timestamp_published', 'tp', 'tp.entity_id = n.nid');
      $query->leftJoin('url_alias', 'ua', 'substring(ua.source, 6) = n.nid');

      $query->addField('n', 'title');
      $query->addField('psc', 'field_petition_signature_count_value', 'signature_count');
      $query->addfield('tp', 'field_timestamp_published_value', 'published');
      $query->addField('ua', 'alias', 'nice_url');

      $query->condition('n.type', 'petition', '=');
      $query->condition('n.nid', $id_slice, 'IN');
      $result = $query->execute()->fetchAllAssoc('n.nid');

      $arr = array();
      foreach ($id_slice as $key => $val) {
        foreach ($result as $res) {
          if ($res['_id'] == $val) {
            array_push($arr, $res);
          }
        }
      }
    }
  }

  $total = count($ids);

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
  $login_required = (variable_get('petitions_signing_method', WH_PETITIONS_SIGNING_METHOD_REGISTRATION_REQUIRED) == WH_PETITIONS_SIGNING_METHOD_REGISTRATION_REQUIRED) ? TRUE : FALSE;

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
    for ($i = 0; $i < sizeof($petition['issues']); $i++) {
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
  $published = $petition['published'];
  $end_date = strtotime('+' . $petition['review_timeframe'] . ' days', $published);
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

    if (petitions_data_mysql_writes_are_enabled() && function_exists("petitionevents_get_administer_link")) {
      $admin_link = petitionevents_get_administer_link();
    }

    else {
      $admin_link = l(t('Administer This Petition'), 'admin/petitions/' . check_plain($petition_id), array('attributes' => array('classes' => array('no-follow'))));
    }
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
    'published' => date("M d, Y", $published),
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
      'uid' => (int) $user->uid,
      'petition_id' => (string) $petition_id,
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
  // Do not display a form if user already signed it.
  if ($already_signed) {
    $signature_form = '';
  }
  // Petition hasn't been signed.
  else {
    // Check if login is required or if simplified login is on.
    switch ($login_required) {
      case TRUE:
        // Prepare a button that will appear grayed out if the user is not logged in and the login/registration is required to sign.
        if (!$logged_in) {
          $signature_form = l('Sign This Petition', 'user/login', array('attributes' => array('id' => 'button-sign-this-petition')));
          $why_text = variable_get('wh_petitions_tooltip_why', '');
        }
        // Display a direct sign button if user is authenticated and login is required to sign the petition.
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
        break;
      case FALSE:
      default:
        // Regardless of user authentication, display a simplied signing form if chosen in settings.
        // Both anonymous and authenticated users will be shown the same form.
        // $signature_form = drupal_get_form('petitionssignatureform_signature_form', check_plain($petition_id));
        // $signature_form = drupal_render($signature_form);

        // Hide form in favor of displaying a side rail block version.
        $signature_form = '';
        break;
    }
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
    if (is_int((int) $get_page)) {
      $page = intval($get_page);
      $sigs_per_page = 19;
      $last_id = preg_replace('/[^a-zA-Z0-9]/', '', $_GET['last']);
    }
  }
  $signatures = wh_petitions_get_signatures($petition_id, $petition['uid'], $page, $has_more, FALSE, $petition['signature_count'], $last_id);

  $last_id = '';
  $previously_found_creator = '0';
  if (!empty($signatures)) {
    $last_id = $signatures[(count($signatures) -1)]['id'];
    $previously_found_creator = $signatures[(count($signatures) -1)]['previously_found_creator'];
  }

  $signature_html = theme('wh_petitions_display_signatures', array(
    'signatures' => $signatures,
    'petition_id' => check_plain($petition_id),
    'page' => $page,
    'has_more' => $has_more,
    'sigs_per_page' => $sigs_per_page,
    'last_id' => $last_id,
    'nice_url' => $petition['nice_url'],
    'previously_found_creator' => $previously_found_creator,
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
    'login_required' => $login_required,
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
  $nice_url = check_plain($_GET['q']);
  if (!petitions_data_mongo_reads_are_enabled()) {
    $nid = '';
    // Find corresponding petition node by legacy path.
    $result = db_query('select entity_id from {field_data_field_legacy_path} WHERE field_legacy_path_value = :nice_url AND bundle=\'petition\'', array(':nice_url' => $nice_url));
    $nid = $result->fetchColumn(0);
    // Redirect to corresponding petition node, if found.
    if (!empty($nid)) {
      $petition_node_path = 'node/' . $nid;
      drupal_goto($petition_node_path, array(), 301);
    }
    else {
      // If no node petition found and mongo reads are disabled, return not found.
      drupal_not_found();
    }
    return;
  }

  $conn = wh_petitions_mongo_petition_connection();
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
 * previously_found_creator = whether the creator's signature has been found in previous query results.
 */
function wh_petitions_get_signatures($petition_id, $petition_uid, $page, &$has_more, $from_ajax = FALSE, $signature_count = 0, $last_id, $previously_found_creator = FALSE) {

  global $user;
  $signatures = array();
  if (empty($page)) {
    $page = 1;
  }
  if ($from_ajax && $page != 1) {
    if ($previously_found_creator) {
      $per_page = 20;
    }
    else {
      // The creator's signature is always displayed first.
      // Get one extra signature in case we also find the creator's
      // signature in the following results.
      $per_page = 21;
    }
  }
  else {
    $per_page = 19;
  }

  // Get the signatures we should be retrieving
  if ($page == 1) {
    $skip = 0;
    $max = 19;
    $count_offset = 1;
  }
  elseif ($previously_found_creator) {
    $skip = 19 + (($page - 2) * $per_page);
    $max = $skip + $per_page;
    $count_offset = 1;
  }
  else {
    $skip = 18 + (($page - 2) * ($per_page - 1));
    $max = $skip + $per_page;
    $count_offset = 0;
  }

  // If we're on the first page, we want to get the creator's signature to display first.
  if ($page == 1) {
    $creator_signature = SignaturesSelectQueryFactory::create()
      ->setPetitionId((string)$petition_id)
      ->setUid((int)$petition_uid)
      ->setLimit(1)
      ->execute()
      ->getResultObjects();
  }

  // Use non-realtime Solr to find signatures excluding the creator's.
  $signatures = SignaturesSelectQueryFactory::create(FALSE)
    ->setPetitionId((string)$petition_id);

  // Sorting signatures in MySQL is too expensive, so only sort if signatures are served by Solr.
  if (petitions_data_solr_signature_queries_are_enabled()) {
    $signatures = $signatures
      ->setOrderBy(SignaturesSelectQuery::SELECT_QUERY_ORDER_BY_FIELD_TIMESTAMP, SignaturesSelectQuery::SELECT_QUERY_ORDER_BY_DESC);
  }
  $signatures = $signatures->setLimit(($per_page));

  // If we just got a page number, not a last id, do the query anyway using skip
  if ($page != 1) {
    $signatures->setOffset($skip);
  }
  $signatures = $signatures->execute()
                  ->getResultObjects();

  if (empty($signatures) && empty($creator_signature)) {
    $has_more = FALSE;
    return FALSE;
  }

  // Check if we selected the creator's signature again.
  // If so, we will remove it so it is not displayed twice.
  if (!$previously_found_creator) {
    // Look for creator's signature in $signatures
    $creator_signature_found = FALSE;
    foreach ($signatures as $key => $signature) {
      if((int) $signature->getUid() == (int) $petition_uid) {
        $creator_signature_found = TRUE;
        break;
      }
    }
    if ($creator_signature_found) {
      // Remove the creator from retrieved signatures so we don't display it twice.
      unset($signatures[$key]);
      // Following queries don't need to worry about removing
      // creator from results.
      $previously_found_creator = TRUE;
    }
    // If we have still not found it, but have selected more
    // signatures than we can display, then remove a signature
    // from the array.
    elseif ((($page != 1) && count($signatures) == 21) || (($page == 1) && count($signatures) == 19)) {
      // Remove the last signature because we have 1 too many.
      array_pop($signatures);
    }
  }

  // If we have a creator's signature, make it the first one.
  if (($page == 1) && !empty($creator_signature[0])) {
    array_unshift($signatures, $creator_signature[0]);
  }

  $result = array();
  $i = 0;
  // Put the results into an array
  foreach ($signatures as $signature) {
    if (empty($signature)) {
      continue;
    }
    $result[$i] = array(
      'id' => $signature->getId(),
      'petition_id' => $signature->getPetitionId(),
      'uid' => $signature->getUid(),
      'user' => array(
        'first_name' => $signature->getFirstName(),
        'last_name' => $signature->getLastName(),
        'username' => $signature->getUser()->name,
        'country' => $signature->getUserCountry(),
        ),
      'timestamp' => $signature->getCreated(),
      );

    $result[$i]['user']['city'] = $signature->getCity();
    $result[$i]['user']['state'] = $signature->getState();
    $result[$i]['user']['zip'] = $signature->getZip();
    $i++;
  }

  // Determine if there are more signatures to display after these
  if ($max >= $signature_count) {
    $has_more = FALSE;
  }

  // Set some themeing variables that will be available to the template for displaying the results.
  $result_count = count($result);
  for ($i = 0; $i < $result_count; $i++) {
    $signature_number = $signature_count - $i - $skip + $count_offset;
    $result[$i]['number'] = wh_petitions_format_number($signature_number);

    if ($result[$i]['uid'] == $petition_uid) {
      $result[$i]['is_creator'] = TRUE;
      $result[$i]['number'] = wh_petitions_format_number(1);
    }
    if ($result[$i]['uid'] == $user->uid) {
      $result[$i]['is_user'] = TRUE;
    }

    $first_name = (!empty($result[$i]['user']['first_name'])) ? $result[$i]['user']['first_name'] : '';
    $last_name = (!empty($result[$i]['user']['last_name'])) ? $result[$i]['user']['last_name'] : '';
    $name = petitions_signatures_display_style($first_name, $last_name);

    $zipcode = (!empty($result[$i]['user']['zip'])) ? $result[$i]['user']['zip'] : '';
    $city = (!empty($result[$i]['user']['city'])) ? $result[$i]['user']['city'] : '';
    $state = (!empty($result[$i]['user']['state'])) ? $result[$i]['user']['state'] : '';
    $location = petitions_signatures_display_location_formatted($zipcode, $city, $state);

    if ($location) {
      $result[$i]['location'] = check_plain($location);
    }
    $result[$i]['full_name'] = check_plain($name);
  }
  if ($previously_found_creator) {
    $result[(count($result) -1)]['previously_found_creator'] = '1';
  }
  else {
    $result[(count($result) -1)]['previously_found_creator'] = '0';
  }

  return $result;
}

/**
 * Function to display more signatures from an ajax request
 */
function wh_petitions_more_signatures($petition_id, $page, $last_id, $previously_found_creator) {
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

  $petition = PetitionsController::loadObject($petition_id);

  // If it wasn't a valid petition id, exit
  if (empty($petition)) {
    print '';
  }

  $has_more = TRUE;
  $signatures = wh_petitions_get_signatures($petition_id, $petition->getUid(), $page, $has_more, TRUE, $petition->getSignatureCount(), $last_id, (bool) $previously_found_creator);

  // Set the last id that we found in the signature list for this page so we can pass that on to the next pagination query
  $last_id = '';
  $previously_found_creator = '0';
  if (!empty($signatures)) {
    $last_id = $signatures[(count($signatures) -1)]['id'];
    $previously_found_creator = $signatures[(count($signatures) -1)]['previously_found_creator'];
  }

  $signature_html = theme('wh_petitions_display_signatures', array(
    'signatures' => $signatures,
    'petition_id' => check_plain($petition_id),
    'page' => $page + 1,
    'has_more' => $has_more,
    'count' => sizeof($signatures),
    'total' => wh_petitions_format_number($petition->getSignatureCount()),
    'from_ajax' => TRUE,
    'sigs_per_page' => 20,
    'last_id' => $last_id,
    'nice_url' => $petition->getNiceUrl(),
    'previously_found_creator' => $previously_found_creator,
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
  $query = PetitionsSelectQueryFactory::create();
  $query->setUid($user->uid);
  //@todo Set order by published desc
  $result = $query->execute()->getResultObjects();

  $draft_html = '';
  $created_html = '';
  $signed_html = '';
  $draft_petitions_count = 0;
  $finished_petitions_count = 0;
  $signed_petitions_count = 0;

  module_load_include('inc', 'wh_petitions', 'wh_petitions.form');
  $delete_text = variable_get('wh_petitions_delete_modal_text', '');

  foreach ($result as $res) {
    if($res->getStatus() == WH_PETITION_STATUS_DRAFT) {
      // Delete Modal for drafts.
      $petition_status = WH_PETITION_STATUS_DRAFT;
      $delete_form = drupal_get_form('wh_petitions_delete_petition_form', $res->getId());
      $delete_form = drupal_render($delete_form);

      $draft_html .= theme('wh_petitions_display_list_petition', array(
        'title' => check_plain($res->getTitle()),
        'signature_count' => wh_petitions_format_number($res->getSignatureCount()),
        'nice_url' => '',
        'petition_link' => l(t('Finish'), 'petition/create/' . check_plain($res->getId()), array('attributes' => array('class' => 'finish'))),
        'cols' => 2,
        'status' => $petition_status,
        'delete_link' => TRUE,
        'petition_id' => check_plain($res->getId()),
        'entry_num' => $draft_petitions_count++,
        'dashboard' => TRUE,
        'draft' => TRUE,
        'delete_form' => $delete_form,
        'delete_text' => $delete_text,
      ));
    }
    else {
      // Petition is not in draft status.
      $petition_status = '';

      switch ($res->getStatus()) {
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

      $created_html .= theme('wh_petitions_display_list_petition', array(
        'title' => check_plain($res->getTitle()),
        'signature_count' => wh_petitions_format_number($res->getSignatureCount()),
        'nice_url' => $res->getNiceUrl(),
        'petition_link' => l(t('View'), $res->getNiceUrl(), array('attributes' => array('class' => 'view'))),
        'cols' => 2,
        'petition_status' => $petition_status,
        'petition_id' => check_plain($res->getId()),
        'entry_num' => $finished_petitions_count++,
        'dashboard' => TRUE,
      ));
    }
  }

  // Get the petitions this user has signed.
  $sig_query = SignaturesSelectQueryFactory::create();
  $sig_query->setUid($user->uid);
  $result = $sig_query->execute()->getResultObjects();
  $petition_ids = array();

  if($result) {
    foreach ($result as $res) {
      $petition_ids[] = $res->getPetitionId(FALSE);
    }

    $petition_query = PetitionsSelectQueryFactory::create();
    $petition_query->setPetitionIds($petition_ids);
    $petition_result = $petition_query->execute()->getResultObjects();

    foreach ($petition_result as $res) {
      // Skip any petitions that are draft and that that aren't created by this user.
      if (($res->getUid() != (int) $user->uid) && ($res->getStatus() != (int) WH_PETITION_STATUS_DRAFT)) {
        $petition_status = '';

        switch ($res->getStatus()) {
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
        $signed_html .= theme('wh_petitions_display_list_petition', array(
          'title' => check_plain($res->getTitle()),
          'signature_count' => wh_petitions_format_number($res->getSignatureCount()),
          'nice_url' => $res->getNiceUrl(),
          'petition_link' => l(t('View'), $res->getNiceUrl(), array('attributes' => array('class' => 'view'))),
          'cols' => 2,
          'petition_status' => $petition_status,
          'petition_id' => check_plain($res->getId()),
          'entry_num' => $signed_petitions_count++,
          'dashboard' => TRUE,
        ));
      }
    }
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
