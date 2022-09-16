#ifndef __stew_ui_screen_hpp__
#define __stew_ui_screen_hpp__

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#include <ui-widget.hpp>

namespace stew::ui
{
  struct position
  {
    std::size_t x = 0;
    std::size_t y = 0;
  };

  using marker_position = position;
  using cursor_position = position;

  class cursor
  {
    std::ostream &_os;
    cursor_position _pos;

  public:
    ~cursor() = default;
    cursor(std::ostream &os);
    cursor(const cursor &) = default;
    cursor(cursor &&) = default;
    cursor &operator=(const cursor &) = default;
    cursor &operator=(cursor &&) = default;

  public:
    void erase();
    void up(std::size_t n);
    void down(std::size_t n);
    void left(std::size_t n);
    void right(std::size_t n);
    void origin();
    void at(marker_position mpos);

  public:
    void pushc(char c);
    void pushln();
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
    screen() = default;
    screen(const screen &) = default;
    screen(screen &&) = default;
    screen &operator=(const screen &) = default;
    screen &operator=(screen &&) = default;

  public:
    const std::vector<screen_marker> &markers() const;

  public:
    void println();
    void print(char c);
    void print_marker(const std::string &id);
    void clear();

  public:
    void refresh();
    std::string get_input(const std::string &id);

  private:
    void paint_one(const ui::widget &w0);

  public:
    void paint(std::vector<const widget *> ws)
    {
      clear();

      for (const widget *w : ws)
      {
        if (w != nullptr)
        {
          paint_one(*w);
        }
      }

      refresh();
    }

    void notify(std::vector<widget *> ws)
    {
      for (const auto &[id, _] : markers())
      {
        std::string in = get_input(id);
        
        for (widget *w : ws)
        {
          if (w != nullptr)
          {
            w->notify(id, in);
          }
        }
      }
    }
  };
}

#endif