#include <ui.hpp>
#include <event.hpp>

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

int main(int argc, char **argv)
{
  ui::screen scr;

  auto v = ui::make_view<10, 50>(ui::text_field("login"), ui::text_field("password"));
  v.draw();
  v.show(scr);

  stew::pause();

  return EXIT_SUCCESS;
}
