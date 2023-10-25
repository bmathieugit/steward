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
  ~raw_file() {
    if constexpr (not_console_mode<m>) {
      close();
    }
  }

  raw_file(file_descriptor fd)
    requires console_mode<m>
      : _fd(fd) {}

  raw_file(const char* fname)
    requires not_console_mode<m>
  {
    open(fname);
  }

  raw_file(char_input_stream auto fname)
    requires not_console_mode<m>
  {
    open(fname);
  }

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

  bool open(char_input_stream auto fname)
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

  bool readall(output_stream auto os)
    requires read_mode<m>
  {
    using T = typename decltype(os)::type;

    bool isread = true;

    while (isread) {
      T buff;
      isread &= read(buff);

      if (isread) {
        if (not os.add(buff)) {
          break;
        }
      }
    }

    return isread;
  }

  template <typename T>
  bool write(const T& t)
    requires write_mode<m>
  {
    return _fd != null_file and fwrite(&t, sizeof(T), 1, _fd) == 1;
  }

  bool writeall(input_stream auto is)
    requires write_mode<m>
  {
    using T = typename decltype(is)::type;
    bool res = true;

    while (is.has() and res) {
      res &= write<T>(is.next());
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

template <typename T, mode m>
class file : public raw_file<m> {
 public:
  using type = T;

 public:
  file(file_descriptor fd)
    requires console_mode<m>
      : raw_file<m>(fd) {}

  file(const char* name)
    requires not_console_mode<m>
      : raw_file<m>(name) {}

  file(char_input_stream auto name)
    requires not_console_mode<m>
      : raw_file<m>(name) {}

  size_t len() { return raw_file<m>::len() / sizeof(T); }
};

template <typename T, mode m>
  requires read_mode<m>
class file_input_stream {
 public:
  using type = T;

 private:
  file<T, m>& _f;
  size_t _pos;
  size_t _len;

 public:
  constexpr file_input_stream(file<T, m>& f) : _f(f), _pos(0), _len(f.len()) {}

 public:
  constexpr bool has() const { return _f.opened() and _len != _pos; }

  constexpr type next() {
    _pos += 1;
    type tmp;
    _f.read(tmp);
    return tmp;
  }
};

template <typename T, mode m>
constexpr auto istream(file<T, m>& f) {
  return file_input_stream(f);
}

template <typename T, mode m>
  requires write_mode<m>
class file_output_stream {
 public:
  using type = T;

 private:
  file<T, m>& _f;

 public:
  file_output_stream(file<T, m>& f) : _f(f) {}

 public:
  bool add(const type& t) { return _f.write(t); }
};

template <typename T, mode m>
constexpr auto ostream(file<T, m>& f) {
  return file_output_stream(f);
}

template <character C>
static auto ferr = file<C, mode::cerr>(stderr);
static auto serr = ostream(ferr<char>);
static auto wserr = ostream(ferr<wchar_t>);

template <character C>
static auto fout = file<C, mode::cout>(stdout);
static auto sout = ostream(fout<char>);
static auto wsout = ostream(fout<wchar_t>);

template <character C>
static auto fin = file<C, mode::cin>(stdin);
static auto sin = istream(fin<char>);
static auto wsin = istream(fin<wchar_t>);

#endif
