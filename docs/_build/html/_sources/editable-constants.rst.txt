
Editable Constants
====================

The `ChadoCustomSearch <https://github.com/UofS-Pulse-Binfo/chado_custom_search/blob/master/chado_custom_search/ChadoCustomSearch.inc>`_ defines a number of editable constants to make it easy to customize your search.

Title
------

The human readable title of this search. This will be used in listings and shown on the search page as the title.

.. code-block:: php

  public static $title = 'Custom Chado Search';

Module
--------

The machine name of the module providing this search.

.. code-block:: php

  public static $module = 'custom_chado_search';

Description
-------------

A description of this search. This is shown at the top of the search page and used for the menu item.

.. code-block:: php

  public static $description = 'A default search which must be extended.';

Permissions
-------------

The machine names of the permissions with access to this search. This is used to map your search to existing permissions. It must be an array and is used in hook_menu(). Some examples include 'access content' and 'administer tripal'.

.. code-block:: php

  public static $permissions = ['access content'];

If you want to use custom permissions, make sure to defined them first in your module file using `hook_permission() <https://www.drupal.org/docs/7/creating-custom-modules/specifying-a-custom-permission-for-a-new-page#custom-permission>`_.

Require Submit
----------------

If TRUE, this search will require the submit button to be clicked before executing the query; whereas, if FALSE it will be executed on the first page load without parameters.

.. code-block:: php

  public static $require_submit = TRUE;

.. note::

  Keep this in mind when building your query in the ``getQuery()`` method. If this variable is set to FALSE, then defaults will be supplied to ``getQuery()`` as the filter values.

Paging
-------

If ``$pager`` is true, the API will add a simple pager to your search results with a previous and next page link as well as indication of what page they are on. A simple pager is used for speed since it doesn't require counting the search results which is notoriously slow in PostgreSQL. You can also control the number of items per page using ``$num_items_per_page``.

.. code-block:: php

  public static $pager = TRUE;
  public static $num_items_per_page = 25;

.. note::

  The API handles genering the pager on the search page, determining which links should be available and changing the filter values to keep track of the page. **You need to ensure your getQuery() method returns the appropriate results for a given page.**

Menu
------

This defined the hook_menu definition for this search. At a minimum the path is required.

.. code-block:: php

  public static $menu = [
    'path' => 'search/chado',
  ];

Attached CSS/JS
-----------------

This allows you to add custom CSS and JS files to your search form and results page.

.. code-block:: php

  public static $attached = [
    'css' => [],
    'js' => [],
  ];

Field/Filter Information
--------------------------

This is arguably the most important editable constant. This is where you indicate the columns you want displayed in your results table (fields) and the filters you want made available to your users (filters).

.. code-block:: php

  public static $info = [
    // Lists the columns in your results table.
    'fields' => [
      'column_name' => [
        'title' => 'Title',
        // This keyval is optional. It's used to make the current
        // column a link. The link is made automagically as long as
        // you add the id_column to your query.
        'entity_link' => [
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

This API supports result links for Tripal Entities. Define the entity link for any field/column in your results table and the API will look up the appropriate bio_data_id given the chado base table id and the name of the base table. This is done using the ``chado_get_record_entity_by_table()`` API function provided by Tripal.

Button Text
--------------

This allows you to customize the text shown on the submit button at the bottom of the filter form.

.. code-block:: php

  public static $button_text = 'Search';
