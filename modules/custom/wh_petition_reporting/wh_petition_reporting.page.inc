<?php
/**
 * @file
 *
 * functionality for the main petition reporting page
 */

/**
 * Page callback for the admin/petition-reports URL
 *
 * @return html string of the page
 */
function petition_reports_page() {
  $html = "<h2>Please select one of the following reports:<h2>";

  $report_links = array();
  foreach (wh_petition_reporting_menu() as $path => $report) {
    if ($path == $_GET['q'] || $report['type'] == MENU_CALLBACK) { //dont include the current page or any callbacks, cause that's just dumb
      continue;
    }
    $report_links[] = l($report['title'], $path, array('attributes' => array('title' => $report['description'])));
  }
  $list_variables = array(
    'items' => $report_links,
    'title' => t('Petition Reports'),
    'type' => 'ul',
    'attributes' => array(),
  );
  $html .= theme_item_list($list_variables);
  return $html;
}
