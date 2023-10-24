#ifndef __n_io_hpp__
#define __n_io_hpp__

#include <stdio.h>
#include <core/string.hpp>

using file_descriptor = FILE*;

static constexpr file_descriptor null_file = nullptr;

enum class mode { none, r, w, a, cin, cout, cerr };

template <mode m>
static constexpr auto smode = "";

template <>
constexpr auto smode<mode::r> = "r";

template <>
constexpr auto smode<mode::w> = "w";

template <>
constexpr auto smode<mode::a> = "a";

template <>
constexpr auto smode<mode::cerr> = "";

template <>
constexpr auto smode<mode::cin> = "";

template <>
constexpr auto smode<mode::cout> = "";

template <mode m>
constexpr auto read_mode = m == mode::cin or m == mode::r;

template <mode m>
constexpr auto write_mode =
    m == mode::cout or m == mode::cerr or m == mode::w or m == mode::a;

template <mode m>
constexpr auto not_console_mode =
    m != mode::cerr and m != mode::cin and m != mode::cout;

template <mode m>
constexpr auto console_mode = not not_console_mode<m>;

template <mode m>
class raw_file {
 private:
  file_descriptor _fd = null_file;

 public:
  inline bool close()
    requires not_console_mode<m>
  {
    if (_fd != null_file and fclose(_fd) == 0) {
      _fd = null_file;
      return true;
    }

    return false;
  }

  bool open(file_descriptor fd)
    requires console_mode<m>
  {
    if (_fd == null_file) {
      _fd = fd;
      return true;
    }

    return false;
  }

  bool open(const char* fname)
    requires not_console_mode<m>
  {
    if (_fd != null_file) {
      close();
    }

    if (_fd == null_file) {
      _fd = fopen(fname, smode<m>);

      if (_fd != null_file) {
        return true;
      }
    }

    return false;
  }

  bool open(char_iterator auto fname)
    requires not_console_mode<m>
  {
    string s(100);
    copy(fname, s);
    s.add('\0');
    return open(s.data());
  }

  bool opened() { return _fd != null_file; }

 public:
  template <typename T>
  bool read(T& t)
    requires read_mode<m>
  {
    return _fd != null_file and fread(&t, sizeof(T), 1, _fd) == 1;
  }

  template <typename T>
  bool write(const T& t)
    requires write_mode<m>
  {
    return _fd != null_file and fwrite(&t, sizeof(T), 1, _fd) == 1;
  }

  bool readn(iterator auto i)
    requires read_mode<m>
  {
    using T = typename decltype(i)::type;
    bool res = true;

    while (i.has() and res) {
      res &= read<T>(i.get());

      if (res) {
        i.next();
      }
    }

    return res;
  }

  // bool writen(iterator auto i)
  //   requires write_mode<m>
  // {
  //   using T = typename decltype(i)::type;
  //   bool res = true;

  //   while (i.has() and res) {
  //     res &= write<T>(i.get());

  //     if (res) {
  //       i.next();
  //     }
  //   }

  //   return res;
  // }

  bool readn(output_stream auto o)
    requires read_mode<m>
  {
    using T = typename decltype(o)::type;
    bool res = true;

    while (res) {
      T tmp;
      res &= read<T>(tmp);

      if (res) {
        o.add(tmp);
      }
    }

    return res;
  }

  bool writen(input_stream auto i)
    requires write_mode<m>
  {
    using T = typename decltype(i)::type;
    bool res = true;

    while (i.has() and res) {
      res &= write<T>(i.next());
    }

    return res;
  }

  size_t len() {
    size_t len = 0;

    if (_fd != null_file) {
      auto pos = ftell(_fd);
      fseek(_fd, 0L, SEEK_END);
      len = ftell(_fd);
      fseek(_fd, pos, SEEK_SET);
    }

    return len;
  }
};

template <mode m>
class scoped_file : public raw_file<m> {
 private:
  bool close() { return raw_file<m>::close(); }

 public:
  ~scoped_file() {
    if constexpr (not_console_mode<m>) {
      if (raw_file<m>::opened()) {
        close();
      }
    }
  }

  scoped_file(file_descriptor fd)
    requires console_mode<m>
  {
    raw_file<m>::open(fd);
  }

