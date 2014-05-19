<?php

/**
 * @file
 * Defines SignaturesQueueDev.
 */

/**
 * Provides SignaturesQueueDev class to generate dummy data.
 */
class SignaturesQueueDevel extends SignaturesQueue {

  /**
   * Gets a dummy item.
   *
   * @param string $name
   *   Valid queue name.
   *
   * @param array $overrides
   *   Designate values to override whatever would normally be generated as
   *   dummy data by default.
   *
   * @return array
   *   Dummy data for specified queue.
   */
  public static function getDummyItem($name, $overrides = array()) {
    // Signatures overrides.
    $email                           = (!empty($overrides['email'])) ? $overrides['email'] : '';
    $first_name                      = (!empty($overrides['first_name'])) ? $overrides['first_name'] : '';
    $last_name                       = (!empty($overrides['last_name'])) ? $overrides['last_name'] : '';
    $petition_id                     = (!empty($overrides['petition_id'])) ? $overrides['petition_id'] : '';
    $signature_source_api_key        = (!empty($overrides['signature_source_api_key'])) ? $overrides['signature_source_api_key'] : '';
    $signup                          = (!empty($overrides['signup'])) ? $overrides['signup'] : 0;
    $timestamp_petition_close        = (!empty($overrides['timestamp_petition_close'])) ? $overrides['timestamp_petition_close'] : 0;
    $timestamp_submitted             = (!empty($overrides['timestamp_submitted'])) ? $overrides['timestamp_submitted'] : 0;
    $timestamp_validation_close      = (!empty($overrides['timestamp_validation_close'])) ? $overrides['timestamp_validation_close'] : 0;
    $timestamp_validation_email_sent = (!empty($overrides['timestamp_validation_email_sent'])) ? $overrides['timestamp_validation_email_sent'] : 0;
    $zip                             = (!empty($overrides['zip'])) ? $overrides['zip'] : '';
    $secret_validation_key           = (!empty($overrides['secret_validation_key'])) ? $overrides['secret_validation_key'] : '';
    $timestamp_processed             = (!empty($overrides['timestamp_processed'])) ? $overrides['timestamp_processed'] : 0;

    // Validations overrides.
    $timestamp_validated             = (!empty($overrides['timestamp_validated'])) ? $overrides['timestamp_validated'] : 0;
    $timestamp_validation_close      = (!empty($overrides['timestamp_validation_close'])) ? $overrides['timestamp_validation_close'] : 0;
    $client_ip                       = (!empty($overrides['client_ip'])) ? $overrides['client_ip'] : '';

    $item = array();
    switch ($name) {
      case 'signatures_submitted_queue':
        $grace_period = variable_get('signatures_queue_validation_grace_period', 0);
        $item['first_name'] = ($first_name) ? $first_name : self::generateRandomString(6);
        $item['last_name'] = ($last_name) ? $last_name : self::generateRandomString(8);
        $item['email'] = ($email) ? $email : $item['first_name'] . '@mailinator.com';
        $item['petition_id'] = ($petition_id) ? $petition_id : self::generateRandomPetitionID();
        $item['signature_source_api_key'] = ($signature_source_api_key) ? $signature_source_api_key : self::generateRandomString(20);
        $item['signup'] = ($signup) ? $signup : (rand(0, 100) >= 50) ? TRUE : FALSE;
        $item['timestamp_submitted'] = ($timestamp_submitted) ? $timestamp_submitted : time();
        // This just adds 10 days to time submitted.
        $item['timestamp_petition_close'] = ($timestamp_petition_close) ? $timestamp_petition_close : $item['timestamp_submitted'] + 864000;
        // This just adds 10 days to petition close.
        $item['timestamp_validation_close'] = ($timestamp_validation_close) ? $timestamp_validation_close : $item['timestamp_petition_close'] + 864000;
        $item['timestamp_validation_email_sent'] = ($timestamp_validation_email_sent) ? $timestamp_validation_email_sent : '';
        $item['zip'] = ($zip) ? $zip : rand(10000, 99999);
        $item['secret_validation_key'] = '';
        $item['timestamp_processed'] = ($timestamp_processed) ? $timestamp_processed : 0;
        break;

      case 'signatures_pending_validation_queue':
        $salt = variable_get('signatures_queue_salt', 'some random string');
        $grace_period = variable_get('signatures_queue_validation_grace_period', 0);
        $item['first_name'] = ($first_name) ? $first_name : self::generateRandomString(6);
        $item['last_name'] = ($last_name) ? $last_name : self::generateRandomString(8);
        $item['email'] = ($email) ? $email : $item['first_name'] . '@mailinator.com';
        $item['petition_id'] = ($petition_id) ? $petition_id : self::generateRandomPetitionID();
        $item['signature_source_api_key'] = ($signature_source_api_key) ? $signature_source_api_key : self::generateRandomString(20);
        $item['signup'] = ($signup) ? $signup : (rand(0, 100) >= 50) ? TRUE : FALSE;
        $item['timestamp_petition_close'] = ($timestamp_petition_close) ? $timestamp_petition_close : time() + 10 * 24 * 60 * 60;
        $item['timestamp_validation_close'] = ($timestamp_validation_close) ? $timestamp_validation_close : time() + $grace_period * 24 * 60 * 60;
        $item['timestamp_validation_email_sent'] = ($timestamp_validation_email_sent) ? $timestamp_validation_email_sent : time();
        $item['zip'] = ($zip) ? $zip : rand(10000, 99999);
        $item['timestamp_submitted'] = ($timestamp_submitted) ? $timestamp_submitted : $item['timestamp_validation_email_sent'] - 60;
        $item['secret_validation_key'] = ($secret_validation_key) ? $secret_validation_key : sha1(self::generateRandomString(25));
        $item['timestamp_processed'] = ($timestamp_processed) ? $timestamp_processed : 0;
        break;

      case 'validations_queue':
        $salt = variable_get('signatures_queue_salt', 'some random string');
        $grace_period = variable_get('signatures_queue_validation_grace_period', 0);
        $email = self::generateRandomString(10) . '@mailinator.com';
        $item['petition_id'] = ($petition_id) ? $petition_id : self::generateRandomPetitionID();
        $item['secret_validation_key'] = ($secret_validation_key) ? $secret_validation_key : sha1(self::generateRandomString(25));
        $item['timestamp_validated'] = ($timestamp_validated) ? $timestamp_validated : time();
        // Make sure this defaults to greater than timestamp_validated,
        // otherwise nothing gets counted.
        $item['timestamp_validation_close'] = ($timestamp_validation_close) ? $timestamp_validation_close : time() + 1 + $grace_period * 24 * 60 * 60;
        $item['client_ip'] = ($client_ip) ? $client_ip : ip_address();
        $item['timestamp_processed'] = ($timestamp_processed) ? $timestamp_processed : 0;
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
    $query = PetitionsSelectQueryFactory::create()->setStatus('open');
    $petitions = $query->execute()->getResult();

    if (!empty($petitions)) {
      shuffle($petitions);
      foreach ($petitions as $petition) {
        if ($petition['deadline'] > time()) {
          return $petition['id'];
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
