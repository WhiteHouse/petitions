<?php
/**
 * @file
 * Page callbacks and related functions for petitions_stats_users module.
 */

/**
 * Page callback for Users Statistics.
 *
 * Generates an array of user statistics for a given date.
 * Searches through all relevant JSON files to build a nested
 * array of user statistics, sorted by date and hour.
 *
 * @param string $date
 *   A date matching the format yyyy-mm-dd.
 *   This is used to select the appropriate data directory.
 */
function petitions_stats_users_page($date = NULL) {
  $output = '';

  // Set default date params.
  $format = 'Y-m-d';
  if (!$date) {
    $date = date($format);
  }

  // Grab date parts.
  $date_obj = new DateTime($date);
  $year = $date_obj->format('Y');
  $month = $date_obj->format('m');
  $day = $date_obj->format('d');

  // Build date selector form.
  $date_form = drupal_render(drupal_get_form('petitions_stats_users_date_form', $date, $format));
  $output .= $date_form;

  // Load statistics.
  if ($date_stats = _petitions_stats_users_load_db_statistics($year, $month, $day)) {
    // Create date stub.
    $statistics['users']['creation']['dates'][$date] = array(
      'title' => $date,
      'count' => 0,
    );

    foreach ($date_stats as $hour => $count) {
      // Create hour strub.
      $statistics['users']['creation']['dates'][$date]['hours'][$hour] = array(
        'title' => $hour,
        'count' => $count,
      );
      $statistics['users']['creation']['dates'][$date]['count'] += $count;
    }

    // Calculate percentages.
    foreach ($statistics['users']['creation']['dates'] as $date => $date_data) {
      ksort($statistics['users']['creation']['dates'][$date]['hours']);
      foreach ($date_data['hours'] as $hour => $hour_data) {
        $percentage = round(($hour_data['count'] / $date_data['count']) * 100, 2);
        $statistics['users']['creation']['dates'][$date]['hours'][$hour]['percentage'] = $percentage;
      }
    }

    $vars = array(
      'date' => $date,
      'statistics' => $statistics,
    );

    drupal_add_css(drupal_get_path('module', 'petitions_stats_users') . '/theme/petitions_stats_users.css');
    $output .= theme('petitions_stats_users', $vars);
  }
  else {
    $output .= t('There are no user creation statistics for @date', array('@date' => $date));
  }
  return $output;
}

/**
 * Form callback for date picker on users statistics page.
 *
 * @param string $date
 *   A date in the form yyyy-mm-dd.
 *
 * @param string $format
 *   The date format of $date, as accepted by PHP's date() function.
 */
function petitions_stats_users_date_form($form, $form_state, $date, $format) {
  module_load_include('inc', 'date', 'date_api_elements');

  // Set starting year.
  $psu_start_year = variable_get('psu_start_year', date(Y) - 3);
  $start_year = date(Y) - $psu_start_year;
  $date_year_range = '-' . $start_year . ':+0';

  $form = array();

  // See other available attributes and what they do in date_api_elements.inc
  $form['date'] = array(
    '#title' => t('Select Date'),
    '#type' => 'date_select',
    '#default_value' => $date,
    '#date_format' => $format,
    '#date_year_range' => $date_year_range,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Go to date'),
  );

  return $form;
}

/**
 * Submit callback for petitions_stats_users_date_form().
 */
function petitions_stats_users_date_form_submit($form, &$form_state) {
  $date = $form_state['values']['date'];
  drupal_goto('admin/people/stats/' . $date);
}

/**
 * Page callback for administrative page.
 */
function petitions_stats_users_admin_form() {

  // Set default year.
  $year = date(Y);
  $default_year = $year - 3;

  $form = array();

  // This module was initially built to control file size directly. E.g., by
  // imposing a 1MB limit on data files. However, that method of control made
  // it very difficult to incrementally increase or decrease the contents of
  // a data file efficiently. By controlling the array size of the JSON
  // contents, we have more granular control.
  /*
  $form['psu_max_array_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum array size per data file'),
    '#description' => t('The maximum number of array rows that may be stored in a single data file. This indirectly controls the maximum size of the data file, which can have performance ramifications.'),
    '#default_value' => variable_get('psu_max_array_size', '10000'),
  );
  $form['psu_data_dir'] = array(
    '#type' => 'textfield',
    '#title' => t('Data directory'),
    '#description' => t('This is the directory in which JSON files will be stored. Enter a directory name relative to the Drupal files directory. Do not include trailing slashes.'),
    '#default_value' => variable_get('psu_data_dir', 'petitions_stats_users_data'),
  );
  */
  $form['psu_request_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Request URL'),
    '#description' => t('The Petitions API URL to which the request will be made. Do not include parameters.'),
    '#default_value' => variable_get('psu_request_url', 'http://petitions.whitehouse.gov/api/v1/users.json'),
  );
  $form['psu_batch_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Batch size'),
    '#description' => t('Batch size for Petitions API requests. For example, a batch size of 100 and a total result count of 1000 would generate 10 requests for 100 items.'),
    '#default_value' => variable_get('psu_batch_size', 100),
  );
  $form['psu_max_cron_tasks'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum queue tasks per cron'),
    '#description' => t('The maximum number of tasks that may be queue via the Queue API in a given cron run.'),
    '#default_value' => variable_get('psu_max_cron_tasks', 5000),
  );

  $form['psu_start_year'] = array(
    '#type' => 'date_select',
    '#title' => t('Starting year'),
    '#description' => t('The first year to provide statistics for.'),
    '#default_value' => variable_get('psu_start_year', $default_year),
    '#date_format' => 'Y',
    '#date_year_range' => '-10:+0',
  );

  return system_settings_form($form);
}

/**
 * Validation callback for petitions_stats_users_admin_form().
 */
function petitions_stats_users_admin_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  /*
  if (!ctype_digit($values['psu_max_array_size']) || $values['psu_max_array_size'] < 1) {
    form_set_error('psu_max_array_size', t('Max array size must be a positive integer.'));
  }
  */
  if (!valid_url($values['psu_request_url'])) {
    form_set_error('psu_request_url', t('Please enter a valid URL.'));
  }
  if (!ctype_digit($values['psu_batch_size']) || $values['psu_batch_size'] < 1) {
    form_set_error('psu_batch_size', t('Batch size must be a positive integer.'));
  }
  if (!ctype_digit($values['psu_max_cron_tasks']) || $values['psu_max_cron_tasks'] < 1) {
    form_set_error('psu_max_cron_tasks', t('Maximum queue tasks per cron must be a positive integer.'));
  }
}
