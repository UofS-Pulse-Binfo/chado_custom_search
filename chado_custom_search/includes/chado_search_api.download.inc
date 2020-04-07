<?php

/**
 * @file
 *
 * Implements the Tripal Download API to provide CSV and TSV downloads for
 * ALL chado custom searches.
 */

/**
 * Implements hook_register_trpdownload_type().
 */
function chado_custom_search_register_trpdownload_type() {
  $types = array();

  // The key is the machine name of my download type.
  $types['chado_custom_search_csv'] = [
    // A human readable name to show in an administrative interface one day.
    'type_name' => 'Chado Custom Search CSV',
    // A human readable description of the format.
    'format' => 'Comma-separated Values',
    // An array of functions that the API will use to customize your experience.
    'functions' => [
      // The function that tripal jobs will call to generate the file.
      'generate_file' => 'chado_custom_search_csv_generate_file',
      // OPTIONAL: determine your own filename.
      'get_filename' => 'chado_custom_search_get_filename',
      // OPTIONAL: Change the file suffix (defaults to .txt)
      'get_file_suffix' => 'chado_custom_search_csv_get_suffix',
    ],
  ];

  // The key is the machine name of my download type.
  $types['chado_custom_search_tsv'] = [
    // A human readable name to show in an administrative interface one day.
    'type_name' => 'Chado Custom Search TSV',
    // A human readable description of the format.
    'format' => 'Tab-delimited Values',
    // An array of functions that the API will use to customize your experience.
    'functions' => [
      // The function that tripal jobs will call to generate the file.
      'generate_file' => 'chado_custom_search_tsv_generate_file',
      // OPTIONAL: determine your own filename.
      'get_filename' => 'chado_custom_search_get_filename',
      // OPTIONAL: Change the file suffix (defaults to .txt)
      'get_file_suffix' => 'chado_custom_search_tsv_get_suffix',
    ],
  ];

  return $types;
}


/**
 * Generates a file for download in the specified format.
 *
 * @param $variables
 *   An associative array of parameters including:
 *     - q: all the query paramters.
 *     - site_safe_name: a sanitized version of your site name for use in variables & filenames.
 *     - type_info: an array of info for the download type.
 *     - suffix: the file format suffix.
 *     - filename: the filename of the file to generate not including path.
 *     - fullpath: the full path and filename of the file to generate.
 *     - format_name: a human-readable description of the format.
 * @param $job_id
 *   The ID of the tripal job executing this function ;-).
 */
function chado_custom_search_csv_generate_file($vars, $job_id = NULL) {
  if (!isset($vars['drush'])) { $vars['drush'] = TRUE; }

  // Create the file and ready it for writting to.
  $filepath = variable_get('trpdownload_fullpath', '') . $vars['filename'];
  if ($vars['drush']) { drush_print("File: " . $filepath); }
  $FILE = fopen($filepath, 'w') or die ("Unable to create file to write to.\n");

  // First pull out our query paramters.
  $q = $vars['q'];

  // Next, create an instance of the class to query.
  $class = $vars['q']['class'];
  $search = new $class;

  // Compile our values array based on paramters.
  $doQuery = FALSE;
  $values = NULL;
  if ($search::$require_submit == FALSE) {
    $doQuery = TRUE;

    // Ensure that initial query takes into account URL parameters if present.
    $values = [];
    foreach ($class::$info['filters'] as $name => $details) {
      if (isset($q[$name])) {
        $values[$name] = $q[$name];
      }
    }
  }
  elseif(!empty($q)) {
    $doQuery = TRUE;

    // Ensure that initial query takes into account URL parameters if present.
    $values = [];
    foreach ($class::$info['filters'] as $name => $details) {
      if (isset($q[$name])) {
        $values[$name] = $q[$name];
      }
    }
  }
  else {
    tripal_report_error(
      'chado_search_api',
      TRIPAL_ERROR,
      "Unable to download data for job :id as there were no values supplied.",
      [':id' => $job_id]
    );
  }

  // Now, query the database for the results.
  if ($doQuery) {

    // First set the filter criteria for our search.
    $search->setValues($values, TRUE);
    // Now ensure we start with the first page.
    $offset = 0;

    // Now, we start our loop to page through the results.
    do {
      // Retrieve the results for the current page.
      $results = $search->getResults($offset);

      // First generate the header and the template row.
      $header = [];
      $template_row = [];
      foreach ($class::$info['fields'] as $name => $details) {
        $header[$name] = $details['title'];
        $template_row[$name] = '';
      }

      // If this is the first page then print the header.
      if ($offset == 0) {
        fputcsv($FILE, $header);
      }

      // Now compile each row from the results.
      $num_rows = 0;
      foreach ($results as $r) {
        $num_rows++;
        $row = [];
        foreach ($template_row as $key => $default) {
          if (isset($r->{$key})) {
            $row[$key] = $r->{$key};
          }
          else {
            $row[$key] = '';
          }
        }
        fputcsv($FILE, $row);
      }

      // Finally, flip to the next page by increasing the offset
      // by the number of records we've processed.
      $offset = $offset + $num_rows;

    // Now we loop to the next page, assuming this wasn't the last page.
    } while ($num_rows != 0);
  }
}

