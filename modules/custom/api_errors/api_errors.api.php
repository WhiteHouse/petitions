<?php

/**
 * @file
 * Example usage of api_errors module.
 */

/**
 * How to throw an API error.
 */
function something_involved_in_creating_a_reasponse() {
  // Do somethings here.

  if ($things_go_as_expected) {
    $result = get_data($things_go_as_expected);
  }
  else {
    // Something has gone wrong.
    $status = 400;
    $error_code = '38';
    $developer_message = t("This is not the resource you're looking for...");
    $user_message = t("An error occured when attempting to access the API.");
    $more_info = t("See issue #38 on github");

    // Throw the error.
    api_errors_throw_error($status, $developer_message, $error_code, $user_message, $more_info);
  }

  return $result
}
