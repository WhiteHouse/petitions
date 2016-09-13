<?php

/**
 * @file
 * Contains SignaturesQueue.
 */

/**
 * Factory class for interacting with signature queues.
 */
class SignaturesQueue extends DrupalQueue {

  /**
   * {@inheritdoc}
   */
  public static function get($name, $reliable = FALSE) {

    // Make sure the queue being created is a valid SignaturesQueue queue name.
    if (!self::isValidQueueName($name)) {
      watchdog('signatures_queue', '@name is not a valid queue name.', array('@name' => $name));
      return;
    }

    // Return the queue object.
    try {
      return DrupalQueue::get($name, $reliable);
    }
    catch (Exception $e) {
      logger_event('exceptions.signatures_queue.2d9873a');
      throw $e;
    }
  }

  /**
   * Tests whether a given queue name is valid.
   *
   * @param string $name
   *   The queue name to test.
   *
   * @param bool $show_messages
   *   TRUE if messages should be shown to the user or FALSE if they should not.
   *
   * @return bool
   *   TRUE if the queue name is valid or FALSE if it is not.
   */
  public static function isValidQueueName($name, $show_messages = FALSE) {
    $is_valid = TRUE;
    $text = '';

    // Enforce naming conventions. Prevent confusion between tables used for
    // processing and queues.
    if (substr($name, -6) != '_queue') {
      $text .= t('Valid signatures_queue queues end with the suffix "_queue".');
      $is_valid = FALSE;
    }

    // Validate queue name.
    if (!in_array($name, array_keys(signatures_queue_get_queue_names()))) {
      $text .= t("Valid signatures_queue queues must be included in the list returned by: signatures_queue_get_queue_names().");
      $is_valid = FALSE;
    }

    if (!$is_valid && $show_messages) {
      $text = t('Invalid queue name: @name.', array('@name' => $name));
      drupal_set_message($text, 'warning');
    }

    return $is_valid;
  }
}
