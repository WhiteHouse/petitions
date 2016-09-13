<?php
/**
 * @file
 * Petition constants, helper functions.
 */

/**
 * Constants for Petition Statuses.
 */

/**
 * Saved as a draft, only visible to creator
 */
define('WH_PETITION_STATUS_DRAFT', 0);

/**
 * Published, but does not have the required number of
 * signatures to become public, only visible by direct url
 */
define('WH_PETITION_STATUS_PRIVATE', 1);

/**
 * Has the required number of signatures to become public,
 * visible everywhere on site
 */
define('WH_PETITION_STATUS_PUBLIC', 2);

/**
 * Did not receive the required number of signatures to receive a
 * response, only visible by direct url
 */
define('WH_PETITION_STATUS_CLOSED', 3);

/**
 * Has received the required number of signatures, visible on site
 */
define('WH_PETITION_STATUS_UNDER_REVIEW', 4);

/**
 * Has received a response
 */
define('WH_PETITION_STATUS_REVIEWED', 5);

/**
 * Has been removed from lists by administrative action on the dashboard.
 * This may have been prompted by it exceeding the threshold for the number of
 * abuse flags.  It shows on the dashboard as "removed".
 */
define('WH_PETITION_STATUS_FLAGGED', 6);

/**
 * Constants for Response Statuses
 */

// Petition has not been reviewed.
define('WH_PETITION_RESPONSE_STATUS_UNANSWERED', 0);

// Petition is awaiting action.
define('WH_PETITION_RESPONSE_STATUS_PENDING', 1);

// Petition has been responded to.
define('WH_PETITION_RESPONSE_STATUS_ANSWERED', 2);

/**
 * Misc Constants
 */

// Petitions to display per page on the Open Petitions page.
define('WH_PETITION_PETITIONS_PER_PAGE', 20);

define('WH_PETITION_NICE_URL', 'petition/[petition:title]');

/**
 * Petitions signing methods
 */

/**
 * Registration required.
 */
define('WH_PETITIONS_SIGNING_METHOD_REGISTRATION_REQUIRED', 0);

/**
 * Simplified signing.
 */
define('WH_PETITIONS_SIGNING_METHOD_SIMPLIFIED_SIGNING', 1);

/**
 * Helper Functions
 */

/**
 * Returns an array of status id's that are to be treated as signable.
 *
 * If a petition has an actual status (not the friendly display status) that
 * is contained in the array returned by this function, it can be considered
 * 'signable' and  may be returned as such by the API.  The following statuses
 * are considered signable:
 *  - WH_PETITION_STATUS_PRIVATE: Is signable but only visible by direct link.
 *  - WH_PETITION_STATUS_PUBLIC: Is public and signable.
 *  - WH_PETITION_STATUS_UNDER_REVIEW: Has reached enough signatures for a
 *    response, but has not been responded to, so can get more signatures.
 *
 * @return array
 *   Array of petition status id's considered signable
 */
function wh_petitions_signable_statuses() {
  return array(
    WH_PETITION_STATUS_PRIVATE,
    WH_PETITION_STATUS_PUBLIC,
    WH_PETITION_STATUS_UNDER_REVIEW,
  );
}

/**
 * Returns an array of status IDs that are all the possible statuses.
 *
 * @return array
 *   Array of petition all defined petitions status IDs.
 */
function wh_petitions_all_statuses() {
  return array(
    WH_PETITION_STATUS_DRAFT,
    WH_PETITION_STATUS_PRIVATE,
    WH_PETITION_STATUS_PUBLIC,
    WH_PETITION_STATUS_CLOSED,
    WH_PETITION_STATUS_UNDER_REVIEW,
    WH_PETITION_STATUS_REVIEWED,
    WH_PETITION_STATUS_FLAGGED,
  );
}

/**
 * Returns an array of status id's that are to be treated as public.
 *
 * If a petition has an actual status (not the friendly display status) that
 * is contained in the array returned by this function, it can be considered
 * 'public' and  may be returned as such by the API.  The following statuses are
 * considered public:
 *  - WH_PETITION_STATUS_PUBLIC: By definition this is public.
 *  - WH_PETITION_STATUS_UNDER_REVIEW:  No action has been taken on this.
 *  - WH_PETITION_STATUS_REVIEWED: Has been reviewed and responded to.
 *  - WH_PETITION_STATUS_CLOSED: Is closed but should still be visible.
 *
 * @return array
 *   Array of petition status id's considered public.
 */
