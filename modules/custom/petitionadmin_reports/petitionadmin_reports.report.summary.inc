<?php
/**
 * @file petitionadmin_reports.report.summary.inc.
 * Code for the Petition UI - Reports - summary report.
 */

class PetitionUISummaryManager {
  public $lastBuild;
  public $dataSummary;
  public $dataIssue;

  /**
   * Implements constructor.
   */
  public function __construct() {
    $this->lastBuild = variable_get('petitionadmin_reports_summary_lastrun', 0);

    if ((time() - $this->lastBuild) >= 3600) {
      // Set last run datetime.
      $this->lastBuild = time();
      variable_set('petitionadmin_reports_summary_lastrun', $this->lastBuild);

      $this->buildSummaryData();
      $this->buildIssueData();
    }
    else {

      // Load cached records.
      $this->loadSummaryData();
      $this->loadIssueData();
    }
  }

  /**
   * Implements building summary data.
   *
   * @return mixed
   *   Returns array of summary values.
   */
  public function buildSummaryData() {

    try {

      // All users.
      $user_all_query = db_select('users', 'u1');
      $user_all_query->condition('u1.uid', 0, '<>');
      $user_all_query->addExpression('count(u1.uid)', 'user_all_count');

      // User Valid.
      $user_valid_query = db_select('users', 'u2');
      $user_valid_query->condition('u2.login', 0, '>');
      $user_valid_query->condition('u2.uid', 0, '<>');
      $user_valid_query->addExpression('count(u2.uid)', 'user_valid_count');

      // Start Petition Queries.
      // All Petitions.
      $petition_all_query = db_select('node', 'n1');
      $petition_all_query->addExpression('count(n1.nid)', 'petition_all_count');

      // Active Petitions.
      $petition_active_query = db_select('node', 'n');
      $petition_active_query->addExpression('count(n.nid)', 'petition_active_count');
      $petition_active_query->join('field_data_field_petition_status', 'ps', 'ps.entity_id = n.nid');
      $petition_active_query->condition(
        'ps.field_petition_status_value',
        array(
          WH_PETITION_STATUS_PRIVATE,
          WH_PETITION_STATUS_PUBLIC,
          WH_PETITION_STATUS_UNDER_REVIEW,
          WH_PETITION_STATUS_REVIEWED,
        ),
        'IN');

      // Public petitions.
      $petition_public_query = db_select('node', 'n');
      $petition_public_query->addExpression('count(n.nid)', 'petition_public_count');
      $petition_public_query->join('field_data_field_petition_status', 'ps', 'ps.entity_id = n.nid');
      $petition_public_query->condition(
        'ps.field_petition_status_value',
        array(
          WH_PETITION_STATUS_PUBLIC,
          WH_PETITION_STATUS_UNDER_REVIEW,
          WH_PETITION_STATUS_REVIEWED,
        ),
        'IN');

      // Awaiting Response.
      $petition_awaiting_response_query = db_select('node', 'n');
      $petition_awaiting_response_query->addExpression('count(n.nid)', 'petition_awaiting_response_count');
      $petition_awaiting_response_query->join('field_data_field_response_status', 'rs', 'rs.entity_id = n.nid');
      $petition_awaiting_response_query->condition('rs.field_response_status_value', 1, '=');

      // Responded Petitions.
      $petition_responded_query = db_select('node', 'n');
      $petition_responded_query->addExpression('count(n.nid)', 'petition_responded_count');
      $petition_responded_query->join('field_data_field_response_status', 'rs', 'rs.entity_id = n.nid');
      $petition_responded_query->condition('rs.field_response_status_value', WH_PETITION_STATUS_PUBLIC, '=');

      // Removed Petitions.
      $petition_removed_query = db_select('node', 'n');
      $petition_removed_query->addExpression('count(n.nid)', 'petition_removed_count');
      $petition_removed_query->join('field_data_field_petition_status', 'rs', 'rs.entity_id = n.nid');
      $petition_removed_query->condition('rs.field_petition_status_value', WH_PETITION_STATUS_FLAGGED, '=');

      // Expired Petitions.
      $petition_expired_query = db_select('node', 'n');
      $petition_expired_query->addExpression('count(n.nid)', 'petition_expired_count');
      $petition_expired_query->join('field_data_field_petition_status', 'rs', 'rs.entity_id = n.nid');
      $petition_expired_query->condition('rs.field_petition_status_value', WH_PETITION_STATUS_CLOSED, '=');

      // Signaure Query.
      $signature_all_query = db_select('signature_mail', 'sm');
      $signature_all_query->addExpression('count(sm.id)', 'signature_all_count');

      // Define key values.
      $data_array = array(
        'user_all'                   => '',
        'user_valid'                 => '',
        'petition_all'               => '',
        'petition_active'            => '',
        'petition_public'            => '',
        'petition_awaiting_response' => '',
        'petition_responded'         => '',
        'petition_removed'           => '',
        'petition_expired'           => '',
        'signature_all'              => '',
      );

      $data_summary = array();

      // Loop through values and run each query.
      foreach ($data_array as $key => $value) {

        $query_count_var = $key . '_count';
        $query = ${$key . '_query'};
        $query_result = $query->execute()->fetchAllAssoc($query_count_var);
        if (count($query_result) > 0) {
          $query_data = array_pop($query_result);
        }
        else {
          (object) $query_data = '';
        }

        $data_summary[$query_count_var] = (property_exists($query_data, $query_count_var) && !is_null($query_data->$query_count_var)) ? (int) $query_data->$query_count_var : 0;
      }

      // Delete old data.
      db_delete('petitionadmin_reports_report_summary')->execute();

      // Save summary data.
      db_insert('petitionadmin_reports_report_summary')
        ->fields(
          array(
            'user_all'                   => $data_summary['user_all_count'],
            'user_valid'                 => $data_summary['user_valid_count'],
            'petition_all'               => $data_summary['petition_all_count'],
            'petition_active'            => $data_summary['petition_active_count'],
            'petition_public'            => $data_summary['petition_public_count'],
            'petition_awaiting_response' => $data_summary['petition_awaiting_response_count'],
            'petition_responded'         => $data_summary['petition_responded_count'],
            'petition_removed'           => $data_summary['petition_removed_count'],
            'petition_expired'           => $data_summary['petition_expired_count'],
            'signature_all'              => $data_summary['signature_all_count'],
          ))
        ->execute();

      // Save data summary array values.
      $this->dataSummary = $data_summary;
    }
    catch (Exception $e) {
      watchdog(
        'petitionadmin_reports', "Error.\n\n@exception",
        array(
          '@exception' => print_r($e, TRUE),
        ), WATCHDOG_ERROR);
    }
  }

