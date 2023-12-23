#ifndef __stew_core_data_hpp__
#define __stew_core_data_hpp__

#include <core/algorithm.hpp>
#include <core/core.hpp>
#include <core/tuple.hpp>

template <typename I, typename M>
class transform_iterator {
 private:
  I _it;
  M _transformper;

 public:
  using type = iterator_type<I>;

  constexpr transform_iterator(I it, M transformper) : _it(it), _transformper(transformper) {}

  constexpr transform_iterator& operator++() {
    ++_it;
    return *this;
  }

  constexpr transform_iterator operator++(int) {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

  constexpr bool operator==(const transform_iterator& other) const {
    return _it == other._it;
  }

  constexpr bool operator!=(const transform_iterator& other) const {
    return _it != other._it;
  }

  constexpr auto operator*() const -> decltype(auto) { return _transformper(*_it); }
};

template <typename M>
class transform_provider {
 private:
  M _transformper;

 public:
  constexpr transform_provider(const M& transformper) : _transformper(transformper) {}

  template <iterator I>
  constexpr auto provide(I i, I) {
    return transform_iterator<I, M>(i, _transformper);
  }
};

template <typename M>
constexpr auto transform(const M& transformper) {
  return transform_provider(transformper);
}

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

  bool operator==(const filter_iterator& other) const {
    return current_ == other.current_;
  }

  bool operator!=(const filter_iterator& other) const {
    return current_ != other.current_;
  }

  auto operator*() const -> decltype(auto) { return *current_; }

 private:
  I current_;
  I end_;
  Pred pred_;
};

template <typename Pred>
class filter_provider {
 private:
  Pred _transformper;

 public:
  constexpr filter_provider(const Pred& transformper) : _transformper(transformper) {}

  template <iterator I>
  constexpr auto provide(I i, I sentinel) {
    return filter_iterator<I, Pred>(i, sentinel, _transformper);
  }
};

template <typename P>
constexpr auto filter(const P& transformper) {
  return filter_provider(transformper);
}

template <iterator I, typename O>
class collector_oterator {
 private:
  I _it;
  O& _ot;

 public:
  using type = collector_oterator;

  constexpr collector_oterator(I it, O& ot) : _it(it), _ot(ot) {}

  constexpr collector_oterator& operator++() {
    ++_it;
    return *this;
  }

  constexpr collector_oterator operator++(int) {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

  constexpr bool operator==(const collector_oterator& other) const {
    return _it == other._it;
  }

  constexpr bool operator!=(const collector_oterator& other) const {
    return _it != other._it;
  }

  constexpr auto operator*() const -> decltype(auto) {
    _ot.add(*_it);
    return *this;
  }
};

template <typename O>
class collector_provider {
 private:
  O& _out;

 public:
  constexpr collector_provider(O& out) : _out(out) {}

  template <iterator I>
  constexpr auto provide(I i, I) {
    return collector_oterator<I, O>(i, _out);
  }
};

template <typename O>
constexpr auto collect(O& transformper) {
  return collector_provider(transformper);
}

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
      for_each(provide<0>(b, e), provide<0>(e, e), [](auto&& i) {});
    }
  }
};

;

#endif