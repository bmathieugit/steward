#ifndef __steward_stack_hpp__
#define __steward_stack_hpp__

#include <stew/vector.hpp>

namespace stew {
template <typename T, size_t N>
class static_stack : public static_vector<T, N> {
 public:
  constexpr ~static_stack() = default;
  constexpr static_stack() = default;
  constexpr explicit static_stack(size_t max) : static_vector<T, N>(max) {}

  template <input_range R>
  constexpr static_stack(R &&r)
      : static_vector<T, N>(stew::end(relay<R>(r)) - stew::begin(relay<R>(r))) {
    push(relay<R>(r));
  }

  constexpr static_stack(const static_stack &) = default;
  constexpr static_stack(static_stack &) = default;
  constexpr static_stack &operator=(const static_stack &) = default;
  constexpr static_stack &operator=(static_stack &&) = default;

  template <input_range R>
  constexpr static_stack &operator=(R &&r) {
    return (*this = static_stack(relay<R>(r)));
  }

 public:
  constexpr auto begin() {
    return reverse_iterator(static_vector<T, N>::end());
  }

  constexpr auto end() {
    return reverse_iterator(static_vector<T, N>::begin());
  }
  constexpr auto begin() const {
    return reverse_iterator(static_vector<T, N>::end());
  }

  constexpr auto end() const {
    return reverse_iterator(static_vector<T, N>::begin());
  }
};

template <typename T>
class fixed_stack : public fixed_vector<T> {
 public:
  constexpr ~fixed_stack() = default;
  constexpr fixed_stack() = default;
  constexpr explicit fixed_stack(size_t max) : fixed_vector<T>(max) {}

  template <input_range R>
  constexpr fixed_stack(R &&r)
      : fixed_vector<T>(stew::end(relay<R>(r)) - stew::begin(relay<R>(r))) {
    push(relay<R>(r));
  }

  constexpr fixed_stack(const fixed_stack &) = default;
  constexpr fixed_stack(fixed_stack &) = default;
  constexpr fixed_stack &operator=(const fixed_stack &) = default;
  constexpr fixed_stack &operator=(fixed_stack &&) = default;

  template <input_range R>
  constexpr fixed_stack &operator=(R &&r) {
    return (*this = fixed_stack(relay<R>(r)));
  }

 public:
  constexpr auto begin() { return reverse_iterator(fixed_vector<T>::end()); }

  constexpr auto end() { return reverse_iterator(fixed_vector<T>::begin()); }
  constexpr auto begin() const {
    return reverse_iterator(fixed_vector<T>::end());
  }

  constexpr auto end() const {
    return reverse_iterator(fixed_vector<T>::begin());
  }
};

template <typename T>
class stack : public vector<T> {
 public:
  constexpr ~stack() = default;
  constexpr stack() = default;
  constexpr explicit stack(size_t max) : vector<T>(max) {}
  template <input_range R>
  constexpr stack(R &&r)
    requires distanciable_iterator<decltype(stew::begin(r))>
      : stack(stew::end(r) - stew::begin(r)) {
    push(relay<R>(r));
  }

  constexpr stack(const stack &) = default;
  constexpr stack(stack &&) = default;

  constexpr stack &operator=(const stack &) = default;
  constexpr stack &operator=(stack &&) = default;

  template <input_range R>
  constexpr stack &operator=(R &&r) {
    return (*this = stack(relay<R>(r)));
  }

 public:
  constexpr auto begin() { return reverse_iterator(vector<T>::end()); }

  constexpr auto end() { return reverse_iterator(vector<T>::begin()); }
  constexpr auto begin() const { return reverse_iterator(vector<T>::end()); }

  constexpr auto end() const { return reverse_iterator(vector<T>::begin()); }
};
}  // namespace stew

#endif
