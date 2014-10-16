<?php

/**
 * @file
 * Rules for wh_core.
 */

/**
 * Implements hook_rules_action_info().
 */
function wh_core_rules_action_info() {
  $actions['login_user'] = array(
    'parameter' => array(
      'account' => array(
        'type'  => 'user',
        'label' => t('user'),
      ),
    ),
    'group' => t('WH Core'),
    'label' => t('Login a User'),
    'base'  => 'wh_core_login_user',
  );
  return $actions;
}

/**
 * Provides a call back for the login_user Rules action.
 */
function wh_core_login_user($account) {
  global $user;
  $edit = array();
  $user = user_load($account->uid);
  user_login_finalize($edit);
}
