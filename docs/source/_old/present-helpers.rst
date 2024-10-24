.. Copyright (c) 2022-2024, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.


Presentation helpers
====================

This page contains the documentation for various helper functions for
manipulating ``Presentation`` objects. All such functions are contained in the
submodule ``libsemigroups_pybind11.presentation``.

Contents
--------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:func:`add_identity_rules`
     - Add rules for an identity element.

   * - :py:func:`add_inverse_rules`
     - Add rules for inverses.

   * - :py:func:`add_rule_and_check`
     - Add a rule to the presentation by reference and check.

   * - :py:func:`add_rule`
     - Add a rule to the presentation.

   * - :py:func:`add_rules`
     - Add a rule to the presentation from another presentation.

   * - :py:func:`add_zero_rules`
     - Add rules for a zero element.

   * - :py:func:`are_rules_sorted`
     - Check if the rules :math:`u_1 = v_1, \ldots, u_n = v_n` satisfy :math:`u_1
       v_1 < \cdots < u_n v_n`, where :math:`<` is the shortlex order.

   * - :py:func:`change_alphabet`
     - Change or re-order the alphabet.

   * - :py:func:`character`
     - Return a possible character by index.

   * - :py:func:`first_unused_letter`
     - Return a possible character by index.

   * - :py:func:`greedy_reduce_length`
     - Greedily reduce the length of the presentation using
       :py:func:`longest_common_subword`.

   * - :py:func:`is_strongly_compressible`
     - Returns ``True`` if a :math:`1`-relation presentation can be strongly
       compressed.

   * - :py:func:`length`
     - Return the sum of the lengths of the rules.

   * - :py:func:`letter`
     - Return the sum of the lengths of the rules.

   * - :py:func:`longest_common_subword`
     - Return the longest common subword of the rules.

   * - :py:func:`longest_rule_length`
     - Return the longest length of any rule.

   * - :py:func:`longest_rule`
     - Return the index of the left hand side of the longest rule.

   * - :py:func:`make_semigroup`
     - Convert a monoid presentation to a semigroup presentation.

   * - :py:func:`make`
     - Make a presentation from another type of presentation or a
       ``FroidurePin`` instance.

   * - :py:func:`normalize_alphabet`
     - Modify the presentation so that the alphabet is :math:`\{0, \ldots, n -
       1\}` (or equivalent), and rewrites the rules to use this alphabet.

   * - :py:func:`reduce_complements`
     - If there are rules :math:`u = v` and :math:`v = w` where :math:`\lvert w
       \rvert < \lvert v \rvert`, then replace :math:`u = v` with :math:`u =
       w`.

   * - :py:func:`reduce_to_2_generators`
     - Reduce the number of generators in a :math:`1`-relation presentation to
       ``2``.

   * - :py:func:`redundant_rule`
     - Returns the index of the left hand side of a redundant rule.

   * - :py:func:`remove_duplicate_rules`
     - Remove duplicate rules.

   * - :py:func:`remove_redundant_generators`
     - Remove any trivially redundant generators.

   * - :py:func:`remove_trivial_rules`
     - Remove rules consisting of identical words.

   * - :py:func:`replace_subword`
     - Replace non-overlapping instances of a subword.

   * - :py:func:`replace_word`
     - Replace instances of a word occupying either side of a rule.

   * - :py:func:`reverse`
     - Reverse every rule.

   * - :py:func:`shortest_rule_length`
     - Return the shortest length of any rule.

   * - :py:func:`shortest_rule`
     - Return the index of the left hand side of the shortest rule.

   * - :py:func:`sort_each_rule`
     - Sort each rule :math:`u = v` so that the left hand side is shortlex
       greater than the right hand side.

   * - :py:func:`sort_rules`
     - Sort the rules :math:`u_1 = v_1, \ldots, u_n = v_n` so that :math:`u_1
       v_1 < \cdots < u_n v_n`, where :math:`<` is the shortlex order.

   * - :py:func:`strongly_compress`
     - Strongly compress a :math:`1`-relation presentation.

Full API
--------

