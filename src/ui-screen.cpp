#include <ui-screen.hpp>

namespace stew::ui
{

  void screen::paint(std::vector<const widget *> ws)
  {
    _buffer.clear();
    _buffer.push_back("");
    _markers.clear();

    for (const widget *w : ws)
    {
      if (w == nullptr)
      {
        continue;
      }

      widget_drawing wd(w->paint());

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
              marker &m = wd.markers[midx];
              bool found = false;
              position pos{_buffer.back().size(), _buffer.size() - 1};

              for (screen_marker &sm : _markers)
              {
                if (sm._id == m._id)
                {
                  found = true;
                  sm._pos = pos;
                  sm._validator = m._validator;
                  break;
                }
              }

              if (!found)
              {
                _markers.push_back(
                    screen_marker{
                        ._id = m._id,
                        ._pos = pos,
                        ._validator = m._validator});
              }

              midx = midx + 1;
            }

            _buffer.back().push_back(c);
          }

          _buffer.push_back("");
        }
      }
    }

    _curs.origin();
    _curs.erase();

    for (const std::string &row : _buffer)
    {
      for (const char c : row)
      {
        if (c == '%')
        _curs.backred_mode();
        _curs.pushc(c);
        _curs.reset_mode();
      }

      _curs.pushln();
    }
  }

  std::map<std::string, std::string> screen::get_inputs(std::vector<widget *> ws)
  {
    std::map<std::string, std::string> res;

    for (const screen_marker &m : _markers)
    {
      
      std::string resp;
      bool validated = m._validator ? true : false;

      do
      {
        
      } while (!validated);
      


      _curs.at(m._pos);
      std::getline(std::cin, resp);
  
      // if (m._validator(resp))
      // {
      //   return resp;
      // }

      res[m._id] = resp;
    }

    return res;
  }
}