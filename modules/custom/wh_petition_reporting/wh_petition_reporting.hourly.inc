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
class HourlyReportManager {
  private $debug = FALSE;
  
  private $lastHourRun;

  /**
   * Constructor
   *
   * Checks time of last build and sets indicator if new build is needed
   *
   * @todo: figour out what the default variable should be
   */
  function __construct($debug = FALSE) {
    $this->debug = $debug;
 
    $this->lastHourRun = variable_get('wh_petition_reporting_hourly_last_run_hour', 1320105600);  //November 1, 2011 at 00:00:00

    $this->buildDataSet();
  }

  /**
   * Generates header including date report data was aggregated
   *
   */
  public function getReportHeader() {
    return '<h2>' . t('Data as of %date', array('%date' => date('m-d-Y H:i:s', $this->lastHourRun))) . '</h2>';
  }

  /**
   * Build the page markup for display
   */
  public function loadPage() {
    $markup = "";
    $data_rows = array();

    $dataCollection = mongodb_collection('hourly_report_data');
    $dataSet = $dataCollection->find()->sort(array('_id' => -1));

    foreach($dataSet as $row) {
      $this->addRowToHourlyTable($data_rows, $row);
    }    

    $markup .= '<div id="petition-hourly-report"><h2>' . t('Users, Petitions, and Signatures by Hour') . '</h2>';
    $headers = array(
      t('Date Range'),
      t('Total Users'),
      t('New Users'),
      t('Total Petitions'),
      t('New Petitions'),
      t('Total Signatures'),
      t('New Signatures'),
    );
    $markup .= theme_table(array('header' => $headers, 'rows' => $data_rows, 'attributes' => array('id' => 'hourly-count-table'), 'caption' => '', 'colgroups' => array(), 'sticky' => TRUE, 'empty' => t('No data found')));
    $markup .= '</div><!-- Closing petition-hourly-report div -->';

    return $markup;
  }

  private function addRowToHourlyTable(&$data_rows, $data) {
     $dateTimeFormat = 'Y-m-d H:i';
     $dateFormat = 'Y-m-d';
     $timeFormat = 'H:i';
     $from = date($dateTimeFormat, $data['_id']['startTS']);

     //only display full date for TO if date has changed, i.e., if the row is showing from 2300 to 0000
     if (date($dateFormat, $data['_id']['startTS']) == date($dateFormat, $data['_id']['endTS'])) {
       $to   = date($timeFormat, $data['_id']['endTS']); 
     }
     else {
       $to   = date($dateTimeFormat, $data['_id']['endTS']); 
     }
     
     $date_range_cell = array('data' => $from . ' to ' . $to, 'header' => TRUE); 

     $newUsers = number_format($data['newUsers']);
     $totalUsers = number_format($data['totalUsers']);
     $newPetitions = number_format($data['newPetitions']);
     $totalPetitions = number_format($data['totalPetitions']);
     $newSignatures = number_format($data['newSignatures']);
     $totalSignatures = number_format($data['totalSignatures']);

     $data_rows[] = array('data' => 
           array(
               $date_range_cell, 
               $totalUsers, 
               $newUsers, 
               $totalPetitions, 
               $newPetitions, 
               $totalSignatures,
               $newSignatures, 
           ),
     );
  }

  /**
   *  Check for new data and aggregate them into the set
   */
  private function buildDataSet() {
    $currentTS = time();
    $currentHourTS = mktime(date('H', $currentTS), 0, 0, date('n', $currentTS), date('j', $currentTS), date('Y', $currentTS) );

    $hourInterval = 3600; //number of seconds in an hour
    $max = $currentHourTS;
    $min = $currentHourTS - $hourInterval;
 
    //put all the processing into a batch
    $operations = array();
    while ($min >= $this->lastHourRun) {
      $operations[] = array('hourBatchProcessor', array($min, $max));
      $max = $min;
      $min = $min - $hourInterval;
    }
    
    $this->updateLastRun($currentHourTS);

    if (count($operations)) {
      $batch = array(
        'operations' => $operations,
        'init_message' => t('Beginning processing of new data'),
        'title' => t('Aggregating new data'),
        'progress_message' => t('Completed aggregating @current of @total hours of new data'),
        'file' => drupal_get_path('module', 'wh_petition_reporting') . '/wh_petition_reporting.hourly.inc',
      );
      batch_set($batch);
      batch_process();
    } 
  }

