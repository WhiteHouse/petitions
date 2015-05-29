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
 * Users hit this page when they click a validation link in an email to confirm
 * their signature. The link will look like this:
 *
 *   /thank-you?k=[secret-validation-key]&m=[md5-hash-of-key]&p=[petition-id]
 *
 * This page invokes the receive_signature_validation workflow which returns a
 * URL for a drupal_goto(), which will say "thank you" or provide and error
 * message to the user.
 */
function _signatures_queue_validation_page_callback() {
  // Sanitize data coming in through URL.
  $validation_key = check_plain($_GET['k']);
  $md5 = check_plain($_GET['m']);
  $petition_id = check_plain($_GET['p']);
  $validation_expiration = check_plain($_GET['d']);

  // Pass info from URL to signatures_queue_receive_signature_validation
  // workflow via $options array.
  $options = array(
    'secret_validation_key' => $validation_key,
    'md5' => $md5,
    'petition_id' => $petition_id,
    'validation_expiration' => $validation_expiration,
  );

  // Invoke the workflow.
  $response = signatures_queue_invoke_workflow('receive_signature_validation', $options);

  // Redirect the user according to the status response.
  switch ($response) {
    case SIGNATURES_QUEUE_STATUS_OK:
      drupal_goto("thank-you/{$petition_id}", array(), 301);
      break;

    case SIGNATURES_QUEUE_STATUS_BAD_REQUEST:
    case SIGNATURES_QUEUE_STATUS_NOT_FOUND:
      drupal_not_found();
      break;

    case SIGNATURES_QUEUE_STATUS_SERVER_ERROR:
    default:
      // Use HTTP 307 Temporary Redirect since a server error should have only
      // an "accidental" and temporary relationship with a given validation URL.
      drupal_goto('validation-error', array(), 307);
  }
}

/**
 * Callback for thank-you/%.
 */
function _signatures_queue_validated_page_callback($petition_id) {

  $petition = PetitionsController::loadObject($petition_id);

  if (!empty($petition)) {
    $short_url = $petition->getShortUrl();
    $nice_url = $petition->getNiceUrl();
    $url = (!empty($short_url)) ? $short_url : $nice_url;
    $plain_petition_id = check_plain($petition_id);
    $plain_petition_title = check_plain($petition->getTitle());
    return theme('signatures_queue_validation_thank_you', array(
      'petition' => $petition,
      'page_title' => t('Thank you for signing this petition!'),
      'message' => t('<p>You\'ve successfully signed the petition <a href="@petition_link">"!title"</a>. Your signature has been verified and counted.</p><p>Take the next step and gather support for it by sharing this petition on Twitter and Facebook, or by sharing a link to this petition: <a href="@petition_link">@petition_link</a></p>', array(
        '@petition_link' => url($url, array('absolute' => TRUE)),
        '!title' => drupal_ucfirst($plain_petition_title),
      )),
      'email_link' => wh_petition_tool_email_link(t('E-mail'), t('A We the People Petition'), t('I wanted to share this We the People petition with you: @petition_link', array(
        '@petition_link' => url($url, array('absolute' => TRUE)),
      ))),
      'twitter_link' => wh_petition_tool_twitter_link($plain_petition_id, $plain_petition_title, 'petition', '', $short_url, $nice_url),
      'facebook_link' => wh_petition_tool_facebook_link($plain_petition_id, $plain_petition_title, 'petition', '', $nice_url),
    ));
  }
  else {
    drupal_goto('petition-error', array(), 301);
  }
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
