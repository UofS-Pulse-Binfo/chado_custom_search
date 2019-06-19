<?php

/**
 * @file
 * Provides API functions for this module.
 */

/**
 * Retrieves search information.
 *
 * @param string $machine_name
 *   The machine name of the search you would like returned.
 * @return array
 *   Returns the specified search information array.
 */
function get_chado_custom_search($machine_name) {

  // Each search is defined by a function containing it's $machine_name.
  // We take advantage of that here for a performant way to pull up the
  // information that is not affected by the number of searches or the size
  // of your site code-base.
  $info_fn = $machine_name . '_ccsearch_info';
  if (function_exists($info_fn)) {
    $info = call_user_func($info_fn);
    return $info;
  }
  else {
    tripal_report_error(
      'chado_custom_search',
      TRIPAL_WARNING,
      'No information function defined for :name. Please define :info according to the API specifications.',
      [':name' => $machine_name, ':info' => $info_fn]
    );
    return FALSE;
  }
}

/**
 * Compiles all searches based on the hook and returns them.
 *
 * NOTE: This is intensive, if it all possible use get_chado_custom_search().
 *
 * @return array
 *   An array of all defined searches.
 */
function get_all_chado_custom_searches() {
  $searches = [];

  // Call hook_chado_custom_search() to get a list of chado custom searches.
  $defined_searches = module_invoke_all('chado_custom_search');
  // @debug dpm($defined_searches, 'defined searches');

  // Return results.
  return $defined_searches;
}
