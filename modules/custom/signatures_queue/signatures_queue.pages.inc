<?php
/**
 * @file
 * Page callbacks for signatures_queue module.
 */

/**
 * Callback for admin/config/system/signatures-queue/monitor.
 *
 * Provide basic dashboard for monitoring signatures queues.
 */
function signatures_queue_monitor_page() {
  return 'Hello world!';
}

/**
 * Callback for admin/config/system/petitions/signatures-queue.
 *
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

/**
 * Callback fo thank-you.
 *
 * Compares the second parameter in the request to the MD5 hash of the first
 * parameter (the unique signature validation key).
 */
function _signatures_queue_validation_page_callback() {
  $key = '';
  $md5 = '';
  $petition = '';

  if (isset($_GET['k']) && $_GET['k']) {
    $key = $_GET['k'];
  }

  if (isset($_GET['m']) && $_GET['m']) {
    $md5 = $_GET['m'];
  }

  if (isset($_GET['p']) && $_GET['p']) {
    $petition = $_GET['p'];
  }

  if ($md5 == md5($key)) {

  }
}