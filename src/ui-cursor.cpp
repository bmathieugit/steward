#include <ui-cursor.hpp>

namespace stew::ui
{
  cursor::cursor(std::ostream &os)
      : _os(os) {}

  void cursor::erase()
  {
    _os << "\033[2J";
  }

  void cursor::up(std::size_t n)
  {
    if (n != 0)
    {
      _os << "\033[" << n << 'A';
      _pos._y = _pos._y - n >= 0 ? _pos._y - n : 0;
    }
  }

  void cursor::down(std::size_t n)
  {
    if (n != 0)
    {
      _os << "\033[" << n << 'B';
      _pos._y = _pos._y + n;
    }
  }

  void cursor::left(std::size_t n)
  {
    if (n != 0)
    {
      _os << "\033[" << n << 'D';
      _pos._x = _pos._x - n >= 0 ? _pos._x - n : 0;
    }
  }

  void cursor::right(std::size_t n)
  {
    if (n != 0)
    {
      _os << "\033[" << n << 'C';
      _pos._x = _pos._x + n;
    }
  }

  void cursor::origin()
  {
    _pos._x = 0;
    _pos._y = 0;
    _os << "\033[H";
  }

  void cursor::at(position mpos)
  {
    origin();

    if (mpos._y != 0)
    {
      down(mpos._y);
    }

    if (mpos._x != 0)
    {
      right(mpos._x);
    }
  }

  void cursor::pushc(char c)
  {
    _os << c;
    _pos._x = _pos._x + 1;
  }

  void cursor::pushln()
  {
    _os << '\n';
    _pos._x = 0;
    _pos._y = _pos._y + 1;
  }

  void cursor::backred_mode()
  {
    _os << "\033[41m";
  }

  void cursor::reset_mode()
  {
    _os << "\033[0m";
  }

  position cursor::pos() const
  {
    return _pos;
  }
}