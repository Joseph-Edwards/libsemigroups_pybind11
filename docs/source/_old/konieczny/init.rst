.. Copyright (c) 2022-2024, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Constructors
============

This page contains information about the constructors for the
:py:class:`Konieczny` class.

.. py:method:: Konieczny.__init__(self: Konieczny, coll: List[Element]) -> Konieczny

   Construct from list of generators.

   This function constructs a :py:obj:`Konieczny` instance generated by the
   specified list of generators. There can be duplicate generators and although
   they do not count as distinct elements, they do count as distinct
   generators. In other words, the generators are precisely (a copy of)
   ``coll`` in the same order they occur in ``coll``.

   :Parameters:
     **coll** (List[Element]) the generators
   :Raises:
     **RuntimeError** if ``coll`` is empty.
   :Raises:
     **RuntimeError** if the items in ``coll`` do not all have the same degree.
