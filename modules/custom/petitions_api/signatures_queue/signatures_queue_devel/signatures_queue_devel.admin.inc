<?php
/**
 * @file
 * Provides admin form for generating dummy content.
 */

/**
 * Generate dummy content in queues.
 */
function signatures_queue_devel_generate_form() {
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
    '#default_value' => 10,
    '#size' => 60,
  );
  $form['number_match'] = array(
    '#type' => 'textfield',
    '#title' => t('How many generated signatures in the queues should match each other?'),
    '#description' => t('If greater than 0 then this is the number of items that will match. This value should be less than the total number to generate.'),
    '#default_value' => 0,
    '#size' => 60,
  );

  $form['overrides'] = array(
    '#type' => 'fieldset',
    '#title' => t('Override dummy data defaults with your own designated values here'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $overrides = _signatures_queue_devel_overrides();
  foreach ($overrides as $override) {
    $form['overrides'][$override] = array('#type' => 'textfield', '#title' => $override);
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Generate'),
  );

  return $form;
}

/**
 * Submit handler for signatures_queue_devel_generate form.
 */
function signatures_queue_devel_generate_form_submit($form_id, &$form_state) {
  $number = $form_state['values']['number'];
  $delete_first = $form_state['values']['delete_first'];
  $num_match = $form_state['values']['number_match'];

  $_overrides = _signatures_queue_devel_overrides();
  $overrides = array();
  foreach ($_overrides as $override) {
    $overrides[$override] = (!empty($form_state['values'][$override])) ? $form_state['values'][$override] : '';
  }

  $queue_names = array();
  foreach ($form_state['values']['queues'] as $queue_name => $generate_records) {
    if ($generate_records) {
      $queue_names[] = $queue_name;
    }
  }
  signatures_queue_devel_generate($queue_names, $number, $delete_first, $num_match, $overrides);
}

/**
 * Returns a list of fields for which dummy data will be generated.
 *
 * This is a little helper function to make it easy to make all these
 * overridable.
 *
 * @return array
 *   An indexed array of field names.
 */
function _signatures_queue_devel_overrides() {
  return array(
    // Signatures overrides.
    'email',
    'first_name',
    'last_name',
    'petition_id',
    'signature_source_api_key',
    'signup',
    'timestamp_petition_close',
    'timestamp_received_new_signature',
    'timestamp_validation_close',
    'timestamp_initiated_signature_validation',
    'zip',
    'secret_validation_key',
    'timestamp_processed_signature',
    // Validations overrides.
    'timestamp_received_signature_validation',
    'timestamp_validation_close',
    'client_ip',
  );
}
