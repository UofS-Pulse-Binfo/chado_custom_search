<?php

/**
 * @class
 * Default class for custom chado searches.
 */
class ChadoCustomSearch {

  // --------------------------------------------------------------------------
  //                     EDITABLE STATIC CONSTANTS
  //
  // The following constants SHOULD be set for each descendent class.  They are
  // used by the static functions to provide information to Drupal about
  // the field and it's default widget and formatter.
  // --------------------------------------------------------------------------

  /**
   * The human readable title of this search. This will be used in listings
   * and shown on the search page as the title.
   */
  public static $title = 'Custom Chado Search';

  /**
   * The machine name of the search. This should not include spaces or special
   * characters.
   */
  public static $machine_name = 'custom_chado_search';

  /**
   * The machine name of the module providing this search.
   */
  public static $module = 'custom_chado_search';

  /**
   * A description of this search. This is shown at the top of the search page
   * and used for the menu item.
   */
  public static $description = 'A default search which must be extended.';

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
    'path' => 'search/chado',
    // @todo support menu items.
  ];

  /**
   * Information regarding the fields and filters for this search.
   */
  public static $info = [
    // Lists the columns in your results table.
    'fields' => [
      'column_name' => 'Title',
    ],
    // The filter criteria available to the user.
    // This is used to generate a search form which can be altered.
    'filters' => [
      'column_name' => [
        'title' => 'Title',
        'help' => 'A description for users as to what this filter is.',
      ],
    ],
  ];

  /**
   * Text that should appear on the button at the bottom of the importer
   * form.
   */
  public static $button_text = 'Search';

  // --------------------------------------------------------------------------
  //                  PRIVATE MEMBERS -- DO NOT EDIT or OVERRIDE
  // --------------------------------------------------------------------------

  /**
   * Instantiates a new Chado Custom Search object.
   */
  public function __construct() {

  }

  // --------------------------------------------------------------------------
  //                     OVERRIDEABLE FUNCTIONS
  // --------------------------------------------------------------------------

  public function form ($form, $form_state) {
    $class = get_called_class();

    $form['header'] = [
      '#type' => 'markup',
      '#markup' => '<p>' . $class::$description . '</p>',
    ];

    foreach ($class::$info['filters'] as $name => $details) {
      $form[$name] = [
        '#type' => 'textfield',
        '#title' => $details['title'],
        '#description' => $details['help'],
        '#default_value' => (isset($details['default'])) ? $details['default'] : '',
      ];
    }

    $form['submit'] = [
      '#type' => 'submit',
      '#value' => $class::$button_text,
    ];

    return $form;
  }

  /**
   * Determine the query for the breeding cross search.
   *
   * @param array $filter_results
   *   An array containing values submitted from the filter form by the user
   *   keyed by the filters set in hook_chado_custom_search(). If the form hasn't
   *   been submitted it will contain any defaults set. All filter keys will be
   *   defined regardless of whether it has a value.
   * @param bool $form_submitted
   *   Indicates whether the form has been submitted by the user.
   */
  public function breeding_cross_search_query($filter_results, $form_submitted) {

     // We want to show an example crossing block if the user hasn't searched yet.
     // As such we set defaults in hook_chado_custom_search() for crossing block
     // season and year. This way we can query regardless of whether the form has
     // has been submitted.
     $query = "SELECT child.name as child, mom.name as mom, dad.name as dad
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
    }

    // Maternal Parent.
    if (!empty($filter_results['mom_name'])) {
      $query .= ' AND mom.name = :mom_name';
    }

    // Paternal Parent.
    if (!empty($filter_results['dad_name'])) {
      $query .= ' AND dad.name = :dad_name';
    }

    // Crossingblock Season.
    if (!empty($filter_results['block_season'])) {
      $query .= ' AND season.value = :block_season';
    }

    // Crossingblock Year.
    if (!empty($filter_results['block_year'])) {
      $query .= ' AND year.value = :block_year';
    }

    return $query;
  }

}
