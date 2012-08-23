<?php

/**
 * @file
 * Implementation of drupal_mail_wrapper for the GovDelivery Integration module.
 */

class GovDeliveryMailSystem implements MailSystemInterface {

  public function format(array $message) {
    // Join the body array into one string.
    $message['body'] = implode("\n\n", $message['body']);
    // Convert any HTML to plain-text.
    //$message['body'] = drupal_html_to_text($message['body']);
    // Wrap the mail body for sending.
    $message['body'] = drupal_wrap_mail($message['body']);

    // @todo verify if this is really needed given the above function calls. Including it now just incase.
    // check for XML/XHTML or oldstyle html
    $xml_start = strpos(trim($message['body']), "<?xml");
    $html_start = strpos(trim($message['body']), "<html");
    
    // GovDelivery doesn't let us set the MIME type, it will send as text/html ... wrap with <pre> to
    // allow for clean formatting if message isn't XML or HTML already
    if (!is_numeric($xml_start) && !is_numeric($html_start)) {
      $message['body'] = '<pre>' . $message['body'] . '</pre>';
    }
    return $message;
  }

  public function mail(array $message) {
    $key = md5(print_r($message, TRUE) . microtime() . strval(rand()));
    govdelivery_process_message($key, $message);
    return TRUE;
  }
}
// end GovDeliveryMailSystem
