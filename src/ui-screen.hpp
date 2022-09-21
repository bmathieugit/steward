#ifndef __stew_ui_screen_hpp__
#define __stew_ui_screen_hpp__

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

#include <ui-widget.hpp>
#include <ui-cursor.hpp>
#include <ui-position.hpp>

namespace stew::ui
{
  struct screen_marker
  {
    std::string _id;
    position _pos;
    std::function<bool(std::string_view)> _validator;
  };

  class screen
  {
  private:
    std::vector<std::string> _buffer;
    std::vector<screen_marker> _markers;

    cursor _curs{std::cout};

  public:
    ~screen() = default;
    screen() = default;
    screen(const screen &) = default;
    screen(screen &&) = default;

  public:
    void paint(std::vector<const widget *> ws);
    std::map<std::string, std::string> get_inputs(std::vector<widget *> ws);
  };
}

#endif