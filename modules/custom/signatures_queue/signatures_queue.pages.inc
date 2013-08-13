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
 * Callback for thank-you.
 *
 * Compares the second parameter in the request to the MD5 hash of the first
 * parameter (the unique signature validation key).
 */
function _signatures_queue_validation_page_callback() {
  $output = '';

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
        // If the workflow succeeds (shunt isn't tripped) return output.
        if ($response == TRUE) {
          $output = "Thank you. Your signature has been counted.";
        }
        // If the workflow fails (shunt is tripped) drupal_goto() apology page.
        else {
          drupal_goto('validation-error', array(), 503);
        }
      }
      // The petition has been removed, expired, or doesn't exist. Return error.
      else {
        drupal_goto('petition-error', array(), 404);
      }
    }
    // Hashes do not match. Return error.
    else {
      drupal_not_found();
    }
  }
  // Parameters are missing. Return error.
  else {
    drupal_not_found();
  }

  return $output;
}

/**
 * Callback for validation-error.
 */
function _signatures_queue_validation_error_page_callback() {
  $output = "This service is temporarily unavailable. Please try again later.";
  return $output;
}

/**
 * Callback fo petition-error.
 */
function _signatures_queue_petition_error_page_callback() {
  $output = "Sorry, your petition has been removed, expired, or doesn't exist.";
  return $output;
}
