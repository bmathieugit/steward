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

void view(stew::ui::text_field &f1,
          stew::ui::text_message &mess,
          stew::ui::screen<10, 80> &scr,
          stew::topic &tpc)
{
  namespace ui = stew::ui;

  f1.render(scr);
  mess.render(scr);

  while (true)
  {
    ui::keyevent ev = scr.reader().readc();
    f1.notify(ev, scr, tpc);
  }
}

void consume(stew::ui::text_message &mess,
             stew::ui::screen<10, 80> &scr,
             stew::subscriber &sub)
{
  while (true)
  {
    mess.notify(sub.consume().value(), scr);
  }
}

int main()
{
  namespace ui = stew::ui;

  ui::screen<10, 80> scr;

  ui::text_field f1("login", 50);
  ui::text_message mess;

  stew::topic tpc;

  std::jthread v(&view, std::ref(f1), std::ref(mess), std::ref(scr), std::ref(tpc));
  std::jthread s(&consume, std::ref(mess), std::ref(scr), std::ref(tpc.subscribe()));

  return 0;
}
