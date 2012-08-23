<?php
/**
 * @file
 *
 * defines the page builder function for the summary reports page and the class
 *  which manages that data
 */

/**
 *  class for managing generation, caching, and retrieval of summary report
 *    data
 *
 */
class SummaryReportManager {
  private $lastBuild;

  private $summaryData = array();
  private $issuesData = array();

  /**
   * Constructor
   *
   * Checks time of last build and sets indicator if new build is needed
   */
  function __construct() {
    $this->lastBuild = variable_get('wh_petition_reports_last_build', 0);
    if ((time() - $this->lastBuild) >= 3600) {
      $this->lastBuild = time();
      variable_set('wh_petition_reports_last_build', $this->lastBuild);
      //rebuild the summary data stored in Mongo
      $this->buildSummaryData();
      $this->buildIssuesSummaryData();
    }
    else {
      //the data in mongo is good, so just load it from cache table
      $this->loadSummaryData();
      $this->loadIssuesSummaryData();
    }
  }

  /**
   * Generates header including date report data was aggregated
  *
   */
  public function getReportHeader() {
    return '<h2>' . t('Data generated %date', array('%date' => date('m-d-Y H:i:s', $this->lastBuild))) . '</h2>';
  }

  /**
   * get updated data from the database, set it in the $summaryData variable and stored it back to the db
   */
  private function buildSummaryData() {
    //set up connections
    $userQuery = "select count(uid) as users, sum(if(login>0,1,0)) as verified from {users}";
    $userData = db_query($userQuery)->fetchObject();
    $mongoPetitionCollection = wh_petitions_mongo_petition_connection();
    $mongoSignaturesCollection = wh_petitions_mongo_petition_signatures_connection();

    //store all the data we generated for later use
    $this->summaryData = array(
      'users' => $userData->users,
      'verified' => $userData->verified,
      'petitions' => $mongoPetitionCollection->count(),
      'active' => $mongoPetitionCollection->count(array('petition_status' => array('$in' => array(1, 2, 4, 5)))), //1/Private, 2/Public, 4/Under Review,5/Reviewed
      'public' => $mongoPetitionCollection->count(array('petition_status' => array('$in' => array(2, 4, 5)))),
      'awaiting_response' => $mongoPetitionCollection->count(array('response_status' => 1)),
      'responded_to' => $mongoPetitionCollection->count(array('response_status' => 2)),
      'removed' => $mongoPetitionCollection->count(array('petition_status' => 6)),
      'expired' => $mongoPetitionCollection->count(array('petition_status' => 3)),
      'signatures' => $mongoSignaturesCollection->count(),
    );

    //write generated data to mongo
    $collection = mongodb_collection('summary_report_data');
    $collection->remove(); //purge the table, this is just a snapshot, not a historical record
    $collection->save(array_merge($this->summaryData, array('unix_time' => $this->lastBuild)));
  }

  /**
   * load summary data from cache table
   *
   * @todo: finish developing this
   */
  private function loadSummaryData() {
    //get data from Mongo
    $collection = mongodb_collection('summary_report_data')->findOne();  //should only ever be one

    //force it into holding array
    $this->summaryData = array(
      'users' => $collection['users'],
      'verified' => $collection['verified'],
      'petitions' => $collection['petitions'],
      'active' => $collection['active'],
      'public' => $collection['public'],
      'awaiting_response' => $collection['awaiting_response'],
      'responded_to' => $collection['responded_to'],
      'removed' => $collection['removed'],
      'expired' => $collection['expired'],
      'signatures' => $collection['signatures'],
    );
  }

  /**
   * get updated data from the database, set it in the $summaryIssuesData variable and stored it back to the db
   */
  private function buildIssuesSummaryData() {
    //get list of issues
    $vocab = taxonomy_vocabulary_machine_name_load('issues');
    if (empty($vocab)) {
      throw new Exception("Unable to load taxonomy with machine name 'issues'");
    }
    $issues = taxonomy_get_tree($vocab->vid);
    usort($issues, array($this, 'sortIssues'));

    $issue_data = $this->mapReduceIssues();

    $collection = mongodb_collection('summary_report_data__issues');
    $collection->remove(); //purge the table, this is just a snapshot, not a historical record

    foreach ($issues as $issue) {
      //store the data oof to our variable and write it to mongo for later use
      $this->issuesData[$issue->name] = $issue_data[$issue->tid];
      $collection->save(array_merge(array('name' => $issue->name), $issue_data[$issue->tid]));
    }
  }

