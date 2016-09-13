<?php

/**
 * @file
 * Page callbacks for signatures_queue module.
 */


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

    case SIGNATURES_QUEUE_ERROR_FAILED_VALIDATION:
    case SIGNATURES_QUEUE_ERROR_SIGNATURE_NOT_FOUND:
      drupal_not_found();
      break;

    case SIGNATURES_QUEUE_ERROR_SERVICE_NOT_AVAILABLE:
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

  $petition_item = PetitionsController::loadObject($petition_id);

  if (!empty($petition_item)) {
    // Call the share_links render array.
    $node = node_load($petition_id);
    $share_links = theme('petitions_share_links', array('node' => $node));

    $hpw_block = block_load('petitions_blocks', 'how_petitions_work_block');
    $hpw_render_array = _block_get_renderable_array(_block_render_blocks(array($hpw_block)));
    $hpw_output = drupal_render($hpw_render_array);

    $popular_title = array(
      '#theme' => 'html_tag',
      '#tag' => 'h2',
      '#value' => t('Popular Petitions'),
      '#attributes' => array(
        'class' => array(
          'title',
        ),
      ),
    );
    $popular_output = views_embed_view('petitions_listing', 'popular_petitions');

    // Done here because view Custom Link doesn't allow anchors.
    $more_link = array(
      '#theme' => 'html_tag',
      '#tag' => 'div',
      '#value' => l(t('View more popular petitions'), '/', array('fragment' => 'signapetition')),
      '#attributes' => array(
        'class' => array(
          'more-link',
        ),
      ),
    );
    $popular_output .= drupal_render($more_link);

    $message = t("You've successfully signed the petition below. Your signature has been verified and counted.");

    $thank_you = theme('wh_petitions_page_thank_you', array(
      'thank_you_text' => $message,
      'petition_url' => $petition_item->getNiceUrl(TRUE),
      'short_url' => $petition_item->getShortUrl(),
      'petition_title' => check_plain($petition_item->getTitle()),
      'share_links' => $share_links,
    ));

    $thank_you_wrapper = array(
      '#theme' => 'html_tag',
      '#tag' => 'div',
      '#value' => $thank_you,
      '#attributes' => array(
        'id' => array(
          'petition-create-formwrapper',
        ),
      ),
    );

    $hpw_wrapper = array(
      '#theme' => 'html_tag',
      '#tag' => 'div',
      '#value' => $hpw_output,
      '#attributes' => array(
        'class' => array(
          'how-petitions-work',
        ),
      ),
    );

    $popular_petitions_wrapper = array(
      '#theme' => 'html_tag',
      '#tag' => 'div',
      '#value' => drupal_render($popular_title) . $popular_output,
      '#attributes' => array(
        'class' => array(
          'popular-petitions',
          'block-popular-petitions',
          'block',
          'block-views',
        ),
      ),
    );

    return drupal_render($thank_you_wrapper) . drupal_render($hpw_wrapper) . drupal_render($popular_petitions_wrapper);
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
