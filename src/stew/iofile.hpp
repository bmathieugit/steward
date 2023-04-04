#ifndef __steward_iofile_hpp__
#define __steward_iofile_hpp__

#include <stdio.h>

#include <stew/array.hpp>
#include <stew/format.hpp>
#include <stew/maybe.hpp>
#include <stew/meta.hpp>
#include <stew/string.hpp>
#include <stew/vector.hpp>

namespace stew {

enum class from : int { start = SEEK_SET, end = SEEK_END, cur = SEEK_CUR };
enum class mode : size_t { r = 0, w = 1, rp = 2, wp = 3, a = 4, ap = 5 };
constexpr array<const char *, 6> modechr = {"r", "w", "r+", "w+", "a", "a+"};

template <mode m>
constexpr bool readable_mode =
    m == mode::r || m == mode::rp || m == mode::wp || m == mode::ap;

template <mode m>
constexpr bool writable_mode = m == mode::w || m == mode::a || m == mode::rp ||
                               m == mode::wp || m == mode::ap;

enum class seek : int { set = SEEK_SET, cur = SEEK_CUR, end = SEEK_END };

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
  file(const char *path) : _fd(fopen(path, modechr[size_t(m)])) {}
  file(FILE *fd) : _fd(fd) {}
  file(const file &) = delete;
  file(file &&) = default;
  file &operator=(const file &) = delete;
  file &operator=(file &&) = default;

 public:
  bool opened() const { return _fd != nullptr; }

  void set(from fr, long offset) {
    if (_fd != nullptr) {
      fseek(_fd, offset, int(fr));
    }
  }

  size_t pos() { return _fd != nullptr ? ftell(_fd) : 0; }

 public:
  void push(T &&t)
    requires writable_mode<m>
  {
    if (_fd != nullptr) {
      fwrite(&t, sizeof(rm_cvref<T>), 1, _fd);
    }
  }

  void push(const T &t)
    requires writable_mode<m>
  {
    if (_fd != nullptr) {
      fwrite(&t, sizeof(rm_cvref<T>), 1, _fd);
    }
  }

  maybe<T> pop()
    requires readable_mode<m>
  {
    maybe<T> res;

    if (_fd != nullptr) {
      T buff;

      if (fread(&buff, sizeof(T), 1, _fd) == 1) {
        res = transfer(buff);
      }
    }

    return res;
  }
};

namespace io {

static auto stdr = file<char, mode::r>(stdin);
static auto stdw = file<char, mode::w>(stdout);

template <typename T, mode m>
inline size_t len(file<T, m> &f) {
  size_t cur = f.pos();
  f.set(from::end, 0L);
  size_t l = f.pos();
  f.set(from::start, cur);
  return l / sizeof(T);
}

template <typename T, mode m>
inline vector<T> readall(file<T, m> &f)
  requires readable_mode<m>
{
  size_t n = len(f);
  vector<T> content(n);
  maybe<T> c;

  while ((c = f.pop()).has()) {
    content.push(*c);
  }

  return content;
}

template <character C>
inline void print(string_view<C> s) {
  stdw.push(s);
}

template <character C>
inline void println(string_view<C> s) {
  stdw.push(s);
  stdw.push('\n');
}
template <character C, typename... T>
inline void printf(string_view<C> fmt, const T &...t) {
  format_to(stdw, fmt, t...);
}

template <character C, typename... T>
inline void printfln(string_view<C> fmt, const T &...t) {
  format_to(stdw, fmt, t...);
  stdw.push('\n');
}

}  // namespace io
}  // namespace stew

#endif
