#ifndef __stew_ui_hpp__
#define __stew_ui_hpp__

#include <cstddef>
#include <iostream>
#include <string_view>
#include <string>
#include <tuple>
#include <variant>

#ifdef _WIN32
#include <conio.h>
#endif
#ifdef __unix__
#include <termios.h>
#endif

#include <format.hpp>
#include <event.hpp>

namespace stew::ui
{

  enum class arrow_event
  {
    UP,
    DOWN,
    RIGHT,
    LEFT
  };

  struct backspace_event
  {
  };

  struct enter_event
  {
  };

  using keyboard_event = std::variant<
      char,
      arrow_event,
      backspace_event,
      enter_event>;

  struct position
  {
    std::size_t _row = 0;
    std::size_t _col = 0;
  };

  using xy = position;

  template <std::size_t R, std::size_t C>
  class screen_reader
  {
    position &_pos;

  public:
    ~screen_reader() = default;
    screen_reader(position &pos) : _pos(pos) {}
    screen_reader(const screen_reader &) = default;
    screen_reader(screen_reader &&) = default;
    screen_reader &operator=(const screen_reader &) = default;
    screen_reader &operator=(screen_reader &&) = default;

  public:
    keyboard_event readc()
    {
      keyboard_event k;

      int c = nextch();

      if (c == '\033')
      {
        nextch();

        switch (nextch())
        {
        case 'A':
          return arrow_event::UP;
        case 'B':
          return arrow_event::DOWN;
        case 'C':
          return arrow_event::RIGHT;
        case 'D':
          return arrow_event::LEFT;
        }
      }
      else if (c == '\n')
      {
        return enter_event();
      }
      else if (c == 127)
      {
        return backspace_event();
      }

      return (char)c;
    }

  private:
    int nextch()
    {
#ifdef _WIN32
      return getch();
#endif
#ifdef __unix__
      termios oldt, newt;
      int ch;
      tcgetattr(STDIN_FILENO, &oldt);
      newt = oldt;
      newt.c_lflag &= ~(ICANON | ECHO);
      tcsetattr(STDIN_FILENO, TCSANOW, &newt);
      ch = getchar();
      tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
      return ch;
#endif
    }
  };

  template <std::size_t R, std::size_t C>
  class screen_writer
  {
    position &_pos;

  public:
    ~screen_writer() = default;
    screen_writer(position &pos) : _pos(pos) {}
    screen_writer(const screen_writer &) = default;
    screen_writer(screen_writer &&) = default;
    screen_writer &operator=(const screen_writer &) = default;
    screen_writer &operator=(screen_writer &&) = default;

  public:
    position write(char c)
    {
      if (c == '\n')
      {
        if (_pos._row < R)
        {
          _pos._col = 0;
          _pos._row = _pos._row + 1;
          std::cout << '\n';
        }
      }
      else
      {
        if (_pos._col < C)
        {
          _pos._col = _pos._col + 1;
          std::cout << c;
        }
      }

      return _pos;
    }

    position write_n(std::size_t n, char c = ' ')
    {
      for (std::size_t i(0); i < n; ++i)
      {
        write(c);
      }

      return _pos;
    }
  };

  template <std::size_t R, std::size_t C>
  class screen_move
  {
    position &_pos;

  public:
    ~screen_move() = default;
    screen_move(position &pos) : _pos(pos) {}
    screen_move(const screen_move &) = default;
    screen_move(screen_move &&) = default;
    screen_move &operator=(const screen_move &) = default;
    screen_move &operator=(screen_move &&) = default;

  public:
    void origin()
    {
      std::cout << "\033[H";
      _pos = position();
    }

    void at(position pos)
    {
      origin();

      if (pos._row)
      {
        std::cout << "\033[" << pos._row << 'B';
      }

      if (pos._col)
      {
        std::cout << "\033[" << pos._col << 'C';
      }

      _pos = pos;
    }

    void left()
    {
      if (_pos._col > 0)
      {
        _pos._col = _pos._col - 1;
        std::cout << "\033[D";
      }
    }

    void right()
    {
      if (_pos._col < C)
      {
        _pos._col = _pos._col + 1;
        std::cout << "\033[C";
      }
    }

    void up()
    {
      if (_pos._row > 0)
      {
        _pos._row = _pos._row - 1;
        std::cout << "\033[A";
      }
    }

    void down()
    {
      if (_pos._row < R)
      {
        _pos._row = _pos._row + 1;
        std::cout << "\033[B";
      }
    }
  };

  template <std::size_t R, std::size_t C>
  class screen
  {
    position _pos;

    screen_move<R, C> _move = _pos;
    screen_writer<R, C> _writer = _pos;
    screen_reader<R, C> _reader = _pos;

  public:
    ~screen() { close(); }
    screen() { open(); }
    screen(const screen &) = default;
    screen(screen &&) = default;
    screen &operator=(const screen &) = default;
    screen &operator=(screen &&) = default;

  public:
    screen_move<R, C> &move()
    {
      return _move;
    }

    screen_writer<R, C> &writer()
    {
      return _writer;
    }

    screen_reader<R, C> &reader()
    {
      return _reader;
    }

    position pos()
    {
      return _pos;
    }

  private:
    void open()
    {
      savec();
      saves();
      erase();
      _move.origin();
    }

    void close()
    {
      restores();
      restorec();
    }

  public:
    void erase()
    {
      std::cout << "\033[2J";
    }

    void savec()
    {
      std::cout << "\033[s";
    }

    void saves()
    {
      std::cout << "\033[?47h";
    }

    void restorec()
    {
      std::cout << "\033[u";
    }

    void restores()
    {
      std::cout << "\033[?47l";
    }
  };
}

#endif
