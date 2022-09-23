#include <ui-screen.hpp>

namespace stew::ui
{

  void screen::paint(const widget &w)
  {
    _curs.erase();
    _curs.origin();

    _markers.clear();

    widget_drawing wd(w.paint());

    std::size_t nbm = 0;

    for (const std::string &r : wd.drawing)
    {
      for (const char c : r)
      {
        if (c == '%')
        {
          nbm = nbm + 1;
        }
      }
    }

    if (nbm == wd.markers.size())
    {
      std::size_t mi = 0;

      for (const std::string &r : wd.drawing)
      {
        for (const char c : r)
        {
          if (c == '%')
          {
            marker m = wd.markers[mi];

            _markers.push_back(
                screen_marker{._id = m._id,
                              ._pos = _curs.pos(),
                              ._validator = m._validator});
            mi = mi + 1;
          }

          _curs.pushc(c);
        }

        _curs.pushln();
      }

      _curs.pushln();
      _curs.pushln();
      _messpos = _curs.pos();
    }
  }

  std::map<std::string, std::string> screen::get_inputs()
  {
    std::map<std::string, std::string> res;

    for (const screen_marker &m : _markers)
    {
      std::string resp;
      bool validated = false;

      do
      {
        _curs.at(m._pos);
        std::getline(std::cin, resp);

        if (m._validator)
        {
          validated = m._validator(resp);
        }
        else
        {
          validated = true;
        }

        if (!validated)
        {
          _curs.at(m._pos);

          for (char c : std::string(resp.size(), ' '))
          {
            _curs.pushc(c);
          }

          _curs.at(_messpos);
          _curs.backred_mode();

          for (const char c : std::string("input invalide for ") + m._id)
          {
            _curs.pushc(c);
          }

          _curs.reset_mode();
          _curs.at(m._pos);
          _curs.pushc('%');
          _curs.at(m._pos);
        }
        else 
        {
          _curs.at(_messpos);

          for (const char c : std::string("input invalide for ") + m._id)
          {
            _curs.pushc(' ');
          }

          _curs.pushc('\n');
        }

      } while (!validated);

      res[m._id] = resp;
    }

    return res;
  }
}