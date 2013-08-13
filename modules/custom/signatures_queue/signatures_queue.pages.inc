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
  // Confirm that expected parameters were submitted via URL.
  if (isset($_GET['k']) && isset($_GET['m']) && isset($_GET['p'])) {
    // Confirm that 'm' is the MD5 hash of 'k'.
    if ($_GET['m'] == md5($_GET['k'])) {
      // Load the petition via the API retrieve method.
      $petition = PetitionsRetrieveRawFactory::create()->load($_GET['p']);
      // Confirm that petition 'p' hasn't expired.
      if ($petition['results'][0]['status'] == 'open') {
        //Validate the signature.
        $options = array(
          'validation_key' => $_GET['k'],
          'petition_id' => $_GET['p'],
        );
        $response = signatures_queue_invoke_workflow('receive_signature_validation', SIGNATURES_QUEUE_SERVER_NAME_DEFAULT, SIGNATURES_QUEUE_WORKER_NAME_DEFAULT, $options);
        return $response;
      }
      // The petition has expired. Return error.
      else {
        return FALSE;
      }
    }
    // Hashes do not match. Return error.
    else {
      return FALSE;
    }
  }
  // Parameters are missing. Return error.
  else {
    return FALSE;
  }
}