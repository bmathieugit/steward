#ifndef __n_io_hpp__
#define __n_io_hpp__

#include <stdio.h>

#include <core/format.hpp>
#include <core/result.hpp>
#include <core/string.hpp>
#include <core/vector.hpp>


enum class from : int { start = SEEK_SET, end = SEEK_END, cur = SEEK_CUR };
enum class mode : size_t {
  r = 0,
  w = 1,
  rp = 2,
  wp = 3,
  a = 4,
  ap = 5,
  std_in = 6,
  std_out = 7
};

constexpr const char *modechr[] = {"r", "w", "r+", "w+", "a", "a+"};

template <mode m>
constexpr bool stdin_mode = m == mode::std_in;

template <mode m>
constexpr bool stdout_mode = m == mode::std_out;

template <mode m>
constexpr bool pathable_mode = !stdout_mode<m> && !stdin_mode<m>;

template <mode m>
constexpr bool readable_mode = m == mode::r || m == mode::rp || m == mode::wp ||
                               m == mode::ap || m == mode::std_in;

template <mode m>
constexpr bool writable_mode =
    m == mode::w || m == mode::a || m == mode::rp || m == mode::wp ||
    m == mode::ap || m == mode::std_out;

template <mode m>
constexpr bool settable_mode = pathable_mode<m>;

enum class seek : int { set = SEEK_SET, cur = SEEK_CUR, end = SEEK_END };

template <typename T, mode m>
class file;

template <typename T, mode m>
  requires readable_mode<m>
class file_iterator {
 private:
  file<T, m> *_file = nullptr;
  mutable maybe<T> _cur;
  mutable bool _consumed = true;

 public:
  ~file_iterator() = default;
  file_iterator() = default;
  file_iterator(file<T, m> *f) : _file(f) {}
  file_iterator(const file_iterator &) = default;
  file_iterator(file_iterator &&) = default;
  file_iterator &operator=(const file_iterator &) = default;
  file_iterator &operator=(file_iterator &&) = default;

 public:
  bool has_next() const {
    if (_file != nullptr && _file->opened()) {
      if (_consumed) {
        _cur = _file->pop();
        _consumed = false;
      }
    }

    return _cur.has();
  }

  T next() {
    _consumed = true;
    return move(_cur.get());
  }
};

template <typename T, mode m>
  requires writable_mode<m>
class file_oterator {
 private:
  file<T, m> *_file;

 public:
  file_oterator() = default;
  file_oterator(file<T, m> *f) : _file(f) {}

  file_oterator(const file_oterator &) = default;
  file_oterator(file_oterator &&) = default;
  file_oterator &operator=(const file_oterator &) = default;
  file_oterator &operator=(file_oterator &&) = default;

 public:
  void sext(const T &t) {
    if (_file != nullptr and _file->opened()) {
      _file->push(t);
    }
  }

  void sext(T &&t) {
    if (_file != nullptr and _file->opened()) {
      _file->push(move(t));
    }
  }
};

template <typename T, mode m>
class file {
 private:
  FILE *_fd = nullptr;

 private:
  void close() {
    if (_fd != nullptr) {
      fflush(_fd);
      fclose(_fd);
      _fd = nullptr;
    }
  }

 public:
  ~file() { close(); }
  file() = default;
  file(const char *path)
    requires pathable_mode<m>
      : _fd(fopen(path, modechr[size_t(m)])) {}
  file(FILE *fd) : _fd(fd) {}
  file(const file &) = delete;
  file(file &&) = default;
  file &operator=(const file &) = delete;
  file &operator=(file &&) = default;

 public:
  bool opened() const { return _fd != nullptr; }

  void set(from fr, long offset)
    requires settable_mode<m>
  {
    if (_fd != nullptr) {
      fseek(_fd, offset, int(fr));
    }
  }

  size_t pos()
    requires settable_mode<m>
  {
    return _fd != nullptr ? ftell(_fd) : 0;
  }

 public:
  void push(const T &t)
    requires writable_mode<m>
  {
    if (_fd != nullptr) {
      fwrite(&t, sizeof(rm_cref<T>), 1, _fd);
    }
  }

  maybe<T> pop()
    requires readable_mode<m>
  {
    maybe<T> res;

    if (_fd != nullptr) {
      T buff;

      if (fread(&buff, sizeof(T), 1, _fd) == 1) {
        res = move(buff);
      }
    }

    return res;
  }

 public:
  auto iter()
    requires readable_mode<m>
  {
    return file_iterator<T, m>(this);
  }

  auto oter()
    requires writable_mode<m>
  {
    return file_oterator<T, m>(this);
  }
};

static auto stdr = file<char, mode::std_in>(stdin);
static auto stdw = file<char, mode::std_out>(stdout);

template <character C, formattable<C, decltype(file<C, mode::std_out>{}.oter())>... T>
void __printf(cstring_iterator<C> fmt, const T &...t) {
  format_to(stdw.oter(), fmt, t...);
}

template <formattable<char, decltype(file<char, mode::std_out>{}.oter())>... T>
void printf(cstring_iterator<char> fmt, const T &...t) {
  __printf(fmt, t...);
}



#endif