  /**
   * load issues summary data from cache table
   *
   * @todo: make this work
   */
  private function loadIssuesSummaryData() {
    //get data from Mongo
    $collection = mongodb_collection('summary_report_data__issues')->find();

    //force it into holding array
    foreach ($collection as $record) {
      $name = $record['name'];
      unset($record['name']);
      unset($record['_id']);

      $this->issuesData[$name] = $record;
    }
  }

  /**
   *  MapReduce mongo petitions to get counts by issue
   *
   */
  private function mapReduceIssues() {

    //JAVASCRIPT function
    //for this purpose, Removed Petitions do not count
    $map_function = new MongoCode("function () {
      var public = 0;
      if (this.petition_status != 6) {
        if (this.petition_status == 2 || this.petition_status == 4 || this.petition_status ==5) {
          public = 1;
        }
        var signatures = this.signature_count;

        if (this.issues != null) {
          for (var i = 0; i<this.issues.length; i++) {
            emit(this.issues[i], {'petitions' : 1, 'public' : public, 'signatures' : signatures});
          }
        }
      }
    }");

    //JAVASCRIPT function
    $reduce_function = new MongoCode("function(key, values) {
      var petitions_sum = 0;
      var public_sum = 0;
      var signatures_sum = 0;

      values.forEach(function(value) {
        petitions_sum += value['petitions'];
        public_sum += value['public'];
        signatures_sum += value['signatures'];
      });
      return {'petitions': petitions_sum , 'public' : public_sum, 'signatures' : signatures_sum};
    }");

    $command = array(
      'mapreduce' => 'petitions', //the name of the collection
      'map' => $map_function,
      'reduce' => $reduce_function,
      'out' => array('inline' => 1),  //eventually we'll want to store these off, I think
    );

    $db = mongodb();
    $issues_data_collection = $db->command($command);

    //convert what we get from mongo into a structure more easily merged with the other data
    //result is an array where keys are issue tids and values are array of data
    $issues_data = array();
    foreach ($issues_data_collection['results'] as $issue) {
      $issues_data[$issue['_id']] = $issue['value'];
    }

    return $issues_data;
  }

  /**
   * Sort array of issues taxonomy objects by issue name
   *
   * @see http://php.net/manual/en/function.usort.ph
   */
  public function sortIssues($a, $b) {
    if ($a->name == $b->name) {
      return 0;
    }
    return ($a->name < $b->name) ? -1 : 1;
  }


  /**
   * Add a row for the given data point to the data rows array for summary table
   *
   * @param $data_rows array
   *   array to which the row will be added and which will eventually be fed to
   *    theme_table()
   * @param $label string
   *   the translated text which should be written into the label column for the row,
   * @param $data string
   *   string of the data which should be written into the data column for the row
   * @param $label_inset boolean
   *   optional (defaults to FALSE) indicator of if the label should be indented
   * @param $data_pct_of integer
   *   optional; if given the percent value of $data/$data_pct_of is included in the data cell
   */
  private function addRowToSummaryTable(&$data_rows, $label, $data, $label_inset = FALSE, $data_pct_of = FALSE) {
    if ($data_pct_of) {
      if (!is_numeric($data_pct_of) || !is_numeric($data)) {
        throw new Exception('Seriously? You asked me to give a percentage but either $data or $data_pct_of are not numeric. Go back and try again.');
      }
      $data_string = t('(@percent%) @data', array('@percent' => round(($data/$data_pct_of) * 100, 2), '@data' => number_format($data)));
    }
    else {
      $data_string = t('@data', array('@data' => number_format($data)));
    }

    $title_cell = array('data' => $label . ':', 'header' => TRUE, 'class' => ($label_inset ? 'inset' : ''));
    $data_cell = array('data' => $data_string);
    $data_rows[] = array('data' => array($title_cell, $data_cell));
  }

  /**
   * Add a row for the given data point to the data rows array for summary table
   *
   * @param $data_rows array
   *   array to which the row will be added and which will eventually be fed to
   *    theme_table()
   * @param $label string
   *   the translated text which should be written into the label column for the row,
   * @param $data_cells string
   *   string of the data which should be written into the data column for the row
   * @param $label_inset boolean
   *   optional (defaults to FALSE) indicator of if the label should be indented
   */
  private function addRowToIssuesTable(&$data_rows, $label, $data_cells, $label_inset = FALSE) {
    $title_cell = array('data' => $label . ':', 'header' => TRUE, 'class' => ($label_inset ? 'inset' : ''));
    $petitions = number_format($data_cells['petitions']);
    $petitions_pct = round(($petitions / ($this->summaryData['petitions'] - $this->summaryData['removed']))  * 100, 2) . '%';
    $public = number_format($data_cells['public']);
    $public_pct = round(($public / $this->summaryData['public']) * 100, 2) . '%';
    $signatures = number_format($data_cells['signatures']);

    $data_rows[] = array('data' => array($title_cell, $petitions, $petitions_pct, $public, $public_pct, $signatures));
  }

