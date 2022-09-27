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
#include <ui-screen-content.hpp>

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
    screen_content _content;
    std::vector<screen_marker> _markers;
    
    position _messpos;
    cursor _curs{std::cout};

  public:
    ~screen() = default;
    screen() = default;
    screen(const screen &) = default;
    screen(screen &&) = default;

  public:
    void paint(widget & w);
    std::map<std::string, std::string> get_inputs();
  };
}

#endif