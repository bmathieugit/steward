#ifndef __steward_iterator_hpp__
#define __steward_iterator_hpp__

#include <assert.h>
#include <stew/meta.hpp>
#include <stew/utils.hpp>
#include <stew/algorithm.hpp>

namespace stew {
template <typename G>
class generator_iterator {
  G _gener;
  decltype(_gener()) _value;

 public:
  constexpr generator_iterator(G &&gener)
      : _gener(transfer(gener)), _value(_gener()) {}

  template <convertible_to<decltype(_value)> T>
  constexpr generator_iterator(G &&gener, T &&sentinel)
      : _gener(transfer(gener)), _value(relay<T>(sentinel)) {}

 public:
  constexpr auto operator==(const generator_iterator &o) const {
    return _value == o._value;
  }

  constexpr generator_iterator &operator++() {
    _value = _gener();
    return *this;
  }

  constexpr generator_iterator operator++(int) {
    auto copy = *this;
    ++*this;
    return copy;
  }

  constexpr auto operator*() -> decltype(auto) { return _value; }

  constexpr auto operator-(const generator_iterator &o) const {
    return _value - o._value;
  }
};

namespace impl {
template <plain_integral I>
class incrementer {
 private:
  I _current = 0;
  I _step = 1;

 public:
  constexpr incrementer(I current, I step) : _current(current), _step(step) {}

 public:
  constexpr I operator()() {
    auto copy = _current;
    _current += _step;
    return copy;
  }
};
}  // namespace impl

template <plain_integral I>
constexpr auto upto(I from, I to, I step = 1) {
  assert(from <= to);
  assert(((to - from) % step) == 0);

  return view<generator_iterator<impl::incrementer<I>>>(
      generator_iterator<impl::incrementer<I>>(
          impl::incrementer<I>(from, step)),
      generator_iterator<impl::incrementer<I>>(impl::incrementer<I>(to, step),
                                               to));
}

template <plain_integral I>
constexpr auto downto(I from, I to, I step = 1) {
  assert(from >= to);
  assert(((from - to) % step) == 0);

  return view<generator_iterator<impl::incrementer<I>>>(
      generator_iterator<impl::incrementer<I>>(
          impl::incrementer<I>(from, -step)),
      generator_iterator<impl::incrementer<I>>(impl::incrementer<I>(to, -step),
                                               to));
}

template <typename T, push_container<T> C>
class push_iterator {
 private:
  C *_container = nullptr;

 public:
  constexpr push_iterator(C &container) : _container(&container) {}

  push_iterator &operator=(const T &t) {
    _container->push(t);
    return *this;
  }

  push_iterator &operator=(T &&t) {
    _container->push(transfer(t));
    return *this;
  }

  push_iterator &operator*() { return *this; }

  push_iterator &operator++() { return *this; }

  push_iterator &operator++(int) { return *this; }

  auto operator==(const push_iterator &o) const {
    return _container == o._container;
  }
};

template <typename T, push_container<T> C>
push_iterator<T, C> pushing(C &c) {
  return push_iterator<T, C>(c);
}

template <input_iterator I>
class transfer_iterator {
 private:
  I _iter;

 public:
  constexpr transfer_iterator(I iter) : _iter(iter) {}

 public:
  constexpr auto operator*() -> decltype(auto) { return transfer(*_iter); }

  constexpr bool operator==(const transfer_iterator &o) const {
    return _iter == o._iter;
  }

  constexpr transfer_iterator &operator++() {
    ++_iter;
    return *this;
  }

  constexpr transfer_iterator operator++(int) {
    auto copy = *this;
    ++*this;
    return copy;
  }

  constexpr auto operator-(const transfer_iterator &o) const
    requires distanciable_iterator<I>
  {
    return _iter - o._iter;
  }
};

template <input_iterator I>
constexpr view<transfer_iterator<I>> transfer_view(I b, I e) {
  return view<transfer_iterator<I>>(transfer_iterator<I>(b),
                                    transfer_iterator<I>(e));
}

template <input_range R>
constexpr auto transfer_view(R &&r) {
  return transfer_view(begin(relay<R>(r)), end(relay<R>(r)));
}

template <bidirectional_iterator I>
class reverse_iterator {
 protected:
  I _iter;

