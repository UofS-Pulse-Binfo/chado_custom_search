
getQuery(): Define your search query
=======================================

This API does not programmatically generate the query for you. Instead, you must define the query yourself in the ``getQuery()`` method of your custom search class. This design decision was made based on the core goal of the API (Faster, optimized searches) and ensures I don't make assumptions about your data but rather leave it in the hands of the expert: you!

.. note::

  I highly recommend testing your search queries on a full clone of your production site as the PostgreSQL query planner will base the underlying method on the data being queried. As such, the most performant query on a small test site will not be the same as on your production site.

.. code-block:: php

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
   * @param $offset
   *   The current offset. This is primarily used for pagers.
   */
  public function getQuery(&$query, &$args, $offset) {}

To start, define the above method in your custom search class. At the top of your method, the ``$query`` variable will be empty -it is up to you to defined the query as a string. Lets look at a simple example with no filters:

.. code-block:: php

  public function getQuery(&$query, &$args, $offset) {

    $query = 'SELECT organism_id, genus, species FROM {organism}';

  }

This custom search will display all organisms in your chado database to the user in the results table. It does not take any filter criteria into account nor does it support paging.

Filter Criteria
-----------------

Filtering of queries is recommended to be done through Drupal placeholders. This helps protect your site from SQL injection and other security concerns. To implement Drupal placeholders in your query, add ``:placeholdername`` where you would normally concatenate a value and then add ``:placeholdername => $value`` to the ``$args`` array. The Chado Custom Search API will handle the rest!

.. code-block:: php

  public function getQuery(&$query, &$args, $offset) {

    // Retrieve the filter results already set.
    $filter_results = $this->values;

    // Define the base query.
    $query = 'SELECT organism_id, genus, species FROM {organism}';

    // If a filter value is available then add it to a list of
    // where clauses to be added.
    $where = [];
    // -- genus
    if (!empty($filter_results['genus'])) {
      $where[] = 'genus = :genus';
      $args[':genus'] = $filter_results['genus'];
    }
    // -- species
    if (!empty($filter_results['species'])) {
      $where[] = 'species = :species';
      $args[':species'] = $filter_results['species'];
    }

    // Then after you've checked all your filter criteria,
    // add the where clauses to your base query.
    $query .= ' WHERE ' . implode(' AND ', $where);
  }

Paging
--------

The Chado Custom Search API handles determining the offset required for the page the user is on based on the number of items per page you set in the editable constants. As such, all you need to do in your query, is add the limit/offset as so:

.. code-block:: php

  public function getQuery(&$query, &$args, $offset) {

    // Determine what your query should be based on the filter criteria.

    // Handle paging.
    $limit = $this::$num_items_per_page + 1;
    $query .= "\n" . ' LIMIT ' . $limit . ' OFFSET ' . $offset;

  }
