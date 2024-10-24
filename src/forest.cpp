//
// libsemigroups_pybind11
// Copyright (C) 2021-2024 James D. Mitchell
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

// C std headers....
#include <cstddef>  // for size_t

// C++ stl headers....
#include <initializer_list>  // for initializer_list

// libsemigroups....
#include <libsemigroups/forest.hpp>  // for Forest

// pybind11....
#include <pybind11/operators.h>  // for py::operator
#include <pybind11/pybind11.h>   // for class_, init, make_iterator, module
#include <pybind11/stl.h>        // for conversion of C++ to py types

// libsemigroups_pybind11....
#include "main.hpp"  // for init_forest

namespace py = pybind11;

namespace libsemigroups {
  void init_forest(py::module& m) {
    m.def(
        "to_forest",
        [](std::vector<size_t> const& parents,
           std::vector<size_t> const& labels) {
          return to_forest(parents, labels);
        },
        py::arg("parents"),
        py::arg("labels"),
        R"pbdoc(
:sig=(parents:List[int], labels:List[int])->Forest:
Construct a :any:`Forest` from list of parents and labels.

:param parent: the list of parents of nodes
:type parent: List[int]
:param labels: the list of edge labels
:type labels: List[int]

:returns:
  A newly constructed Forest with parents *parents* and edge labels *labels*.
:rtype:
  Forest

:raises LibsemigroupsError: if  *parent* and *labels* have different sizes;
:raises LibsemigroupsError:
  *parent* and *labels* do not have the value :any:`UNDEFINED` in the same
  positions (these values indicate where the roots of the trees in the forest
  are located and so must coincide).
:raises LibsemigroupsError:
  :any:`set_parent_and_label` throws for ``parent[i]`` and ``edge_labels[i]`` for any value of ``i``.
)pbdoc");

    using node_type = Forest::node_type;

    py::class_<Forest> thing(m,
                             "Forest",
                             R"pbdoc(
This class represents the collection of spanning trees of the strongly
connected components of a word graph.
)pbdoc");

    thing.def("__repr__",
              [](Forest const& f) { return to_human_readable_repr(f); });
    thing.def("__copy__", [](Forest const& f) { return Forest(f); });

    thing.def(py::init<size_t>(), R"pbdoc(
Constructs a forest with *n* nodes.

Constructs a forest with *n* nodes, that is initialised so that the
:any:`parent()` and :any:`label()` of every node is :any:`UNDEFINED`.

:param n: the number of nodes, defaults to ``0``.
:type n: int
)pbdoc");
    thing.def("add_nodes",
              &Forest::add_nodes,
              py::arg("n"),
              R"pbdoc(
Add nodes to the :any:`Forest`.

This function adds *n* nodes to the forest, but no edges.

:param n: the number of nodes to add.
:type n: int

:complexity: At most linear in ``number_of_nodes() + n``.
)pbdoc");
    thing.def("empty",
              &Forest::empty,
              R"pbdoc(
Check if there are any nodes in the forest. This function returns
``True`` if there are ``0`` nodes in the forest, and ``False`` otherwise.

:returns:
   Whether or not the forest is empty.
:rtype:
   bool

:complexity:
   Constant.
)pbdoc");
    thing.def("init",
              &Forest::init,
              py::arg("n"),
              R"pbdoc(
Reinitialize an existing :any:`Forest` object.

This function reinitializes an existing :any:`Forest` object so that it is in
the same state as if it had just be constructed as ``Forest(n)``.

:param n: the number of nodes, defaults to ``0``.
:type n: int

:returns: ``self``.
:rtype: Forest
)pbdoc");
    thing.def("label",
              &Forest::label,
              py::arg("i"),
              R"pbdoc(
Returns the label of the edge from a node to its parent.

:param i:
   the node whose label is sought.
:type i:
  init

:returns:
   The label of the edge from the parent of *i* to *i*.
:rtype:
   int

:raises LibsemigroupsError:
   if *i* exceeds ``number_of_nodes()``.

:complexity:
   Constant.
)pbdoc");
    thing.def("labels",
              &Forest::labels,
              R"pbdoc(
Returns the list of edge labels in the :any:`Forest`. The value
in position ``i`` of this list is the label of the edge from the
parent of node ``i`` to ``i``. If the parent equals :any:`UNDEFINED`,
then node ``i`` is a root node.

:returns:
   The edge labels of the forest.
:rtype:
   List[int]

:complexity:
   Constant.
)pbdoc");
    thing.def("number_of_nodes",
              &Forest::number_of_nodes,
              R"pbdoc(
Returns the number of nodes in the forest. Returns the number of nodes
in the forest.

:returns:
   The number of nodes in the forest.
:rtype:
   int

:complexity:
   Constant.
)pbdoc");
    thing.def(py::self != py::self, py::arg("that"));
    thing.def(py::self == py::self, py::arg("that"));
    thing.def("parent",
              &Forest::parent,
              py::arg("i"),
              R"pbdoc(
Returns the parent of a node.

:param i:
   the node whose parent is sought.
:type i:
   int

:returns:
   The parent of *i*.
:rtype:
   int

:raises LibsemigroupsError:
   if *i* exceeds ``number_of_nodes()``.

:complexity:
   Constant
)pbdoc");
    thing.def("parents",
              &Forest::parents,
              R"pbdoc(
Returns a list of parents in the :any:`Forest` . The value in position ``i`` of
this list is the parent of node ``i`` . If the parent equals :any:`UNDEFINED`,
then node ``i`` is a root node.

:returns:
   The parents of the nodes in the forest.

:rtype:
   List[int].

:complexity:
   Constant.
)pbdoc");
    thing.def(
        "path_to_root",
        [](Forest const& self, node_type i) { return self.path_to_root(i); },
        py::arg("i"),
        R"pbdoc(
Returns a list containing the labels of the edges on the path from a root node
to *i*.

:param i: the node.
:type i: int

:returns: The word labelling the path from the root to *i*.
:rtype: List[int]

:raises LibsemigroupsError:
  if *i* is greater than or equal to :any:`number_of_nodes`.
)pbdoc");
    thing.def("set_parent_and_label",
              &Forest::set_parent_and_label,
              py::arg("node"),
              py::arg("parent"),
              py::arg("gen"),
              R"pbdoc(
Set the parent and edge label for a node. This function sets the parent of
*node* to be *parent*, and the associated edge-label to be *gen*.

:param node: the node whose parent and label to set.
:type node: int

:param parent: the parent node
:type parent: int

:param gen: the label of the edge from parent to node.
:type gen: int

:returns: ``self``
:rtype: Forest

:raises LibsemigroupsError:
  if *node* or *parent* exceeds :any:`number_of_nodes()`.

:complexity: Constant.
)pbdoc");
  }
}  // namespace libsemigroups
