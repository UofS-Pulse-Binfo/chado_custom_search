<?php
namespace Tests;

use StatonLab\TripalTestSuite\DBTransaction;
use StatonLab\TripalTestSuite\TripalTestCase;

class CoreFunctionTest extends TripalTestCase {
  // Uncomment to auto start and rollback db transactions per test method.
  use DBTransaction;

  /**
   * get_all_chado_custom_searches() test.
   */
  public function testGetAllSearches() {

    $searches = get_all_chado_custom_searches();
    $this->assertArrayHasKey('CoreTestingSearch', $searches,
      'The test search was not returned by get_all_chado_custom_searches().');
  }

  /**
   * hook_menu test.
   */
  public function testMenu() {

    $menu_items = chado_custom_search_menu();

    // Ensure the Breeding Cross example is present.
    $testSearchPath = 'admin/tripal/extension/chado_custom_search/test-search';
    $this->assertArrayHasKey($testSearchPath, $menu_items,
      'The test search was not even listed in the menu items.');

    if (isset($menu_items[$testSearchPath])) {
      $test_menu_item = $menu_items[$testSearchPath];

      $this->assertEquals('TEST Search', $test_menu_item['title'],
        'The menu title did not match what we expected.');
      $this->assertEquals(
        'An extremely simple search for testing purposes only.', $test_menu_item['description'],
        'The menu description did not match what we expected.');
      $this->assertEquals('CoreTestingSearch', $test_menu_item['page arguments'][1],
        'The page arguments did not match what we expected.');
    }
  }

  /**
   * Core form test.
   */
  public function testCoreForm() {

    // Mock the arguments.
    $form_state = [];
    $form_state['build_info']['args'][0] = 'CoreTestingSearch';
    $form = chado_custom_search_form([], $form_state);

    // Check the filters have been converted to form elements.
    $this->assertArrayHasKey('genus', $form,
      'The test search form did not include the genus filter.');
    $this->assertArrayHasKey('species', $form,
      'The test search form did not include the species filter.');
    // We check this to make sure the filters array specifically is being used.
    $this->assertArrayNotHasKey('common_name', $form,
      'There was a common_name filter even though there should not have been.');

    // We set the test search to not require submit therefore there
    // should already be results.
    $this->assertArrayHasKey('results', $form,
      'There was no results even though we set the test form to not require submission to query.');
    $this->assertCount(3, $form['results']['#header'],
      'The results table header should contain THREE elements.');
    $this->assertEquals('Genus', $form['results']['#header']['genus'],
      'The genus results table header is not what we expected.');
  }
}
