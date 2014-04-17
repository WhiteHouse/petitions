<?php

/**
 * @file
 * Hooks provided by the Signatures Queue module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Respond to a petition signatory opting in.
 *
 * This hook is invoked whenever a petition is signed with the "signup" flag.
 *
 * @param array $signatory
 *   An associative array of signatory details:
 *   - first_name: First name.
 *   - last_name: Last name.
 *   - email: Email address.
 *   - zip: Zip code.
 *   - petition_id: The ID of the petition the signatory signed.
 */
function hook_petition_signatory_opt_in(array $signatory) {
  // Save the signatory details to a database, add them to a mailing list, or
  // otherwise capture their record here.
}

/**
 * @} End of "addtogroup hooks".
 */
