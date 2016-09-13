<?php
/**
 * @file
 * Provides admin settings for signatures_queue module.
 */

/**
 * Configuration for signatures queues.
 *
 * @see system_settings_form()
 */
function signatures_queue_configure() {
  module_load_include('inc', 'signatures_queue');

  $form = array();

  $form['all'] = array(
    '#type' => 'fieldset',
    '#title' => t('All workflows'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['all']['signatures_queue_debug'] = array(
    '#type' => 'checkbox',
    '#title' => 'Enable debugging',
    '#description' => t('Enable debugging mode. This does things like increase the amount of details logged via watchdog throughout the signature queue pipeline.'),
    '#default_value' => variable_get('signatures_queue_debug', FALSE),
  );
  $form['all']['signatures_queue_notify_email'] = array(
    '#type' => 'textfield',
    '#title' => 'Admin email to notify',
    '#description' => t('If the system detects an issue a notification message will be sent to this address. Defaults to address of user 1.'),
    '#default_value' => variable_get('signatures_queue_notify_email'),
  );

  $form['initiate_signature_validation'] = array(
    '#type' => 'fieldset',
    '#title' => t('"Initiate signature validation" workflow'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['initiate_signature_validation']['signatures_queue_salt'] = array(
    '#type' => 'textfield',
    '#title' => t('Salt'),
    '#description' => 'This makes secret validation keys truely secret.',
    '#default_value' => variable_get('signatures_queue_salt', ''),
  );
  $form['initiate_signature_validation']['signatures_queue_initiate_signature_validation_batch_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Batch size'),
    '#description' => 'The number of signatures to initiate validation for per batch.',
    '#default_value' => variable_get('signatures_queue_initiate_signature_validation_batch_size', SIGNATURES_QUEUE_BATCH_SIZE_DEFAULT),
  );

  $form['preprocess_signatures'] = array(
    '#type' => 'fieldset',
    '#title' => t('Step 4: Preprocess Signatures'),
    '#description' => t('Store pending and validated signatures in table for matching in next step.'),
    '#weight' => 4,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['preprocess_signatures']['signatures_queue_preprocess_signatures_batch_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Batch size'),
    '#default_value' => variable_get('signatures_queue_preprocess_signatures_batch_size', SIGNATURES_QUEUE_BATCH_SIZE_DEFAULT),
    '#description' => t('Number of signatures to preprocess in a batch.'),
  );

  $form['process_signatures'] = array(
    '#type' => 'fieldset',
    '#title' => t('Step 5: Process Signatures'),
    '#description' => t('Save and count validated signatures.'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['process_signatures']['signatures_queue_process_signatures_batch_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Batch size'),
    '#default_value' => variable_get('signatures_queue_process_signatures_batch_size', SIGNATURES_QUEUE_BATCH_SIZE_DEFAULT),
    '#description' => t('Number of signatures to process in a batch.'),
  );
  $form['process_signatures']['petitions_alerts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Petition Alert Settings'),
    '#description' => t('Fraud alert notification and threshold settings.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => FALSE,
  );
  $form['process_signatures']['petitions_alerts']['petitions_alert_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Petition Alert Emailing'),
    '#default_value' => variable_get('petitions_alert_enabled', FALSE),
    '#description' => t('Enables emailing of petition fraud alerts.'),
  );
  $form['process_signatures']['petitions_alerts']['petitions_alert_signature_threshold'] = array(
    '#type' => 'textfield',
    '#title' => t('Signature Threshold'),
    '#default_value' => variable_get('petitions_alert_signature_threshold', FALSE),
    '#description' => t('# of signatures before alerting will engage.'),
  );
  $form['process_signatures']['petitions_alerts']['petitions_alert_email_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Petition Alert Email'),
    '#default_value' => variable_get('petitions_alert_email_address'),
    '#description' => t('Email address(es) for petition fraud alerts. Separate multiple addresses with a comma.'),
  );
  $form['process_signatures']['petitions_alerts']['petitions_alert_email_throttle'] = array(
    '#type' => 'textfield',
    '#title' => t('Petition Alert Email Throttling'),
    '#default_value' => variable_get('petitions_alert_email_throttle'),
    '#description' => t('Number of seconds between alerts on a petition.'),
  );
  $form['process_signatures']['petitions_alerts']['thresholds'] = array(
    '#type' => 'item',
    '#markup' => '<label>Petition Alert Thresholds</label>',
    '#description' => '&nbsp;&nbsp;&nbsp;' . t('Ratios are from 0.0 to 1.0. Use values of 0 for lower and 1 for upper to disable.'),
  );
  $metric_fields = PetitionsSelectQueryMysql::getPetitionMetricFieldsMap();
  foreach ($metric_fields as $key => $field_params) {
    $lower_key = 'petitions_alert_threshold_lower_' . $key;
    $upper_key = 'petitions_alert_threshold_upper_' . $key;
    $title = $field_params['title'] . ':';

    $form['process_signatures']['petitions_alerts']['petitions_alert_threshold_' . $key] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#attributes' => array('class' => array('container-inline')),
      '#tree' => FALSE,
      $lower_key => array(
        '#type' => 'textfield',
        '#title' => $title,
        '#suffix' => ' <= Lower / Upper => ',
        '#size' => 10,
        '#default_value' => variable_get($lower_key),
      ),
      $upper_key => array(
        '#type' => 'textfield',
        '#default_value' => variable_get($upper_key),
        '#size' => 10,
      ),
    );
  }
  $form['archive_signatures'] = array(
    '#type' => 'fieldset',
    '#title' => t('Step 6: Archive Signatures'),
    '#description' => t('Archive signatures whose petitions have closed and were never validated.'),
    '#weight' => 6,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['archive_signatures']['signatures_queue_archive_invalid_signatures'] = array(
    '#type' => 'checkbox',
    '#title' => t('Archive Invalidated Signatures'),
    '#default_value' => variable_get('signatures_queue_archive_invalid_signatures', 1),
  );
  $form['archive_signatures']['signatures_queue_minimum_signature_lifetime'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum signature lifetime'),
    '#description' => t('A signature must live for at least this long before being archived, regardless of other expiration criteria. Use <a href="@url">relative date format</a>, e.g., "2 weeks".', array(
      '@url' => 'http://www.php.net/manual/en/datetime.formats.relative.php',
    )),
    '#default_value' => variable_get('signatures_queue_minimum_signature_lifetime', '2 weeks'),
  );
  $form['archive_signatures']['signatures_queue_validation_grace_period'] = array(
    '#type' => 'textfield',
    '#title' => t('Validation Grace Period (in Days)'),
    '#default_value' => variable_get('signatures_queue_validation_grace_period', 2),
    '#size' => 12,
    '#maxlength' => 12,
    '#description' => t('Number of days a signature validation link is valid.'),
    '#required' => TRUE,
  );
  $form['archive_signatures']['signatures_queue_archive_batch_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Archive batch size'),
    '#default_value' => variable_get('signatures_queue_archive_batch_size', 5000),
    '#size' => 12,
    '#maxlength' => 12,
    '#description' => t('The number of signatures to archive per batch.'),
    '#required' => TRUE,
  );

  $form['all']['#prefix'] = signatures_queue_overview();
  return system_settings_form($form);
}

/**
 * Generate dummy content in queues.
 */
function signatures_queue_generate_form() {
  $form = array();

  $queues = signatures_queue_get_queue_names();
  $real_queues = array();
  foreach ($queues as $name => $description) {
    if (SignaturesQueue::isValidQueueName($name)) {
      $real_queues[$name] = $name;
    }
  }

  $form['queues'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Signatures Queues'),
    '#description' => t('The "signatures queue" is really made up of several queues. Select which ones you want to generate records in.'),
    '#options' => $real_queues,
  );
  $form['delete_first'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete all content in these queues before generating new content'),
  );
  $form['number'] = array(
    '#type' => 'textfield',
    '#title' => t('How many signatures would you like to generate?'),
    '#size' => 60,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Generate'),
  );

  return $form;
}

/**
 * Submit handler for signatures_queue_generate form.
 */
function signatures_queue_generate_form_submit($form_id, &$form_state) {
  $number = $form_state['values']['number'];
  $delete_first = $form_state['values']['delete_first'];

  foreach ($form_state['values']['queues'] as $queue_name => $generate_records) {
    if ($generate_records) {
      signatures_queue_generate($queue_name, $number, $delete_first);
    }
  }
}

/**
 * Provide basic overview of the Signatures Queue system.
 */
function signatures_queue_overview() {
  $rows = array();
  $queues = signatures_queue_get_queue_names();
  foreach ($queues as $queue_name => $description) {
    $rows[] = array($queue_name, $description);
  }
  $rows[] = array(t('NOTE'), t("Real queues are named something_queue. The tables listed above that don't end in the suffix \"_queue\" aren't actually queues. Conceptually they're part of the \"signatures queue\" in the sense that these are holding bins where submitted signatures go before they're done being processed and counted. But they're not DrupalQueue queues. They cannot be instantiated with SignaturesQueue::get()."));

  $output = theme('table', array(
    'header' => array(
      t('Queue name'),
      t('Description')),
    'rows' => $rows,
  ));

  return $output;
}
