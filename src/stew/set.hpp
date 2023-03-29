#ifndef __steward_set_hpp__
#define __steward_set_hpp__

#include <stew/vector.hpp>

namespace stew {
template <typename T>
class set {
 public:
  struct iterator {
    size_t _idx = 0;
    vector<size_t> *_index = nullptr;
    vector<T> *_data = nullptr;

   public:
    constexpr iterator(size_t idx, vector<T> *data = nullptr,
                       vector<size_t> *index = nullptr)
        : _idx(idx), _data(data), _index(index) {}

   public:
    constexpr iterator &operator++() {
      if (_data != nullptr && _index != nullptr) ++_idx;

      return *this;
    }

    constexpr iterator operator++(int) {
      auto copy = *this;
      ++(*this);
      return copy;
    }

    constexpr bool operator==(const iterator &o) const {
      return _data == o._data && _index == o._index && _idx == o._idx;
    }

    constexpr auto operator*() -> decltype(auto) {
      assert(_data != nullptr && _index == nullptr);
      return ((*_data)[(*_index)[_idx]]);
    }
  };

  struct citerator {
    size_t _idx = 0;
    const vector<size_t> *_index = nullptr;
    const vector<T> *_data = nullptr;

   public:
    constexpr citerator(size_t idx, const vector<T> *data = nullptr,
                        const vector<size_t> *index = nullptr)
        : _idx(idx), _data(data), _index(index) {}

   public:
    constexpr citerator &operator++() {
      if (_data != nullptr && _index != nullptr) ++_idx;

      return *this;
    }

    constexpr citerator operator++(int) {
      auto copy = *this;
      ++(*this);
      return copy;
    }

    constexpr bool operator==(const citerator &o) const {
      return _data == o._data && _index == o._index && _idx == o._idx;
    }

    constexpr auto operator*() -> decltype(auto) {
      assert(_data != nullptr && _index == nullptr);
      return ((*_data)[(*_index)[_idx]]);
    }
  };

 private:
  vector<T> _data;
  vector<size_t> _index;

 public:
  constexpr ~set() = default;
  constexpr set() = default;
  constexpr set(size_t max) : _data(max), _index(max) {}
  constexpr set(const set &) = default;
  constexpr set(set &&) = default;
  constexpr set &operator=(const set &) = default;
  constexpr set &operator=(set &&) = default;

 public:
  constexpr size_t size() const { return _data.size(); }
  constexpr bool empty() const { return _data.empty(); }
  constexpr bool full() const { return _data.full(); }

 public:
  constexpr auto begin() { return iterator(0, &_data, &_index); }
  constexpr auto end() { return iterator(_data.size(), &_data, &_index); }

  constexpr auto begin() const { return citerator(0, &_data, &_index); }
  constexpr auto end() const {
    return citerator(_data.size(), &_data, &_index);
  }

 public:
  template <convertible_to<T> U>
  constexpr void insert(iterator i, U &&u) {
    _index.push(0);

    for (size_t j = _index.size() - 1; j > i._idx; --j) {
      _index[j] = _index[j - 1];
    }

    _index[i._idx] = _data.size();
    _data.push(relay<U>(u));
  }

 public:
  template <convertible_to<T> U>
  constexpr void push(U &&u) {
    auto fnd = find(*this, [&u](const T &i) { return !(i < u); });

    if (fnd == end() || *fnd != u) {
      insert(fnd, relay<U>(u));
    }
  }

  template <input_range R>
  constexpr void push(R &&r) {
    for (auto &&i : relay<R>(r)) {
      push(relay<decltype(i)>(i));
    }
  }
  // TODO: implement pop method.
  // maybe<T> pop() {}
};
}  // namespace stew

#endif