  /**
   * Implements build issue data.
   *
   * @return mixed
   *   Returns multidimensional array of issue summary data.
   */
  public function buildIssueData() {
    try {

      $query = db_select('taxonomy_term_data', 'ttd');
      $query->addJoin('LEFT OUTER', 'taxonomy_vocabulary', 'tv', 'tv.vid =  ttd.vid');
      $query->condition('tv.name', 'Issues', '=');
      $query->addField('ttd', 'tid');

      $core_data = $query->execute()->fetchAllAssoc('tid');

      // Delete issues records.
      db_delete('petitionadmin_reports_report_issues')->execute();

      // Insert new records.
      // Insert Statement.
      $insert_query = db_insert('petitionadmin_reports_report_issues')
        ->fields(
          array(
            'tid',
            'term_name',
            'petition_all',
            'petition_public',
            'signature_all',
          ));

      // Loop through coreData and map to table.
      foreach ($core_data as $key => $value) {
        $taxonomy_data = $this->buildIssueDataByTerm($value->tid);
        $insert_query->values($taxonomy_data);
      }

      $insert_query->execute();

      $this->dataIssue = $core_data;

    }
    catch (Exception $e) {
      watchdog(
        'petitionadmin_reports', "Error.\n\n@exception",
        array(
          '@exception' => print_r($e, TRUE),
        ),
        WATCHDOG_ERROR);
    }
  }

