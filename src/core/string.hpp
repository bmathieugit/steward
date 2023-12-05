#ifndef __n_string_hpp__
#define __n_string_hpp__

#include <core/allocator.hpp>
#include <core/core.hpp>
#include <core/vector.hpp>

template <character C>
class basic_string {
 public:
  using type = C;
  using reference = C&;
  using const_reference = const C&;
  using move_reference = C&&;
  using pointer = C*;
  using const_pointer = const C*;

 private:
  allocator<C> _alloc;
  size_t _max = 0;
  size_t _len = 0;
  pointer _data = nullptr;

 public:
  constexpr ~basic_string() {
    _alloc.destroy(_data, _len);
    _alloc.deallocate(_data);
  }

  constexpr basic_string() = default;

  constexpr basic_string(size_t max)
      : _max(max), _len(0), _data(_alloc.allocate(max + 1)) {}

  constexpr basic_string(const basic_string& v)
      : _max(v._max), _len(v._len), _data(_alloc.allocate(v._max + 1)) {
    _alloc.copy(_data, v._data, _len);
  }

  constexpr basic_string(basic_string&& v)
      : _max(v._max), _len(v._len), _data(v._data) {
    v._max = 0;
    v._len = 0;
    v._data = nullptr;
  }

  constexpr basic_string& operator=(const basic_string& v) {
    if (this != &v) {
      _max = v._max;
      _len = v._len;
      _data = _alloc.allocate(_max + 1);
      _alloc.copy(_data, v._data, _len);
    }

    return *this;
  }

  constexpr basic_string& operator=(basic_string&& v) {
    if (this != &v) {
      auto old_max = _max;
      auto old_len = _len;
      auto old_data = _data;

      _max = v._max;
      _len = v._len;
      _data = v._data;

      v._max = old_max;
      v._len = old_len;
      v._data = old_data;
    }

    return *this;
  }

  constexpr auto len() const { return _len; }

  constexpr auto empty() const { return _len == 0; }

  constexpr auto max() const { return _max; }

  constexpr auto full() const { return _len == _max; }

  constexpr bool has(size_t p) const { return p < _len; }

  constexpr pointer data() { return _data; }

  constexpr const_pointer data() const { return _data; }

  constexpr void clear() {
    _alloc.destroy(_data, _len);
    _len = 0;

    if (_data != nullptr) {
      _data[0] = 0;
    }
  }

  constexpr reference at(size_t p) {
    if (p < _len) {
      return _data[p];
    } else {
      throw out_of_range();
    }
  }

  constexpr const_reference at(size_t p) const {
    if (p < _len) {
      return _data[p];
    } else {
      throw out_of_range();
    }
  }

  template <convertible_to<C> U>
  constexpr void add(U&& u, size_t p) {
    if (p > _len) {
      throw out_of_range();
    }

    if (full()) {
      _max = _max * 2 + 10;
      auto dtmp = _alloc.allocate(_max + 1);

      for (size_t i = 0; i < _len; ++i) {
        dtmp[i] = move(_data[i]);
      }

      _data = dtmp;
    }

    if (not full()) {
      for (size_t i = _len; i > p; --i) {
        _data[i] = move(_data[i - 1]);
      }

      _data[p] = relay<U>(u);
      ++_len;
    }
  }

  template <convertible_to<C> U>
  constexpr void add(U&& u) {
    add(relay<U>(u), _len);
  }

  constexpr void exchange(size_t p1, size_t p2) {
    if (p1 >= _len or p2 >= _len) {
      throw out_of_range();
    }

    if (p1 != p2) {
      auto tmp1 = move(_data[p1]);
      _data[p1] = move(_data[p2]);
      _data[p2] = move(tmp1);
    }
  }

  template <convertible_to<C> U>
  constexpr void modify(U&& u, size_t p) {
    if (p >= _len) {
      throw out_of_range();
    }

    _data[p] = relay<U>(u);
  }

  constexpr void remove(size_t p) {
    if (p >= _len) {
      throw out_of_range();
    }

    for (size_t i = p; i < _len - 1; ++i) {
      _data[i] = _data[i + 1];
    }

    _data[_len - 1] = 0;
    _len -= 1;
  }
};

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;

template <character C>
constexpr auto begin(const basic_string<C>& s) {
  return s.data();
}

template <character C>
constexpr auto end(const basic_string<C>& s) {
  return s.data() + s.len();
}

template <character C>
constexpr auto begin(basic_string<C>& s) {
  return s.data();
}

template <character C>
constexpr auto end(basic_string<C>& s) {
  return s.data() + s.len();
}

// template <character C>
// class basic_string_view {
//  public:
//   using type = C;
//   using reference = C&;
//   using const_reference = const C&;
//   using move_reference = C&&;
//   using pointer = C*;
//   using const_pointer = const C*;

//  private:
//   pointer _begin = nullptr;
//   pointer _end = nullptr;

//  public:
//   constexpr basic_string_view(const_pointer begin, const_pointer end)
//       : _begin(begin), _end(end) {}

//   constexpr basic_string_view(const_pointer begin, size_t len)
//       : _begin(begin), _end(begin + len) {}

//   constexpr basic_string_view(const_pointer begin) : _begin(_begin) {
//     while (*begin != '\0') {
//       ++begin;
//     }

//     _end = begin;
//   }

//   constexpr basic_string_view(const basic_string<C>& s)
//       : _begin(begin(s)), _end(end(s)) {}

//   constexpr basic_string_view(const basic_string_view&) = default;

//   constexpr basic_string_view(basic_string_view&&) = default;

//   constexpr basic_string_view& operator=(const_pointer begin) {
//     _begin = begin;

