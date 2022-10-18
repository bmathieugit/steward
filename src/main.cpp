#include <ui.hpp>
#include <ui-component.hpp>

#include <event.hpp>
#include <format.hpp>

#include <thread>
#include <iostream>
#include <variant>
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

int main()
{
  namespace ui = stew::ui;

  ui::screen<10, 40> scr;

  ui::text_field f1("login");
  ui::text_field f2("password");

  f1.render(scr);
  f2.render(scr);

  std::map<std::string, std::string> vals;

  f1.collect(scr, vals);
  f2.collect(scr, vals);

  f1.notify(scr, vals);
  f2.notify(scr, vals);
  
  stew::pause();

  return 0;
}