  /**
   * Builds statistics of petitions and signatures per issue term.
   *
   * @param int $tid
   *   Taxonomy term id.
   *
   * @return null|array
   *   Returns array of statistics
   */
  private function buildIssueDataByTerm($tid) {

    // Get taxonomy Term.
    $query_term = db_select('taxonomy_term_data', 'ttd');
    $query_term->addJoin('LEFT OUTER', 'taxonomy_vocabulary', 'tv', 'tv.vid =  ttd.vid');
    $query_term->addField('ttd', 'tid');
    $query_term->addField('ttd', 'name');
    $query_term->condition('tv.name', 'Issues', '=');
    $query_term->condition('ttd.tid', $tid, '=');

    $term_data = $query_term->execute()->fetchAllAssoc('tid');

    // Get all Petitions by taxonomy term.
    $query = db_select('field_data_field_petition_issues', 'fpi');
    $query->addJoin('INNER', 'node', 'n', 'n.nid = fpi.entity_id');
    $query->addJoin('LEFT OUTER', 'taxonomy_term_data', 'ttd', 'ttd.tid = fpi.field_petition_issues_tid');
    $query->addJoin('LEFT OUTER', 'taxonomy_vocabulary', 'tv', 'tv.vid =  ttd.vid');

    $query->condition('tv.name', 'Issues', '=');
    $query->condition('n.type', 'petition', '=');
    $query->condition('ttd.tid', $tid, '=');

    $query->addField('fpi', 'field_petition_issues_tid', 'tid');
    $query->addField('ttd', 'name');
    $query->addExpression('count(fpi.entity_id)', 'petition_count');

    $query->groupBy('fpi.field_petition_issues_tid');
    $query->orderBy('ttd.name', 'ASC');

    $core_data = $query->execute()->fetchAllAssoc('tid');

    // Get public petitions by taxonomy term.
    $query_public = db_select('field_data_field_petition_status', 'fps');
    $query_public->addJoin('LEFT', 'field_data_field_petition_issues', 'fpi', 'fpi.entity_id = fps.entity_id');
    $query_public->addJoin('LEFT OUTER', 'taxonomy_term_data', 'ttd', 'ttd.tid = fpi.field_petition_issues_tid');
    $query_public->addJoin('LEFT OUTER', 'taxonomy_vocabulary', 'tv', 'tv.vid = ttd.vid');

    $query_public->condition('tv.name', 'Issues', '=');
    $query_public->condition('fps.bundle', 'petition', '=');
    $query_public->condition('fps.field_petition_status_value', WH_PETITION_STATUS_PUBLIC, '=');
    $query_public->condition('ttd.tid', $tid, '=');

    $query_public->addField('fpi', 'field_petition_issues_tid', 'tid');
    $query_public->addField('ttd', 'name');
    $query_public->addExpression('count(fps.entity_id)', 'petition_public_count');

    $query_public->groupBy('fpi.field_petition_issues_tid');
    $query_public->orderBy('ttd.name', 'ASC');

    $public_data = $query_public->execute()->fetchAllAssoc('tid');

    // Get signatures by taxonomy term.
    $query_signature = db_select('signature_mail', 'sm');
    $query_signature->addJoin('INNER', 'field_data_field_petition_issues', 'fpi', 'fpi.entity_id = sm.petition_id');
    $query_signature->addJoin('LEFT OUTER', 'taxonomy_term_data', 'ttd', 'ttd.tid = fpi.field_petition_issues_tid');
    $query_signature->addJoin('LEFT OUTER', 'taxonomy_vocabulary', 'tv', 'tv.vid = ttd.vid');

    $query_signature->condition('tv.name', 'Issues', '=');
    $query_signature->condition('fpi.bundle', 'petition', '=');
    $query_signature->condition('ttd.tid', $tid, '=');

    $query_signature->addField('fpi', 'field_petition_issues_tid', 'tid');
    $query_signature->addField('ttd', 'name');
    $query_signature->addExpression('count(sm.id)', 'signature_count');

    $query_signature->groupBy('ttd.tid');
    $query_signature->orderBy('ttd.name', 'ASC');

    $signature_data = $query_signature->execute()->fetchAllAssoc('tid');

    // Merge result sets.
    foreach ($term_data as $key => $value) {
      if (array_key_exists($key, $core_data)) {
        $term_data[$key] = (object) array_merge((array) $term_data[$key], (array) $core_data[$key]);
      }
      else {
        $term_data[$key]->petition_count = 0;
      }
      if (array_key_exists($key, $public_data)) {
        $term_data[$key] = (object) array_merge((array) $term_data[$key], (array) $public_data[$key]);
      }
      else {
        $term_data[$key]->petition_public_count = 0;
      }
      if (array_key_exists($key, $signature_data)) {
        $term_data[$key] = (object) array_merge((array) $term_data[$key], (array) $signature_data[$key]);
      }
      else {
        $term_data[$key]->signature_count = 0;
      }
    }

    $value = array_pop($term_data);

    return array(
      'tid'             => (int) $value->tid,
      'term_name'       => $value->name,
      'petition_all'    => (int) $value->petition_count,
      'petition_public' => (int) $value->petition_public_count,
      'signature_all'   => (int) $value->signature_count,
    );
  }

