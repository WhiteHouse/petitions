<?php
/**
 * Saves request results to JSON files.
 *
 * @param array $results
 *   A numerical array of results. Each row is a user object.
 *
 * @return boolean
 *   Returns true only if every row in $results is successfully saved.
 */
function _petitions_stats_users_save_json_data($results) {
  module_load_include('inc', 'petitions_stats_users', 'petitions_stats_users.files');
  $data_dir = variable_get('psu_data_dir', 'petitions_stats_users_data');
  $data = array();

  // Created formatted array of user data.
  foreach ($results as $user) {
    // Get date params.
    $date = date('Y-m-d', $user->created);
    $hour = date('H', $user->created);

    // Save user data object in structured array.
    $data[$date][$hour][] = array(
      'id' => $user->id,
    );
  }

  // Write formatted data array to disk.
  foreach ($data as $date => $hours) {
    foreach ($hours as $hour => $rows) {
      // Specify directory and filename.
      $path = $data_dir . '/' . $date . '/' . $hour;
      $directory = file_build_uri($path);
      $filename = 'data.json';

      // Attempt to write to directory.
      if (lock_acquire($directory)) {
        $saved = _petitions_stats_users_write_json_data($directory, $filename, $rows);
        lock_release($directory);
        if (!$saved) {
          return FALSE;
        }
      }
      // Wait 30s and try again.
      elseif (lock_wait($directory)) {
        $saved = _petitions_stats_users_write_json_data($directory, $filename, $rows);
        lock_release($directory);
        if (!$saved) {
          return FALSE;
        }
      }
      // Fail.
      else {
        $vars = array(
          '@filename' => $filename,
          '@directory' => $directory,
          '@data' => var_export($rows, TRUE),
        );
        $message = 'Unable to write @filename to @directory. The directory is locked.';
        watchdog('petitions_stats_users', $message, $vars, WATCHDOG_ERROR);
        return FALSE;
      }
    }
  }

  return TRUE;
}

/**
 * Writes data to disk as JSON by creating new file or appending to existing.
 *
 * @param string $directory
 *   The directory to which the data file will be saved.
 * @param string $filename
 *   The filename (with extension) to be save.
 * @param array $data
 *   The data array of contents to be saved to disk.
 *
 * @return boolean
 *   Returns true if data was successfully written to disk.
 */
function _petitions_stats_users_write_json_data($directory, $filename, $data) {
  $file_uri = $directory . '/' . $filename;

  // Ensure that destination directory exists.
  if (file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
    // If the file exists.
    if (file_exists($file_uri)) {
      // Attempt to append new contents to existing data file. If the data file
      // has reached its maximum size, a new, suffixed one will be created.
      return _petitions_stats_users_write_json_data_append($directory, $filename, $data);
    }
    // Create a new file containing data.
    else {
      return _petitions_stats_users_write_json_data_create($directory, $filename, $data);
    }
  }
  else {
    $vars = array('@directory' => $directory);
    $message = 'Unable to prepare directory @directory';
    watchdog('petitions_stats_users', $message, $vars, WATCHDOG_ERROR);
    return FALSE;
  }
}

/**
 * Writes a new data file to disk.
 *
 * This function may split data into multiple files to ensure that the maximum
 * permitted JSON array size is not exceeded.
 *
 * @param string $directory
 *   The directory to which the data file will be saved.
 * @param string $filename
 *   The filename (with extension) to be save.
 * @param array $data
 *   The data array of contents to be saved to disk.
 *
 * @return boolean
 *   Returns true if data was successfully written to disk.
 */
function _petitions_stats_users_write_json_data_create($directory, $filename, $data) {
  $file_uri = $directory . '/' . $filename;

  // Ensure that data set does not exceed maximum array size.
  $max_array_size = variable_get('psu_max_array_size', '10000');
  $size = count($data);
  if ($size > $max_array_size) {
    // Split into two arrays: the first of acceptable size, the second
    // containing overflow.
    $first = array_slice($data, 0, $max_array_size - 1);
    $second = array_slice($data, $max_array_size - 1, $size - 1);

    // Save data by recursing. Note that the $second data will be written to a
    // separate file following the naming pattern "/basename_[0-9]+/".
    $success =  _petitions_stats_users_write_json_data_create($directory, $filename, $first) &&
    _petitions_stats_users_write_json_data_create($directory, $filename, $second);
  }
  // Otherwise, save the file normally.
  else {
    $contents = json_encode($data);
    $success = file_unmanaged_save_data($contents, $file_uri, FILE_EXISTS_RENAME);
  }

  // Log error on fail.
  if (!$success) {
    $vars = array('@file_uri' => $file_uri);
    $message = 'Unable to create new file @file_uri. Note that Drupal must have write access to both the temporary and public/private file directories for this write to succeed.';
    watchdog('petitions_stats_users', $message, $vars, WATCHDOG_ERROR);
  }

  return $success;
}

