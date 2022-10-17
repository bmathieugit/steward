#ifndef __stew_ui_hpp__
#define __stew_ui_hpp__

#include <cstddef>
#include <iostream>
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
  template <std::size_t R, std::size_t C>
  class screen_command
  {
  public:
    ~screen_command() = default;
    screen_command() = default;
    screen_command(const screen_command &) = delete;
    screen_command(screen_command &&) = delete;
    screen_command &operator=(const screen_command &) = delete;
    screen_command &operator=(screen_command &&) = delete;

  public:
    void erase();
    void savec();
    void saves();
    void restorec();
    void restores();
  };

  template <std::size_t R, std::size_t C>
  void screen_command<R, C>::erase()
  {
    std::cout << "\033[2J";
  }

  template <std::size_t R, std::size_t C>
  void screen_command<R, C>::savec()
  {
    std::cout << "\033[s";
  }

  template <std::size_t R, std::size_t C>
  void screen_command<R, C>::saves()
  {
    std::cout << "\033[?47h";
  }

  template <std::size_t R, std::size_t C>
  void screen_command<R, C>::restorec()
  {
    std::cout << "\033[u";
  }

  template <std::size_t R, std::size_t C>
  void screen_command<R, C>::restores()
  {
    std::cout << "\033[?47l";
  }

  template <std::size_t R, std::size_t C>
  class screen_move
  {
    position _pos = {0, 0};

  public:
    ~screen_move() = default;
    screen_move() = default;
    screen_move(const screen_move &) = delete;
    screen_move(screen_move &&) = delete;
    screen_move &operator=(const screen_move &) = delete;
    screen_move &operator=(screen_move &&) = delete;

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
  position screen_move<R, C>::up()
  {
    if (_pos._row > 0)
    {
      _pos._row = _pos._row - 1;
    }

    std::cout << "\033[A";
    return _pos;
  }

  template <std::size_t R, std::size_t C>
  position screen_move<R, C>::down()
  {
    if (_pos._row > 0)
    {
      _pos._row = _pos._row + 1;
    }

    std::cout << "\033[B";
    return _pos;
  }

  template <std::size_t R, std::size_t C>
  position screen_move<R, C>::origin()
  {
    _pos._col = 0;
    _pos._row = 0;
    std::cout << "\033[H";
    return _pos;
  }

  template <std::size_t R, std::size_t C>
  position screen_move<R, C>::at(position pos)
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

    return _pos;
  }

  template <std::size_t R, std::size_t C>
  class screen_init
  {
  public:
    ~screen_init();
    screen_init();
    screen_init(const screen_init &) = delete;
    screen_init(screen_init &&) = delete;
    screen_init &operator=(const screen_init &) = delete;
    screen_init &operator=(screen_init &&) = delete;
  };

  template <std::size_t R, std::size_t C>
  screen_init<R, C>::screen_init()
  {
    screen_command<R, C> sc;
    sc.savec();
    sc.saves();
    sc.erase();
    screen_move<R, C>().origin();
  }

  template <std::size_t R, std::size_t C>
  screen_init<R, C>::~screen_init()
  {
    screen_command<R, C> sc;
    sc.restores();
    sc.restorec();
  }

  enum class screen_cell_style
  {

  };

  struct screen_cell
  {
    char _c = ' ';
    std::optional<std::vector<screen_cell_style>> _style;
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
    position pos();
  };

  template <std::size_t R, std::size_t C>
  position screen_writer<R, C>::pos()
  {
    return _pos;
  }

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
    screen_move<R, C>().origin();

    for (std::size_t r(0); r < R; ++r)
    {
      for (std::size_t c(0); c < C; ++c)
      {
        std::cout << _data[xy{r, c}]._c;
      }

      std::cout << '\n';
    }
  }

  template <std::size_t R, std::size_t C>
  class screen_collector
  {
  public:
    ~screen_collector() = default;
    screen_collector() = default;
    screen_collector(const screen_collector &) = delete;
    screen_collector(screen_collector &&) = delete;
    screen_collector &operator=(const screen_collector &) = delete;
    screen_collector &operator=(screen_collector &&) = delete;

  public:
    int readc();
  };

  template <std::size_t R, std::size_t C>
  int screen_collector<R, C>::readc()
  {
    return getch();
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
  template <typename T, std::size_t R, std::size_t C>
  concept display_component = requires(T &t, screen_writer<R, C> &scr)
  {
    t.render(scr);
  };

  template <typename T, std::size_t R, std::size_t C>
  concept collect_component = requires(T &t, screen_collector<R, C> &scr)
  {
    t.collect(scr);
  };

  template <typename T>
  concept notify_component = requires(T &t, message &mess)
  {
    t.notify(mess);
  };
}

namespace stew::ui
{
  template <typename... COMP>
  class view
  {
    std::tuple<COMP...> _components;

  public:
    ~view() = default;
    view(const COMP &...comps);
    view(const view &) = default;
    view(view &&) = default;
    view &operator=(const view &) = default;
    view &operator=(view &&) = default;

  public:
    template <std::size_t R, std::size_t C>
    void render(screen_writer<R, C> &scr);

    template <std::size_t R, std::size_t C>
    void collect(screen_collector<R, C> &scr);
  };

  template <typename... COMP>
  view<COMP...>::view(const COMP &...comp) : _components(comp...) {}

  template <typename... COMP>
  template <std::size_t R, std::size_t C>
  void view<COMP...>::render(screen_writer<R, C> &scr)
  {
    constexpr auto render_one = [](auto &comp, screen_writer<R, C> &scr)
    {
      if constexpr (display_component<std::decay_t<decltype(comp)>, R, C>)
      {
        comp.render(scr);
      }
    };

    screen_command<R, C>().erase();
    screen_move<R, C>().origin();

    std::apply([&scr, render_one](COMP &...comp)
               { (render_one(comp, scr), ...); },
               _components);
  }

  template <typename... COMP>
  template <std::size_t R, std::size_t C>
  void view<COMP...>::collect(screen_collector<R, C> &scr)
  {
    constexpr auto collect_one = [](auto &comp, screen_collector<R, C> &scr) {

    };
  }

  template <typename... COMP>
  constexpr view<std::decay_t<COMP>...> make_view(const COMP &...comp)
  {
    return view<std::decay_t<COMP>...>(comp...);
  }
}

#endif
