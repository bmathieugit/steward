#ifndef __n_io_hpp__
#define __n_io_hpp__

#include <stdio.h>

#include <core/result.hpp>
#include <core/string.hpp>
#include <core/vector.hpp>

template <typename T>
class file_reader {
 public:
  using type = maybe<T>;
  using position = size_t;

 private:
  FILE* _fd = nullptr;
  mutable size_t _len;

 public:
  ~file_reader() {
    if (_fd != nullptr) {
      fclose(_fd);
      _fd = nullptr;
    }
  }

  file_reader(FILE* fd) : _fd(fd) {
    if (_fd == nullptr) {
      _len = 0;
    }

    else {
      fseek(_fd, 0L, SEEK_END);
      _len = ftell(_fd) / sizeof(T);
      fseek(_fd, 0L, SEEK_SET);
    }
  }

  file_reader(const char* path) : file_reader(fopen(path, "r")) {}
  file_reader(const file_reader&) = delete;
  file_reader(file_reader&&) = default;
  file_reader& operator=(const file_reader&) = delete;
  file_reader& operator=(file_reader&&) = default;

 public:
  size_t len() const { return _len; }
  bool empty() const { return _len == 0; }
  bool has(position p) const { return p < _len; }

  type at(position p) const {
    type tmp;

    if (p < _len) {
      fseek(_fd, p * sizeof(T), SEEK_SET);
    }

    T t;

    if (fread(&t, 1, sizeof(T), _fd) == 1) {
      tmp = move(t);
    }

    return tmp;
  }
};

template <typename T>
class file_reader_iterator {
 public:
  using type = typename file_reader<T>::type;
  using position = typename file_reader<T>::position;

 private:
  file_reader<T>& _file;
  position _pos = 0;

 public:
  constexpr file_reader_iterator(file_reader<T>& f) : _file(f) {}

 public:
  constexpr bool has() const { return _file.has(_pos); }
  constexpr auto get() { return _file.at(_pos); }
  constexpr void next() { _pos += 1; }
  constexpr position pos() const { return _pos; }
};

template <typename T>
constexpr auto iter(file_reader<T>& fr) {
  return file_reader_iterator<T>(fr);
}

template <typename T>
class file_writer {
 public:
  using type = maybe<T>;
  using position = size_t;

 private:
  FILE* _fd = nullptr;
  mutable size_t _len;

 public:
  ~file_writer() {
    if (_fd != nullptr) {
      fclose(_fd);
      _fd = nullptr;
    }
  }

  file_writer(FILE* fd) : _fd(fd) {
    if (_fd == nullptr) {
      _len = 0;
    }

    else {
      fseek(_fd, 0L, SEEK_END);
      _len = ftell(_fd) / sizeof(T);
      fseek(_fd, 0L, SEEK_SET);
    }
  }

  file_writer(const char* path, bool append = false)
      : file_writer(fopen(path, append ? "a" : "w")) {}
  file_writer(const file_writer&) = delete;
  file_writer(file_writer&&) = default;
  file_writer& operator=(const file_writer&) = delete;
  file_writer& operator=(file_writer&&) = default;

 public:
  size_t len() const { return _len; }

  bool empty() const { return _len == 0; }

  bool add(const T& t) {
    if (_fd != nullptr) {
      return fwrite(&t, sizeof(T), 1, _fd) == 1;
    }

    else {
      return false;
    }
  }
};

template <typename T>
class file_writer_iterator {
 public:
  using type = typename file_reader<T>::type;
  using position = typename file_reader<T>::position;

 private:
  file_reader<T>& _file;
  position _pos = 0;

 public:
  constexpr file_reader_iterator(file_reader<T>& f) : _file(f) {}

 public:
  constexpr bool has() const { return _file.has(_pos); }
  constexpr auto get() { return _file.at(_pos); }
  constexpr void next() { _pos += 1; }
  constexpr position pos() const { return _pos; }
};

template <typename T>
constexpr auto oter(file_writer<T>& fw) {
  return file_writer_oterator<T>(fw);
}

// static auto stdr = file_reader<char>(stdin);
// static auto stdw = file<char>(stdout);

#endif
