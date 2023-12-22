#ifndef __stew_core_data_hpp__
#define __stew_core_data_hpp__

#include <core/algorithm.hpp>
#include <core/core.hpp>
#include <core/tuple.hpp>

template <typename I, typename M>
class map_iterator {
 private:
  I _it;
  M _mapper;

 public:
  using type = iterator_type<I>;

  constexpr map_iterator(I it, M mapper) : _it(it), _mapper(mapper) {}

  constexpr map_iterator& operator++() {
    ++_it;
    return *this;
  }

  constexpr map_iterator operator++(int) {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

  constexpr bool operator==(const map_iterator& other) const {
    return _it == other._it;
  }

  constexpr bool operator!=(const map_iterator& other) const {
    return _it != other._it;
  }

  constexpr auto operator*() const -> decltype(auto) { return _mapper(*_it); }
};

template <typename M>
class map_provider {
 private:
  M _mapper;

 public:
  constexpr map_provider(const M& mapper) : _mapper(mapper) {}

  template <iterator I>
  constexpr auto provide(I i, I) {
    return map_iterator<I, M>(i, _mapper);
  }
};

template <typename I, typename Pred>
class filter_iterator {
 public:
  using type = iterator_type<I>;

  filter_iterator(I current, I end, Pred pred)
      : current_(current), end_(end), pred_(pred) {
    while (current_ != end_ && !pred_(*current_)) {
      ++current_;
    }
  }

  // Opérateur d'incrément
  filter_iterator& operator++() {
    do {
      ++current_;
    } while (current_ != end_ && !pred_(*current_));
    return *this;
  }

  constexpr auto operator++(int) {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

  // Opérateur d'égalité
  bool operator==(const filter_iterator& other) const {
    return current_ == other.current_;
  }

  // Opérateur de non-égalité
  bool operator!=(const filter_iterator& other) const {
    return current_ != other.current_;
  }

  // Opérateur de déréférencement
  auto operator*() const -> decltype(auto) { return *current_; }

 private:
  I current_;
  I end_;
  Pred pred_;
};

template <typename M>
class filter_provider {
 private:
  M _mapper;

 public:
  constexpr filter_provider(const M& mapper) : _mapper(mapper) {}

  template <iterator I>
  constexpr auto provide(I i, I sentinel) {
    return filter_iterator<I, M>(i, sentinel, _mapper);
  }
};

template <typename... PROVIDER>
class query {
 private:
  tuple<PROVIDER...> _providers;

 public:
  constexpr query(const PROVIDER&... providers) : _providers(providers...) {}

 public:
  constexpr auto then(const auto& provider) -> decltype(auto) {
    return then(provider, make_index_sequence<sizeof...(PROVIDER)>{});
  }

 private:
  template <typename B, size_t... I>
  constexpr auto then(const B& provider, index_sequence<I...>) {
    return query<PROVIDER..., B>(get<I>(_providers)..., provider);
  }

 private:
  template <size_t I>
  constexpr auto provide(iterator auto i, iterator auto sentinel) {
    if constexpr (I == sizeof...(PROVIDER) - 1) {
      return get<I>(_providers).provide(i, sentinel);
    }

    else {
      return provide<I + 1>(get<I>(_providers).provide(i, sentinel),
                            get<I>(_providers).provide(sentinel, sentinel));
    }
  }

 public:
  template <iterator I>
  void execute(I b, I e) {
    if constexpr (sizeof...(PROVIDER) > 0) {
      for_each(provide<0>(b, e), provide<0>(e, e),
               [](auto&& i) { write(sout, i, '\n'); });
    }
  }
};

;

#endif