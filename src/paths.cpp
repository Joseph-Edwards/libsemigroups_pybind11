//
// libsemigroups_pybind11
// Copyright (C) 2023-2024 Murray T. Whyte
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
#include <array>             // for array
#include <cstddef>           // for uint32_t
#include <cstdint>           // for uint64_t
#include <initializer_list>  // for initializer_list
#include <iosfwd>            // for string
#include <string>            // for to_string, basic_string
#include <vector>            // for vector

// libsemigroups....
#include <libsemigroups/constants.hpp>   // for operator!=, operator==
#include <libsemigroups/order.hpp>       // for order
#include <libsemigroups/paths.hpp>       // for Paths
#include <libsemigroups/word-graph.hpp>  // for WordGraph, word_graph

// pybind11....
#include <pybind11/operators.h>  // for self, self_t, operator!=, operator*
#include <pybind11/pybind11.h>   // for class_, make_iterator, init, enum_
#include <pybind11/stl.h>        // for conversion of C++ to py types

// libsemigroups_pybind11....
#include "main.hpp"  // for init_paths

namespace py = pybind11;

namespace libsemigroups {
  using node_type = uint32_t;
  using size_type = typename WordGraph<node_type>::size_type;

  void init_paths(py::module& m) {
    ////////////////////////////////////////////////////////////////////////
    // Paths
    ////////////////////////////////////////////////////////////////////////

    using Paths_ = Paths<node_type>;

    py::class_<Paths_> thing1(m,
                              "Paths",
                              R"pbdoc(
This class represents a range object that facilitates iterating through the
paths in a :any:`WordGraph` from a given :any:`source` (to a possible
:any:`target` node) in a particular :any:`order`.

.. note::
  Changing the value of :any:`source`, :any:`target`, :any:`min`, :any:`max`,
  or :any:`order` resets the :any:`Paths` object to point at the first word in
  the specified range.
)pbdoc");
    thing1.def("__repr__",
               [](Paths_ const& p) { return to_human_readable_repr(p); });
    thing1.def("__copy__", [](Paths_ const& p) { return Paths_(p); });
    thing1.def("__or__", [](Paths_ const& p, ToString const& to_str) {
      using rx::operator|;
      return p | to_str;
    });
    // __len__ is not allowed to return anything other than an int, hence
    // __len__ and count don't have the same behaviour.
    thing1.def("__len__", [](Paths_ const& p) {
      p.throw_if_source_undefined();
      auto result = p.count();
      if (result == POSITIVE_INFINITY) {
        return py::module_::import("sys").attr("maxsize").cast<uint64_t>();
      }
      return result;
    });
    thing1.def("__iter__", [](Paths_ const& p) {
      return py::make_iterator(rx::begin(p), rx::end(p));
    });
    thing1.def(
        "init",
        [](Paths_& self, WordGraph<node_type> const& wg) -> Paths_& {
          return self.init(wg);
        },
        py::arg("wg"),
        R"pbdoc(
Reinitialize a :any:`Paths` object.

This function puts a :any:`Paths` object back into the same state as if it had
been newly constructs from the :any:`WordGraph` *wg*.

:param wg: the word graph.
:type wg: WordGraph

:returns: ``self``
:rtype: Paths
)pbdoc");
    thing1.def(py::init<WordGraph<node_type> const&>(),
               py::arg("wg"),
               R"pbdoc(
Construct from a :any:`WordGraph`.

This function constructs a :any:`Paths` object from the :any:`WordGraph`
*wg*.

:param wg: the word graph.
:type wg: WordGraph
)pbdoc");
    thing1.def(
        "at_end",
        [](Paths_& p) {
          p.throw_if_source_undefined();
          return p.at_end();
        },
        R"pbdoc(
Check if the range is exhausted. This function returns ``True`` if there
are no more paths in the range, and ``False`` otherwise.

:returns:
   Whether or not the range is exhausted.
:rtype:
   bool

:raises LibsemigroupsError: if ``source() == UNDEFINED``.
)pbdoc");

    // _count's doc is in paths.py
    thing1.def("_count", [](Paths_& p) {
      p.throw_if_source_undefined();
      return p.count();
    });
    thing1.def("current_target",
               &Paths_::current_target,
               R"pbdoc(
Get the current target node of the path labelled by :any:`Paths.get`. This
function returns the current target node of the path labelled by
:any:`Paths.get` . If there is no such path (because, for example, the source
node hasn't been defined, then :any:`UNDEFINED` is returned).

:returns:
   The current target node of the path labelled by :any:`Paths.get` or
   :any:`UNDEFINED`.
:rtype:
   int
)pbdoc");
    thing1.def(
        "get",
        [](Paths_& p) {
          p.throw_if_source_undefined();
          return p.get();
        },
        R"pbdoc(
Get the current path in the range.

:returns:
   The current path.
:rtype:
   List[int]

:raises LibsemigroupsError: if ``source() == UNDEFINED``.
)pbdoc");
    // Same issue as with _count
    thing1.def("_max", [](Paths_ const& self) { return self.max(); });
    thing1.def("_max",
               [](Paths_& self, PositiveInfinity const& val) -> Paths_& {
                 return self.max(val);
               });
    thing1.def("_max", [](Paths_& self, size_type val) -> Paths_& {
      return self.max(val);
    });
    thing1.def(
        "min",
        [](Paths_ const& self) { return self.min(); },
        R"pbdoc(
Get the minimum length of path in the range. This function returns the
current minimum length of paths in the range. The initial value is
``0``.

:returns:
   The minimum length of paths in the range.
:rtype:
   int
)pbdoc");
    thing1.def(
        "min",
        [](Paths_& self, size_type val) -> Paths_& { return self.min(val); },
        py::arg("val"),
        R"pbdoc(
Set the minimum length of path in the range.

This function can be used to set the minimum length of paths that will be
contained in the range. If this function is not called, then the range will
contain paths starting with length ``0``.

:param val: the minimum path length.
:type val: int

:returns: ``self``.
:rtype: Paths
)pbdoc");
    thing1.def(
        "next",
        [](Paths_& p) {
          p.throw_if_source_undefined();
          p.next();
        },
        R"pbdoc(
Advance to the next path in the range. Advance to the current path in
the range. If :any:`at_end` returns ``True``, then this function does
nothing.

:raises LibsemigroupsError: if ``source() == UNDEFINED``.
)pbdoc");
    thing1.def(
        "order",
        [](Paths_ const& self) { return self.order(); },
        R"pbdoc(
Get the order of the paths in the range. This function returns the
current order of the paths in the range defined by a :any:`Paths`
object. The initial value is :any:`Order.shortlex`.

:returns:
   The order of the paths in the range.

:rtype:
   Order
)pbdoc");
    thing1.def(
        "order",
        [](Paths_& self, Order val) -> Paths_& { return self.order(val); },
        py::arg("val"),
        R"pbdoc(
Set the order of the paths in the range.

This function can be used to set the order of the paths in the range defined by
a :any:`Paths` object. The initial value is :any:`Order.shortlex`.

:param val: the order of the paths in the range.
:type val: Order

:returns: ``self``.
:rtype: Paths

:raises LibsemigroupsError:
  if ``val`` is not :any:`Order.shortlex` or :any:`Order.lex`.
)pbdoc");
    thing1.def(
        "source",
        [](Paths_ const& self) { return self.source(); },
        R"pbdoc(
Get the current source node of every path in the range. This function
returns the current source node of the every path in the range defined
by a :any:`Paths` object. This initial value is :any:`UNDEFINED`.

:returns:
   The current source node.

:rtype:
   int
)pbdoc");
    thing1.def(
        "source",
        [](Paths_& self, node_type n) -> Paths_& { return self.source(n); },
        py::arg("n"),
        R"pbdoc(
Set the source node of every path in the range. This function can be used to
set the source node of all of the paths in the range.

:param n: the source node.
:type n: int

:returns: ``self``.
:rtype: Paths

:raises LibsemigroupsError:
  if ``n`` is not a node in the underlying :any:`WordGraph` (:any:`word_graph`).
)pbdoc");
    thing1.def(
        "target",
        [](Paths_ const& self) { return self.target(); },
        R"pbdoc(
Get the current target node of every path in the range. This function
returns the target node of the every path in the range defined by a
:any:`Paths` object. This initial value is :any:`UNDEFINED`.

:returns:
   The target node.

:rtype:
   int
)pbdoc");
    thing1.def(
        "target",
        [](Paths_& self, node_type n) -> Paths_& { return self.target(n); },
        py::arg("n"),
        R"pbdoc(
Set the target node of every path in the range.

This function can be used to set the target node (or the "to" node) of all of
the paths in the range. It is not necessary to set this value. If the target
node is set to :any:`UNDEFINED`, then the range will contain every path from
:any:`source` to every possible target in the underlying :any:`WordGraph`
(:any:`word_graph`).

:param n: the target node.
:type n: int

:returns: ``self``.
:rtype: Paths

:raises LibsemigroupsError:
  if *n* is not a node in the underlying :any:`WordGraph` (:any:`word_graph`)
  and *n* is not :any:`UNDEFINED`.
)pbdoc");
    thing1.def("word_graph",
               &Paths_::word_graph,
               R"pbdoc(
The underlying word graph. This function returns underlying
:any:`WordGraph` of the :any:`Paths` object. This is the
:any:`WordGraph` defining the paths in the range.

:returns:
   The underlying word graph.
:rtype:
   WordGraph
)pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // ReversiblePaths
    ////////////////////////////////////////////////////////////////////////

    using ReversiblePaths_ = ReversiblePaths<node_type>;

    py::class_<ReversiblePaths_> thing2(m,
                                        "ReversiblePaths",
                                        R"pbdoc(
This class represents a range object that facilitates iterating through the
paths in a :any:`WordGraph` from a given :any:`source` (to a possible
:any:`target` node) in a particular :any:`order`.

.. note::
  Changing the value of :any:`source`, :any:`target`, :any:`min`, :any:`max`,
  or :any:`order` resets the :any:`ReversiblePaths` object to point at the
  first word in the specified range.
)pbdoc");
    thing2.def("__repr__", [](ReversiblePaths_ const& p) {
      return to_human_readable_repr(p);
    });
    thing2.def("__copy__",
               [](ReversiblePaths_ const& p) { return ReversiblePaths_(p); });
    thing2.def("__or__", [](ReversiblePaths_ const& p, ToString const& to_str) {
      using rx::operator|;
      return p | to_str;
    });
    thing2.def("__len__", &ReversiblePaths_::count);
    thing2.def("__iter__", [](ReversiblePaths_ const& p) {
      return py::make_iterator(rx::begin(p), rx::end(p));
    });
    thing2.def(
        "init",
        [](ReversiblePaths_& self, WordGraph<node_type> const& wg)
            -> ReversiblePaths_& { return self.init(wg); },
        py::arg("wg"),
        R"pbdoc(
Reinitialize a :any:`ReversiblePaths` object.

This function puts a :any:`ReversiblePaths` object back into the same state as
if it had been newly constructs from the :any:`WordGraph` *wg*.

:param wg: the word graph.
:type wg: WordGraph

:returns: ``self``
:rtype: ReversiblePaths
)pbdoc");
    thing2.def(py::init<WordGraph<node_type> const&>(),
               py::arg("wg"),
               R"pbdoc(
Construct from a :any:`WordGraph`.

This function constructs a :any:`ReversiblePaths` object from the
:any:`WordGraph` *wg*.

:param wg: the word graph.
:type wg: WordGraph
)pbdoc");
    thing2.def(
        "at_end",
        [](ReversiblePaths_& p) {
          p.throw_if_source_undefined();
          return p.at_end();
        },
        R"pbdoc(
Check if the range is exhausted. This function returns ``True`` if there
are no more paths in the range, and ``False`` otherwise.

:returns:
   Whether or not the range is exhausted.
:rtype:
   bool

:raises LibsemigroupsError: if ``source() == UNDEFINED``.
)pbdoc");
    // _count's doc is in paths.py
    thing2.def("_count", [](ReversiblePaths_& p) {
      p.throw_if_source_undefined();
      return p.count();
    });
    thing2.def("current_target",
               &ReversiblePaths_::current_target,
               R"pbdoc(
Get the current target node of the path labelled by :any:`ReversiblePaths.get`.
This function returns the current target node of the path labelled by
:any:`ReversiblePaths.get` . If there is no such path (because, for example,
the source node hasn't been defined, then :any:`UNDEFINED` is returned).

:returns:
   The current target node of the path labelled by :any:`ReversiblePaths.get` or
   :any:`UNDEFINED`.
:rtype:
   int
)pbdoc");
    thing2.def(
        "get",
        [](ReversiblePaths_& p) {
          p.throw_if_source_undefined();
          return p.get();
        },
        R"pbdoc(
Get the current path in the range.

:returns:
   The current path.
:rtype:
   List[int]

:raises LibsemigroupsError: if ``source() == UNDEFINED``.
)pbdoc");
    thing2.def("_max", [](ReversiblePaths_ const& self) { return self.max(); });
    thing2.def("_max",
               [](ReversiblePaths_& self, size_type val) -> ReversiblePaths_& {
                 return self.max(val);
               });
    thing2.def("_max",
               [](ReversiblePaths_& self, PositiveInfinity const& val)
                   -> ReversiblePaths_& { return self.max(val); });
    thing2.def(
        "min",
        [](ReversiblePaths_ const& self) { return self.min(); },
        R"pbdoc(
Get the minimum length of path in the range. This function returns the
current minimum length of paths in the range. The initial value is
``0``.

:returns:
   The minimum length of paths in the range.
:rtype:
   int
)pbdoc");
    thing2.def(
        "min",
        [](ReversiblePaths_& self, size_type val) -> ReversiblePaths_& {
          return self.min(val);
        },
        py::arg("val"),
        R"pbdoc(
Set the minimum length of path in the range.

This function can be used to set the minimum length of paths that will be
contained in the range. If this function is not called, then the range will
contain paths starting with length ``0``.

:param val: the minimum path length.
:type val: int

:returns: ``self``.
:rtype: ReversiblePaths
)pbdoc");
    thing2.def(
        "next",
        [](ReversiblePaths_& p) {
          p.throw_if_source_undefined();
          p.next();
        },
        R"pbdoc(
Advance to the next path in the range. Advance to the current path in
the range. If :any:`at_end` returns ``True``, then this function does
nothing2.

:raises LibsemigroupsError: if ``source() == UNDEFINED``.
)pbdoc");
    thing2.def(
        "order",
        [](ReversiblePaths_ const& self) { return self.order(); },
        R"pbdoc(
Get the order of the paths in the range. This function returns the
current order of the paths in the range defined by a :any:`ReversiblePaths`
object. The initial value is :any:`Order.shortlex`.

:returns:
   The order of the paths in the range.

:rtype:
   Order
)pbdoc");
    thing2.def(
        "order",
        [](ReversiblePaths_& self, Order val) -> ReversiblePaths_& {
          return self.order(val);
        },
        py::arg("val"),
        R"pbdoc(
Set the order of the paths in the range.

This function can be used to set the order of the paths in the range defined by
a :any:`ReversiblePaths` object. The initial value is :any:`Order.shortlex`.

:param val: the order of the paths in the range.
:type val: Order

:returns: ``self``.
:rtype: ReversiblePaths

:raises LibsemigroupsError:
  if ``val`` is not :any:`Order.shortlex` or :any:`Order.lex`.
)pbdoc");
    thing2.def(
        "source",
        [](ReversiblePaths_ const& self) { return self.source(); },
        R"pbdoc(
Get the current source node of every path in the range. This function
returns the current source node of the every path in the range defined
by a :any:`ReversiblePaths` object. This initial value is :any:`UNDEFINED`.

:returns:
   The current source node.

:rtype:
   int
)pbdoc");
    thing2.def(
        "source",
        [](ReversiblePaths_& self, node_type n) -> ReversiblePaths_& {
          return self.source(n);
        },
        py::arg("n"),
        R"pbdoc(
Set the source node of every path in the range. This function can be used to
set the source node of all of the paths in the range.

:param n: the source node.
:type n: int

:returns: ``self``.
:rtype: ReversiblePaths

:raises LibsemigroupsError:
  if ``n`` is not a node in the underlying :any:`WordGraph` (:any:`word_graph`).
)pbdoc");
    thing2.def(
        "target",
        [](ReversiblePaths_ const& self) { return self.target(); },
        R"pbdoc(
Get the current target node of every path in the range. This function
returns the target node of the every path in the range defined by a
:any:`ReversiblePaths` object. This initial value is :any:`UNDEFINED`.

:returns:
   The target node.

:rtype:
   int
)pbdoc");
    thing2.def(
        "target",
        [](ReversiblePaths_& self, node_type n) -> ReversiblePaths_& {
          return self.target(n);
        },
        py::arg("n"),
        R"pbdoc(
Set the target node of every path in the range.

This function can be used to set the target node (or the "to" node) of all of
the paths in the range. It is not necessary to set this value. If the target
node is set to :any:`UNDEFINED`, then the range will contain every path from
:any:`source` to every possible target in the underlying :any:`WordGraph`
(:any:`word_graph`).

:param n: the target node.
:type n: int

:returns: ``self``.
:rtype: ReversiblePaths

:raises LibsemigroupsError:
  if *n* is not a node in the underlying :any:`WordGraph` (:any:`word_graph`)
  and *n* is not :any:`UNDEFINED`.
)pbdoc");
    thing2.def("word_graph",
               &ReversiblePaths_::word_graph,
               R"pbdoc(
The underlying word graph. This function returns underlying
:any:`WordGraph` of the :any:`ReversiblePaths` object. This is the
:any:`WordGraph` defining the paths in the range.

:returns:
   The underlying word graph.
:rtype:
   WordGraph
)pbdoc");

    thing2.def(
        "reverse",
        [](ReversiblePaths_ const& self) -> bool { return self.reverse(); },
        R"pbdoc(
Get whether or not paths in the range should be reversed. This function returns
``True`` if the paths returned by :any:`ReversiblePaths.get` are reversed and
``False`` if they are not.

:returns:
   Whether or not the paths returned by :any:`ReversiblePaths.get` are reversed.
:rtype:
   bool
)pbdoc");
    thing2.def(
        "reverse",
        [](ReversiblePaths_& self, bool val) -> ReversiblePaths_& {
          return self.reverse(val);
        },
        py::arg("val"),
        R"pbdoc(
Set whether or not to reverse the paths in the range.

This function can be used to set whether or not the paths returned by
:any:`ReversiblePaths.get` should be reversed. Paths are not reversed by
default.

:param val: paths should be reversed or not.
:type val: bool

:returns: A reference to ``self``.
:rtype: ReversiblePaths
)pbdoc");
  }
}  // namespace libsemigroups