  /**
   * update the Drupal var and class attribte marking the last hour for which this ran
   */
  private function updateLastRun($lastRunTS) {
    variable_set('wh_petition_reporting_hourly_last_run_hour', $lastRunTS);
    $this->lastHourRun = $lastRunTS;
  }

  /**
   *  Wrapper to handle retireving data for an hour and writing it to the mongo table
   */
  public function processHourRecord($hour_start_timestamp, $hour_end_timestamp) {
    HourlyReportManager::writeHourRecord(HourlyReportManager::getHourRecord($hour_start_timestamp, $hour_end_timestamp));     
  }

  /**
   * Build petition data list
   *
   * @param $hour_start_timestamp int
   *   Unix timestamp for when the hour to get should start
   * @param $hour_end_timestamp int
   *   Unix timestamp for when the hour to get should end
   * @param $delta_only boolean
   *   Indicator of whether to get the total vlues or just the new records in that hour
   *
   * @return array
   */
  public function getHourRecord($hour_start_timestamp, $hour_end_timestamp, $delta_only = FALSE) {

    //set up connections
    $petitionsCollection = wh_petitions_mongo_petition_connection();
    $signaturesCollection = wh_petitions_mongo_petition_signatures_connection();

    //petitions
    $newPetitionCount = $petitionsCollection->count(
      array('created' => 
        array(
          '$lt'  => (int) $hour_end_timestamp,
          '$gte' => (int) $hour_start_timestamp,
        )
      )
    );
    //signatures
    $newSignatureCount = $signaturesCollection->count(
      array('timestamp' => 
        array(
          '$lt'  => (int) $hour_end_timestamp,
          '$gte' => (int) $hour_start_timestamp,
        )
      )
    );
        //users
    $newUserCountQuery = "select count(*) from {users} where created >= :start and created < :end";
    $newUserCount = db_query($newUserCountQuery, array(':start' => $hour_start_timestamp, ':end' => $hour_end_timestamp))->fetchField();

    //push all the data into a new object which will populate our Mongo table
    $newObject = array(
      '_id' => array('startTS' => $hour_start_timestamp, 'endTS' => $hour_end_timestamp),
      'newUsers' => (int) $newUserCount,
      'newPetitions' => (int) $newPetitionCount,
      'newSignatures' => (int) $newSignatureCount,
    );
   
    if (!$delta_only) {
      //if we're not asking for only the deltas, get the total counts too
      $totalPetitionCount = $petitionsCollection->count(
          array('created' => array('$lt' => (int) $hour_end_timestamp))
      );
    
      $totalSignatureCount = $signaturesCollection->count(
          array('timestamp' => array('$lt' => (int) $hour_end_timestamp))
      );
      
      $totalUserCountQuery = "select count(*) from {users} where created < :end";
      $totalUserCount = db_query($totalUserCountQuery, array(':end' => $hour_end_timestamp))->fetchField();
      
      $newObject['totalUsers'] = (int) $totalUserCount;
      $newObject['totalPetitions'] = (int) $totalPetitionCount;
      $newObject['totalSignatures'] = (int) $totalSignatureCount;
    }
    return $newObject;
  }

  /**
   *  wte a record to the mongo table
   */
  public function writeHourRecord($dataObject) { 
    $collection = mongodb_collection('hourly_report_data');
    $collection->save($dataObject);
  }
}

/**
 * Build the hourly report page
 */
function hourly_page() {
  drupal_add_css(drupal_get_path('module', 'wh_petition_reporting') . '/css/counts.css', array('type' => 'file'));
  $reportManager = new HourlyReportManager(TRUE);

  $markup = "<div id='petition-hourly-count-page'>";

  //report header
  $markup .= $reportManager->getReportHeader();
  
  //report body
  $markup .= $reportManager->loadPage();

  $markup .= "</div><!--closing petition-summary-page div-->";
  return $markup;

}

/**
 * callback function for the batch api
 */
function hourBatchProcessor($min, $max, &$context) {
  HourlyReportManager::processHourRecord($min, $max);     
}
