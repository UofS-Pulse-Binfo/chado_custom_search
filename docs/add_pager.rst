
add_pager: Override pager
===========================

To change the pager, override the ``add_pager()`` method.

.. code-block:: php

  /**
   * Adds a pager to the form.
   *
   * @param $form
   *   The form array to add the pager to.
   * @param $form_state
   *   The current state of the form.
   * @return
   *   The original form with the pager added.
   */
  public function add_pager($form, $num_results) {

    // Determine the current page and offset using the query parameters.
    $q = drupal_get_query_parameters();
    $offset = (isset($q['offset'])) ? $q['offset'] : 0;
    $page_num = (isset($q['page_num'])) ? $q['page_num'] : 1;

    // Do your logic to render your pager.
    $output = '';

    // Add the pager to the result page.
    $form['pager'] = [
      '#type' => 'markup',
      '#markup' => $output,
      '#prefix' => '<div class="pager-container"><div class="pager">',
      '#suffix' => '</div></div>',
      '#weight' => 100,
    ];

    return $form;
  }
