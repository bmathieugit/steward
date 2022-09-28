#ifndef __stew_ui_screen_hpp__
#define __stew_ui_screen_hpp__

#include <iostream>
#include <string_view>

namespace stew::ui
{
  class screen
  {
  public:
    void write(char c);
    void write(std::string_view txt);
  };

  void screen::write(char c)
  {
    std::cout << c;
  }

  void screen::write(std::string_view txt)
  {
    std::cout << txt;
  }
}

#endif