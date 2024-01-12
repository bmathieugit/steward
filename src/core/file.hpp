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
constexpr bool read_mode = m == mode::cin or m == mode::r;

template <mode m>
constexpr bool write_mode =
    m == mode::cout or m == mode::cerr or m == mode::w or m == mode::a;

template <mode m>
constexpr bool not_console_mode =
    m != mode::cerr and m != mode::cin and m != mode::cout;

template <mode m>
constexpr bool console_mode = not not_console_mode<m>;

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

  raw_file(string_view fname)
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

  bool open(string_view fname)
    requires not_console_mode<m>
  {
    return open(fname.data());
  }

  bool opened() { return _fd != null_file; }

 public:
  template <typename T>
  bool read(T& t)
    requires read_mode<m>
  {
    return opened() and fread(&t, sizeof(T), 1, _fd) == 1;
  }

  bool readall(oterator auto os)
    requires read_mode<m>
  {
    using T = typename decltype(os)::type;

    bool isread = true;

    while (isread) {
      T buff;
      isread &= read(buff);

      if (isread) {
        if (not os.sext(buff)) {
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
    return opened() and fwrite(&t, sizeof(T), 1, _fd) == 1;
  }

  bool writeall(iterator auto it)
    requires write_mode<m>
  {
    using T = typename decltype(it)::type;
    bool res = true;

    while (it.has_next() and res) {
      res &= write<T>(it.next());
    }

    return res;
  }

  size_t len() {
    size_t len = 0;

    if (opened()) {
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

  file(string_view name)
    requires not_console_mode<m>
      : raw_file<m>(name) {}

  size_t len() { return raw_file<m>::len() / sizeof(T); }
};

template <typename T, mode m>
  requires read_mode<m> and not_console_mode<m>
class file_iterator {
 public:
  using type = T;

 private:
  file<T, m>& _f;
  size_t _pos;
  size_t _len;

 public:
  constexpr file_iterator(file<T, m>& f) : _f(f), _pos(0), _len(f.len()) {}

 public:
  constexpr bool has_next() const { return _f.opened() and _pos != _len; }
  constexpr type next() {
    _pos += 1;
    type tmp;
    _f.read(tmp);
    return tmp;
  }
};

template <character C, mode m>
  requires(m == mode::cin)
class cin_iterator {
 public:
  using type = C;

 private:
  file<C, m>& _f;
  mutable C _c;

 public:
  constexpr cin_iterator(file<C, m>& f) : _f(f) {}

 public:
  constexpr bool has_next() const {
    return _f.opened() and _f.read(_c) and _c != '\n';
  }

  constexpr auto next() { return _c; }
};

template <typename T, mode m>
  requires write_mode<m>
class file_oterator {
 public:
  using type = T;

 private:
  file<T, m>& _f;

 public:
  file_oterator(file<T, m>& f) : _f(f) {}

 public:
  bool sext(const type& t) { return _f.write(t); }
  bool sext(type&& t) { return _f.write(move(t)); }
};

template <character C>
static auto __ferr = file<C, mode::cerr>(stderr);
static auto serr = file_oterator(__ferr<char>);
static auto wserr = file_oterator(__ferr<wchar_t>);

template <character C>
static auto __fout = file<C, mode::cout>(stdout);
static auto sout = file_oterator(__fout<char>);
static auto wsout = file_oterator(__fout<wchar_t>);

template <character C>
static auto __fin = file<C, mode::cin>(stdin);
static auto sin = cin_iterator(__fin<char>);
static auto wsin = cin_iterator(__fin<wchar_t>);

#endif