/**
 * Appends data to an existing file.
 *
 * This function may split data into multiple files to ensure that the maximum
 * permitted JSON array size is not exceeded.
 *
 * @param string $directory
 *   The directory to which the data file will be saved.
 * @param string $filename
 *   The filename (with extension) to be save.
 * @param array $data
 *   The data array of contents to be saved to disk.
 *
 * @return boolean
 *   Returns true if data was successfully written to disk.
 */
function _petitions_stats_users_write_json_data_append($directory, $filename, $data) {
  $max_array_size = variable_get('psu_max_array_size', '10000');

  // We know that the file at $file_uri exists. Let's check to see if files
  // matching the pattern "/basename_[0-9]+/" also exist. If so, find the
  // most recent URI.
  $file_uri = _petitions_stats_users_most_recent_fileuri($directory, $filename);

  // Load contents of existing file.
  $existing_file_contents = _petitions_stats_users_load_data_file($file_uri);

  // If the size of the existing file already equals the maximum,
  // create a new file rather than appending data.
  $existing_data_size = count($existing_file_contents);
  if ($existing_data_size >= $max_array_size) {
    return _petitions_stats_users_write_json_data_create($directory, $filename, $data);
  }

  // We first need to encode the new contents as JSON so that it has the
  // same form as the content in the existing file. This is necessary
  // because json_encode() converts some arrays into objects,
  // fundamentally changing the data structure.
  $new_contents = json_encode($data);
  $new_contents = json_decode($new_contents);
  $new_contents_size = count($new_contents);

  // Merge existing file content with new content.
  $combined_contents = array_merge($existing_file_contents, $new_contents);
  // Ensure that there are no duplicate rows.
  $combined_contents = _petitions_stats_users_unique_json($combined_contents);
  $combined_contents_size = count($combined_contents);

  // If the new combined data exceeds the maximum allowed size, append only
  // permissible data, and create a new file for overflow.
  if ($combined_contents_size > $max_array_size) {
    // Split into two arrays: the first of acceptable size, the second
    // containing overflow.
    $append_contents = array_slice($combined_contents, 0, $max_array_size - 1);
    $overflow_contents = array_slice($combined_contents, $max_array_size - 1, $combined_contents_size - 1);

    // JSON encode append contents.
    $append_contents = json_encode($append_contents);

    return file_unmanaged_save_data($append_contents, $file_uri, FILE_EXISTS_REPLACE) &&
    _petitions_stats_users_write_json_data_create($directory, $filename, $overflow_contents);
  }

  // JSON encode new, merged data.
  $combined_contents = json_encode($combined_contents);

  // Overwrite existing file.
  if (file_unmanaged_save_data($combined_contents, $file_uri, FILE_EXISTS_REPLACE)) {
    return TRUE;
  }
  else {
    $vars = array('@file_uri' => $file_uri);
    $message = 'Unable to append new data to @file_uri';
    watchdog('petitions_stats_users', $message, $vars, WATCHDOG_ERROR);
    return FALSE;
  }
}

/**
 * Loads statistics from JSON files.
 *
 * @param string $date
 *   A date matching the format yyyy-mm-dd.
 *   This is used to select the appropriate data directory.
 *
 * @return array
 */