  /**
   * Implements loading summary data from database cache.
   *
   * @return mixed
   *   Returns array of summary data.
   */
  public function loadSummaryData() {

    try {
      $summary_query = db_select('petitionadmin_reports_report_summary', 'purrs');
      $summary_query->addField('purrs', 'user_all', 'user_all_count');
      $summary_query->addField('purrs', 'user_valid', 'user_valid_count');
      $summary_query->addField('purrs', 'petition_all', 'petition_all_count');
      $summary_query->addField('purrs', 'petition_active', 'petition_active_count');
      $summary_query->addField('purrs', 'petition_public', 'petition_public_count');
      $summary_query->addField('purrs', 'petition_awaiting_response', 'petition_awaiting_response_count');
      $summary_query->addField('purrs', 'petition_responded', 'petition_responded_count');
      $summary_query->addField('purrs', 'petition_removed', 'petition_removed_count');
      $summary_query->addField('purrs', 'petition_expired', 'petition_expired_count');
      $summary_query->addField('purrs', 'signature_all', 'signature_all_count');
      $this->dataSummary = $summary_query->execute()->fetchAssoc();

      return $this->dataSummary;
    }
    catch (Exception $e) {
      watchdog(
        'petitionadmin_reports', "Error.\n\n@exception",
        array(
          '@exception' => print_r($e, TRUE),
        ),
        WATCHDOG_ERROR);
    }
  }

  /**
   * Implements loading of issue summary data from database cache.
   *
   * @return mixed
   *   Returns multidimensional array of issue data.
   */
  public function loadIssueData() {
    try {
      $issue_query = db_select('petitionadmin_reports_report_issues', 'purri');
      $issue_query->addField('purri', 'tid');
      $issue_query->addField('purri', 'term_name', 'name');
      $issue_query->addField('purri', 'petition_all', 'petition_count');
      $issue_query->addField('purri', 'petition_public', 'petition_public_count');
      $issue_query->addField('purri', 'signature_all', 'signature_count');
      $this->dataIssue = $issue_query->execute()->fetchAllAssoc('tid');

      return $this->dataIssue;
    }
    catch (Exception $e) {
      watchdog(
        'petitionadmin_reports', "Error.\n\n@exception",
        array(
          '@exception' => print_r($e, TRUE),
        ),
        WATCHDOG_ERROR);
    }
  }

