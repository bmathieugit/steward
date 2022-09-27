#include <ui-screen.hpp>
#include <ui-pencil.hpp>

namespace stew::ui
{

  void screen::paint(widget &w)
  {
    _curs.erase();
    _curs.origin();
    _markers.clear();

    pencil pen = w.paint();
    sketch skt = pen.draw_sktech();

    std::size_t nbm = 0;

    for (const std::unique_ptr<sketch_content> &content : skt._contents)
    {
      if (content)
      {
        content->push_to(_content);
      }
    }

    for (std::vector<screen_cell> &row : _content._table)
    {
      for (screen_cell &cell : row)
      {
        if (cell._c == '\n')
        {
          _curs.pushln();
        }
        else
        {
          _curs.pushc(cell._c);
        }
      }
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