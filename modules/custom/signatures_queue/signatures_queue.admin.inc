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

  $form['signatures_queue'] = array(
    '#type' => 'fieldset',
    '#title' => 'Signatures Queue',
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['signatures_queue']['signatures_queue_hello_world'] = array(
    '#type' => 'item',
    '#title' => 'Hello world!',
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
