<?php

/**
 * Creates test petitions.
 * -num: number of petitions to create
 * -uid: specific uid to create them for ( default - rand )
 * -status: default petition status to create them with ( default - rand )
 */
function create_petitions($num, $uid='', $status='') {
  $conn = wh_petitions_mongo_petition_connection();
  $vocab = taxonomy_vocabulary_machine_name_load('issues');
  $issues = array();
  if (!empty($vocab)) {
    $issues_list = taxonomy_get_tree($vocab->vid);
    foreach ($issues_list as $issue) {
      array_push($issues, $issue->tid);
    }
  }
  
  for ($i=0;$i<$num;$i++) {
    // Published, Reached Public dates
    $days = rand(1, 40);
    $published = strtotime("- " . $days . " days");
    $to_public = rand(1, 29);
    $public = strtotime("+ " . $to_public . " days", $published);
  
    // Issues, between 1 and 3
    $issue_arr = array();
    $issue_num = rand(1, 3);
    $issue_size = sizeof($issues);
    for ($j=0; $j<$issue_num; $j++) {
      $key = rand(0, ($issue_size - 1));
      array_push($issue_arr, (int)$issues[$key]);
    }
    
    $title = create_title(rand(1, 10));
    $arr = explode(" ", $title);
    $words = array();
    foreach ($arr as $word) { 
      $word = preg_replace('/[^A-Za-z0-9]/', '', $word);
      array_push($words, trim($word));
    }
    $title_keys = $words;
    $body = create_title(rand(1, 100));
    $arr = explode(" ", $body);
    $words = array();
    foreach ($arr as $word) { 
      $word = preg_replace('/[^A-Za-z0-9]/', '', $word);
      array_push($words, trim($word));
    }
    $body_keys = $words;    
    
    if (empty($uid)) {
      $pet_uid = 161;  // dleonard
    }
    else {
      $pet_uid = $uid;
    }
    
    if ($status == '') {
      $pet_status = rand(0, 6);
    }
    else {
      $pet_status = $status;
    }
    
    $ready = 0;
    $response_status = WH_PETITION_RESPONSE_STATUS_UNANSWERED;
    if ($pet_status == WH_PETITION_STATUS_DRAFT) {
      $sig_count = 0;
    }
    elseif ($pet_status == WH_PETITION_STATUS_PRIVATE) {
      $sig_count = rand(1, variable_get('wh_petitions_public_signatures', 20));
    }
    elseif ($pet_status == WH_PETITION_STATUS_PUBLIC) {
      $sig_count = rand(variable_get('wh_petitions_public_signatures', 20), variable_get('wh_petitions_review_signatures', 500));
    }
    elseif ($pet_status == WH_PETITION_STATUS_UNDER_REVIEW) {
      $sig_count = rand((variable_get('wh_petitions_review_signatures', 500) + 1), 100000);
      $days = rand(1, 20);
      $ready = strtotime("- " . $days . " days");  
      $response_status = WH_PETITION_RESPONSE_STATUS_PENDING;
    }      
    elseif ($pet_status == WH_PETITION_STATUS_CLOSED) {
      $sig_count = rand(1, variable_get('wh_petitions_review_signatures', 500));
    }
    elseif ($pet_status == WH_PETITION_STATUS_REVIEWED) {
      $sig_count = rand((variable_get('wh_petitions_review_signatures', 500) + 1), 100000);
      $days = rand(1, 20);
      $ready = strtotime("- " . $days . " days");  
      $response_status = WH_PETITION_RESPONSE_STATUS_ANSWERED;
    }                
    elseif ($pet_status == WH_PETITION_STATUS_FLAGGED) {
      $sig_count = rand(1, variable_get('wh_petitions_review_signatures', 500));
    }
    $timeframe = rand(1, 30);
  
    $petition = array(
      'uid'               => (int)$pet_uid,       // Drupal UID of the creator
      'title'             => $title,      // Display title of the petition
      'title_keywords'    => $title_keys,
      'body'              => $body,      // The body/description of the petition
      'body_keywords'     => $body_keys,
      'body_revisions'    => array(), // Array of previous petition object ids; only admins can edit a non-draft petition
      'response_id'       => 0,       // Drupal NID of the response instance
      'issues'            => $issue_arr, // Array of taxonomy term IDs
      'user_tags'         => array(), // Array of taxonomy term IDs for user-submitted tags
      'private_tags'      => array(), // Array of taxonomy term IDs for private admin-added tags
      'related_petitions' => array(), // Array of Mongo IDs of related petitions
      'petition_status'   => (int)$pet_status,       // State of the petition: public, private, in review, reviewed
      'response_status'   => $response_status,       // state of the response: none, presidential, etc.
      'published'         => (int)$published,       // timestamp for when this petition was published
      'reached_public'    => (int)$public,       // timestamp for when this petition reached the required signatures to become public
      'reached_ready'     => (int)$ready,       // timestamp for when this petition reached the required signatures to receive a review
      'closed'            => 0,       // timestamp for when this petition was responded to
      'signature_count'   => (int)$sig_count,       // count of signatures
      'abuse_flags'       => array(), // Drupal UIDs of people who have flagged this as abuse      
      'abuse_count'       => rand(0, 5000), // Count of times this has been reported for abuse
      'review_timeframe'  => $timeframe,       // Number of days the petition has to reach X signatures and receive a review
      'response_signatures' => variable_get('wh_petitions_review_signatures', 500),     // Number of signatures required to receive a response
      'public_signatures' => variable_get('wh_petitions_public_signatures', 500),       // Number of signatures needed to become public
      'featured'          => rand(0, 1),  
      'bookmarked'        => array(),
      'hidden'            => 0,
    );

    wh_petitions_generate_nice_url($petition);
    
    $petition_id = wh_petitions_save_petition($conn, $petition);
    
    create_signatures($sig_count, $pet_uid, $published, $petition_id);
  }
}

