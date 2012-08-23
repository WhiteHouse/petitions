<?php

// Report of the GovDelivery Queues status.
// Subject lines are links to a details page.
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function govdelivery_message_queue_report() {
  $queue = 'govdelivery_message_queue';
  // Setup table fields mapping to the data.
  $header = array(
    array('data' => 'Subject'),
    array(
      'data' => 'Created',
      'field' => 'created',
      'sort' => 'DESC',
    ),
    array('data' => 'Form'),
  );

  // Query the database for the data.
  // $dataquery = "SELECT FROM_DAYS(TO_DAYS(FROM_UNIXTIME(created))) day, cid FROM {" . $queue . "} GROUP BY day, cid " . tablesort_sql($header);
  // $countquery = "SELECT COUNT(*) FROM {" . $queue . "}";
  // $result = pager_query($dataquery, 30, 0, $countquery);

  $query = db_select('govdelivery_message_queue', 'q')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->limit(30)
    ->fields('q', array('created', 'cid'))
    ->groupBy('created')
    ->groupBy('cid');
  $result = $query->execute();

  // Assign data to rows.
  $rows = array();

  foreach ($result as $queuedmessage) {
// Get the message unserialized.
    $queuedmessage->data = govdelivery_queue_get($queue, $queuedmessage->cid);
// Build the row.
    if (isset($queuedmessage->data) && !empty($queuedmessage->data)) {
      $subject_text = $queuedmessage->data['subject'] ? $queuedmessage->data['subject'] : t('(no subject)');
      $subject_link = l($subject_text, 'admin/reports/govdelivery_message_queue/details/' . $queuedmessage->cid);
      $rows[] = array($subject_link, $queuedmessage->created, check_plain($queuedmessage->data['id']));
    }
  }
  // End foreach.

  // Empty dataset message.
  if (empty($rows)) {
    $rows[] = array(array(
        'data' => t('No messages currently in the queue.'),
        'colspan' => 3,
      ));
  }

  $output  = theme('table', array('header' => $header, 'rows' => $rows));
  $output .= theme('pager', array('tags' => NULL, 'element' => 0));

  return $output;
}
// End govdelivery_message_queue_status.

// View a single message in the queue.
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function govdelivery_details($cid) {
  $output = '';
  
  $queue = 'govdelivery_message_queue';
  // Build a table to display the message details.
  if (!empty($cid)) {
    // Get the message unserialized.
    $queuedmessage = govdelivery_queue_get($queue, $cid);
    if ($queuedmessage != 0) {
      // Setup table fields mapping to the data.
      $header = array(
        array('data' => t('Field')),
        array('data' => t('Value')),
      );
      // Setup the rows.
      $rows = array(
        array('CID', check_plain($cid)),
        array('ID', check_plain($queuedmessage['id'])),
        array('Module', (isset($queuedmessage['module']) ? check_plain($queuedmessage['module']) : '')),
        array('Key', (isset($queuedmessage['key']) ? check_plain($queuedmessage['key']) : '')),
        array('To', check_plain($queuedmessage['to'])),
        array('Subject', (isset($queuedmessage['subject']) ? check_plain($queuedmessage['subject']) : '')),
        array('Body', (isset($queuedmessage['body']) ? check_plain($queuedmessage['body']) : '')),
      );
      // Language
      if (isset($queuedmessage['language']) && !empty($queuedmessage['language'])) {
        if (is_object($queuedmessage['language'])) {
          $rows[] = array('Language', check_plain($queuedmessage['language']->language));
        }
        else {
          $rows[] = array('Language', check_plain($queuedmessage['language']));
        }
      }
      //params
      if (isset($queuedmessage['params']) && is_array($queuedmessage['params'])) {
        foreach($queuedmessage['params'] as $key => $value) {
          $rows[] = array(check_plain($key), check_plain($value));
        }
      }
      //headers
      if (isset($queuedmessage['headers']) && is_array($queuedmessage['headers'])) {
        foreach($queuedmessage['headers'] as $key => $value) {
          $rows[] = array(check_plain($key), check_plain($value));
        }
      }

    }
  }

  // Empty dataset message.
  // If the data set is empty dont load the form.
  if (empty($rows)) {
    $rows[] = array(array(
        'data' => t('Message no longer in the queue.'),
        'colspan' => 2,
      ));
  }
  else {
    // Im in extreme disstress here as referenced in http://drupal.org/node/224333#unrendered
    // I don't have time or the will to rewrite this whole report.
    // @todo make this support views and vbo and provide a base report.
    $output .= drupal_render(drupal_get_form('govdelivery_details_form', check_plain($cid)));
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));

  return $output;
}

// A form to send a message immediately.
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function govdelivery_details_form($form, $cid) {
  $form = array(
    'govdelivery_details' => array(
      '#type' => 'fieldset',
      '#title' => t('Message Actions'),
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      'cid' => array(
        '#type' => 'value',
        '#value' => $cid,
      ),
      'send' => array(
        '#type' => 'submit',
        '#value' => t('Send Now'),
      ),
    ),
  );
  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function govdelivery_details_form_submit($form, &$postinfo) {
  $cid = $postinfo['build_info']['args'][0];
  // Send the message from the queue immediately.
  if (!empty($cid)) {
    $sent = govdelivery_process_message_queue(NULL, FALSE, $cid);
    drupal_set_message(t('Messages sent from the queue: @sent', array('@sent' => $sent)));
  }
  $postinfo['redirect'] = 'admin/reports/govdelivery_message_queue';
}