  /**
   * Build the Summary Report table
   *
   * @return string
   *  HTML markup of table
   */
  public function getSummaryReportTable() {
    $markup = '';
    $data_rows = array();

    //Registred Users
    $this->addRowToSummaryTable($data_rows, t('Registered Users'), $this->summaryData['users']);

    //Verified Users
    $this->addRowToSummaryTable($data_rows, t('Verified Users'), $this->summaryData['verified'], FALSE, $this->summaryData['users']);

    //Total Petitions
    $this->addRowToSummaryTable($data_rows, t('Total Petitions'), $this->summaryData['petitions']);

    //Active Petitions
    $this->addRowToSummaryTable($data_rows, t('Active'), $this->summaryData['active'], TRUE, $this->summaryData['petitions']);

    //Public
    $this->addRowToSummaryTable($data_rows, t('Public'), $this->summaryData['public'], TRUE, $this->summaryData['petitions']);

    //Awaiting Response
    $this->addRowToSummaryTable($data_rows, t('Awaiting Reponse'), $this->summaryData['awaiting_response'], TRUE, $this->summaryData['petitions']);

    //Responded To
    $this->addRowToSummaryTable($data_rows, t('Responded To'), $this->summaryData['responded_to'], TRUE, $this->summaryData['petitions']);

    //Removed
    $this->addRowToSummaryTable($data_rows, t('Removed'), $this->summaryData['removed'], TRUE, $this->summaryData['petitions']);

    //Expired
    $this->addRowToSummaryTable($data_rows, t('Exprired'), $this->summaryData['expired'], TRUE, $this->summaryData['petitions']);

    //mongo signatures data
    $this->addRowToSummaryTable($data_rows, t('Signatures'), $this->summaryData['signatures']);

    //this is the wrong way to do output, but I'm in a bloody hurry and this is supposed to be a temporary solution
    $markup .= '<div id="petition-summary-report"><h2>' . t('Petition Summary Report') . '</h2>';
    $markup .= theme_table(array('header' => array(), 'rows' => $data_rows, 'attributes' => array('id' => 'petition-summary-table'), 'caption' => '', 'colgroups' => array(), 'sticky' => TRUE, 'empty' => t('No data found.')));
    $markup .= '</div><!-- Closing petition-summary-report div -->';

    return $markup;
  }

  /**
   * Build the Issue Summary Report table
   *
   * @return string
   *  HTML markup of table
   */
  public function getIssuesSummaryReportTable() {
    $markup = '';
    $data_rows = array();
    foreach ($this->issuesData as $title_cell => $data_cells) {
      //put data into table
      $this->addRowToIssuesTable($data_rows, $title_cell, $data_cells);
    }
    //this is the wrong way to do output, but I'm in a bloody hurry and this is supposed to be a temporary solution
    $markup .= '<div id="petition-issues-report"><h2>' . t('Petition Issues Summary Report') . '</h2>';
    $headers = array(
      t('Issue'),
      t('Petitions'),
      t('% of Total'),
      t('Public Petitions'),
      t('% of Total'),
      t('Signatures')
    );
    $markup .= theme_table(array('header' => $headers, 'rows' => $data_rows, 'attributes' => array('id' => 'petition-issues-table'), 'caption' => '', 'colgroups' => array(), 'sticky' => TRUE, 'empty' => t('No data found.')));
    $markup .= '</div><!-- Closing petition-issues-report div -->';

    return $markup;
  }


}



/**
 * Page callback form admin/petitino-reports/summary path
 */
function summary_page() {
  drupal_add_css(drupal_get_path('module', 'wh_petition_reporting') . '/css/summary.css', array('type' => 'file'));
  $reportManager = new SummaryReportManager();

  $markup = "<div id='petition-summary-page'>";

  //report header
  $markup .= $reportManager->getReportHeader();

  //Petition Summary Report table
  $markup .= $reportManager->getSummaryReportTable();

  //Petition Issues Summary Report table
  $markup .= $reportManager->getIssuesSummaryReportTable();

  $markup .= "</div><!--closing petition-summary-page div-->";
  return $markup;
}