/**
 * Generates a file for download in the specified format.
 *
 * @param $variables
 *   An associative array of parameters including:
 *     - q: all the query paramters.
 *     - site_safe_name: a sanitized version of your site name for use in variables & filenames.
 *     - type_info: an array of info for the download type.
 *     - suffix: the file format suffix.
 *     - filename: the filename of the file to generate not including path.
 *     - fullpath: the full path and filename of the file to generate.
 *     - format_name: a human-readable description of the format.
 * @param $job_id
 *   The ID of the tripal job executing this function ;-).
 */
function chado_custom_search_tsv_generate_file($vars, $job_id = NULL) {
  if (!isset($vars['drush'])) { $vars['drush'] = TRUE; }

  // Create the file and ready it for writting to.
  $filepath = variable_get('trpdownload_fullpath', '') . $vars['filename'];
  if ($vars['drush']) { drush_print("File: " . $filepath); }
  $FILE = fopen($filepath, 'w') or die ("Unable to create file to write to.\n");

  // First pull out our query paramters.
  $q = $vars['q'];

  // Next, create an instance of the class to query.
  $class = $vars['q']['class'];
  $search = new $class;

  // Compile our values array based on paramters.
  $doQuery = FALSE;
  $values = NULL;
  if ($search::$require_submit == FALSE) {
    $doQuery = TRUE;

    // Ensure that initial query takes into account URL parameters if present.
    $values = [];
    foreach ($class::$info['filters'] as $name => $details) {
      if (isset($q[$name])) {
        $values[$name] = $q[$name];
      }
    }
  }
  elseif(!empty($q)) {
    $doQuery = TRUE;

    // Ensure that initial query takes into account URL parameters if present.
    $values = [];
    foreach ($class::$info['filters'] as $name => $details) {
      if (isset($q[$name])) {
        $values[$name] = $q[$name];
      }
    }
  }
  else {
    tripal_report_error(
      'chado_search_api',
      TRIPAL_ERROR,
      "Unable to download data for job :id as there were no values supplied.",
      [':id' => $job_id]
    );
  }

  // Now, query the database for the results.
  if ($doQuery) {

    // First set the filter criteria for our search.
    $search->setValues($values, TRUE);
    // Now ensure we start with the first page.
    $offset = 0;

    // Now, we start our loop to page through the results.
    do {
      // Retrieve the results for the current page.
      $results = $search->getResults($offset);

      // First generate the header and the template row.
      $header = [];
      $template_row = [];
      foreach ($class::$info['fields'] as $name => $details) {
        $header[$name] = $details['title'];
        $template_row[$name] = '';
      }

      // If this is the first page then print the header.
      if ($offset == 0) {
        fwrite($FILE, implode("\t", $header) . "\n");
      }

      // Now compile each row from the results.
      $num_rows = 0;
      foreach ($results as $r) {
        $num_rows++;
        $row = [];
        foreach ($template_row as $key => $default) {
          if (isset($r->{$key})) {
            $row[$key] = $r->{$key};
          }
          else {
            $row[$key] = '';
          }
        }
        fwrite($FILE, implode("\t", $row) . "\n");
      }

      // Finally, flip to the next page by increasing the offset
      // by the number of records we've processed.
      $offset = $offset + $num_rows;

    // Now we loop to the next page, assuming this wasn't the last page.
    } while ($num_rows != 0);
  }
}

/**
 * Generate a readable and unique filename for the file to be generated.
 */
function chado_custom_search_get_filename($vars) {
  $filename = $vars['download_args']['safe_site_name'] .'.searchresults.' . date('YMj-his');
  return $filename;
}

/**
 * Determine the file suffix for the file to be generated.
 */
function chado_custom_search_csv_get_suffix($vars) {
  return 'csv';
}

/**
 * Determine the file suffix for the file to be generated.
 */
function chado_custom_search_tsv_get_suffix($vars) {
  return 'tsv';
}
