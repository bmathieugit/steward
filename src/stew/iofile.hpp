#ifndef __steward_iofile_hpp__
#define __steward_iofile_hpp__

#include <stdio.h>
#include <stew/meta.hpp>
#include <stew/smarts.hpp>
#include <stew/maybe.hpp>
#include <stew/iterator.hpp>
#include <stew/array.hpp>
#include <stew/string.hpp>
#include <stew/format.hpp>

namespace stew {

enum class mode : size_t { r = 0, w = 1, rp = 2, wp = 3, a = 4, ap = 5 };

template <mode m>
constexpr bool readable_mode =
    m == mode::r || m == mode::rp || m == mode::wp || m == mode::ap;

template <mode m>
constexpr bool writable_mode = m == mode::w || m == mode::a || m == mode::rp ||
                               m == mode::wp || m == mode::ap;

enum class seek : int { set = SEEK_SET, cur = SEEK_CUR, end = SEEK_END };

template <typename T, mode m>
class file;

template <typename T>
class file_reader {
 private:
  non_owning<FILE> _file;

 public:
  template <mode m>
    requires readable_mode<m>
  file_reader(file<T, m> &f) : _file(f.fd()) {}

  file_reader() = default;

 public:
  class ifile_iterator {
   private:
    non_owning<file_reader> _reader = nullptr;
    maybe<T> _value;

   public:
    ~ifile_iterator() = default;
    ifile_iterator() = default;
    ifile_iterator(file_reader &f) : _reader(&f) { ++(*this); }

    ifile_iterator(const ifile_iterator &) = default;
    ifile_iterator(ifile_iterator &&) = default;
    ifile_iterator &operator=(const ifile_iterator &) = default;
    ifile_iterator &operator=(ifile_iterator &&) = default;

   public:
    auto operator==(const ifile_iterator &o) const {
      return _reader.get() == o._reader.get();
    }

    maybe<T> &operator*() { return _value; }

    ifile_iterator &operator++() {
      if (_reader.get() != nullptr) {
        _value = _reader.get()->pop();

        if (!_value.has()) {
          _reader = nullptr;
        }
      }

      return *this;
    }

    ifile_iterator &operator++(int) { return ++(*this); }
  };

 public:
  auto begin() { return ifile_iterator(*this); }

  auto end() { return ifile_iterator(); }

  auto begin() const { return ifile_iterator(*this); }

  auto end() const { return ifile_iterator(); }

 public:
  maybe<T> pop() {
    maybe<T> res;

    if (_file.get() != nullptr) {
      T buff;

      if (fread(&buff, sizeof(T), 1, _file.get()) == 1) {
        res = transfer(buff);
      }
    }

    return res;
  }

  template <push_container<T> R>
  void pop(R &r, size_t n = size_t(-1)) {
    if (_file.get() != nullptr) {
      T buff;

      for (size_t i : upto(size_t(0), n)) {
        if (fread(&buff, sizeof(T), 1, _file.get()) == 1) {
          r.push(transfer(buff));
        } else {
          break;
        }
      }
    }
  }
};

template <typename T>
class file_writer {
 private:
  non_owning<FILE> _file = nullptr;

 public:
  template <mode m>
    requires writable_mode<m>
  file_writer(file<T, m> &f) : _file(f.fd()) {}

  file_writer() = default;

 public:
  class ofile_iterator {
   private:
    non_owning<file_writer> _writer = nullptr;

   public:
    ~ofile_iterator() = default;
    ofile_iterator() = default;
    ofile_iterator(file_writer &f) : _writer(&f) {}
    ofile_iterator(const ofile_iterator &) = default;
    ofile_iterator(ofile_iterator &&) = default;
    ofile_iterator &operator=(const ofile_iterator &) = default;
    ofile_iterator &operator=(ofile_iterator &&) = default;

   public:
    ofile_iterator &operator*() { return *this; }

    ofile_iterator &operator++() { return *this; }

    ofile_iterator &operator++(int) { return *this; }

    ofile_iterator &operator=(const T &t) {
      if (_writer.get() != nullptr) {
        _writer.get()->push(t);
      }
    }

    ofile_iterator &operator=(T &&t) {
      if (_writer.get() != nullptr) {
        _writer.get()->push(t);
      }
    }
  };

 public:
  auto begin() { return ofile_iterator(*this); }

  auto end() { return ofile_iterator(); }

 public:
  void push(T &&t) {
    if (_file.get() != nullptr) {
      fwrite(&t, sizeof(rm_cvref<T>), 1, _file.get());
    }
  }

  void push(const T &t) {
    if (_file.get() != nullptr) {
      fwrite(&t, sizeof(rm_cvref<T>), 1, _file.get());
    }
  }

  template <input_range R>
  void push(R &&r) {
    for (auto &&i : relay<R>(r)) {
      push(relay<decltype(i)>(i));
    }
  }
};

enum class from : int { start = SEEK_SET, end = SEEK_END, cur = SEEK_CUR };

template <typename T, mode m>
class file {
 private:
  FILE *_fp = nullptr;
  static constexpr array<const char *, 6> modechr = {"r",  "w", "r+",
                                                     "w+", "a", "a+"};

 public:
  ~file() { close(); }
  file() = default;

  template <character C>
  file(string_view<C> path) : _fp(fopen(path.begin(), modechr[size_t(m)])) {}

  file(FILE *fp) : _fp(fp) {}
  file(const file &) = delete;
  file(file &&) = default;
  file &operator=(const file &) = delete;
  file &operator=(file &&) = default;

 public:
  bool opened() { return _fp != nullptr; }

 public:
  void set(from frm, long offset) {
    if (_fp != nullptr) {
      fseek(_fp, offset, (int)frm);
    }
  }

  size_t pos() { return _fp != nullptr ? ftell(_fp) : 0; }

  file_reader<T> reader()
    requires(readable_mode<m>)
  {
    return file_reader<T>(*this);
  }

  file_writer<T> writer()
    requires(writable_mode<m>)
  {
    return file_writer<T>(*this);
  }

  auto fd() { return _fp; }

 private:
  void close() {
    if (_fp != nullptr) {
      fflush(_fp);
      fclose(_fp);
      _fp = nullptr;
    }
  }
};

namespace io {

static auto stdfin = file<char, mode::r>(stdin);
static auto stdfout = file<char, mode::w>(stdout);
static auto stdfr = stdfin.reader();
static auto stdfw = stdfout.writer();

template <typename T, mode m>
inline size_t len(file<T, m> &f) {
  size_t cur = f.pos();
  f.set(from::end, 0L);
  size_t l = f.pos();
  f.set(from::start, cur);
  return l / sizeof(T);
}

template <typename T, mode m>
inline fixed_vector<T> readall(file<T, m> &f) {
  size_t n = len(f);
  fixed_vector<T> content(n);
  f.reader().pop(content, n);
  return content;
}

template <character C, typename... T>
inline void printf(string_view<C> fmt, const T &...t) {
  format_to(stdfw, fmt, t...);
}

template <character C, typename... T>
inline void printfln(string_view<C> fmt, const T &...t) {
  format_to(stdfw, fmt, t...);
  stdfw.push('\n');
}

template <character C>
inline void print(string_view<C> s) {
  stdfw.push(s);
}

template <character C>
inline void println(string_view<C> s) {
  stdfw.push(s);
  stdfw.push('\n');
}

}  // namespace io

}  // namespace stew

#endif
