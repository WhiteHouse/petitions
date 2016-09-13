<?php

class PetitionsStatistics {
  const PETITION_CROSSED_SIGNATURE_THRESHOLD_1 = 'signatures_queue.workflow.process_signatures.notifications.wh_petitions_admin_signature_threshold_1';
  const PETITION_CROSSED_SIGNATURE_THRESHOLD_2 = 'signatures_queue.workflow.process_signatures.notifications.wh_petitions_admin_signature_threshold_2';
  const PETITION_CROSSED_SIGNATURE_THRESHOLD_3 = 'signatures_queue.workflow.process_signatures.notifications.wh_petitions_admin_signature_threshold_3';
  const PETITION_FRAUD_METRIC_BASE = 'signatures_queue.workflow.process_signatures.notifications.';
  const PETITION_SIGNATURE_RECEIVED_TO_INITIATED_VALIDATION = 'signatures_queue.data_flow.time_elapsed.received_new_signature__to__initiated_signature_validation';

  /**
   * Retrieve number of petitions created in time period.
   *
   * @param int $time_period
   *   Number of hours from now to include.
   *
   * @returns int
   *   Number of petitions created.
   */
  public static function getPetitionsCreated($time_period = 24) {
    $result = db_select('node', 'n')
      ->condition('n.type', 'petition', '=')
      ->where("created > UNIX_TIMESTAMP( DATE_SUB(NOW(), INTERVAL :hours HOUR) )", array(':hours' => $time_period))
      ->countQuery()->execute()
      ->fetchField();
    return $result;
  }

  /**
   * Retrieve number of petitions published in time period.
   *
   * @param int $time_period
   *   Number of hours from now to include.
   *
   * @returns int
   *   Number of petitions published.
   */
  public static function getPetitionsPublished($time_period = 24) {
    $result = db_select('field_data_field_timestamp_published')
      ->condition('entity_type', 'node', '=')
      ->condition('bundle', 'petition', '=')
      ->where("field_timestamp_published_value > UNIX_TIMESTAMP( DATE_SUB(NOW(), INTERVAL :hours HOUR) )", array(':hours' => $time_period))
      ->countQuery()->execute()->fetchField();
    return $result;
  }

  /**
   * Retrieve number of petitions reached public in time period.
   *
   * @param int $time_period
   *   Number of hours from now to include.
   *
   * @returns int
   *   Number of petitions reached public.
   */
  public static function getPetitionsReachedPublic($time_period = 24) {
    $result = db_select('field_data_field_timestamp_reached_public')
      ->condition('entity_type', 'node', '=')
      ->condition('bundle', 'petition', '=')
      ->where("field_timestamp_reached_public_value > UNIX_TIMESTAMP( DATE_SUB(NOW(), INTERVAL :hours HOUR) )", array(':hours' => $time_period))
      ->countQuery()->execute()->fetchField();
    return $result;
  }

  /**
   * Retrieve number of petitions reached ready for response in time period.
   *
   * @param int $time_period
   *   Number of hours from now to include.
   *
   * @returns int
   *   Number of petitions reached ready for response.
   */
  public static function getPetitionsReachedReadyForResponse($time_period = 24) {
    $result = db_select('field_data_field_timestamp_reached_ready')
      ->condition('entity_type', 'node', '=')
      ->condition('bundle', 'petition', '=')
      ->where("field_timestamp_reached_ready_value > UNIX_TIMESTAMP( DATE_SUB(NOW(), INTERVAL :hours HOUR) )", array(':hours' => $time_period))
      ->countQuery()->execute()->fetchField();
    return $result;
  }

