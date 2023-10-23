#ifndef __n_io_hpp__
#define __n_io_hpp__

#include <stdio.h>

#include <core/char-istream.hpp>
#include <core/char-ostream.hpp>
#include <core/result.hpp>
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

enum class mode { r, w, a, cin, cout, cerr };

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

template <typename T, mode m>
class file {
 private:
  file_descriptor _fd = null_file;

 public:
  ~file() {
    if constexpr (not_console_mode<m>) {
      fclose(_fd);
      _fd = nullptr;
    }
  }

  file(file_descriptor fd)
    requires console_mode<m>
      : _fd(fd) {}

  file(const char* name)
    requires not_console_mode<m>
      : _fd(fopen(name, smode<m>)) {}

  file(char_iterator auto name)
    requires not_console_mode<m>
      : _fd(fopen(name, smode<m>)) {}

  bool opened() { return _fd != null_file; }

  file_descriptor fd() { return _fd; }
};

template <typename T, mode m>
constexpr size_t flength(file<T, m>& f) {
  size_t len = 0;

  if (f.fd() != null_file) {
    auto pos = ftell(f.fd());
    fseek(f.fd(), 0L, SEEK_END);
    len = ftell(f.fd()) / sizeof(T);
    fseek(f.fd(), pos, SEEK_SET);
  }

  return len;
}

template <typename T, mode m>
class basic_file_input_stream {
 public:
  using type = T;

 private:
  file<T, m>& _f;
  size_t _pos;
  size_t _len;

 public:
  constexpr basic_file_input_stream(file<T, m>& f)
    requires read_mode<m>
      : _f(f), _pos(0), _len(flength(f)) {}

 public:
  constexpr bool has() const { return _f.fd() != null_file and _len != _pos; }

  constexpr T next() {
    _pos += 1;
    byte_t buf[sizeof(T)];
    fread(&buf, 1, sizeof(T), _f.fd());
    return *reinterpret_cast<T*>(buf);
  }
};

template <typename T, mode m>
class basic_file_output_stream {
 public:
  using type = T;

 private:
  file<T, m>& _f;

 public:
  basic_file_output_stream(file<T, m>& f)
    requires write_mode<m>
      : _f(f) {}

 public:
  bool add(const T& t) {
    return _f.fd() != null_file and fwrite(&t, sizeof(T), 1, _f.fd()) == 1;
  }
};

template <character C, mode m>
using text_file = file<C, m>;

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

#endif
