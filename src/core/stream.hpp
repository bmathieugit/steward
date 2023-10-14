#ifndef __steward_core_stream_hpp__
#define __steward_core_stream_hpp__

#include <core/utils.hpp>

template <typename I>
concept istream = requires { typename I::type; } and requires(I i) {
  { i.get() } -> same_as_declined<typename I::type>;
  { i.has() } -> same_as<bool>;
  { i.next() } -> same_as<void>;
};

template <typename O>
concept ostream =
    requires { typename O::type; } and requires(O o, typename O::type c) {
      { o.add(c) } -> same_as<bool>;
    };

#endif