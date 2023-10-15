#ifndef __n_io_hpp__
#define __n_io_hpp__

#include <stdio.h>

#include <core/result.hpp>
#include <core/char-istream.hpp>
#include <core/char-ostream.hpp>
#include <core/string.hpp>
#include <core/vector.hpp>

using file_descriptor = FILE*;

static constexpr file_descriptor null_file = nullptr;

file_descriptor fopen(char_iterator auto name, const char* mode) {
  string s;
  s << name;
  s << '\0';
  return fopen(s.data(), mode);
}

template <typename T>
constexpr size_t flength(file_descriptor f) {
  size_t len = 0;

  if (f != null_file) {
    auto pos = ftell(f);
    fseek(f, 0L, SEEK_END);
    len = ftell(f) / sizeof(T);
    fseek(f, pos, SEEK_SET);
  }

  return len;
}

template <typename T>
class file_istream {
 public:
  using type = T;

 private:
  file_descriptor _f;
  size_t _pos;
  size_t _len;
  bool _read;

 public:
  constexpr file_istream(file_descriptor f)
      : _f(f), _pos(0), _len(flength<T>(_f)), _read(false) {}

  constexpr file_istream(const char* name) : file_istream(fopen(name, "r")) {}

  constexpr file_istream(char_iterator auto name)
      : file_istream(fopen(name, "r")) {}

 public:
  constexpr bool has() const { return _len != _pos; }

  constexpr T get() {
    byte_t buf[sizeof(T)];

    if (not _read and fread(&buf, 1, sizeof(T), _f) == 1) {
      _read = true;
    }

    return *reinterpret_cast<T*>(buf);
  }

  constexpr void next() {
    _pos += 1;
    _read = false;
  }
};

template <typename T>
class file_ostream {
 public:
  using type = T;

 private:
  file_descriptor _f;

 public:
  constexpr file_ostream(file_descriptor f) : _f(f) {}

  constexpr file_ostream(const char* name) : file_ostream(fopen(name, "w")) {}

  constexpr file_ostream(char_iterator auto name)
      : file_ostream(fopen(name, "w")) {}

 public:
  constexpr bool add(const T& t) {
    return _f != null_file and fwrite(&t, sizeof(T), 1, _f) == 1;
  }
};

template <typename T>
class file_append_ostream {
 public:
  using type = T;

 private:
  file_descriptor _f;

 public:
  constexpr file_append_ostream(file_descriptor f) : _f(f) {}

  constexpr file_append_ostream(const char* name)
      : file_append_ostream(fopen(name, "a")) {}

  constexpr file_append_ostream(char_iterator auto name)
      : file_append_ostream(fopen(name, "a")) {}

 public:
  constexpr bool add(const T& t) {
    return _f != null_file and fwrite(&t, sizeof(T), 1, _f) == 1;
  }
};

// static auto stdr = file_reader<char>(stdin);
// static auto stdw = file<char>(stdout);

#endif
