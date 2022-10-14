#include <ui.hpp>
//#include <ui-component.hpp>

#include <event.hpp>
#include <format.hpp>

#include <thread>
#include <iostream>

// namespace ui = stew::ui;

constexpr int maxi = 1000000;

namespace stew
{
  void pause()
  {
    std::string str;
    std::getline(std::cin, str);
  }
}

int main(void)
{
  namespace ui = stew::ui;

  ui::screen scr;
  scr.init();

  scr.write("Hello World!");
  scr.refresh();

  std::string s;
  int c;

  // while ((c = getch()) != KEY_ENTER)
  // {
  //   s += c;
  // }


  scr.write("you print ");
    ::addch('a'|A_BOLD);



  
  scr.refresh();

  getch();

  scr.close();

  return 0;
}