function wh_petitions_public_statuses() {
  return array(
    WH_PETITION_STATUS_PUBLIC,
    WH_PETITION_STATUS_UNDER_REVIEW,
    WH_PETITION_STATUS_REVIEWED,
    WH_PETITION_STATUS_CLOSED,
  );
}

/**
 * Returns an array of status id's that indicate an "open" petition.
 *
 * @return array
 *   Array of petition status id's considered open.
 */
function wh_petitions_open_statuses() {
  return array(
    WH_PETITION_STATUS_PUBLIC,
    WH_PETITION_STATUS_UNDER_REVIEW,
  );
}

/**
 * Returns an array of status id's that are to be treated as unpublished.
 *
 * @return array
 *   Array of petition status id's considered unpublished.
 */
function wh_petitions_unpublished_statuses() {
  return array(
    WH_PETITION_STATUS_FLAGGED,
    WH_PETITION_STATUS_DRAFT,
  );
}

/**
 * Returns the title associated with a status.
 *
 * @param int $status_id
 *   Status constant.
 * @param string $type
 *   Petition or Response.
 *
 * @return string
 *   Status of title.
 */
function wh_petitions_display_status_name($status_id, $type = 'petition') {
  switch ($type) {
    case 'petition':
      switch ($status_id) {
        case WH_PETITION_STATUS_DRAFT:
          return t('Draft');

        case WH_PETITION_STATUS_PRIVATE:
          return t('Private');

        case WH_PETITION_STATUS_PUBLIC:
          return t('Public');

        case WH_PETITION_STATUS_CLOSED:
          return t('Archived');

        case WH_PETITION_STATUS_UNDER_REVIEW:
          return t('Under Review');

        case WH_PETITION_STATUS_REVIEWED:
          return t('Reviewed');

        case WH_PETITION_STATUS_FLAGGED:
          return t('Removed');

        default:
          return '';
      }
    case 'response':
      switch ($status_id) {
        case WH_PETITION_RESPONSE_STATUS_UNANSWERED:
          return t('Unanswered');

        case WH_PETITION_RESPONSE_STATUS_PENDING:
          return t('Awaiting Action');

        case WH_PETITION_RESPONSE_STATUS_ANSWERED:
          return t('Responded');

        default:
          return '';
      }
    default:
      return '';
  }
}

/**
 * Helper function to format a number correctly.
 *
 * (no decimals, commas for thousands-separators)
 *
 * @param string $number
 *   Number to format.
 *
 * @return string
 *   Formatted string X,XXX
 */
function wh_petitions_format_number($number) {
  return number_format(intval($number), 0, '.', ',');
}

/**
 * Format description for petition detail.
 *
 * Helper function to format a description, changing newlines to
 * <p> tags.
 *
 * @param string $description
 *   Description to format.
 *
 * @return string
 *   Formatted string.
 */
function wh_petitions_format_description($description) {
  if (!empty($description)) {
    // Consolidate newlines.
    $description = preg_replace('/[\r\n]+/', "\n", $description);
    $arr = explode("\n", $description);

    $new_description = '';
    foreach ($arr as $line) {
      $new_description .= '<p>' . check_plain(trim($line)) . '</p>';
    }
    $description = $new_description;
  }

  return $description;
}

/**
 * Generate keywords for fields.
 *
 * @param string $string
 *   String to extract keywords from.
 * @param string $delimiter
 *   String to use as a delimiter for character analysis.
 *
 * @return array
 *   An indexed array of keywords.
 */
function wh_petitions_generate_keywords($string, $delimiter = ' ') {
  $words = array();

  if (!empty($string)) {
    $arr = explode($delimiter, $string);
    foreach ($arr as $word) {
      // Strip all nonalphanumeric, whitespace, etc.
      if ($delimiter != ' ') {
        $word = trim(preg_replace('/[^A-Za-z0-9\s]/', '', $word));
      }
      else {
        $word = preg_replace('/[^A-Za-z0-9]/', '', $word);
      }
      array_push($words, (string) strtolower(trim($word)));
    }
  }

  return $words;
}