.. py:function:: add_identity_rules(p: Presentation, e: Union[str, int]) -> None

   Add rules for an identity element.

   Adds rules of the form :math:`a e = e a = a` for every letter :math:`a` in
   the alphabet of ``p``, where :math:`e` is the second parameter.

   :param p: the presentation to add rules to
   :type p: Presentation
   :param e: the identity element
   :type e: str or int

   :returns: None

   .. doctest::

      >>> from libsemigroups_pybind11 import presentation, Presentation
      >>> p = Presentation("abc")
      >>> presentation.add_identity_rules(p, "c")
      >>> p.rules
      ['ac', 'a', 'ca', 'a', 'bc', 'b', 'cb', 'b', 'cc', 'c']

.. py:function:: add_inverse_rules(p: Presentation, vals: Union[str, List[int], e: Union[str, int]) -> None

   Add rules for inverses.

   The letter ``a`` with index ``i`` in ``vals`` is the inverse of the letter
   in the alphabet of ``p`` with index ``i``. The rules added are :math:`a_i
   b_i = e`, where the alphabet is :math:`\{a_i, \ldots, a_n\}`; the parameter
   ``vals`` is :math:`\{b_1, \ldots, b_n\}`; and :math:`e` is the 3rd
   parameter.

   :param p: the presentation to add rules to
   :type p: Presentation
   :param vals: the inverses
   :type vals: str or List[int]
   :param e: the identity element
   :type e: str or int

   :returns: None

   .. doctest::

      >>> from libsemigroups_pybind11 import presentation, Presentation
      >>> p = Presentation("abc")
      >>> presentation.add_inverse_rules(p, "bac", "c")
      >>> p.rules
      ['ab', 'c', 'ba', 'c']


.. py:function:: add_rule_and_check(p: Presentation, lhop: Union[str, List[int]], rhop: Union[str, List[int]]) -> None

   Add a rule to the presentation, and check that it is valid.

   Adds the rule with left hand side ``lhop`` and right hand side ``rhop`` to the rules,
   after checking that ``lhop`` and ``rhop`` consist entirely of letters in the
   alphabet of ``p`` (see :py:meth:`Presentation.validate_rules`).

   :param p: the presentation
   :type p: Presentation
   :param lhop: the left hand side of the rule
   :type lhop: str or List[int]
   :param rhop: the right hand side of the rule
   :type rhop: str or List[int]

   :returns: None


.. py:function:: add_rule(p: Presentation, lhop: Union[str, List[int]], rhop: Union[str, List[int]]) -> None

   Add a rule to the presentation.

   Adds the rule with left hand side ``lhop`` and right hand side ``rhop``
   to the rules.

   :param p: the presentation
   :type p: Presentation
   :param lhop: the left hand side of the rule
   :type lhop: str or List[int]
   :param rhop: the right hand side of the rule
   :type rhop: str or List[int]

   :returns: None

   :warning:
     No checks that the arguments describe words over the alphabet of the
     presentation are performed.

   .. doctest::

      >>> from libsemigroups_pybind11 import presentation, Presentation
      >>> p = Presentation("ab")
      >>> p.rules
      []
      >>> presentation.add_rule(p, "ab", "baa")
      >>> p.rules
      ['ab', 'baa']
      >>> presentation.add_rule(p, "aaa", "a")
      >>> p.rules
      ['ab', 'baa', 'aaa', 'a']


.. py:function:: add_rules(p: Presentation, q: Presentation) -> None

   Add all the rules from one presentation to another presentation.

   Adds all the rules of the second argument to the first argument, which is modified in-place.

   :param p: the presentation to add rules to
   :type p: Presentation
   :param q: the presentation with the rules to add
   :type q: Presentation

   :returns: None

   .. doctest::

      >>> from libsemigroups_pybind11 import presentation, Presentation
      >>> p = Presentation("ab")
      >>> presentation.add_rule(p, "ab", "baa")
      >>> presentation.add_rule(p, "aaa", "a")
      >>> p.rules
      ['ab', 'baa', 'aaa', 'a']
      >>> q = Presentation("ab")
      >>> presentation.add_rule(q, "bbbb", "b")
      >>> q.rules
      ['bbbb', 'b']
      >>> presentation.add_rules(p, q)
      >>> p.rules
      ['ab', 'baa', 'aaa', 'a', 'bbbb', 'b']
      >>> q.rules
      ['bbbb', 'b']


.. py:function:: add_zero_rules(p: Presentation, z: Union[str, int]) -> None

   Add rules for a zero element.

   Adds rules of the form :math:`a z = z a = z` for every letter :math:`a` in
   the alphabet of ``p``, where :math:`z` is the second parameter.

   :param p: the presentation to add rules to
   :type p: Presentation
   :param z: the zero element
   :type z: str or int

   :returns: None

   .. doctest::

      >>> from libsemigroups_pybind11 import presentation, Presentation
      >>> p = Presentation("abc")
      >>> presentation.add_zero_rules(p, "c")
      >>> p.rules
      ['ac', 'c', 'ca', 'c', 'bc', 'c', 'cb', 'c', 'cc', 'c']


.. py:function:: are_rules_sorted(p: Presentation) -> None

   Check if the rules :math:`u_1 = v_1, \ldots, u_n = v_n` satisfy :math:`u_1
   v_1 < \cdots < u_n v_n`, where :math:`<` is the shortlex order.

   :param p: the presentation to check
   :type p: Presentation

   :returns: ``True`` if the rules are sorted, and ``False`` if not.


.. py:function:: change_alphabet(p: Presentation) -> None

   Change or re-order the alphabet.

   This function replaces ``p.alphabet()`` with ``new_alphabet``, where
   possible, and re-writes the rules in the presentation using the new
   alphabet.


   :param p: the presentation
   :type p: Presentation
   :param new_alphabet: the replacement alphabet
   :type new_alphabet: Union[str, List[int]]

   :returns: None.

   :raises RuntimeError:
     if the size of ``p.alphabet()`` and ``new_alphabet`` do not agree.


.. py:function:: character(i: int) -> str

   Return a possible character by index.

   This function returns the ``i``-th letter in the alphabet consisting of all
   possible characters. This function exists so that visible ASCII characters
   occur before invisible ones, so that when manipulating presentations over
   strings the human readable characters are used before non-readable
   ones.

   :param i: the index
   :type i: int
   :returns: A ``str``.

   :raises RuntimeError:
     if ``i`` exceeds the number of letters in supported by ``str``.

   .. seealso:: :py:func:`letter`


.. py:function:: first_unused_letter(p: Presentation) -> Union[str, int]

   Returns the first letter **not** in the alphabet of a presentation.

   This function returns :py:func:``letter(p, i)`` when ``i`` is the least
   possible value such that ``p.in_alphabet(letter(p, i))`` returns ``False``
   if such a letter exists.

   :param p: the presentation
   :type p: Presentation

   :returns: A ``str`` or an ``int`` depending on ``p``.

   :raises RuntimeError:
     if ``p`` already has an alphabet of the maximum possible size supported.


.. py:function:: greedy_reduce_length(p: Presentation) -> None

   Greedily reduce the length of the presentation using
   :py:func:`longest_common_subword`.

   This function repeatedly calls :py:func:`longest_common_subword` and
   :py:func:`replace_subword` to introduce a new generator and reduce the
   length of the presentation ``p`` until :py:func:`longest_common_subword`
   returns the empty word.

   :param p: the presentation
   :type p: Presentation

   :returns: None

   :raises RuntimeError:
     if :py:func:`longest_common_subword` or :py:func:`replace_word` does.


.. py:function:: is_strongly_compressible(p: Presentation) -> bool

   Returns ``True`` if the :math:`1`-relation presentation can be strongly
   compressed.

   A :math:`1`-relation presentation is *strongly compressible* if both
   relation words start with the same letter and end with the same letter.
   In other words, if the alphabet of the presentation ``p`` is :math:`A` and
   the relation words are of the form :math:`aub = avb` where :math:`a, b\in A`
   (possibly :math:`a = b`) and :math:`u, v\in A^*`, then ``p`` is strongly
   compressible. See `Section 3.2`_ for details.

   .. _Section 3.2: https://doi.org/10.1007/s00233-021-10216-8

   :param p:  the presentation
   :type p: Presentation

   :returns: A value of type ``bool``.

   .. seealso:: :py:func:`strongly_compress`


.. py:function:: length(p: Presentation) -> int

   Return the sum of the lengths of the rules.

   :param p: the presentation
   :type p: Presentation

   :returns: An ``int``.


.. py:function:: letter(p: Presentation, i: int) -> int

    Return a possible letter by index.

    This function returns the ``i``-th letter in the alphabet consisting of
    all possible letters of type Presentation.letter_type. This function exists
    so that visible ASCII characters occur before invisible ones, so that when
    manipulating presentations over strings the human readable characters are
    used before non-readable ones.

    :param p: a presentation
    :type p: Presentation
    :param i: the index
    :type i: int

    :returns: A ``str``.

    :raises RuntimeError:
      if ``i`` exceeds the number of letters in supported by ``str``.

    .. seealso:: :py:func:`character`


.. py:function:: longest_common_subword(p: Presentation) -> Union[str, List[int]]

   Return the longest common subword of the rules.

   If it is possible to find a subword :math:`w` of the rules :math:`u_1 = v_1,
   \ldots, u_n = v_n` such that the introduction of a new generator :math:`z`
   and the relation :math:`z = w` reduces the length (see :py:func:`length`) of
   the presentation, then this function returns the word :math:`w`. If no such
   word can be found, a word of length :math:`0` is returned.

   :param p: the presentation
   :type p: Presentation

   :returns: str or List[int]


.. py:function:: longest_rule_length(p: Presentation) -> int

   Returns the maximum length of a rule in the presentation.

   The *length* of a rule is defined to be the sum of the lengths of its
   left and right hand sides.

   :param p:  the presentation
   :type p: Presentation

   :returns: An ``int``.

   :raises RuntimeError: if the length of ``p.rules`` is odd.


.. py:function:: longest_rule(p: Presentation) -> int

   Returns the index of the left hand side of the first rule in the
   presentation with maximal length.

   The *length* of a rule is defined to be the sum of the lengths of its left
   and right hand sides.

   :param p:  the presentation
   :type p: Presentation

   :returns: An ``int``.

   :raises RuntimeError: if the length of ``p.rules`` is odd.


.. py:function:: make_semigroup(p: Presentation) -> Union[int, str]

   Convert a monoid presentation to a semigroup presentation.

   This function modifies its argument in-place by replacing the empty word in
   all relations by a new generator, and the identity rules for that new
   generator. If ``p.contains_empty_word()`` is ``False``, then the
   presentation is not modified and :py:class:`UNDEFINED` is returned. If a new
   generator is added as the identity, then this generator is returned.

   :param p:  the presentation
   :type p: Presentation

   :returns:
     the new generator added, if any, and :py:class:`UNDEFINED` if not.

   :raises RuntimeError:
     if :py:func:`replace_word` or :py:func:`add_identity_rules` does.


.. py:function:: make(p: Presentation) -> Presentation

   Converts a presentation over strings to one over lists of integers or vice
   versa.

   :param p: the presentation
   :type p: Presentation

   :returns: A ``Presentation``.


.. py:function:: make(S: FroidurePin) -> Presentation
   :noindex:

   Returns a presentation defining a semigroup isomorphic to that represented
   by a ``FroidurePin`` instance.

   :param S: the ``FroidurePin`` instance.
   :type S: FroidurePin

   :returns: A ``Presentation``.


.. py:function:: normalize_alphabet(p: Presentation) -> None

   Modify the presentation so that the alphabet is :math:`\{0, \ldots, n - 1\}`
   (or equivalent), and rewrites the rules to use this alphabet.

   If the alphabet is already normalized, then no changes are made to the
   presentation.

   :param p: the presentation
   :type p: Presentation

   :returns: None


.. py:function:: reduce_complements(p: Presentation) -> None

   If there are rules :math:`u = v` and :math:`v = w` where :math:`\lvert w
   \rvert < \lvert v \rvert`, then replace :math:`u = v` with :math:`u = w`.

   Attempts to reduce the length of the words by finding the equivalence
   relation on the relation words generated by the pairs of identical relation
   words. If :math:`\{u_1, u_2, \ldots, u_n\}` are distinct words in an
   equivalence class and :math:`u_1` is the shortlex minimum word in the class,
   then the relation words are replaced by :math:`u_1 = u_2, u_1 = u_3, \ldots,
   u_1 = u_n`.

   :param p: the presentation
   :type p: Presentation

   :returns: None

   .. doctest::

      >>> from libsemigroups_pybind11 import presentation, Presentation
      >>> p = Presentation("a")
      >>> presentation.add_rule(p, "aaaaa", "aaa")
      >>> presentation.add_rule(p, "aaa", "a")
      >>> p.rules
      ['aaaaa', 'aaa', 'aaa', 'a']
      >>> presentation.reduce_complements(p)
      >>> p.rules
      ['a', 'aaa', 'a', 'aaaaa']


.. py:function:: reduce_to_2_generators(p: Presentation) -> None

     Reduce the number of generators in a :math:`1`-relation presentation to
     ``2``.

     Returns ``True`` if the :math:`1`-relation presentation ``p`` has been
     modified and ``False`` if not.

     A :math:`1`-relation presentation is *left cycle-free* if the relation
     words start with distinct letters. In other words, if the alphabet of the
     presentation ``p`` is :math:`A` and the relation words are of the form
     :math:`au = bv` where :math:`a, b\in A` with :math:`a \neq b` and
     :math:`u, v \in A^*`, then ``p`` is left cycle-free. The word problem for
     a left cycle-free :math:`1`-relation monoid is solvable if the word
     problem for the modified version obtained from this function is solvable.

     :param p:  the presentation
     :type p: Presentation
     :param x:
       determines the choice of letter to use, ``0`` uses
       ``p.rules[0].front()`` and ``1`` uses ``p.rules[1].front()`` (defaults to:
       ``0``).
     :type x: int

     :returns: A value of type ``bool``.

     :raises RuntimeError: if ``index`` is not ``0`` or ``1``.


.. py:function:: remove_duplicate_rules(p: Presentation) -> None

   Remove duplicate rules.

   Removes all but one instance of any duplicate rules (if any). Note that
   rules of the form :math:`u = v` and :math:`v = u` (if any) are considered
   duplicates. Also note that the rules may be reordered by this function even
   if there are no duplicate rules.

   :param p: the presentation
   :type p: Presentation

   :returns: None

   .. doctest::

      >>> from libsemigroups_pybind11 import presentation, Presentation
      >>> p = Presentation("ab")
      >>> presentation.add_rule(p, "ab", "baa")
      >>> presentation.add_rule(p, "baa", "ab")
      >>> p.rules
      ['ab', 'baa', 'baa', 'ab']
      >>> presentation.remove_duplicate_rules(p)
      >>> p.rules
      ['ab', 'baa']


.. py:function:: remove_redundant_generators(p: Presentation) -> None

   Remove any trivially redundant generators.

   If one side of any of the rules in the presentation ``p`` is a letter
   ``a`` and the other side of the rule does not contain ``a``, then this
   function replaces every occurrence of ``a`` in every rule by the other
   side of the rule. This substitution is performed for every such
   rule in the presentation; and the trivial rules (with both sides being
   identical) are removed. If both sides of a rule are letters, then the
   greater letter is replaced by the lesser one.

   :param p: the presentation
   :type p: Presentation

   :returns: None

   :raises RuntimeError: if ``len(p.rules)`` is odd.


.. py:function:: remove_trivial_rules(p: Presentation) -> None

   Remove rules consisting of identical words.

   Removes all instance of rules (if any) where the left hand side and the
   right hand side are identical.

   :param p: the presentation
   :type p: Presentation

   :returns: None

   :raises RuntimeError: if ``len(p.rules)`` is odd.


.. py:function:: replace_subword(p: Presentation, existing: Union[str, List[int]], replacement: Union[str, List[int]]) -> None
   :noindex:

   Replace non-overlapping instances of a subword by another word.

   If ``existing`` and ``replacement`` are words, then this function replaces
   every non-overlapping instance of ``existing`` in every rule by
   ``replacement``. The presentation ``p`` is changed in-place.

   :param p: the presentation
   :type p: Presentation
   :param existing: the word to be replaced
   :type existing: str or List[int]
   :param replacement: the replacement word.
   :type replacement: str or List[int]

   :returns: None

   :raises RuntimeError: if ``existing`` is empty.

    .. doctest::

      >>> from libsemigroups_pybind11 import presentation, Presentation
      >>> p = Presentation([0, 1])
      >>> presentation.add_rule(p, [1, 0, 0, 1, 0], [0, 1, 0, 0, 1])
      >>> p.rules
      [[1, 0, 0, 1, 0], [0, 1, 0, 0, 1]]
      >>> presentation.replace_subword(p, [0, 0, 1], [2])
      >>> p.rules
      [[1, 2, 0], [0, 1, 2]]


.. py:function:: replace_word_with_new_generator(p: Presentation, w: Union[str, List[int]]) -> None
    Replace non-overlapping instances of a word with a new generator.

    If :math:`w` is a word, then this function replaces
    every non-overlapping instance (from left to right) of :math:`w` in
    every rule, adds a new generator :math:`z`, and the rule :math:`w = z`.
    The new generator and rule are added even if :math:`w` is not a subword
    of any rule.

    :param p: the presentation
    :type p: Presentation
    :param w: the subword to replace
    :type w: str or List[int]

    :return: None

    :raises LibsemigroupsError: if *w* is empty.

    .. doctest::

      >>> from libsemigroups_pybind11 import presentation, Presentation
      >>> p = Presentation([0, 1])
      >>> presentation.add_rule(p, [1, 0, 0, 1, 0], [0, 1, 0, 0, 1])
      >>> p.rules
      [[1, 0, 0, 1, 0], [0, 1, 0, 0, 1]]
      >>> presentation.replace_word_with_new_generator(p, [0, 0, 1])
      >>> p.rules
      [[1, 2, 0], [0, 1, 2], [2], [0, 0, 1]]


.. py:function:: replace_word(p: Presentation, existing: Union[str, List[int]], replacement: Union[str, List[int]]) -> None

   Replace instances of a word occupying either side of a rule.

   If ``existing`` and ``replacement`` are words, then this function replaces
   every instance of ``existing`` in every rule of the form
   ``existing`` :math:`= w` or :math:`w =` ``existing``, with the word
   ``replacement``. The presentation ``p`` is changed in-place.

   :param p: the presentation
   :type p: Presentation
   :param existing: the word to be replaced
   :type existing: str or List[int]
   :param replacement: the replacement word
   :type replacement: str or List[int]

   :returns: None


.. py:function:: reverse(p: Presentation) -> None

   Reverse every rule.

   :param p: the presentation
   :type p: Presentation

   :returns: None


.. py:function:: shortest_rule_length(p: Presentation) -> int

   Returns the minimum length of a rule in the presentation.

   The *length* of a rule is defined to be the sum of the lengths of its
   left and right hand sides.

   :param p:  the presentation
   :type p: Presentation

   :returns: An ``int``.

   :raises RuntimeError: if the length of ``p.rules`` is odd.


.. py:function:: shortest_rule(p: Presentation) -> int

   Returns the index of the left hand side of the first rule in the
   presentation with minimal length.

   The *length* of a rule is defined to be the sum of the lengths of its left
   and right hand sides.

   :param p:  the presentation
   :type p: Presentation

   :returns: An ``int``.

   :raises RuntimeError: if the length of ``p.rules`` is odd.


.. py:function:: sort_each_rule(p: Presentation) -> None

   Sort each rule :math:`u = v` so that the left hand side is shortlex greater
   than the right hand side.

   :param p: the presentation
   :type p: Presentation

   :returns: None


.. py:function:: sort_rules(p: Presentation) -> None

   Sort the rules :math:`u_1 = v_1, \ldots, u_n = v_n` so that
   :math:`u_1 < \cdots < u_n`, where :math:`<` is the shortlex order.

   :param p: the presentation
   :type p: Presentation

   :returns: None


.. py:function:: strongly_compress(p: Presentation) -> None

   Strongly compress a :math:`1`-relation presentation.

   Returns ``True`` if the :math:`1`-relation presentation ``p`` has been
   modified and ``False`` if not. The word problem is solvable for the input
   presentation if it is solvable for the modified version.

   :param p:  the presentation
   :type p: Presentation

   :returns: A value of type ``bool``.

   .. seealso:: :py:func:`is_strongly_compressible`
