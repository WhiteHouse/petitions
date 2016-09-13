<?php
/**
 * @file
 * This file contains functionality for displaying petitions.
 */

/*********************** PETITION LIST ****************************/

/**
 * Clears responses and resets the form.
 */
function wh_petitions_clear_responses_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $form_state['flag'] = 1;
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
  if (!in_array($sort, array('all', 'popular'))) {
    $sort = 'popular';
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

  if (!empty($search)) {
    // Check if search term matches an issue.
    $vocabulary = taxonomy_vocabulary_machine_name_load('issues');
    $terms = entity_load('taxonomy_term', FALSE, array('vid' => $vocabulary->vid));
    foreach ($terms as $term) {
      if (strtolower($search) == strtolower($term->name)) {
        $issues = array($term->tid);
        break;
      }
    }
    if ($issues) {
      // Perform search on both title and issue names.
      $petitions_query->setPerformTitleIssueQuery(TRUE);
    }
  }

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
      // Force order by signature count desc for search queries.
      $petitions_query->setOrderBy(PetitionsSelectQuery::SELECT_QUERY_ORDER_BY_FIELD_SIGNATURE_COUNT, PetitionsSelectQuery::SELECT_QUERY_ORDER_BY_DESC);

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
      if (!empty($issues)) {
        // Force order by signature count desc for issue queries.
        $petitions_query->setOrderBy(PetitionsSelectQuery::SELECT_QUERY_ORDER_BY_FIELD_SIGNATURE_COUNT, PetitionsSelectQuery::SELECT_QUERY_ORDER_BY_DESC);
      }
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

/*********************** MISC PAGES ****************************/


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
