<?php

/**
 * @file
 * Separate anything related to our MongoDB to MySQL transition.
 *
 * @todo This will be removed once mongo support is no longer needed.
 *   Remove shunt enabling from signature_mail.install when Mongo is removed.
 */

/**
 * Implements hook_shunt().
 */
function signature_mail_shunt() {
  return array(
    'signature_mail_mysql_save' => t('Disable signature writes to MySQL.'),
  );
}

/**
 * Disable mysql. (This uses a shunt trip that turns off access to mysql.).
 */
function signature_mail_disable_mysql() {
  // Set the initial state of MySQL shunt to disable mysql writes.
  // @todo This default setting should be removed when mongo is fully removed.
  shunt_enable_shunt('signature_mail_mysql_save');
}
