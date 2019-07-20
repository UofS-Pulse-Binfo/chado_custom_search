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
   * If TRUE, this search will require the submit button to be clicked before
   * executing the query; whereas, if FALSE it will be executed on the
   * first page load without parameters.
   *
   * NOTE: to control the results without parameters check $this->submitted
   * in getQuery() and if FALSE return your pre-submit query.
   */
  public static $require_submit = TRUE;

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
      'column_name' => [
        'title' => 'Title',
        // This keyval is optional. It's used to make the current
        // column a link. The link is made automagically as long as
        // you add the id_column to your query.
        'entity_link' = [
          'chado_table' => 'feature',
          'id_column' => 'feature_id',
        ],
      ],
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
   * The values submitted through the filter form by the user.
   */
  public $values;

  /**
   * Whether the user clicked the search button.
   */
  public $submitted;

  /**
   * Instantiates a new Chado Custom Search object.
   */
  public function __construct() {
    $class = get_called_class();

    foreach ($class::$info['filters'] as $name => $details) {
      $this->values[$name] = (isset($details['default'])) ? $details['default'] : NULL;
    }
    $this->submitted = FALSE;
  }

  /**
   * Sets the values from the form based on user input.
   */
  public function setValues($filter_values) {
    $class = get_called_class();

    // If we are setting values then we consider it submitted.
    $this->submitted = TRUE;

    // For each filter value, either set the passed in value
    // or set the default.
    foreach ($class::$info['filters'] as $name => $details) {
      if (isset($filter_values[$name])) {
        $this->values[$name] = $filter_values[$name];
      }
      else {
        $this->values[$name] = (isset($details['default'])) ? $details['default'] : NULL;
      }
    }
  }

  /**
   * Uses the class defined query and values to retrieve the results.
   */
  public function getResults() {

    // Grab the query defined for this specific child.
    $query = '';
    $args = [];
    $this->getQuery($query, $args);

    // Execute it.
    return chado_query($query, $args)->fetchAll();
  }

  // --------------------------------------------------------------------------
  //                     OVERRIDEABLE FUNCTIONS
  // --------------------------------------------------------------------------

  /**
   * Generate the filter form.
   *
   * The base class will generate textfields for each filter defined in $info,
   * set defaults, labels and descriptions, as well as, create the search
   * button.
   *
   * Extend this method to alter the filter form.
   */
  public function form($form, $form_state) {
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
   * Defines the query and arguments to use for the search.
   *
   * MUST OVERRIDE!
   *
   * @param string $query
   *   The full SQL query to execute. This will be executed using chado_query()
   *   so use curly brackets appropriately. Use :placeholders for any values.
   * @param array $args
   *   An array of arguments to pass to chado_query(). Keys must be the
   *   placeholders in the query and values should be what you want them set to.
   */
  public function getQuery(&$query, &$args) {}

  /**
   * Format the results within the $form array.
   *
   * The base class will format the results as a table.
   *
   * @param array $form
   *   The current form array.
   * @param array $results
   *   The results to format. This will be an array of standard objects where
   *   the keys map to the keys in $info['fields'].
   */
  public function formatResults(&$form, $results) {
    $class = get_called_class();

    $table = [
      'attributes' => [],
      'caption' => '',
      'colgroups' => [],
      'sticky' => FALSE,
      'empty' => '',
    ];
    $table['header'] = [];
    $template_row = [];
    $link = [];
    foreach ($class::$info['fields'] as $name => $details) {
      $label = $details['title'];
      $table['header'][$name] = $label;
      $template_row[$name] = '';
      if (isset($details['entity_link'])) {
        $link[$name]['entity_link'] = $details['entity_link'];
      }
      else {
        $link[$name]['entity_link'] = FALSE;
      }
    }

    $table['rows'] = [];
    foreach ($results as $r) {
      $row = [];
      foreach ($template_row as $key => $default) {
        if (isset($r->{$key})) {
          $row[$key] = $r->{$key};
          if ($link[$key]['entity_link']) {
            $entity_id = chado_get_record_entity_by_table(
              $link[$key]['entity_link']['chado_table'],
              $r->{$link[$key]['entity_link']['id_column']}
            );
            $row[$key] = l($r->{$key}, '/bio_data/'.$entity_id);
          }
        }
        else {
          $row[$key] = '';
        }
      }
      $table['rows'][] = $row;
    }

    $form['results'] = [
      '#type' => 'markup',
      '#markup' => theme_table($table),
    ];
  }

}
