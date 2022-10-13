#ifndef __stew_ui_hpp__
#define __stew_ui_hpp__

#include <cstddef>
#include <string_view>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <optional>
#include <tuple>

#include <format.hpp>
#include <event.hpp>

int getch();

namespace std
{
  template <typename T>
  using ptr = std::unique_ptr<T>;
}

namespace stew::ui
{
  enum class keyboard : int
  {
    NUL = 0,
    SOH,
    STX,
    ETX,
    EOT,
    ENQ,
    ACK,
    BEL,
    BS,
    TAB,
    LF,
    VT,
    FF,
    CR,
    SO,
    SI,
    DLE,
    DC1,
    DC2,
    DC3,
    DC4,
    NAK,
    SYN,
    ETB,
    CAN,
    EM,
    SUB,
    ESC,
    FS,
    GS,
    RS,
    US,
    SPACE,
    DEL = 127
  };
}

namespace stew
{
  struct position
  {
    std::size_t _row = 0;
    std::size_t _col = 0;
  };

  using xy = position;
}

namespace stew
{
  template <
      typename T,
      std::size_t R,
      std::size_t C>
  class matrix
  {
    std::array<std::array<T, C>, R> _data;

  public:
    matrix() = default;
    matrix(const matrix &) = default;
    matrix(matrix &&) = default;
    matrix &operator=(const matrix &) = default;
    matrix &operator=(matrix &&) = default;

  public:
    std::size_t rows() const;
    std::size_t cols() const;

    T &operator[](position pos);
    const T &operator[](position pos) const;
  };

  template <typename T, std::size_t R, std::size_t C>
  std::size_t matrix<T, R, C>::rows() const
  {
    return R;
  }

  template <typename T, std::size_t R, std::size_t C>
  std::size_t matrix<T, R, C>::cols() const
  {
    return C;
  }

  template <typename T, std::size_t R, std::size_t C>
  T &matrix<T, R, C>::operator[](position pos)
  {
    return _data[pos._row][pos._col];
  }

  template <typename T, std::size_t R, std::size_t C>
  const T &matrix<T, R, C>::operator[](position pos) const
  {
    return _data[pos._row][pos._col];
  }
}

namespace stew::ui
{
  struct screen_cell
  {
    char _c = ' ';
    std::optional<std::vector<std::string>> _style;
  };

  template <std::size_t R, std::size_t C>
  class screen_writer
  {
    position _pos = {0, 0};
    matrix<screen_cell, R, C> _data;

  public:
    ~screen_writer() = default;
    screen_writer() = default;
    screen_writer(const screen_writer &) = default;
    screen_writer(screen_writer &&) = default;
    screen_writer &operator=(const screen_writer &) = default;
    screen_writer &operator=(screen_writer &&) = default;

  public:
    position write(char c);
    //  position style(std::string_view sty);
    void render();
  };

  template <std::size_t R, std::size_t C>
  position screen_writer<R, C>::write(char c)
  {
    if (c == '\n')
    {
      if (_pos._row < R)
      {
        _pos._col = 0;
        _pos._row = _pos._row + 1;
      }
    }
    else
    {
      if (_pos._col < C)
      {
        _data[_pos]._c = c;
        _pos._col = _pos._col + 1;
      }
    }

    return _pos;
  }

  template <std::size_t R, std::size_t C>
  void screen_writer<R, C>::render()
  {
    for (std::size_t r(0); r < R; ++r)
    {
      for (std::size_t c(0); c < C; ++c)
      {
        std::cout << _data[xy{r, c}]._c;
      }

      std::cout << '\n';
    }
  }

  // template <std::size_t R, std::size_t C>
  // screen<R, C>::screen()
  // {
  //   std::cout << "\033[s";
  //   std::cout << "\033[?47h";
  // }

  // template <std::size_t R, std::size_t C>
  // screen<R, C>::~screen()
  // {
  //   std::cout << "\033[?47l";
  //   std::cout << "\033[u";
  // }

  template <std::size_t R, std::size_t C>
  class screen_move
  {
    position _pos = {0, 0};