  /**
   * Implements rendering of summary HTML table.
   *
   * @param bool $render_header
   *   Optionally enable display of table header.
   *
   * @return string
   *   Returns HTML string of table contents.
   */
  public function renderSummaryTable($render_header = TRUE) {

    drupal_add_css(
      drupal_get_path('module', 'petitionadmin_reports') . '/css/petitionadmin_reports_summary.css',
      array(
        'type'       => 'file',
        'group'      => CSS_DEFAULT,
        'every_page' => FALSE,
      ));

    $output = '';

    // Define key and human readable title.
    $data_array = array(
      'user_all'                   => 'Registered Users',
      'user_valid'                 => 'Verified Users',
      'petition_all'               => 'Total Petitions',
      'petition_active'            => 'Active',
      'petition_public'            => 'Public',
      'petition_awaiting_response' => 'Awaiting Reponse',
      'petition_responded'         => 'Responded To',
      'petition_removed'           => 'Hidden',
      'petition_expired'           => 'Expired',
      'signature_all'              => 'Signatures',
    );

    $summary_data = $this->dataSummary;

    $data_rows = array();

    foreach ($data_array as $key => $value) {
      $row_title = $key . '_count';
      $title_cell = array('data' => $data_array[$key] . ':', 'header' => TRUE);
      $data_cell = array('data' => number_format($summary_data[$row_title]));
      $data_rows[] = array('data' => array($title_cell, $data_cell));
    }

    if ($render_header) {
      $output .= '<div id="petition-summary-report"><h2>' . t('Petition Summary Report') . '</h2>';
    }
    $output .= theme_table(
      array(
        'header'     => array(),
        'rows'       => $data_rows,
        'attributes' => array('id' => 'petition-summary-table'),
        'caption'    => '',
        'colgroups'  => array(),
        'sticky'     => TRUE,
        'empty'      => t('No data found.'),
      ));
    $output .= '</div>';

    return $output;
  }

  /**
   * Implements rendering of summary charts.
   *
   * @param bool $header
   *   Optionally specify if header should render.
   *
   * @throws Exception
   *   Feature not implemented
   */
  public function renderSummaryChart($header = FALSE) {
    throw new Exception('Not implemented');
  }

  /**
   * Implements rendering issue HTML table.
   *
   * @param bool $render_header
   *   Optionally render table header.
   *
   * @return string
   *   Returns full HTML table.
   */
  public function renderIssueTable($render_header = TRUE) {

    $output = '';

    // Get vocabulary ID.
    $vocab = taxonomy_vocabulary_machine_name_load('issues');
    if (empty($vocab)) {
      throw new Exception("Unable to load taxonomy with machine name 'issues'");
    }

    // Get all issue/terms for the vocabulary.
    $query = db_select('taxonomy_term_data', 'ttd')
      ->fields('ttd', array('tid', 'name'));
    $query->condition('ttd.vid', $vocab->vid);
    $query->orderBy('ttd.name', 'ASC');

    $term_data = $query->execute()->fetchAllAssoc('tid');

    // Define header and matching fields.
    $header = array(
      t('Issue')            => 'name',
      t('Petitions')        => 'petition_count',
      t('Public Petitions') => 'petition_public_count',
      t('Signatures')       => 'signature_count',
    );

    // Load Issue Data.
    $issue_data = $this->dataIssue;

    // Loop through all issue terms and match with issue data.
    foreach ($term_data as $key => $value) {

      $title_cell = array(
        'data'   => $term_data[$key]->name . ':',
        'header' => TRUE,
      );

      if (array_key_exists($key, $issue_data)) {
        $petitions = number_format($issue_data[$key]->{$header[t('Petitions')]});
        $public = number_format($issue_data[$key]->{$header[t('Public Petitions')]});
        $signatures = number_format($issue_data[$key]->{$header[t('Signatures')]});
      }
      else {
        $petitions = number_format(0);
        $public = number_format(0);
        $signatures = number_format(0);
      }

      $data_rows[] = array(
        'data' => array(
          $title_cell,
          $petitions,
          $public,
          $signatures,
        ),
      );
    }

    if ($render_header) {
      $output .= '<div id="petition-summary-report"><h2>' . t('Petition Issues Summary Report') . '</h2>';
    }
    $output .= theme_table(
      array(
        'header'     => array_keys($header),
        'rows'       => $data_rows,
        'attributes' => array('id' => 'petition-issues-table'),
        'caption'    => '',
        'colgroups'  => array(),
        'sticky'     => TRUE,
        'empty'      => t('No data found.'),
      ));
    $output .= '</div>';

    return $output;

  }

