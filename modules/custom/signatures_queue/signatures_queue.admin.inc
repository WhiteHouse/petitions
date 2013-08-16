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
  $form = array();

  $form['all'] = array(
    '#type' => 'fieldset',
    '#title' => t('All workflows'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['all']['signatures_queue_log_events'] = array(
    '#type' => 'checkbox',
    '#title' => 'Log events to database',
    '#description' => t('Maintain a log of workflow events, including start and completion times of each job, for reporting. Turn this off to reduce load on the database. Turn it on for debugging.'),
    '#default_value' => variable_get('signatures_queue_log_events', SIGNATURES_QUEUE_LOG_EVENTS_DEFAULT),
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

  // Signature validation email settings.
  $signature_validation_email_token_help = t('Available variables are: [site:name], [signature:first-name], [signature:last-name], [signature:zip], [signature:email], [signature:signup], [signature:timestamp-submitted], [signature:secret-validation-key], [signature:signature-md5-encryption-key], [signature:signature-source-application-key], [signature:validation-url], [petition:title], [petition:url].');

  $form['signature_validation_email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Edit the signature validation email'),
    '#description' => t('Edit the signature validation e-mail message.') . ' ' . $signature_validation_email_token_help,
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['signature_validation_email']['user_mail_initiate_signature_validation_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => _signatures_queue_validation_mail_text('initiate_signature_validation_subject', NULL, array(), FALSE),
    '#maxlength' => 180,
  );
  $form['signature_validation_email']['user_mail_initiate_signature_validation_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => _signatures_queue_validation_mail_text('initiate_signature_validation_body', NULL, array(), FALSE),
    '#rows' => 15,
  );

  $form['preprocess_signatures'] = array(
    '#type' => 'fieldset',
    '#title' => t('Step 4: Preprocess Signatures'),
    '#description' => t('Store pending and validated signatures in table for matching in next step.'),
    '#weight' => 4,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['preprocess_signatures']['signatures_queue_preprocess_batch_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Batch size'),
    '#default_value' => variable_get('signatures_queue_preprocess_batch_size', 100),
    '#size' => 12,
    '#maxlength' => 12,
    '#description' => t('Number of signatures to preprocess in a batch.'),
    '#required' => TRUE,
  );

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