function _petitions_stats_users_load_json_statistics($date) {
  $base_dir = variable_get('petitions_stats_users_data_dir', 'petitions_stats_users_data');
  $date_directory = file_build_uri($base_dir . '/' . $date);
  $statistics     = array();

  // Check to be sure that this is a real directory.
  if (file_exists($date_directory)) {
    module_load_include('inc', 'petitions_stats_users', 'petitions_stats_users.files');
    $statistics = array();
    $statistics['users']['creation']['count'] = 0;

    // Set mask to match all files and folders.
    $mask = '/./';
    if ($contents = array_keys(file_scan_directory($date_directory, $mask))) {
      foreach ($contents as $file_uri) {
        list($data_dir, $date, $hour, $filename) = explode('/', file_uri_target($file_uri));
        $day = array_pop(explode('/', $date_directory));
        $data = _petitions_stats_users_load_data_file($file_uri);

        // If this date has not been populated, create stub.
        if (!isset($statistics['users']['creation']['dates'])) {
          $statistics['users']['creation']['dates'][$date] = array(
            'title' => $date,
            'count' => 0,
          );
        }

        // If this hour has not been populated, create stub.
        if (!isset($statistics['users']['creation']['dates'][$date]['hours'][$hour])) {
          $statistics['users']['creation']['dates'][$date]['hours'][$hour] = array(
            'title' => $hour,
            'count' => 0,
          );
        }

        // The user count for this particular data file.
        $user_count = count($data);

        // Increment totals for this hour and this day.
        $statistics['users']['creation']['dates'][$day]['hours'][$hour]['count'] += $user_count;
        $statistics['users']['creation']['dates'][$day]['count'] += $user_count;
        $statistics['users']['creation']['count'] += $user_count;
      }
    }

    // Calculate percentages.
    foreach ($statistics['users']['creation']['dates'] as $date => $date_data) {
      ksort($statistics['users']['creation']['dates'][$date]['hours']);
      foreach ($date_data['hours'] as $hour => $hour_data) {
        $percentage = ($hour_data['count'] / $date_data['count']) * 100;
        $statistics['users']['creation']['dates'][$date]['hours'][$hour]['percentage'] = $percentage;
      }
    }
  }

  return $statistics;
}

/**
 * Loads and parses JSON from a given data file.
 *
 * @param string $file_uri
 *   The full file path of the file to be loaded.
 *
 * @return string
 *   The file contents.
 */
function _petitions_stats_users_load_data_file($file_uri) {
  // Load contents of existing files.
  $existing_file_contents = file_get_contents($file_uri);

  // Ensure that existing file contents are UTF8 encoded.
  $is_encoded = mb_detect_encoding($existing_file_contents, array('UTF-8'), TRUE);
  if (!$is_encoded) {
    $existing_file_contents = utf8_encode($existing_file_contents);
  }
  // JSON decode existing file contents.
  $existing_file_contents = json_decode($existing_file_contents);

  return $existing_file_contents;
}

/**
 * Finds most recent file matching pattern "/basename_[0-9]+/".
 *
 * @param string $directory
 *   The directory to which the data file will be saved.
 * @param string $filename
 *   The filename (with extension) to be save.
 *
 * @return string
 *   The full system path of the most recently saved data file.
 */
function _petitions_stats_users_most_recent_fileuri($directory, $filename) {
  // Get basename of file (filename without extension).
  $pos = strrpos($filename, '.');
  if ($pos !== FALSE) {
    $basename = substr($basename, 0, $pos);
    $ext = substr($basename, $pos);
  }
  else {
    $basename = $filename;
  }

  $mask = '/' . $basename . '_[0-9]+$/';
  if ($existing_files = file_scan_directory($directory, $mask)) {
    // Find the maximum numerical suffix to determine most recent file.
    $max = NULL;
    foreach ($existing_files as $existing_file_uri => $existing_file) {
      $pos = strrpos($existing_file->name, '_');
      $suffix = substr($existing_file->name, $pos + 1);
      if ($max < $suffix) {
        $max = $suffix;
        $file_uri = $existing_file_uri;
      }
    }
  }
  else {
    $file_uri = $directory . '/' . $filename;
  }

  return $file_uri;
}

/**
 * Ensures that JSON array contains only unique objects.
 *
 * @param array $merged
 *   An array of objects.
 *
 * @see http://stackoverflow.com/questions/4507518/will-array-unique-work-also-with-array-of-objects#4507991
 *
 * @return array
 *   An array of unique objects.
 */
function _petitions_stats_users_unique_json($merged) {
  $final  = array();
  foreach ($merged as $current) {
    // As of PHP5, in_array() can be used on objects.
    if (!in_array($current, $final)) {
      $final[] = $current;
    }
  }
  return $final;
}

/**
 * Delete all JSON files.
 */
function _petitions_stats_users_delete_json_data() {
  // Delete data files.
  $data_dir = variable_get('petitions_stats_users_data_dir', 'petitions_stats_users_data');
  $path = file_build_uri($data_dir);
  $vars = array('@path' => $path);

  if (file_unmanaged_delete_recursive($path)) {
    $message = 'Wiped Petitions Statistics Users data from @path.';
    watchdog('petitions_stats_users', $message, $vars);
    return TRUE;
  }
  else {
    $message = 'Could not wipe Petitions Statistics Users data from @path';
    watchdog('petitions_stats_users', $message, $vars, WATCHDOG_ERROR);
    return FALSE;
  }
}