  /**
   * Implements rendering of Issue Chart.
   *
   * @throws Exception
   *   Feature not implemented.
   */
  public function renderIssueChart() {
    throw new Exception('Not implemented');
  }

  /**
   * Implements building and caching summary data.
   */
  public function saveSummaryData() {
    $this->buildSummaryData();
    $this->buildIssueData();
    variable_set('petitionadmin_reports_summary_lastrun', time());
  }

}

/**
 * Implements hour processing for cron queue.
 *
 * @param mixed $data
 *   Assumes array with start and end date.
 */
function petitionadmin_reporting_summary_processor($data) {
  $petitionadmin_report = new PetitionUISummaryManager();
  $petitionadmin_report->saveSummaryData();
}

/**
 * Implements wrapper function for summary page.
 * @return string
 *   Returns HTML output for page.
 */
function petitionadmin_reports_report_summary() {

  drupal_add_css(
    drupal_get_path('module', 'petitionadmin_reports') . '/css/petitionadmin_reports_admin_tabs.css',
    array(
      'type'       => 'file',
      'group'      => CSS_DEFAULT,
      'every_page' => FALSE,
    ));

  $output = '';

  // Hiding third level menu until we have more reports.
  /*
  $output .= '
    <div class="tabs-secondary clearfix">
        <h2 class="element-invisible">Tertiary tabs</h2>
        <ul class="tabs secondary">
            <li class="active"><a class="active" href="/admin/content/petitions/reports/summary">' . t('Summary') . '<span class="element-invisible">(active tab)</span></a></li>
            <li><a href="/admin/content/petitions/reports/hourly">' . t('Hourly') . '</a></li>
        </ul>
    </div>';
  */

  $petition_summary_manager = new PetitionUISummaryManager();

  $output .= '<div id="petitionadmin_reports_summary-lastrun">
                <strong>Report last updated on:</strong> ' . date('m/d/Y g:i:s a', $petition_summary_manager->lastBuild) . '<br />
                <strong>Next update will occur on:</strong> ' . date('m/d/Y g:i:s a', ($petition_summary_manager->lastBuild + 3600)) . '<br />
                </div>';

  $output .= '<table style="border: none 0px;">';
  $output .= '<tr><td style="width: 400px; vertical-align: text-top;">' . $petition_summary_manager->renderSummaryTable() . '</td>';
  $output .= '<td style="vertical-align: text-top;">' . $petition_summary_manager->renderIssueTable() . '</td></tr>';
  $output .= '</table>';

  // Debug expressions for data.
  /*
  $output .= 'Summary Data:<br />';
  $output .= '<pre>' . print_r($petition_summary_manager->loadSummaryData(), true) . '</pre>';

  $output .= 'Issues Data:<br />';
  $output .= '<pre>' . print_r($petition_summary_manager->loadIssueData(), true) . '</pre>';
  */

  return $output;
}
