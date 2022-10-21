#include <ui.hpp>
#include <ui-component.hpp>

#include <event.hpp>
#include <format.hpp>

#include <thread>
#include <iostream>

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

  ui::screen<10, 80> scr;

  ui::gauge_field gf;
  ui::text_field f1("input", 50);
  ui::text_message tm("input.value");

  f1.render(scr);
  tm.render(scr);
  gf.render(scr);

  f1.attach([&tm, &scr](const stew::message &mess)
            { tm.notify(mess, scr); });

  while (true)
  {
    gf.notify(scr.reader().readc(), scr);
    // f1.notify(scr.reader().readc(), scr);
  }

  return 0;
}