//     while (*begin != '\0') {
//       ++begin;
//     }

//     _end = begin;

//     return *this;
//   }

//   constexpr basic_string_view& operator=(const basic_string_view& s) {
//     _begin = begin(s);
//     _end = end(s);
//     return *this;
//   }

//   constexpr basic_string_view& operator=(const basic_string_view&) = default;

//   constexpr basic_string_view& operator=(basic_string_view&&) = default;

//   constexpr auto len() const { return _end - _begin; }

//   constexpr auto empty() const { return len() == 0; }

//   constexpr bool has(size_t p) const { return p < len(); }

//   constexpr const_pointer data() const { return _begin; }

//   constexpr const_reference at(size_t p) const {
//     if (p < _len) {
//       / return _begin[p];
//     } else {
//       throw out_of_range();
//     }
//   }
// };

// template <character C>
// constexpr auto begin(const basic_string_view<C>& s) {
//   return s.data();
// }

// template <character C>
// constexpr auto end(const basic_string_view<C>& s) {
//   return s.data() + s.len();
// }

// template <character C>
// constexpr auto begin(basic_string_view<C>& s) {
//   return s.data();
// }

// template <character C>
// constexpr auto end(basic_string_view<C>& s) {
//   return s.data() + s.len();
// }

// using string_view = basic_string_view<char>;
// using wstring_view = basic_string_view<wchar_t>;

// template <typename CI>
// concept char_iterator = requires(CI ci) { character<decltype(*ci)>; };

// template <char_iterator S, character C>
// constexpr S& from_chars(S& s, maybe<C>& c) {
//   if (s.has()) {
//     c = s.next();
//   }

//   return s;
// }

// template <char_iterator S, unsigned_integral I>
// constexpr S& from_chars(S& s, maybe<I>& i) {
//   if (s.has()) {
//     auto c = s.next();

//     if ('0' <= c and c <= '9') {
//       I tmp = 0;

//       tmp = static_cast<I>(c - '0');

//       while (s.has()) {
//         c = s.next();

//         if ('0' <= c and c <= '9') {
//           tmp = tmp * 10 + c - '0';
//         } else {
//           break;
//         }
//       }

//       i = tmp;
//     }
//   }

//   return s;
// }

// template <char_iterator S, signed_integral I>
// constexpr S& from_chars(S& s, maybe<I>& i) {
//   maybe<bool> neg;

//   if (s.has()) {
//     auto c = s.next();

//     switch (c) {
//       case '-':
//         neg = true;
//         c = s.next();
//         break;
//       case '+':
//         c = s.next();
//         break;
//     }

//     if ('0' <= c and c <= '9') {
//       I tmp = 0;

//       tmp = static_cast<I>(c - '0');

//       while (s.has()) {
//         c = s.next();

//         if ('0' <= c and c <= '9') {
//           tmp = tmp * 10 + c - '0';
//         } else {
//           break;
//         }
//       }

//       if (neg.has() and neg.get()) {
//         tmp = -tmp;
//       }

//       i = tmp;
//     }
//   }

//   return s;
// }

// template <char_iterator S>
// constexpr S& from_chars(S& s, maybe<bool>& b) {
//   if (s.has()) {
//     auto c = s.next();

//     if (c == '0') {
//       b = true;
//     }

//     else if (c == '1') {
//       b = false;
//     }
//   }

//   return s;
// }

// template <oterator O, character C>
// constexpr O& to_chars(O& o, C c) {
//   o.add(c);
//   return o;
// }

// template <char_oterator O, char_iterator I>
// constexpr O& to_chars(O& o, I i) {
//   while (i.has()) {
//     o.add(i.next());
//   }

//   return o;
// }

// template <char_oterator S, collection C>
// constexpr S& to_chars(S& o, const C& c) {
//   return to_chars(o, iter(c));
// }

// template <char_oterator S, character C, size_t N>
// constexpr S& to_chars(S& o, const C(s)[N]) {
//   return to_chars(o, iter(s));
// }

// template <char_oterator S, character C>
// constexpr S& to_chars(S& o, const C* s) {
//   return to_chars(o, iter(s));
// }

// template <char_oterator S, signed_integral I>
// constexpr S& to_chars(S& o, I i) {
//   basic_static_string<typename S::type, 20> tbuff;

//   if (i == 0) {
//     tbuff.add('0');
//   } else {
//     const bool neg = i < 0;

//     while (i != 0) {
//       tbuff.add("0123456789"[neg ? -(i % 10) : i % 10]);
//       i /= 10;
//     }

//     if (neg) {
//       tbuff.add('-');
//     }
//   }

//   auto ibuff = riter(tbuff);

//   while (ibuff.has()) {
//     o.add(ibuff.next());
//   }

//   return o;
// }

// template <char_oterator S, unsigned_integral I>
// constexpr S& to_chars(S& o, I i) {
//   basic_static_string<typename S::type, 20> tbuff;

//   if (i == 0) {
//     tbuff.add('0');
//   } else {
//     while (i != 0) {
//       tbuff.add("0123456789"[i % 10]);
//       i /= 10;
//     }
//   }

//   auto ibuff = riter(tbuff);

//   while (ibuff.has()) {
//     o.add(ibuff.next());
//   }

//   return o;
// }

// template <char_oterator S>
// constexpr S& to_chars(S& o, bool b) {
//   return to_chars(o, (b ? "true" : "false"));
// }

// template <char_iterator I, typename... C>
// constexpr void read(I i, maybe<C>&... mt) {
//   (from_chars(i, mt), ...);
// }

// template <char_oterator O, typename... C>
// constexpr void write(O o, const C&... t) {
//   (to_chars(o, t), ...);
// }

#endif