  public:
    ~screen_move() = default;
    screen_move() = default;
    screen_move(const screen_move &) = default;
    screen_move(screen_move &&) = default;
    screen_move &operator=(const screen_move &) = default;
    screen_move &operator=(screen_move &&) = default;

  public:
    position left();
    position right();
    position up();
    position down();
    position origin();
    position at(position pos);
  };

  template <std::size_t R, std::size_t C>
  position screen_move<R, C>::left()
  {
    if (_pos._col > 0)
    {
      _pos._col = _pos._col - 1;
    }

    std::cout << "\033[D";
    return _pos;
  }

  template <std::size_t R, std::size_t C>
  position screen_move<R, C>::right()
  {
    if (_pos._col > 0)
    {
      _pos._col = _pos._col + 1;
    }

    std::cout << "\033[C";
    return _pos;
  }

  template <std::size_t R, std::size_t C>
  void screen_move<R, C>::up()
  {
    if (_pos._row > 0)
    {
      _pos._row = _pos._row - 1;
    }

    std::cout << "\033[A";
  }

  template <std::size_t R, std::size_t C>
  void screen_move<R, C>::down()
  {
    if (_pos._row > 0)
    {
      _pos._row = _pos._row + 1;
    }

    std::cout << "\033[B";
  }

  template <std::size_t R, std::size_t C>
  void screen_move<R, C>::origin()
  {
    _pos._col = 0;
    _pos._row = 0;
    std::cout << "\033[H";
  }

  template <std::size_t R, std::size_t C>
  void screen_move<R, C>::at(position pos)
  {
    _pos = pos;

    origin();

    if (pos._row)
    {
      std::cout << "\033[" << pos._row << 'B';
    }

    if (pos._col)
    {
      std::cout << "\033[" << pos._col << 'C';
    }
  
  template <std::size_t R, std::size_t C>
  position screen<R, C>::pos()
  {
    return _pos;
  }

  template <std::size_t R, std::size_t C>
  void screen<R, C>::render()
  {
    std::cout << "\033[2J";

    for (std::size_t r(0); r < R; ++r)
    {
      for (std::size_t c(0); c < C; ++c)
      {

        std::cout << _data[xy{r, c}]._c;
      }
    }
  }
}

namespace stew::ui
{
  enum class style_text_mode
  {
    back_black,
    back_red,
    back_green,
    back_yellow,
    back_blue,
    back_magenta,
    back_cyan,
    back_white,
    back_default,
    fore_black,
    fore_red,
    fore_green,
    fore_yellow,
    fore_blue,
    fore_magenta,
    fore_cyan,
    fore_white,
    fore_default,
    reset
  };

}

namespace stew::ui
{
  class pencil
  {
    screen &_scr;

  public:
    pencil(screen &scr);

  public:
    position text(std::string_view txt);
  };

  pencil::pencil(screen &scr)
      : _scr(scr) {}

  position pencil::text(std::string_view txt)
  {
    for (char c : txt)
    {
      _scr.write(c);
    }
  }
}

namespace stew::ui
{
  template <typename T>
  concept display_component = requires(T &t, screen &scr)
  {
    t.render(scr);
  };

  template <typename T>
  concept input_component = requires(T &t, screen &scr)
  {
    t.collect(scr);
  };

  template <typename T>
  concept notify_component = requires(T &t, message &mess)
  {
    t.notify(mess);
  };

  class text_field
  {
    std::string _label;
    position _vpos;

  public:
    ~text_field() = default;
    text_field() = default;
    text_field(const text_field &) = default;
    text_field(text_field &&) = default;
    text_field &operator=(const text_field &) = default;
    text_field &operator=(text_field &&) = default;

  public:
    void render(screen &scr);
    void notify(const std::pair<std::string, std::string> &v);
    std::pair<std::string, std::string> collect(screen &scr);
  };

  void text_field::render(screen &scr)
  {
    scr.write(std::format("  + {} : ", _label))
        _vpos = scr.pos();
    scr.write("\n");
  }

}

#endif
