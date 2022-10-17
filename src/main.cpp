#include <ui.hpp>
#include <ui-component.hpp>

#include <event.hpp>
#include <format.hpp>

#include <thread>
#include <iostream>

namespace ui = stew::ui;

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

  ui::screen_writer<50, 100> sw;

  ui::text_field f1("login");
  ui::text_field f2("password");

  f1.render(sw);
  f2.render(sw);

  sw.render();

  return 0;
}
