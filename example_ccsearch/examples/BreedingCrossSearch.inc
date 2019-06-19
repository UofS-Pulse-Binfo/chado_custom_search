<?php

/**
 *
 */
class BreedingCrossSearch extends ChadoCustomSearch {

  /**
   * The human readable title of this search. This will be used in listings
   * and shown on the search page as the title.
   */
  public static $title = 'Breeding Cross Search';

  /**
   * The machine name of the search. This should not include spaces or special
   * characters.
   */
  public static $machine_name = 'breeding_cross_search';

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
      'cross_name' => 'Cross Name',
      'mom_name' => 'Maternal Parent',
      'dad_name' => 'Paternal Parent',
      'cross_block' => 'Crossing Block',
    ],
    // The filter criteria available to the user.
    // This is used to generate a search form which can be altered.
    'filters' => [
      'cross_name' => [
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
      'block_season' => [
        'title' => 'Season',
        'help' => 'The season in which the cross was made.',
        'default' => 'Summer',
      ],
      'block_year' => [
        'title' => 'Year',
        'help' => 'The year in which the cross was made.',
        'default' => 2019,
      ],
    ],
  ];

  /**
   * Text that should appear on the button at the bottom of the importer
   * form.
   */
  public static $button_text = 'Search';

}
