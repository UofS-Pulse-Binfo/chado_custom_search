# Chado Custom Search

**UNDER DEVELOPMENT: DESIGN PHASE**

Provides custom search functionality for those who don't wish to use Drupal Views.

Why might you want to use this module?
- Gives you complete control of the query so you can tweak performance.
- Removes query-determination overhead of Drupal Views for better performance.
- Lets you make quick custom searches which can be under version control.
- Saves you from having to render the result table, generate the filter form,
  handle the user input and execute the query.
- Lets you focus on customizing only what you want to!

## Chado Custom Search API

1. Implement `hook_chado_custom_search` to tell the API about the search you would like to create.

```php
/**
 * Implement hook_chado_custom_search().
 *
 * This hook simply lists the machine name of the searches so that we can find
 * the info hook. We've done this for performance reasons.
 */
function example_ccsearch_chado_custom_search() {
  $searches = [];

  $searches['BreedingCrossSearch'] = 'Breeding Cross Search';

  return $searches;
}
```

2. Create a class which extends ChadoCustomSearch. At a minimum you need to set the editable static constants and the getQuery() method. See [BreedingCrossSearch](https://github.com/UofS-Pulse-Binfo/chado_custom_search/blob/master/example_ccsearch/examples/BreedingCrossSearch.inc) for an example.

3. Clear the cache, navigate to the path defined in the class and enjoy your custom search!
