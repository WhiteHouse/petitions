<?php

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function govdelivery_settings($form, &$form_state) {
  $govdelivery_odm_settings = variable_get('govdelivery_odm_settings', array());
  $govdelivery_subscription_settings = variable_get('govdelivery_subscription_settings', array());

  $account_fieldset = array(
    '#type' => 'fieldset',
    '#title' => t('Mail Accounts'),
  );

  if (isset($govdelivery_odm_settings) && isset($govdelivery_odm_settings['accounts']) && is_array($govdelivery_odm_settings['accounts'])) {
    foreach ($govdelivery_odm_settings['accounts'] as $username => $account_settings) {
      $account_fieldset[$username . '%###%'  . 'fromname'] = array(
        '#type' => "textfield",
        '#title' => t('From Name'),
        '#default_value' => check_plain((!empty($account_settings['fromname']) ? $account_settings['fromname'] : '')),
      );
      $account_fieldset[$username . '%###%' . 'username'] = array(
        '#type' => "textfield",
        '#title' => t('Username'),
        '#default_value' => check_plain((!empty($username) ? $username : '')),
      );
      $account_fieldset[$username . '%###%' . 'password'] = array(
        '#type' => "textfield",
        '#title' => t('Password'),
        '#default_value' => '',
      );
    }
  }

  $account_fieldset['fromname'] = array(
    '#type' => "textfield",
    '#title' => t('From Name'),
  );
  $account_fieldset['username'] = array(
    '#type' => "textfield",
    '#title' => t('Username'),
  );
  $account_fieldset['password'] = array(
    '#type' => "textfield",
    '#title' => t('Password'),
  );

  $subscription_fieldset = array(
    '#type' => 'fieldset',
    '#title' => t('Subscription API'),
    '#tree' => TRUE,
  );

  $subscription_fieldset['service_url'] = array(
    '#type' => "textfield",
    '#title' => t('URL for Subscription Service'),
    '#default_value' => (!empty($govdelivery_subscription_settings['service_url']) ? check_url($govdelivery_subscription_settings['service_url']) : ''),
  );

  $subscription_fieldset['cron_subscriptions'] = array(
    '#type' => 'radios',
    '#default_value' => (isset($govdelivery_subscription_settings['cron_subscriptions']) ? $govdelivery_subscription_settings['cron_subscriptions'] : 1),
    '#title' => t('Subscriptions Cron (Will automatically resend the subscriptions queue on cron runs)'),
    '#options' => array('Disabled', 'Enabled'),
  );

  $subscription_fieldset['subscriber_api_url_base'] = array(
    '#type' => 'textfield',
    '#title' => t('Base URL for Subscriber API'),
    '#default_value' => (!empty($govdelivery_subscription_settings['subscriber_api_url_base']) ? check_url($govdelivery_subscription_settings['subscriber_api_url_base']) : ''),
  );

  $subscription_fieldset['default_topic_category_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Category assigned to new Topics.'),
    '#default_value' => (!empty($govdelivery_subscription_settings['default_topic_category_id']) ? $govdelivery_subscription_settings['default_topic_category_id'] : ''),
  );

  $form = array(
    'govdelivery_odm_settings' => array(
      '#type' => 'fieldset',
      '#title' => t('Mail Server Settings'),
      '#tree' => TRUE,
      'accounts' => $account_fieldset,
      'server' => array(
        '#type' => "textfield",
        '#title' => 'server',
        '#default_value' => (!empty($govdelivery_odm_settings['server']) ? check_url($govdelivery_odm_settings['server']) : ''),
      ),
      'enabled' => array(
        '#type' => 'radios',
        '#default_value' => (isset($govdelivery_odm_settings['enabled']) ? $govdelivery_odm_settings['enabled'] : 0),
        '#title' => t('Outbound mail rerouting'),
        '#options' => array('Disabled', 'Enabled'),
      ),
      'queue' => array(
        '#type' => 'radios',
        '#default_value' => (isset($govdelivery_odm_settings['queue']) ? $govdelivery_odm_settings['queue'] : 1),
        '#title' => t('Queue Messages (Messages that fail to send are queued regardless of this setting)'),
        '#options' => array('Disabled', 'Enabled'),
      ),
      'cron_odm' => array(
        '#type' => 'radios',
        '#default_value' => (isset($govdelivery_odm_settings['cron_odm']) ? $govdelivery_odm_settings['cron_odm'] : 1),
        '#title' => t('Messages Cron (Will automatically resend the messages queue on cron runs)'),
        '#options' => array('Disabled', 'Enabled'),
      ),
      'override_from' => array(
        '#type' => 'radios',
        '#default_value' => (isset($govdelivery_odm_settings['override_from']) ? $govdelivery_odm_settings['override_from'] : 0),
        '#title' => t('Override the From address on outgoing messages and failback on the predefined From address in this module. 
                      (This will allow webforms and other modules to configure the From address)'),
        '#options' => array('Disabled', 'Enabled'),
      ),
      'max_bid' => array(
        '#type' => 'textfield',
        '#title' => t('Maximum bins used with queue asynchronous processing.'),
        '#default_value' => (isset($govdelivery_odm_settings['max_bid']) ? $govdelivery_odm_settings['max_bid'] : 5),
      ),
      'external_cron_interval' => array(
        '#type' => 'textfield',
        '#title' => t('External Cron Interval (in seconds).'),
        '#default_value' => (isset($govdelivery_odm_settings['external_cron_interval']) ? $govdelivery_odm_settings['external_cron_interval'] : 120),
      ),
    ),
    'govdelivery_subscription_settings' => $subscription_fieldset,
    'save' => array(
      '#type' => 'submit',
      '#value' => 'Save Configuration',
    ),
  );

  return $form;
//  return system_settings_form($form);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function govdelivery_settings_submit($form, $postinfo) {
  $govdelivery_odm_settings = variable_get('govdelivery_odm_settings', array());
  $govdelivery_subscription_settings = variable_get('govdelivery_subscription_settings', array());
  $account_settings = isset($govdelivery_odm_settings['accounts']) ? $govdelivery_odm_settings['accounts'] : array();
  $new_account = array();

  // change around the account storage to an associative array indexed by username
  $accounts_posted = $postinfo['values']['govdelivery_odm_settings']['accounts'];
  foreach ($accounts_posted as $composite_key => $value) {
    // if we don't have a value we don't want to do anything
    if (isset($value) && !empty($value)) {
      $key_elements = explode('%###%', $composite_key);
      if (count($key_elements) == 2) {
        $username = $key_elements[0];
        $field = $key_elements[1];

        if (!isset($account_settings[$username]) && isset($username) && !empty($username)) {
          $account_settings[$username] = array();
        }
        if ($field != 'username') {
          $account_settings[$username][$field] = $value;
        }
      }
      else {
        if (isset($key_elements[0])) {
          $new_account[$key_elements[0]] = $value;
        }
      }
    }
  }

  if (isset($new_account['username'])) {
    $account_settings[$new_account['username']] = array(
      "password" => $new_account['password'],
      "fromname" => $new_account['fromname'],
    );
  }
  $govdelivery_odm_settings['accounts'] = $account_settings;

  // Server
  $server = $postinfo['values']['govdelivery_odm_settings']['server'];
  $govdelivery_odm_settings['server'] = check_url($server);

  // Subscription URL
  $service_url = $postinfo['values']['govdelivery_subscription_settings']['service_url'];
  $govdelivery_subscription_settings['service_url'] = check_url($service_url);

  // These come in as array place value so 0 or 1.
  // Queue Messages
  $govdelivery_odm_settings['queue'] = $postinfo['values']['govdelivery_odm_settings']['queue'];

  // Messages Cron
  $govdelivery_odm_settings['cron_odm'] = $postinfo['values']['govdelivery_odm_settings']['cron_odm'];

  // Override From address
  $govdelivery_odm_settings['override_from'] = $postinfo['values']['govdelivery_odm_settings']['override_from'];

  // Subscriptions Cron
  $govdelivery_subscription_settings['cron_subscriptions'] = $postinfo['values']['govdelivery_subscription_settings']['cron_subscriptions'];

  // Subscriber API Base URL
  $govdelivery_subscription_settings['subscriber_api_url_base'] = $postinfo['values']['govdelivery_subscription_settings']['subscriber_api_url_base'];

  // Default Category ID when creating new topics.
  $govdelivery_subscription_settings['default_topic_category_id'] = $postinfo['values']['govdelivery_subscription_settings']['default_topic_category_id'];

  // Asynchronous Processing Max BID
  $govdelivery_odm_settings['max_bid'] = $postinfo['values']['govdelivery_odm_settings']['max_bid'];

  $govdelivery_odm_settings['external_cron_interval'] = $postinfo['values']['govdelivery_odm_settings']['external_cron_interval'];

  // Mail Routing
  $govdelivery_odm_settings['enabled'] = $postinfo['values']['govdelivery_odm_settings']['enabled'];
  if ($govdelivery_odm_settings['enabled'] == 1) {
    $mail_system = variable_get('mail_system');
    $mail_system['default-system'] = 'GovDeliveryMailSystem';
    variable_set('mail_system', $mail_system);
  }
  else {
    $mail_system = variable_get('mail_system');
    if (isset($mail_system) && !empty($mail_system['default-system'])) {
      $mail_system['default-system'] = 'DefaultMailSystem';
      variable_set('mail_system', $mail_system);
    }
  }

  // Save the validated changes
  variable_set('govdelivery_subscription_settings', $govdelivery_subscription_settings);
  variable_set('govdelivery_odm_settings', $govdelivery_odm_settings);
}

