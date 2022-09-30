#ifndef __stew_ui_screen_hpp__
#define __stew_ui_screen_hpp__

#include <iostream>
#include <string_view>
#include <string>

#include <ui-position.hpp>

namespace stew::ui
{
  class screen
  {
  public:
    screen();
    ~screen();
  public:
    void origin();
    void at(position pos);
    void erase();
    void save();
    void restore();
    void savec();
    void restorec();
    void write(char c);
    void write(std::string_view txt);
    void read(std::string &resp);
  };
}

#endif