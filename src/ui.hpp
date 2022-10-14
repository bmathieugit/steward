#ifndef __stew_ui_hpp__
#define __stew_ui_hpp__

#include <ncurses.h>
#include <string_view>
#include <cstddef>

namespace ncurses
{
  using WINDOW = ::WINDOW;

  WINDOW *initscr()
  {
    return ::initscr();
  }

  int cbreak()
  {
    return ::cbreak();
  }

  int keypad(WINDOW *win, bool val)
  {
    return ::keypad(::stdscr, true);
  }

  int noecho()
  {
    return ::noecho();
  }

  int echo()
  {
    return ::echo();
  }

  int endwin()
  {
    return ::endwin();
  }

  int refresh()
  {
    return ::refresh();
  }

  int curx()
  {
    return getcurx(stdscr);
  }

  int cury()
  {
    return getcury(stdscr);
  }

  int move(int x, int y)
  {
    return ::move(x, y);
  }

  template <typename... T>
  int prints(const char *pattern, T &&...t)
  {
    return ::printw(pattern, std::forward<T>(t)...);
  }

  int printc(char c)
  {
    return ::addch(c);
  }
}

namespace stew::ui
{
  struct position
  {
    int _row;
    int _col;
  };

  class screen
  {
  public:
    void init();
    void close();
    void refresh();

  public:
    void move(position pos);
    position where();

    void echo_on();
    void echo_off();

  public:
    void write(char c);
    void write(std::string_view str);
  };

  void screen::init()
  {
    // https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/init.html
    ncurses::initscr();
    ncurses::cbreak();
    ncurses::keypad(::stdscr, true);
    ncurses::noecho();
  }

  void screen::close()
  {
    ncurses::endwin();
  }

  void screen::refresh()
  {
    ncurses::refresh();
  }

  void screen::move(position at)
  {
    // https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/printw.html
    ncurses::move(at._row, at._col);
  }

  position screen::where()
  {
    // https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/screen.html

    return position{
        ncurses::curx(),
        ncurses::cury()};
  }

  void screen::echo_on()
  {
    ncurses::echo();
  }
  
  void screen::echo_off()
  {
    ncurses::noecho();
  }

  void screen::write(char c)
  {
    // https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/printw.html
    ncurses::printc(c);
  }

  void screen::write(std::string_view str)
  {
    // https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/printw.html
    ncurses::prints(str.data());
  }
}

#endif