/**
 * Helper function to create the signatures for each petition
 */
function create_signatures($sig_count, $uid, $published, $petition_id) {
  $conn = wh_petitions_mongo_petition_signatures_connection();

  // User signs their own  
  $signature = array(
    'petition_id' => $petition_id,
    'uid' => (int)$uid, 
    'timestamp' => (int)$published, 
    'comment' => create_title(rand(1, 25)), 
    'abuse_flags' => array(), 
    'status' => 1,
    'number' => 1,
    'user' => array(
      'first_name' => 'first-' . rand(1, 1000),
      'last_name' => 'last-' . rand(1, 1000),
      'city' => 'city-' . rand(1, 1000),
      'state' => rand(10, 99),
      'zip' => rand(10000, 99999),
      'username' => 'uname-' . rand(1, 1000),
      'country' => 'Country',
    ),
  );
  wh_petitions_save_signature($signature, $conn);
  $sig_count--;

  for ($i=0;$i<$sig_count;$i++) {
    $uid = rand(1, 1000000);
    $days = rand(0, 50);
    $ts = strtotime("- " . $days . " days");
    $signature =  array(
      'petition_id' => $petition_id,    
      'uid' => (int)$uid, 
      'timestamp' => (int)$ts, 
      'comment' => create_title(rand(1, 25)), 
      'abuse_flags' => array(), 
      'status' => 1,
      'number' => ($i + 2),
      'user' => array(
        'first_name' => 'first-' . rand(1, 1000),
        'last_name' => 'last-' . rand(1, 1000),
        'city' => 'city-' . rand(1, 1000),
        'state' => rand(10, 99),
        'zip' => rand(10000, 99999),
        'username' => 'uname-' . rand(1, 1000),
        'country' => 'Country',
      )
    );
    wh_petitions_save_signature($signature, $conn);
  }

  return true;
}

/**
 * Helper function to create any field that needs words, copied from devel generate content
 */
function create_title($word_count, $title = FALSE) {
  $dictionary = array("abbas", "abdo", "abico", "abigo", "abluo", "accumsan",
    "acsi", "ad", "adipiscing", "aliquam", "aliquip", "amet", "antehabeo",
    "appellatio", "aptent", "at", "augue", "autem", "bene", "blandit",
    "brevitas", "caecus", "camur", "capto", "causa", "cogo", "comis",
    "commodo", "commoveo", "consectetuer", "consequat", "conventio", "cui",
    "damnum", "decet", "defui", "diam", "dignissim", "distineo", "dolor",
    "dolore", "dolus", "duis", "ea", "eligo", "elit", "enim", "erat",
    "eros", "esca", "esse", "et", "eu", "euismod", "eum", "ex", "exerci",
    "exputo", "facilisi", "facilisis", "fere", "feugiat", "gemino",
    "genitus", "gilvus", "gravis", "haero", "hendrerit", "hos", "huic",
    "humo", "iaceo", "ibidem", "ideo", "ille", "illum", "immitto",
    "importunus", "imputo", "in", "incassum", "inhibeo", "interdico",
    "iriure", "iusto", "iustum", "jugis", "jumentum", "jus", "laoreet",
    "lenis", "letalis", "lobortis", "loquor", "lucidus", "luctus", "ludus",
    "luptatum", "macto", "magna", "mauris", "melior", "metuo", "meus",
    "minim", "modo", "molior", "mos", "natu", "neo", "neque", "nibh",
    "nimis", "nisl", "nobis", "nostrud", "nulla", "nunc", "nutus", "obruo",
    "occuro", "odio", "olim", "oppeto", "os", "pagus", "pala", "paratus",
    "patria", "paulatim", "pecus", "persto", "pertineo", "plaga", "pneum",
    "populus", "praemitto", "praesent", "premo", "probo", "proprius",
    "quadrum", "quae", "qui", "quia", "quibus", "quidem", "quidne", "quis",
    "ratis", "refero", "refoveo", "roto", "rusticus", "saepius",
    "sagaciter", "saluto", "scisco", "secundum", "sed", "si", "similis",
    "singularis", "sino", "sit", "sudo", "suscipere", "suscipit", "tamen",
    "tation", "te", "tego", "tincidunt", "torqueo", "tum", "turpis",
    "typicus", "ulciscor", "ullamcorper", "usitas", "ut", "utinam",
    "utrum", "uxor", "valde", "valetudo", "validus", "vel", "velit",
    "veniam", "venio", "vereor", "vero", "verto", "vicis", "vindico",
    "virtus", "voco", "volutpat", "vulpes", "vulputate", "wisi", "ymo",
    "zelus");
  $dictionary_flipped = array_flip($dictionary);

  $greeking = '';

  if (!$title) {
    $words_remaining = $word_count;
    while ($words_remaining > 0) {
      $sentence_length = mt_rand(3, 10);
      $words = array_rand($dictionary_flipped, $sentence_length);
      $sentence = implode(' ', $words);
      $greeking .= ucfirst($sentence) . '. ';
      $words_remaining -= $sentence_length;
    }
  }
  else {
    // Use slightly different method for titles.
    $words = array_rand($dictionary_flipped, $word_count);
    $greeking = ucwords(implode(' ', $words));
  }

  // Work around possible php garbage collection bug. Without an unset(), this
  // function gets very expensive over many calls (php 5.2.11).
  unset($dictionary, $dictionary_flipped);
  return trim($greeking);
}
