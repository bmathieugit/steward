#ifndef __n_algorithm_v2_hpp__
#define __n_algorithm_v2_hpp__

#include <core/core.hpp>
#include <core/tuple.hpp>

template <typename... A>
class prepared {
 private:
  tuple<A...> _algorithms;

 public:
  constexpr prepared(A&&... algorithms)
      : _algorithms(relay<A>(algorithms)...) {}

 private:
  template <size_t RANK, typename IT>
  constexpr auto __eval(IT it) {
    if constexpr (RANK < (sizeof...(A) - 1)) {
      return __eval<RANK + 1>(get<RANK>(_algorithms)(it));
    }

    else {
      return get<RANK>(_algorithms)(it);
    }
  }

 public:
  template <typename IT>
  constexpr auto eval(IT it) -> decltype(auto) {
    if constexpr (sizeof...(A) > 0) {
      return __eval<0>(it);
    }
  }
};

template <typename... A>
constexpr auto prepare(A&&... a) {
  return prepared(relay<A>(a)...);
}

template <typename I, typename T>
class transform_iterator {
 private:
  I _it;
  T _transformer;

 public:
  using type = typename I::type;

  constexpr transform_iterator(I it, T transformer)
      : _it(it), _transformer(transformer) {}

  constexpr bool has_next() { return _it.has_next(); }

  constexpr auto next() -> decltype(auto) { return _transformer(_it.next()); }
};

template <typename T>
constexpr auto transform(T transformer) -> decltype(auto) {
  return [=]<iterator I>(I it) constexpr mutable {
    return transform_iterator<I, T>(it, transformer);
  };
}

constexpr auto count() {
  return []<iterator I>(I it) constexpr mutable {
    if constexpr (distanciable<I>) {
      return it.distance();
    }

    else {
      size_t cnt = 0;

      while (it.has_next()) {
        it.next();
        ++cnt;
      }

      return cnt;
    }
  };
}

template <typename T>
constexpr auto count(const T& t) {
  return [&]<iterator I>(I it) constexpr mutable {
    size_t cnt = 0;

    while (it.has_next()) {
      if (it.next() == t) {
        ++cnt;
      }
    }

    return cnt;
  };
}

template <typename P>
constexpr auto count_if(P&& pred) {
  return [pred = relay<P>(pred)]<iterator I>(I it) constexpr mutable {
    size_t cnt = 0;

    while (it.has_next()) {
      if (pred(it.next())) {
        ++cnt;
      }
    }

    return cnt;
  };
}

template <typename T>
constexpr auto find(const T& t) {
  return [&]<iterator I>(I it) constexpr mutable {
    while (it.has_next()) {
      auto tmp = it;

      if (it.next() == t) {
        return tmp;
      }
    }

    return it;
  };
}

template <typename P>
constexpr auto find_if(P&& pred) {
  return [pred = relay<P>(pred)]<iterator I>(I it) constexpr mutable {
    while (it.has_next()) {
      auto tmp = it;

      if (pred(it.next())) {
        return tmp;
      }
    }

    return it;
  };
}

template <typename P>
constexpr auto find_if_not(P&& pred) {
  return [pred = relay<P>(pred)]<iterator I>(I it) constexpr mutable {
    while (it.has_next()) {
      auto tmp = it;

      if (not pred(it.next())) {
        return tmp;
      }
    }

    return it;
  };
}

template <typename P>
constexpr auto all_of(P&& pred) {
  return [pred = relay<P>(pred)]<iterator I>(I it) constexpr mutable {
    return not find_if_not(pred)(it).has_next();
  };
}

template <typename P>
constexpr auto none_of(P&& pred) {
  return [pred = relay<P>(pred)]<iterator I>(I it) constexpr mutable {
    return not find_if(pred)(it).has_next();
  };
}

template <typename P>
constexpr auto any_of(P&& pred) {
  return [pred = relay<P>(pred)]<iterator I>(I it) constexpr mutable {
    return find_if(pred)(it).has_next();
  };
}

template <iterator I1>
constexpr auto starts_with(I1 i1) {
  return [=]<iterator I2>(I2 i2) constexpr mutable {
    while (i1.has_next() and i2.has_next() and i1.next() == i2.next()) {
    }

    return not i1.has_next();
  };
}

template <iterator I1>
constexpr auto equals(I1 i1) {
  return [=]<iterator I2>(I2 i2) constexpr mutable {
    while (i1.has_next() and i2.has_next() and i1.next() == i2.next()) {
    }

    return not i1.has_next() and not i2.has_next();
  };
}

template <typename F>
constexpr auto for_each(F&& func) {
  return [func = relay<F>(func)](auto it) constexpr mutable {
    while (it.has_next()) {
      func(it.next());
    }
  };
}

#endif