  /**
   * Retrieve number of signatures started in time period.
   *
   * @param int $time_period
   *   Number of hours from now to include.
   *
   * @returns int
   *   Number of signatures started.
   */
  public static function getSignaturesStarted($time_period = 24) {
    $pending_signatures_count = db_select('signatures_pending_validation_queue')->where("timestamp_received_new_signature > UNIX_TIMESTAMP( DATE_SUB(NOW(), INTERVAL :hours HOUR) )", array(':hours' => $time_period))
      ->countQuery()
      ->execute()
      ->fetchField();

    $completed_signatures_count = db_select('signature_mail')->where("timestamp > UNIX_TIMESTAMP( DATE_SUB(NOW(), INTERVAL :hours HOUR) )", array(':hours' => $time_period))
      ->countQuery()
      ->execute()
      ->fetchField();

    return $pending_signatures_count + $completed_signatures_count;
  }

  /**
   * Retrieve number of signatures verified in time period.
   *
   * @param int $time_period
   *   Number of hours from now to include.
   *
   * @returns int
   *   Number of signatures verified.
   */
  public static function getSignaturesVerified($time_period = 24) {
    $result = db_select('signature_mail')->where("timestamp > UNIX_TIMESTAMP( DATE_SUB(NOW(), INTERVAL :hours HOUR) )", array(':hours' => $time_period))
      ->countQuery()
      ->execute()
      ->fetchField();

    return $result;
  }

  /**
   * Retrieve number of petitions crossing threshold in time period.
   *
   * @param int $threshold
   *   Threshold 1-3 to check.
   * @param int $time_period
   *   Number of hours from now to include.
   *
   * @returns int
   *   Number of petitions crossing threshold.
   */
  public static function getPetitionThresholdCrossed($threshold, $time_period = 24) {
    $description = constant("self::PETITION_CROSSED_SIGNATURE_THRESHOLD_{$threshold}");
    $result = db_select('event_log')
      ->condition('description', $description, '=')
      ->where("created > UNIX_TIMESTAMP( DATE_SUB(NOW(), INTERVAL :hours HOUR) )", array(':hours' => $time_period))
      ->countQuery()
      ->execute()
      ->fetchField();

    return $result;
  }

  /**
   * Retrieve number of petitions crossing threshold in time period.
   *
   * @param array $alerts
   *   Optional - Array of alerts to check, defaults to all.
   * @param int $time_period
   *   Number of hours from now to include.
   *
   * @returns array
   *   Array of data keyed by $result[alert_name][petition_id].
   *   - {alert_count} - Number of times alert occurred in time period.
   *   - {alert_name} - Shortened name of alert without fraud metric base.
   */
  public static function getPetitionAlerts($alerts = array(), $time_period = 24) {
    $open_petition_statuses = array(WH_PETITION_STATUS_PUBLIC, WH_PETITION_STATUS_UNDER_REVIEW);

    $query = db_select('event_log', 'el')
      ->fields('el', array('lid', 'description', 'ref_numeric'))
      ->condition('el.created', strtotime('-' . $time_period . ' hours'), '>')
      ->condition('fps.field_petition_status_value', $open_petition_statuses, 'IN');

    $query->join('field_data_field_petition_status', 'fps', 'fps.entity_id = el.ref_numeric');
    $query->join('node', 'n', 'n.nid = el.ref_numeric');
    $query->fields('n', array('title'));

    if (!empty($alerts)) {
      $query->condition('el.description', $alerts, 'IN');
    }
    else {
      $description = self::PETITION_FRAUD_METRIC_BASE . 'petitions_alert_threshold_';
      $query->condition('el.description', db_like($description) . '%', 'LIKE');
    }

    $results = $query->execute()->fetchAllAssoc('lid');
    $alert_results = array();
    if ($results) {
      foreach ($results as $data) {
        $alert_description = str_replace(self::PETITION_FRAUD_METRIC_BASE, '', $data->description);
        $alert_results[$alert_description][$data->ref_numeric]['alert_path'] = $data->description;
        $alert_results[$alert_description][$data->ref_numeric]['petition_title'] = $data->title;
      }
    }
    return $alert_results;
  }