  scoped_file(const char* name)
    requires not_console_mode<m>
  {
    raw_file<m>::open(name);
  }

  scoped_file(char_iterator auto name)
    requires not_console_mode<m>
  {
    raw_file<m>::open(name);
  }
};

template <typename T, mode m>
class typed_file : public raw_file<m> {
 public:
  typed_file(file_descriptor fd)
    requires console_mode<m>
  {
    raw_file<m>::open(fd);
  }

  typed_file(const char* name)
    requires not_console_mode<m>
  {
    raw_file<m>::open(name);
  }

  typed_file(char_iterator auto name)
    requires not_console_mode<m>
  {
    raw_file<m>::open(name);
  }

  size_t len() { return raw_file<m>::len() / sizeof(T); }
};

template <typename T, mode m>
class scoped_typed_file : public typed_file<T, m> {
 private:
  bool close() { return typed_file<T, m>::close(); }

 public:
  ~scoped_typed_file() {
    if constexpr (not_console_mode<m>) {
      if (typed_file<T, m>::opened()) {
        close();
      }
    }
  }

  scoped_typed_file(file_descriptor fd)
    requires console_mode<m>
      : typed_file<T, m>(fd) {}

  scoped_typed_file(const char* name)
    requires not_console_mode<m>
      : typed_file<T, m>(name) {}

  scoped_typed_file(char_iterator auto name)
    requires not_console_mode<m>
      : typed_file<T, m>(name) {}

  size_t len() { return typed_file<T, m>::len() / sizeof(T); }
};

template <typename T, mode m>
class basic_file_input_stream {
 public:
  using type = T;

 private:
  scoped_typed_file<T, m>& _f;
  size_t _pos;
  size_t _len;

 public:
  constexpr basic_file_input_stream(scoped_typed_file<T, m>& f)
    requires read_mode<m>
      : _f(f), _pos(0), _len(f.len()) {}

 public:
  constexpr bool has() const { return _f.opened() and _len != _pos; }

  constexpr T next() {
    _pos += 1;
    T tmp;
    _f.read(tmp);
    return tmp;
  }
};

template <typename T, mode m>
class basic_file_output_stream {
 public:
  using type = T;

 private:
  scoped_typed_file<T, m>& _f;

 public:
  basic_file_output_stream(scoped_typed_file<T, m>& f)
    requires write_mode<m>
      : _f(f) {}

 public:
  bool add(const T& t) { return _f.write(t); }
};

template <character C, mode m>
using text_file = scoped_typed_file<C, m>;

template <character C>
using read_text_file = text_file<C, mode::r>;

template <character C>
using write_text_file = text_file<C, mode::w>;

template <character C>
using append_text_file = text_file<C, mode::a>;

template <character C>
using cin_text_file = text_file<C, mode::cin>;

template <character C>
using cout_text_file = text_file<C, mode::cout>;

template <character C>
using cerr_text_file = text_file<C, mode::cerr>;

template <character C, mode m>
using text_file_input_stream = basic_file_input_stream<C, m>;

template <mode m>
using byte_file_input_stream = basic_file_input_stream<byte_t, m>;

template <character C, mode m>
using text_file_output_stream = basic_file_output_stream<C, m>;

template <mode m>
using byte_file_output_stream = basic_file_output_stream<byte_t, m>;

template <character C>
static auto ferr = cerr_text_file<C>(stderr);
static auto serr = text_file_output_stream(ferr<char>);
static auto wserr = text_file_output_stream(ferr<wchar_t>);

template <character C>
static auto fout = cout_text_file<C>(stdout);
static auto sout = text_file_output_stream(fout<char>);
static auto wsout = text_file_output_stream(fout<wchar_t>);

template <character C>
static auto fin = cin_text_file<C>(stdin);
static auto sin = text_file_input_stream(fin<char>);
static auto wsin = text_file_input_stream(fin<wchar_t>);

// TODO c'est surement pas le bon endroit pour mettre cette fonction
template <character C, mode m>
bool getline(string& s, basic_file_input_stream<C, m>& out) {
  C c;
  s.clear();
  while (out.has() and (c = out.next()) != '\n') {
    s.add(c);
  }

  return not s.empty();
}

#endif
