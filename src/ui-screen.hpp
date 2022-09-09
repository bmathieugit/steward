#ifndef __stew_ui_screen_hpp__
#define __stew_ui_screen_hpp__

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#include <ui-widget.hpp>

namespace stew::ui::screen
{
  struct position
  {
    std::size_t x = 0;
    std::size_t y = 0;
  };

  using marker_position = position;
  using cursor_position = position;

  struct cursor
  {
    std::ostream &os;
    cursor_position pos;

    void erase()
    {
      std::cout << "\033[2J";
    }

    void up(std::size_t n)
    {
      os << "\033[" << n << 'A';
      pos.y = pos.y - n >= 0 ? pos.y - n : 0;
    }

    void down(std::size_t n)
    {
      os << "\033[" << n << 'B';
      pos.y = pos.y + n;
    }

    void left(std::size_t n)
    {
      os << "\033[" << n << 'D';
      pos.x = pos.x - n >= 0 ? pos.x - n : 0;
    }

    void right(std::size_t n)
    {
      os << "\033[" << n << 'C';
      pos.x = pos.x + n;
    }

    void origin()
    {
      pos.x = 0;
      pos.y = 0;
      os << "\033[H";
    }

    void at(marker_position mpos)
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
  };

  struct screen_marker
  {
    std::string id;
    marker_position pos;
  };

  class screen
  {
  private:
    std::vector<std::string> _data;
    std::vector<screen_marker> _markers;

    cursor curs{std::cout};

  public:
    ~screen() = default;
    screen() { clear(); }
    screen(const screen &) = default;
    screen(screen &&) = default;
    screen &operator=(const screen &) = default;
    screen &operator=(screen &&) = default;

  public:
    const std::vector<screen_marker> &markers() const
    {
      return _markers;
    }

  public:
    void println()
    {
      _data.push_back("");
    }

    void print(char c)
    {
      if (_data.empty())
        println();

      _data.back().push_back(c);
    }

    void print_marker(const std::string &id)
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

    void clear()
    {
      _data.clear();
      _markers.clear();
    }


  public:
    void refresh()
    {
      curs.origin();
      curs.erase();

      for (const std::string &row : _data)
      {
        std::cout << row << '\n';
      }
    }

    std::string get_input(const std::string &id)
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
        std::cin >> resp;
      }

      return resp;
    }
  };

  struct screen_painter
  {
  private:
    void paint_one(
        screen &scr,
        const widget::widget auto &w0) const
    {
      draw::widget_drawing wd(w0.paint());

      std::size_t nbm = std::count(
          wd.drawing.begin(),
          wd.drawing.end(), '%');

      if (nbm == wd.markers.size())
      {
        std::size_t midx = 0;

        for (char c : wd.drawing)
        {
          if (c == '\n')
          {
            scr.println();
          }
          else if (c == '%')
          {
            scr.print_marker(wd.markers[midx]);
            midx = midx + 1;
          }
          else
          {
            scr.print(c);
          }
        }
      }
    }

  public:
    void paint(
        screen &scr,
        const widget::widget auto &...wn) const
    {
      scr.clear();
      (paint_one(scr, wn), ...);
      scr.refresh();
    }
  };

  struct screen_user_input
  {
    void notify_user_inputs(
        screen &scr,
        widget::widget auto &... w)
    {
      for (const auto &[id, _] : scr.markers())
      {
        std::string in = scr.get_input(id);
        (w.notify_value(id, in), ...);
      }
    }
  };
}

#endif