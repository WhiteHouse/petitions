<?php
/**
 * @file
 */

/**
 * Implements hook_token_info().
 */
function wh_petitions_token_info() {
  $types['petition'] = array(
    'name' => t('Petition data'),
    'description' => t('Tokens related to petitions'),
    'needs-data' => 'petition',
  );
  $petition['title'] = array(
    'name' => t('Petition title'),
    'description' => t('Petition title'),
  );
  return array(
     'types' => $types,
     'tokens' => array('petition' => $petition),
  );
}
 
/**
 * Implements hook_tokens().
 */
function wh_petitions_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  $sanitize = isset($options, $options['sanitize']) ? $options['sanitize'] : FALSE;

  if ($type == 'petition' && !empty($data['petition'])) {
    $petition = $data['petition'];
    // Future support for more tokens
    foreach ($tokens as $name => $original) {
      switch ($name){
        case 'title':
          $title = $petition['title'];
          $replacements[$original] = $sanitize ? check_plain($title) : $title;
          break;
      }
    }
  }
  return $replacements;
}