/**
 * Generate URL obfuscation string.
 *
 * Generate a random 8-char hash to append to the end of the
 * url for obfuscation.
 *
 * @param int $length
 *   Length of random string
 *
 * @return string
 *   Character hash of $length
 */
function wh_petitions_generate_hash($length = 8) {
  $characters = 'bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ0123456789';
  $count = mb_strlen($characters) - 1;
  $hash = '';

  for ($i = 0; $i < $length; $i++) {
    $hash .= $characters[mt_rand(0, $count)];
  }

  return $hash;
}

/**
 * Determine if string length exceeds char count.
 *
 * @param string $str
 *   Input string.
 * @param int $chars
 *   Max # of chars allowed.
 *
 * @return bool
 *   FALSE if exceeds count, TRUE otherwise.
 */
function _wh_petitions_exceed_char_count($str, $chars) {
  $str = str_replace("\r\n", "\n", $str);
  if (mb_strlen($str) > $chars) {
    return TRUE;
  }
  return FALSE;
}


/**
 * Formats a form checkbox element into 3 (mostly-equal) columns.
 *
 * @param array $form_element
 *   Checkbox Form Element.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Altered form_element split into 3 columns.
 */
function _wh_petitions_checkbox_3column_format($form_element, &$form_state) {
  $size = count($form_element['#options']);

  if ($size > 3) {
    $in_each = ceil($size / 3);
    $i = 1;
    foreach ($form_element['#options'] as $key => $val) {
      if ($i == 1) {
        $form_element[$key]['#prefix'] = '<div class="group">';
      }
      elseif ($i == ($in_each + 1)) {
        $form_element[$key]['#prefix'] = '</div><div class="group">';
      }
      elseif ($i == (($in_each * 2) + 1)) {
        $form_element[$key]['#prefix'] = '</div><div class="group">';
      }
      elseif ($i == $size) {
        $form_element[($key)]['#suffix'] = '</div>';
      }

      $i++;
    }
  }
  return $form_element;
}

/**
 * Adds form error messages above fields by adding message and divs.
 *
 * @param array $form
 *   Form array
 * @param array $form_state
 *   Form state
 */
function _wh_petitions_add_field_error_labels(&$form, &$form_state) {
  if (!isset($_SESSION['messages'])) {
    return;
  }

  $errors = form_get_errors();
  if ($errors) {
    foreach ($errors as $field => $message) {
      if (isset($form['#form_id']) && $form['#form_id'] == 'user_register_form' && in_array($field, array(
          'mail',
          'name'
        ))
      ) {
        // Profile fields are under account.
        $field = "account][" . $field;
      }
      elseif ($field == 'captcha_response') {
        // Form element is captcha for field captcha_response.
        $field = "captcha";
      }

      $field_esc = '';
      $elements = explode('][', $field);
      foreach ($elements as $element) {
        $field_esc .= is_numeric($element) ? "[" . $element . "]" : "['" . $element . "']";
      }

      // Get a reference to the form element.
      eval('$form_element = &$form' . $field_esc . ';');
      if (isset($form_element)) {
        $input_error_msg = '<div class="input-error"><div class="input-error-message">' . $message . '</div>';
        $form_element['#prefix'] = isset($form_element['#prefix']) ? $input_error_msg . $form_element['#prefix'] : $input_error_msg;
        $form_element['#suffix'] = isset($form_element['#suffix']) ? '</div>' . $form_element['#suffix'] : '</div>';

        $error_id = array_search($message, $_SESSION['messages']['error']);

        // Clear out the session errors so they are not displayed above.
        if ($error_id !== FALSE) {
          unset($_SESSION['messages']['error'][$error_id]);
          $_SESSION['messages']['error'] = array_values($_SESSION['messages']['error']);

          if (count($_SESSION['messages']['error']) <= 0) {
            unset($_SESSION['messages']['error']);
          }
        }
      }
    }
  }
}
