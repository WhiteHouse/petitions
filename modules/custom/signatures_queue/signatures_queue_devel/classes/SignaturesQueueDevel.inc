<?php
/**
 * @file
 * Provides SignaturesQueueDev class to generate dummy data.
 */

/**
 *
 */
class SignaturesQueueDevel extends SignaturesQueue {

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
        $item['email'] = $item['first_name'] . '@mailinator.com';
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
        $item['email'] = $item['first_name'] . '@mailinator.com';
        $item['signup'] = (rand(0, 100) >= 50) ? TRUE : FALSE;
        $item['timestamp_validation_email_sent'] = time();
        $item['timestamp_submitted'] = $item['timestamp_validation_email_sent'] - 60;
        $item['secret_validation_key'] = sha1($item['email'] . $item['timestamp_submitted'] . $item['petition_id'] . $salt);
        $item['signature_source_api_key'] = self::generateRandomString(20);
        break;

      case 'signatures_validated_queue':
        $salt = variable_get('signatures_queue_salt', 'some random string');
        $email = 'jane.smith@mailinator.com';
        $timestamp = time();
        $item['petition_id'] = self::generateRandomPetitionID();
        $item['secret_validation_key'] = sha1($email . $timestamp . $item['petition_id'] . $salt);
        $item['timestamp_validated'] = $timestamp;
        $item['client_ip'] = ip_address();
        break;

      default:
        $item['error'] = t('Sorry. The getDummyItem method does not support queues named: @name.', array('@name' => $name));
    }

    return $item;
  }

  /**
   * Returns a random petition ID.
   *
   * Returns an ID associated with a real, open petition, if one is available,
   * or generates a random string if not.
   *
   * @return string
   *   A 25 character alpha-numeric ID.
   */
  protected static function generateRandomPetitionID() {
    // Get a real, open petition ID if one is available.
    $petitions = PetitionsIndexRawFactory::create()->setStatus('open')->load();
    if (!empty($petitions['results'])) {
      shuffle($petitions['results']);
      foreach ($petitions['results'] as $result) {
        if ($result['deadline'] > time()) {
          return $result['id'];
        }
      }
    }

    // Fall back to returning a random string.
    return self::generateRandomString(25);
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
