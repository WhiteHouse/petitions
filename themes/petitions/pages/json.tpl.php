<?php 

$css = drupal_get_css();
$css = explode("\n", $css);
$css_array = array();
for ($i=0;$i<sizeof($css);$i++) {
  preg_match('/\("(.*)"\)/', $css[$i], $matches);
  if (!empty($matches[1])) {
    array_push($css_array, $matches[1]);
  }
}

$page_title = filter_xss(menu_get_active_title(), array());
if (empty($page_title)) {
  $page_title = t('Petitions');
}
$page_title .= ' | ' . t('The White House');

$data = array(
  'page_title' => $page_title,
  'css' => drupal_get_css(),
  'js' => drupal_get_js(),
  'css_array' => implode(':', $css_array),
  'full_markup' => $page['#children'] . '<br clear="all" />',
);
$petition_status = context_get('petition','status');
if (!empty($petition_status) && $petition_status == WH_PETITION_STATUS_FLAGGED) {
  $data['is_flagged'] = 1;
  $data['full_markup'] = '<div class="spacer-for-cutup"></div><div id="petition-outer" class="clear"><div id="petition-inner" class="clearfix"><div id="petitions-removed" class="main-content">' . render($page['content']) . '</div></div></div>';
}

print json_encode($data);
