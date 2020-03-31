<?php

/**
 *
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