// Send a test message to the specificed addresses.
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function govdelivery_test_settings($form, &$form_state) {
  $form = array(
    'govdelivery_test_settings' => array(
      '#type' => 'fieldset',
      '#title' => t('GovDelivery Test Settings'),
      '#tree' => TRUE,
      'from' => array(
        '#type' => 'textfield',
        '#title' => t('From address (Optional)'),
      ),
      'recipients' => array(
        '#type' => 'textfield',
        '#title' => t('Recipient\'s Email (Comma seperated)'),
      ),
      'test' => array(
        '#type' => 'submit',
        '#value' => t('Send test message'),
      ),
    ),
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function govdelivery_test_settings_submit($form, $postinfo) {
  $recipients = $postinfo['values']['govdelivery_test_settings']['recipients'];
  // Send message.
  if (isset($recipients) && !empty($recipients)) {
    $from = $postinfo['values']['govdelivery_test_settings']['from'];
    // Pass on the list to be sent.
    if(!empty($from)) {
      $status = govdelivery_send_test_message($recipients, $from);
    }
    else {
      $status = govdelivery_send_test_message($recipients);
    }
  }
  drupal_set_message(t('Your test message has been sent.'));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function govdelivery_send_test_message($send_to, $from = NULL) {
  return drupal_mail('govdelivery', 'test_id', $send_to, language_default(), array(), $from, TRUE);
}

// Signup an email address for a specified list and pass any known questions and answers along.
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function govdelivery_test_subscriptions($form, &$form_state) {
  $form = array(
    'govdelivery_test_subscriptions' => array(
      '#type' => 'fieldset',
      '#title' => t('GovDelivery Settings Test - Subscriptions'),
      '#tree' => TRUE,
      'subscriber' => array(
        '#type' => 'textfield',
        '#title' => t('Subscriber\'s Email'),
      ),
      'list_code' => array(
        '#type' => 'textfield',
        '#title' => t('Subscription List Code'),
      ),
      // Use Queue option
      'test' => array(
        '#type' => 'submit',
        '#value' => t('Test send message'),
      ),
    ),
  );

  return $form;
}

// Need validation that there is only one subscriber or change to handle multiple subscriptions.

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function govdelivery_test_subscriptions_submit($form, $postinfo) {
  $subscriber = $postinfo['values']['govdelivery_test_subscriptions']['subscriber'];
  $list_code = $postinfo['values']['govdelivery_test_subscriptions']['list_code'];

  //if (isset($subscriber) && !empty($subscriber) && isset($list_code) && !empty($list_code)) {
  $status = govdelivery_subscribe($list_code, $subscriber, FALSE);
  //}

  drupal_set_message(t('%response', array('%response' => $status ? 'Subscription Added.' : 'Subscription Failed.')));
}

