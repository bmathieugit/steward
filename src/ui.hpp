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
    int readc()
    {
      return getch();
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