 public:
  constexpr reverse_iterator(I iter) : _iter(iter) {}

  constexpr bool operator==(const reverse_iterator &o) const {
    return _iter == o._iter;
  }

  constexpr decltype(auto) operator*() const {
    auto copy = _iter;
    --copy;
    return *copy;
  }

  constexpr reverse_iterator &operator++() {
    --_iter;
    return *this;
  }
  constexpr reverse_iterator operator++(int) {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

  constexpr reverse_iterator &operator--() {
    ++_iter;
    return *this;
  }

  constexpr reverse_iterator operator--(int) {
    auto tmp = *this;
    --(*this);
    return tmp;
  }

  constexpr auto operator-(const reverse_iterator &o) const
    requires distanciable_iterator<I>
  {
    return _iter - o._iter;
  }
};

template <bidirectional_iterator I>
constexpr view<reverse_iterator<I>> reverse_view(I b, I e) {
  return view<reverse_iterator<I>>(reverse_iterator<I>(e),
                                   reverse_iterator<I>(b));
}

template <bidirectional_range R>
constexpr auto reverse_view(R &&r) {
  return reverse_view(begin(relay<R>(r)), end(relay<R>(r)));
}

template <input_iterator I, typename M>
class map_iterator {
 private:
  I _iter;
  M _map;

 public:
  constexpr map_iterator(I iter, M map)
      : _iter(transfer(iter)), _map(transfer(map)) {}

 public:
  constexpr map_iterator &operator++() {
    ++_iter;
    return *this;
  }

  constexpr map_iterator operator++(int) {
    auto copy = *this;
    ++(*this);
    return copy;
  }

  constexpr bool operator==(const map_iterator &o) const {
    return _iter == o._iter;
  }

  constexpr auto operator*() const { return _map(*_iter); }

  constexpr auto operator-(const map_iterator &o) const
    requires distanciable_iterator<I>
  {
    return _iter - o._iter;
  }
};

template <input_iterator I, typename M>
constexpr view<map_iterator<I, M>> map_view(I b, I e, M map) {
  return view<map_iterator<I, M>>(map_iterator<I, M>(b, map),
                                  map_iterator<I, M>(e, map));
}

template <input_range R, typename M>
constexpr auto map_view(R &&r, M map) {
  return map_view(begin(relay<R>(r)), end(relay<R>(r)), map);
}

template <input_iterator I, predicate<decltype(*I{})> F>
class filter_iterator {
 private:
  I _iter;
  I _end;
  F _filter;

 public:
  constexpr filter_iterator(I iter, I end, F filter)
      : _iter(iter), _end(end), _filter(filter) {}

 public:
  constexpr filter_iterator &operator++() {
    ++_iter;

    while (_iter != _end && !_filter(*_iter)) {
      ++_iter;
    }

    return *this;
  }

  constexpr filter_iterator operator++(int) {
    auto copy = *this;
    ++(*this);
    return copy;
  }

  constexpr bool operator==(const filter_iterator &o) const {
    return _iter == o._iter;
  }

  constexpr auto operator*() {
    while (_iter != _end && !_filter(*_iter)) {
      ++_iter;
    }

    return *_iter;
  }

  constexpr auto operator-(const filter_iterator &o) const
    requires distanciable_iterator<I>
  {
    return _iter - o._iter;
  }
};

template <input_iterator I, typename F>
constexpr view<filter_iterator<I, F>> filter_view(I b, I e, F filter) {
  return view<filter_iterator<I, F>>(filter_iterator<I, F>(b, e, filter),
                                     filter_iterator<I, F>(e, e, filter));
}

template <input_range R, typename F>
constexpr auto filter_view(R &&r, F filter) {
  return filter_view(begin(relay<R>(r)), end(relay<R>(r)), filter);
}

}  // namespace stew

#endif
