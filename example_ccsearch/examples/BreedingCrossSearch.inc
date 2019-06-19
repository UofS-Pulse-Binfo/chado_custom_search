<?php

/**
 * Provides a search at `search-crosses` for looking up parents of germplasm.
 *
 * ASSUMPTION: Parents are connected to a cross by is_maternal_parent and
 *   is_paternal_parent cvterms.
 */
class BreedingCrossSearch extends ChadoCustomSearch {

  /**
   * The human readable title of this search. This will be used in listings
   * and shown on the search page as the title.
   */
  public static $title = 'Breeding Cross Search';

  /**
   * The machine name of the module providing this search.
   */
  public static $module = 'example_ccsearch';

  /**
   * A description of this search. This is shown at the top of the search page
   * and used for the menu item.
   */
  public static $description = 'A simple search for finding parents of breeding crosses.';

  /**
   * The machine names of the permissions with access to this search. This is
   * used to map your search to existing permissions. It must be an array and
   * is used in hook_menu(). Some examples include 'access content'
   * and 'administer tripal'.
   */
  public static $permissions = ['access content'];

  /**
   * This defined the hook_menu definition for this search. At a minimum the
   * path is required.
   */
  public static $menu = [
    'path' => 'search-crosses',
    // @todo support menu items.
  ];

  /**
   * Information regarding the fields and filters for this search.
   */
  public static $info = [
    // Lists the columns in your results table.
    'fields' => [
      'child_name' => 'Cross Name',
      'mom_name' => 'Maternal Parent',
      'dad_name' => 'Paternal Parent',
    ],
    // The filter criteria available to the user.
    // This is used to generate a search form which can be altered.
    'filters' => [
      'child_name' => [
        'title' => 'Cross Number',
        'help' => 'The unique cross number within the breeding program.',
      ],
      'mom_name' => [
        'title' => 'Maternal Parent',
        'help' => 'The germplasm used as the maternal parent for the cross.',
      ],
      'dad_name' => [
        'title' => 'Paternal Parent',
        'help' => 'The germplasm used as the paternal parent for the cross.',
      ],
    ],
  ];

  /**
   * Text that should appear on the button at the bottom of the importer
   * form.
   */
  public static $button_text = 'Search';

  /**
   * Determine the query for the breeding cross search.
   *
   * Searches for the parents of a given cross. It can be filtered by cross
   * name or either parent.
   *
   * ASSUMPTION: Parents are connected to a cross by is_maternal_parent and
   *   is_paternal_parent cvterms.
   *
   * @param string $query
   *   The full SQL query to execute. This will be executed using chado_query()
   *   so use curly brackets appropriately. Use :placeholders for any values.
   * @param array $args
   *   An array of arguments to pass to chado_query(). Keys must be the
   *   placeholders in the query and values should be what you want them set to.
   */
  public function getQuery(&$query, &$args) {

    // Retrieve the filter results already set.
    $filter_results = $this->values;
    $form_submitted = $this->submitted;

    $query = "SELECT
        child.name as child_name, mom.name as mom_name, dad.name as dad_name
       FROM {stock} mom
       LEFT JOIN {stock_relationship} relmom ON relmom.subject_id=mom.stock_id
       LEFT JOIN {stock} child ON relmom.object_id=child.stock_id
       LEFT JOIN {stock_relationship} reldad ON reldad.object_id=child.stock_id
       LEFT JOIN {stock} dad ON dad.stock_id=reldad.subject_id
       WHERE
         relmom.type_id IN (SELECT cvterm_id FROM chado.cvterm WHERE name~'maternal') AND
         reldad.type_id IN (SELECT cvterm_id FROM chado.cvterm WHERE name~'paternal')";

    // Now we add the where arguments based on the filter results.
    // NOTE: make your placeholders match the key in the $filter_results array.
    //--------------------------------------------------------------------------
    // Cross Name.
    if (!empty($filter_results['cross_name'])) {
      $query .= ' AND child.name = :cross_name';
      $args[':cross_name'] = $filter_results['cross_name'];
    }

    // Maternal Parent.
    if (!empty($filter_results['mom_name'])) {
      $query .= ' AND mom.name = :mom_name';
      $args[':mom_name'] = $filter_results['mom_name'];
    }

    // Paternal Parent.
    if (!empty($filter_results['dad_name'])) {
      $query .= ' AND dad.name = :dad_name';
      $args[':dad_name'] = $filter_results['dad_name'];
    }

    $query .= ' ORDER BY child.name ASC';
    $query .= ' LIMIT 50';
  }
}
