//
// libsemigroups_pybind11
// Copyright (C) 2024 Joseph Edwards
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

// C++ stl headers....
#include <string>  // for string
#include <vector>  // for vector

// libsemigroups....
#include <libsemigroups/aho-corasick.hpp>  // for AhoCorasick, AhoCorasick::...
#include <libsemigroups/dot.hpp>           // for Dot
#include <libsemigroups/types.hpp>         // for word_type

// pybind11....
#include <pybind11/cast.h>           // for arg
#include <pybind11/detail/common.h>  // for const_, overload_cast, ove...
#include <pybind11/detail/descr.h>   // for operator+
#include <pybind11/pybind11.h>       // for class_, init, module
#include <pybind11/pytypes.h>        // for sequence, str_attr_accessor
#include <pybind11/stl.h>            // for std::vector conversion

// libsemigroups_pybind11....
#include "main.hpp"  // for init_aho_corasick

namespace py = pybind11;

namespace libsemigroups {

  void init_aho_corasick(py::module& m) {
    using index_type = AhoCorasick::index_type;
    py::class_<AhoCorasick> thing(m,
                                  "AhoCorasick",
                                  R"pbdoc(
For an implementation of the Aho-Corasick algorithm.

This class implements a trie based data structure with suffix links to be used
with the Aho-Corasick dictionary searching algorithm. An introduction to this
algorithm can be found `here <https://en.wikipedia.org/wiki/Aho%E2%80%93Corasick_algorithm>`_.

The implementation of :any:`AhoCorasick` uses two different types of node;
*active* and *inactive* . An active node is a node that is currently a node
in the trie. An inactive node is a node that used to be part of the trie, but
has since been removed. It may later become active again after being
reinitialised, and exists as a way of minimising how frequently memory needs
to be allocated and deallocated for nodes. This function validates whether the
given index *i* corresponds to an active node.

Several helper functions are provided in the ``aho_corasick`` module, documented
:doc:`here <ac-helpers>`.
)pbdoc");

    thing.def("__repr__", &to_string);

    thing.attr("root") = &AhoCorasick::root;

    thing.def(py::init<>(), R"pbdoc(
Construct an empty AhoCorasick.

Construct an :any:`AhoCorasick` containing only the root that corresponds to
the empty word :math:`\varepsilon`.)pbdoc");

    thing.def(
        "__copy__",
        [](const AhoCorasick& that) { return AhoCorasick(that); },
        R"pbdoc(
Default copy constructor.

Default copy constructor)pbdoc");

    thing.def("child",
              &AhoCorasick::child,
              py::arg("parent"),
              py::arg("letter"),
              R"pbdoc(
Return the child of *parent* with edge-label *letter*

This function returns the index of the child of the node with index
*parent* along the edge labelled by *letter*. If no such child exists,
:any:`UNDEFINED` is returned.

:param parent: the index of the node whose child is sought.
:type parent: int

:param letter: the edge-label connecting the parent to the desired child.
:type letter: int

:returns: the index of the child.
:rtype: int

:raises LibsemigroupsError:  if ``validate_active_node_index(parent)`` throws.

:complexity: Constant.

.. seealso:: :any:`validate_active_node_index`.

)pbdoc");

    thing.def("height",
              &AhoCorasick::height,
              py::arg("i"),
              R"pbdoc(
Calculate the height of a node.

:param i: the index of the node whose height is sought
:type i: int

:returns: the height.
:rtype: int

:raises LibsemigroupsError:  if ``validate_active_node_index(i)`` throws.

:complexity: Linear in the return value which is, at worst, the maximum length of a word in the trie

.. seealso::  :any:`validate_active_node_index`.

)pbdoc");

    thing.def("init",
              &AhoCorasick::init,
              R"pbdoc(
Reinitialise an existing AhoCorasick object.

This function puts an :any:`AhoCorasick` object back into the same state as
if it had been newly default constructed.

:complexity: Linear in the number of nodes in the trie

:returns: ``self``.
:rtype: AhoCorasick
)pbdoc");

    thing.def("number_of_nodes",
              &AhoCorasick::number_of_nodes,
              R"pbdoc(
Returns the number of nodes in the trie.

This function Returns the number of nodes in the trie.


:complexity: Constant

:returns: The number of nodes>
:rtype: int
)pbdoc");

    thing.def(
        "signature",
        py::overload_cast<index_type>(&AhoCorasick::signature, py::const_),
        py::arg("i"),
        R"pbdoc(
Find the signature of a node

Return the the signature of the node with index *i*. Recall that the
*signature* of a node  :math:`n` is the word consisting of the edge labels
of the unique path from the root to
:math:`n`.

:param i: the index of the node whose signature is sought
:type i: int

:returns: The signature
:rtype: List[int]

:complexity: Linear in the height of the node
)pbdoc");

    thing.def("suffix_link",
              &AhoCorasick::suffix_link,
              py::arg("current"),
              R"pbdoc(
Calculate the index of the suffix link of a node.

Calculate the index of a suffix link of a node. Recall that the *suffix link* of
a node with signature :math:`W` is the node with the signature equal to that of
the longest proper suffix of :math:`W` contained in the trie.

:param current: the index of the node whose suffix link is sought
:type current: int

:returns: The index of the suffix link.
:rtype: int

:raises LibsemigroupsError:  if ``validate_active_node_index(current)`` throws.

:complexity: Linear in the height of the node.

.. seealso:: :any:`validate_active_node_index`.

)pbdoc");

    thing.def("traverse",
              &AhoCorasick::traverse,
              py::arg("current"),
              py::arg("a"),
              R"pbdoc(
Traverse the trie using suffix links where necessary.

This function traverses the trie using suffix links where necessary, behaving
like a combination of the *goto* function and the *fail* function in :cite:`Aho1975aa`.

If *current* is the index of a node with signature :math:`W`, and *a* is the
letter :math:`a`, then `traverse_no_checks(current, a)` returns the index of the
node with signature equal to the longest suffix of :math:`Wa` contained in the
trie.

:param current: the index of the node to traverse from
:type current: int

:param a: the letter to traverse
:type a: int

:returns: The index of the node traversed to
:rtype: int

:raises LibsemigroupsError:  if ``validate_active_node_index(current)`` throws.

.. seealso:: :any:`validate_active_node_index`.

)pbdoc");

    thing.def("validate_active_node_index",
              &AhoCorasick::validate_active_node_index,
              py::arg("i"),
              R"pbdoc(
Check if an index corresponds to a node currently in the trie.

:param i: the index to validate
:type i: int

:raises LibsemigroupsError:  if ``validate_node_index(i)`` throws, or if *i* is
    not an active node.

:complexity: Constant

.. seealso::  :any:`validate_node_index`.

)pbdoc");

    thing.def("validate_node_index",
              &AhoCorasick::validate_node_index,
              py::arg("i"),
              R"pbdoc(
Check if an index corresponds to a node.

This function checks if the given index *i* corresponds to the index of a
node; either active or inactive.

:param i: the index to validate
:type i: index_type

:raises LibsemigroupsError:  if *i* does not correspond to the index of a
    node; that is, if *i* is larger than the size of the container storing the
    indices of nodes.

:complexity: Constant
)pbdoc");

    // Helpers
    m.def("add_word",
          &aho_corasick::add_word<word_type>,
          py::arg("ac"),
          py::arg("w"),
          R"pbdoc(
Add a word to the trie of *ac*

Calling this function immediately adds the word *w* to the trie of *ac*, and
makes the final node on the path labelled by this word terminal (if it
wasn't already). After adding a word, existing suffix links become
invalid. If an identical word has already been added to the trie of *ac*, then
this function does nothing.

:param ac: object whose trie is to be added to
:type ac: AhoCorasick

:param w: the word to add
:type w: List[int]

:returns: The index corresponding to the final node added to the trie of *ac*.
    This node will have a :any:`signature` equal to that of *w*.
:rtype: int

:complexity: Linear in the length of *w*.

.. seealso:: :any:`AhoCorasick.signature`

)pbdoc");
    m.def("add_word",
          &aho_corasick::add_word<std::string>,
          py::arg("ac"),
          py::arg("w"),
          R"pbdoc(
Add a word to the trie of *ac*

This function performs the same as ``add_word(AhoCorasick ac, List[int] w)``,
but *w* is a :any:`string` rather than List[:any:`int`].

)pbdoc");
    m.def("rm_word",
          &aho_corasick::rm_word<word_type>,
          py::arg("ac"),
          py::arg("w"),
          R"pbdoc(
Remove a word from the trie of *ac*.

From the trie of *ac*, remove each node of the given word *w* that is not part of
the prefix of a different word.

If the word *w* corresponds to a terminal node with no children, then
calling this function removes the nodes :math:`n_i` from the trie of *ac*
that correspond to the largest suffix *w*, such that each :math:`n_i` has either
zero children or one. After this, existing suffix links become invalid.

If *w* corresponds to a terminal node :math:`n` with children, then calling this
function makes :math`n` not terminal.

If *w* does not correspond to a terminal node, then calling this function does
nothing.

:param ac: object whose trie is to be removed from
:type ac: AhoCorasick

:param w: the word to remove
:type w: List[int]

:returns: The index corresponding to the node with signature equal to *w*.
:rtype: int

:complexity: Linear in the length of *w*.

.. seealso:: :any:`AhoCorasick.signature`

)pbdoc");
    m.def("rm_word",
          &aho_corasick::rm_word<std::string>,
          py::arg("ac"),
          py::arg("w"),
          R"pbdoc(
Remove a word from the trie of *ac*.

This function performs the same as ``rm_word(AhoCorasick ac, List[int] w)``,
but *w* is a :any:`string` rather than List[:any:`int`].

)pbdoc");
    m.def(
        "traverse_word",
        [](AhoCorasick const& ac, index_type start, word_type const& w) {
          return aho_corasick::traverse_word(ac, start, w);
        },
        py::arg("ac"),
        py::arg("start"),
        py::arg("w"),
        R"pbdoc(
Traverse the trie of *ac* using suffix links where necessary.

This function traverses the trie of *ac*, starting at the node with
index *start*, and traversing using the letters in the word *w*.

:param ac: object to traverse.
:type ac: AhoCorasick

:param w: Word to traverse by
:type w: List[int]

:returns: The result of the traversal
:rtype: int

)pbdoc");
    m.def(
        "traverse_word",
        [](AhoCorasick const& ac, index_type start, std::string const& w) {
          return aho_corasick::traverse_word(ac, start, w);
        },
        py::arg("ac"),
        py::arg("start"),
        py::arg("w"),
        R"pbdoc(
Traverse the trie of *ac* using suffix links where necessary.

This function performs the same as ``traverse_word(AhoCorasick ac, List[int] w)``,
but *w* is a :any:`string` rather than List[:any:`int`].
)pbdoc");
    m.def(
        "traverse_word",
        [](AhoCorasick const& ac, word_type const& w) {
          return aho_corasick::traverse_word(ac, w);
        },
        py::arg("ac"),
        py::arg("w"),
        R"pbdoc(
Traverse the trie of *ac* from the root using suffix links where necessary.

This function performs the same as ``traverse_word(ac, AhoCorasick.root, w)``
)pbdoc");
    m.def(
        "traverse_word",
        [](AhoCorasick const& ac, std::string const& w) {
          return aho_corasick::traverse_word(ac, w);
        },
        py::arg("ac"),
        py::arg("w"),
        R"pbdoc(
Traverse the trie of *ac* from the root using suffix links where necessary.

This function performs the same as ``traverse_word(ac, AhoCorasick.root, w)``
)pbdoc");

    m.def(
        "aho_corasick_dot",
        [](AhoCorasick& ac) { return aho_corasick::dot(ac); },
        py::arg("ac"),
        R"pbdoc(
:sig=(ac: AhoCorasick) -> Dot:

Construct a :any:`Dot` object representing the trie of *ac*.

:param ac: the :any:`AhoCorasick` object whose trie we are trying to visualise.
:type ac: AhoCorasick

:returns: A :any:`Dot` object representing *ac*.
:rtype: Dot
)pbdoc");
  }  // init_aho_corasick

}  // namespace libsemigroups
