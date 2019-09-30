<?php

/**
 * @file
 * Provides API functions for this module.
 */

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
