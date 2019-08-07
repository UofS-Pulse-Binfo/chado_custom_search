
formatResults: Customize result display
================================================

The ``formatResults()`` method is used to format the results from the query. The results are formatted as a table by default. If you wish to override the result display, make sure to add your custom formatted results to the ``$form`` array for them to be displayed.

.. code-block:: php

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

    // Format the results in the $results array.
    $output = '';

    // Then add the formatted results to the form.
    $form['results'] = [
      '#type' => 'markup',
      '#markup' => $output,
    ];
  }
