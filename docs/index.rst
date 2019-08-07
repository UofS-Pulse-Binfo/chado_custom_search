.. Chado Custom Search API documentation master file, created by
   sphinx-quickstart on Tue Aug  6 14:07:52 2019.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Chado Custom Search API
===========================

This module provides a class-based API for creating fast Chado-focused searches. If you find Drupal Views-based searches too slow due to the amount of data available or simply difficult to customize to your needs, this API is your solution!

For each search, define a single class and list it in ``hook_chado_custom_search()``. Just list your columns/filters and define the query to be used and you have a fully functional search with the following features:

- automatically generated filter form
- table listing of results
- links to Tripal 3 entities
- simple but fast pager
- supports either "require input" or "basic search"

This API also gives you complete control over the search. Override any portion of the default filter form, supply custom form validation, optimize your query, etc. For more information on what you can accomplish or how to get started, see the following documentation:

.. toctree::
   :maxdepth: 2

   quickstart
   editable-constants
   getQuery
   form
   formatResults
   add_pager
