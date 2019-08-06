
Quick Start
=============

1. Implement `hook_chado_custom_search` to tell the API about the search you would like to create.

 .. code-block:: php

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

2. Create a class which extends ``ChadoCustomSearch``. At a minimum you need to set the editable static constants and the ``getQuery()`` method. Just copy `this basic search <https://github.com/UofS-Pulse-Binfo/chado_custom_search/blob/master/example_ccsearch/examples/BreedingCrossSearch.inc>`_ and change it to suit your needs.

3. Clear the cache, navigate to the path defined in the class and enjoy your custom search!
