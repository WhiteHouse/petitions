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

  $queue_names = array();
  foreach ($form_state['values']['queues'] as $queue_name => $generate_records) {
    if ($generate_records) {
      $queue_names[] = $queue_name;
    }
  }
  signatures_queue_devel_generate($queue_names, $number, $delete_first, $num_match);
}
