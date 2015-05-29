<?php
/**
 * @file
 * Petition views access plugin.
 */

/**
 * Access plugin that provides property based access control.
 */
class PetitionUiViewsAccessPlugin extends views_plugin_access {

  /**
   * Summary title.
   *
   * @return null|string
   *   returning views access plugin title.
   */
  public function summary_title() {
    return t('Petition Admin Index Access');
  }

  /**
   * First access callback registration.
   *
   * @param obj|null $account
   *   Passes the target user account or nothing.
   *
   * @return bool
   *   Returns true or false if user has access.
   */
  public function access($account) {
    return petitionadmin_views_access_callback($account);
  }

  /**
   * Second Access callback registration.
   *
   * @return null|array
   *   returns array defining callback.
   */
  public function get_access_callback() {
    return array('petitionadmin_views_access_callback', NULL);
  }
}
