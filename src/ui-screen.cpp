#include <ui-screen.hpp>

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
      _pos.y = _pos.y - n >= 0 ? _pos.y - n : 0;
    }
  }

  void cursor::down(std::size_t n)
  {
    if (n != 0)
    {
      _os << "\033[" << n << 'B';
      _pos.y = _pos.y + n;
    }
  }

  void cursor::left(std::size_t n)
  {
    if (n != 0)
    {
      _os << "\033[" << n << 'D';
      _pos.x = _pos.x - n >= 0 ? _pos.x - n : 0;
    }
  }

  void cursor::right(std::size_t n)
  {
    if (n != 0)
    {
      _os << "\033[" << n << 'C';
      _pos.x = _pos.x + n;
    }
  }

  void cursor::origin()
  {
    _pos.x = 0;
    _pos.y = 0;
    _os << "\033[H";
  }

  void cursor::at(marker_position mpos)
  {
    origin();

    if (mpos.y != 0)
    {
      down(mpos.y);
    }

    if (mpos.x != 0)
    {
      right(mpos.x);
    }
  }

  void cursor::pushc(char c)
  {
    _os << c;
    _pos.x = _pos.x + 1;
  }

  void cursor::pushln()
  {
    _os << '\n';
    _pos.x = 0;
    _pos.y = _pos.y + 1;
  }

  const std::vector<screen_marker> &screen::markers() const
  {
    return _markers;
  }

  void screen::println()
  {
    _data.push_back("");
  }

  void screen::print(char c)
  {
    if (_data.empty())
      println();

    _data.back().push_back(c);
  }

  void screen::print_marker(const std::string &id)
  {
    auto found = std::find_if(
        _markers.begin(), _markers.end(),
        [&id](const screen_marker &m)
        {
          return m.id == id;
        });

    screen_marker m = {
        id, marker_position{
                _data.back().size(),
                _data.size() - 1}};

    if (found != _markers.end())
    {
      *found = std::move(m);
    }
    else
    {
      _markers.push_back(std::move(m));
    }

    print('%');
  }

  void screen::clear()
  {
    _data.clear();
    _markers.clear();
  }

  void screen::refresh()
  {
    curs.origin();
    curs.erase();

    for (const std::string &row : _data)
    {
      for (const char c : row)
      {
        curs.pushc(c);
      }

      curs.pushln();
    }
  }

  std::string screen::get_input(const std::string &id)
  {
    std::string resp;
    auto found = std::find_if(
        _markers.begin(), _markers.end(),
        [&id](const screen_marker &m)
        {
          return m.id == id;
        });

    if (found != _markers.end())
    {
      curs.at((*found).pos);
      std::getline(std::cin, resp);
    }

    return resp;
  }

  void screen::paint_one(
      const widget &w0)
  {
    widget_drawing wd(w0.paint());
    std::size_t nbm = 0;

    for (const std::string &r : wd.drawing)
    {
      nbm += std::count(r.begin(), r.end(), '%');
    }

    if (nbm == wd.markers.size())
    {
      std::size_t midx = 0;

      for (const std::string &s : wd.drawing)
      {
        for (char c : s)
        {
          if (c == '%')
          {
            print_marker(wd.markers[midx]);
            midx = midx + 1;
          }
          else
          {
            print(c);
          }
        }

        println();
      }
    }
  }
}