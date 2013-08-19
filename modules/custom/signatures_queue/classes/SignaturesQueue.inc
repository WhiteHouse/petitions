<?php
/**
 * @file
 * Provides SignaturesQueue class to handle special signature queuing logic.
 */

/**
 * TODO: This class is intended to manage special logic for scaling signature
 *   queuing. For now, this is really just a placeholder so we can instantiate
 *   SignaturesQueue objects while building out the back-end for per-signature
 *   validation. Later, under PT-960 when we move the backend for these queues
 *   out of mysql into AWS SQS, we'll add additional logic.
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

    // @todo PT-960 - Insert logic before returning to check how many workers
    // are running and create additional queues as-needed.
    return DrupalQueue::get($name, $reliable);
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

    // Make sure nobody is creating numbered queues manually. This is our job.
    if (is_numeric(substr($name, -1))) {
      $text .= t("Don't instantiate a numbered signatures queue SignaturesQueue will help you scale horizontally by creating and decommissioning numbered queues as needed.");
      $is_valid = FALSE;
    }

    // Enforce naming conventions. Prevent confussion between tables used for
    // processing and queues.
    if (substr($name, -6) != '_queue') {
      $text .= t('Valid signatures_queue queues end with the suffix "_queue".');
      $is_valid = FALSE;
    }

    // Note: This next check is probably a bit rigid and simplistic. For now,
    // this is an easy way to enforce some basic documentation. In the future,
    // if other modules are going to hook in and extend the signatures queue
    // (e.g. maybe people who sign some special type of petition, or signing via
    // some third party like Facebook that has already validated the signer's
    // identity, modules could add additional queues and processing steps, or
    // skip certain steps), we should make this a proper info hook, and provide
    // some clarity on how implementing modules hook in and alter the queue
    // processing workflow.
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

  /**
   * Gets a dummy item.
   *
   * @param string $name
   *   Valid queue name.
   *
   * @return array
   *   Dummy data for specified queue.
   */
  public static function getDummyItem($name) {
    $item = array();
    switch ($name) {
      case 'signatures_submitted_queue':
        $item['petition_id'] = self::generateRandomPetitionID();
        $item['first_name'] = self::generateRandomString(6);
        $item['last_name'] = self::generateRandomString(8);
        $item['zip'] = rand(10000, 99999);
        $item['email'] = $item['first_name'] . '@example.com';
        $item['signup'] = (rand(0, 100) >= 50) ? TRUE : FALSE;
        $item['timestamp_submitted'] = time();
        $item['signature_source_api_key'] = self::generateRandomString(20);
        break;

      case 'signatures_pending_validation_queue':
        $salt = variable_get('signatures_queue_salt', 'some random string');
        $item['petition_id'] = self::generateRandomPetitionID();
        $item['timestamp_petition_close'] = time() + 10 * 24 * 60 * 60;
        $item['first_name'] = self::generateRandomString(6);
        $item['last_name'] = self::generateRandomString(8);
        $item['zip'] = rand(10000, 99999);
        $item['email'] = $item['first_name'] . '@example.com';
        $item['signup'] = (rand(0, 100) >= 50) ? TRUE : FALSE;
        $item['timestamp_validation_email_sent'] = time();
        $item['timestamp_submitted'] = $item['timestamp_validation_email_sent'] - 60;
        $item['secret_validation_key'] = sha1($item['email'] . $item['timestamp_submitted'] . $item['petition_id'] . $salt);
        $item['signature_source_api_key'] = self::generateRandomString(20);
        break;

      case 'signatures_validated_queue':
        $salt = variable_get('signatures_queue_salt', 'some random string');
        $email = 'jane.smith@example.com';
        $timestamp = time();
        $item['petition_id'] = self::generateRandomPetitionID();
        $item['secret_validation_key'] = sha1($email . $timestamp . $item['petition_id'] . $salt);
        $item['timestamp_validated'] = time();
        $item['client_ip'] = ip_address();
        break;

      default:
        $item['error'] = t('Sorry. The getDummyItem method does not support queues named: @name.', array('@name' => $name));
    }

    return $item;
  }

  /**
   * Returns a random petition ID associated to an open petition, or generates a
   * random petition ID.
   *
   * @return string
   *   A random petition ID associated to an open petition, or a 25 character
   *   random alpha-numeric ID. This is like mongo-generated petition IDs.
   */
  protected static function generateRandomPetitionID() {
    $petitions = PetitionsIndexRawFactory::create()->setStatus('open')->load();
    if (!empty($petitions['results'] && $petition['results'][0]['deadline'] > time()) {
      shuffle($petitions['results']);
      return $petitions['results'][0]['id'];
    }
    else {
      return self::generateRandomString(25);
    }
  }

  /**
   * Generates a random string.
   *
   * @param int $length
   *   Desired length.
   *
   * @return string
   *   Random alpha-numeric string.
   */
  protected static function generateRandomString($length = 10) {
    $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $random_string = '';
    for ($i = 0; $i < $length; $i++) {
      $random_string .= $characters[rand(0, strlen($characters) - 1)];
    }
    return $random_string;
  }
}
