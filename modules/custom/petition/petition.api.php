<?php

/**
 * @file
 * Hooks provided by the Petition module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * This hook is invoked when signatures may have been added/removed.
 *
 * Signatures may also have not changed when invoked.
 *
 * @param int $nid
 *   Petition node ID.
 */
function hook_petition_signatures_updated($nid) {
  // Perform actions necessary when signature counts may have changed here.
}

/**
 * This hook is invoked when a petition status has been updated.
 *
 * @param int $nid
 *   Petition node ID.
 * @param int $previous_status
 *   Previous petition status.
 * @param int $new_status
 *   New petition status.
 */
function hook_petition_status_updated($nid, $previous_status, $new_status) {
  // Perform actions necessary when petition status has changed.
}

/**
 * @} End of "addtogroup hooks".
 */
