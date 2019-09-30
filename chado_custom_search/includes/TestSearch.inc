<?php
/**
 * @file
 * An Chado custom search for testing purposes only.
 */

 /**
  * Implement hook_chado_custom_search().
  *
  * This hook simply lists the machine name of the searches so that we can find
  * the info hook. We've done this for performance reasons.
  */
 function chado_custom_search_chado_custom_search() {
   $searches = [];

   $searches['CoreTestingSearch'] = 'Core Testing Search';

   return $searches;
 }


/**
 * The class.
 */
class CoreTestingSearch extends ChadoCustomSearch {

   /**
    * The human readable title of this search. This will be used in listings
    * and shown on the search page as the title.
    */
   public static $title = 'TEST Search';

   /**
    * The machine name of the module providing this search.
    */
   public static $module = 'chado_custom_search';

   /**
    * A description of this search. This is shown at the top of the search page
    * and used for the menu item.
    */
   public static $description = 'An extremely simple search for testing purposes only.';

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
     'path' => 'admin/tripal/extension/chado_custom_search/test-search',
     // @todo support menu items.
   ];

   /**
    * Information regarding the fields and filters for this search.
    */
   public static $info = [
     // Lists the columns in your results table.
     'fields' => [
       'genus' => [
          'title' => 'Genus'
        ],
       'species' => [
          'title' => 'Species',
       ],
       'common_name' => [
          'title' => 'Common Name',
        ],
     ],
     // The filter criteria available to the user.
     // This is used to generate a search form which can be altered.
     'filters' => [
       'genus' => [
         'title' => 'Genus',
         'help' => 'The genus of the organism.',
       ],
       'species' => [
         'title' => 'Species',
         'help' => 'The species of the organism.',
       ],
     ],
   ];

   /**
    * Text that should appear on the button at the bottom of the importer
    * form.
    */
   public static $button_text = 'Search';

   public static $require_submit = FALSE;

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

     $query = "SELECT * FROM {organism}";

     // Now we add the where arguments based on the filter results.
     // NOTE: make your placeholders match the key in the $filter_results array.
     //--------------------------------------------------------------------------
     // Genus.
     if (!empty($filter_results['genus'])) {
       $query .= ' AND genus = :genus';
       $args[':genus'] = $filter_results['genus'];
     }

     // Species.
     if (!empty($filter_results['species'])) {
       $query .= ' AND species = :species';
       $args[':species'] = $filter_results['species'];
     }

     $query .= ' ORDER BY genus ASC, species ASC';
     $query .= ' LIMIT 10';
   }
}
