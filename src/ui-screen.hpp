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

    void origin();
    void at(position pos);
    void erase();
    void write(char c);
    void write(std::string_view txt);
    void read(std::string &resp);
  };

  void screen::origin()
  {
    std::cout << "\033[H";
  }

  void screen::at(position pos)
  {
    origin();

    if (pos._row)
    {
      std::cout << "\033[" << pos._row << 'B';
    }

    if (pos._col)
    {
      std::cout << "\033[" << pos._col << 'C';
    }
  }

  void screen::erase()
  {
    std::cout << "\033[2J";
  }

  void screen::write(char c)
  {
    std::cout << c;
  }

  void screen::write(std::string_view txt)
  {
    std::cout << txt;
  }

  void screen::read(std::string &resp)
  {
    std::getline(std::cin, resp);

  }
}

#endif