  /**
   * Retrieve average elapsed daily time of signature initiated to validation.
   *
   * @param int $time_period
   *   Number of hours from now to include.
   *
   * @returns float
   *   Average time in ms between signature initiated to validation sent.
   */
  public static function getSignatureToInitiatedValidationElapsed($time_period = 24) {
    $description = self::PETITION_SIGNATURE_RECEIVED_TO_INITIATED_VALIDATION;
    $results = db_select('event_log')
      ->fields('event_log', array('description', 'ref_numeric', 'info'))
      ->condition('description', $description, '=')
      ->where("created > UNIX_TIMESTAMP( DATE_SUB(NOW(), INTERVAL :hours HOUR) )", array(':hours' => $time_period))
      ->execute();

    $avg_elapsed_time = 0;

    if ($results->rowCount()) {
      $total_elapsed_time = 0;

      while ($result = $results->fetchObject()) {
        $total_elapsed_time += $result->info;
      }

      // Time elapsed is delivered in microseconds, convert to milliseconds.
      $avg_elapsed_time = sprintf("%0.2f", ($total_elapsed_time / $results->rowCount()) / 100);
    }
    return $avg_elapsed_time;
  }

  /**
   * Get top signed open petitions.
   *
   * @param int $num_petitions
   *   Number of petitions to retrieve.
   *
   * @return mixed
   *   Array of petitions data.
   */
  public static function getTopSignedPetitions($num_petitions = 5) {
    $query = db_select('field_data_field_petition_signature_count', 'fpsc')
      ->fields('n', array('nid', 'title'))
      ->condition('fpsc.entity_type', 'node', '=')
      ->condition('fpsc.bundle', 'petition', '=')
      ->condition('fps.field_petition_status_value', WH_PETITION_STATUS_PUBLIC, '=');

    $query->addField('fpsc', 'field_petition_signature_count_value', 'signature_count');
    $query->addJoin('INNER', 'node', 'n', 'n.nid = fpsc.entity_id');
    $query->addJoin('INNER', 'field_data_field_petition_status', 'fps', 'fpsc.entity_id = fps.entity_id');
    $query->orderBy('fpsc.field_petition_signature_count_value', 'DESC');
    $query->range(0, $num_petitions);

    $results = $query->execute()->fetchAllAssoc('nid');
    return $results;
  }

  /**
   * Gather petition statistics for a time period.
   *
   * @param int $time_period
   *   Number of hours from now to include.
   *
   * @return object
   *   Object containing statistic data for time period
   */
  public static function getPetitionStatistics($time_period = 24) {
    $data = new stdClass();
    $data->time_period = $time_period;
    $data->petitions_created = self::getPetitionsCreated($time_period);
    $data->petitions_published = self::getPetitionsPublished($time_period);
    $data->petitions_reached_public = self::getPetitionsReachedPublic($time_period);
    $data->petitions_reached_ready_for_response = self::getPetitionsReachedReadyForResponse($time_period);
    $data->signatures_started = self::getSignaturesStarted($time_period);
    $data->signatures_verified = self::getSignaturesVerified($time_period);
    $data->signature_threshold_1 = variable_get('wh_petitions_admin_signature_threshold_1', 25000);
    $data->signature_threshold_2 = variable_get('wh_petitions_admin_signature_threshold_2', 50000);
    $data->signature_threshold_3 = variable_get('wh_petitions_admin_signature_threshold_3', 75000);
    $data->crossed_signature_threshold_1 = self::getPetitionThresholdCrossed(1, $time_period);
    $data->crossed_signature_threshold_2 = self::getPetitionThresholdCrossed(2, $time_period);
    $data->crossed_signature_threshold_3 = self::getPetitionThresholdCrossed(3, $time_period);
    $data->alerts = self::getPetitionAlerts(NULL, $time_period);
    $data->top_signed_petitions = self::getTopSignedPetitions();
    $data->signature_to_initiated_validation_elapsed = self::getSignatureToInitiatedValidationElapsed($time_period);

    return $data;
  }
}
