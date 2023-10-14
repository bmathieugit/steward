#ifndef __n_io_hpp__
#define __n_io_hpp__

#include <stdio.h>

#include <core/result.hpp>
#include <core/string.hpp>
#include <core/vector.hpp>

template <typename T>
class file_reader {
 public:
  using type = T;
  using position = size_t;

 private:
  FILE* _fd = nullptr;
  mutable size_t _len = static_cast<size_t>(-1);

 public:
  ~file_reader() {
    if (_fd != nullptr) {
      fclose(_fd);
      _fd = nullptr;
    }
  }

  file_reader(const char* path) : _fd(fopen(path, "r")) {}
  file_reader(FILE* fd) : _fd(fd) {}
  file_reader(const file_reader&) = delete;
  file_reader(file_reader&&) = default;
  file_reader& operator=(const file_reader&) = delete;
  file_reader& operator=(file_reader&&) = default;

 public:
  size_t len() const {
    if (_len == static_cast<size_t>(-1)) {
      if (_fd == nullptr) {
        _len = 0;
      }

      else {
        size_t cur = ftell(_fd);
        fseek(_fd, 0L, SEEK_END);
        _len = ftell(_fd);
        fseek(_fd, cur, SEEK_SET);
      }
    }

    return _len;
  }

  bool empty() const { return len() == 0; }

  bool has(position p) const { return p < len(); }

  T at(position p) const {
    fseek(_fd, p, SEEK_SET);
    T tmp;
    fread(&tmp, 1, sizeof(T), _fd);
    return tmp;
  }
};

template <typename T>
class file_reader_iterator {
 public:
  using type = T;
  using position = size_t;

 private:
  file_reader<T>& _file;
  position _pos = 0;

 public:
  constexpr file_reader_iterator(file_reader<T>& f) : _file(f) {}

 public:
  constexpr bool has() const { return _file.has(_pos); }

  constexpr T get() { return _file.at(_pos); }

  constexpr void next() { _pos += 1; }

  constexpr position pos() const { return _pos; }
};

template <typename T>
constexpr auto iter(file_reader<T>& fr) {
  return file_reader_iterator<T>(fr);
}

static auto stdr = file_reader<char>(stdin);
// static auto stdw = file<char>(stdout);

#endif
