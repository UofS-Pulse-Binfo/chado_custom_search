# Chado Custom Search

**UNDER DEVELOPMENT: DESIGN PHASE**

Provides custom search functionality for those who don't wish to use Drupal Views.

## Chado Custom Search API

1. Implement `hook_chado_custom_search` to tell the API about the search you would like to create.

```php
/**
 * Implement hook_chado_custom_search().
 */
function mymodule_chado_custom_search() {
  $searches = [];
  
  $searches[] = [
    'title' => 'Breeding Cross Search',
    'description' => '',
    'path' => 'search/cross',
    'menu' => [
      '
    ],
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
    'query_function' => 'breeding_cross_search_query',
    'form_alter' => 'breeding_cross_search_form',
  ];
  
  return $searches;
}
```

2. Implement the query function to dictate how the search should be performed. This can be as simple as returning the same query in all cases or you can determine the query based on the criteria passed in through the form by the user.

```php
/**
 * Determine the query for the breeding cross search.
 *
 * @param array $filter_results
 *    An array containing values submitted from the filter form by the user keyed by the 
 *    filters set in hook_chado_custom_search(). If the form hasn't been submitted it
 *    will contain any defaults set. All filter keys will be defined regardless of
 *    whether it has a value.
 * @param bool $form_submitted
 *    Indicates whether the form has been submitted by the user.
 */
 function breeding_cross_search_query($filter_results, $form_submitted) {
 
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

```

3. Clear the cache, navigate to the path defined in hook_chado_custom_search() and enjoy your custom search!
