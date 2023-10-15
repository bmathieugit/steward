#ifndef __steward_core_stream_hpp__
#define __steward_core_stream_hpp__

#include <core/collection.hpp>
#include <core/utils.hpp>

template <typename I>
concept input_stream = requires { typename I::type; } and requires(I i) {
  { i.next() } -> same_as_declined<typename I::type>;
  { i.has() } -> same_as<bool>;
};

template <iterator I>
class iterator_input_stream {
 public:
  using type = typename I::type;

 private:
  I _i;

 public:
  constexpr iterator_input_stream(I i) : _i(i) {}

 public:
  constexpr auto has() const { return _i.has(); }
  constexpr auto next() -> decltype(auto) {
    auto&& item = _i.get();
    _i.next();
    return relay<decltype(item)>(item);
  }
};

template <typename O>
concept ostream =
    requires { typename O::type; } and requires(O o, typename O::type c) {
      { o.add(c) } -> same_as<bool>;
    };

#endif