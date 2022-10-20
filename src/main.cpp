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

  ui::text_field f1("login", 50);
  ui::hidden_text_field f2("password", 50, '-');

  f1.render(scr);
  f2.render(scr);

  std::map<std::string, std::string> vals;

  f1.collect(scr, vals);
  f2.collect(scr, vals);

 f1.notify(scr, vals);
 f2.notify(scr, vals);

  for (const auto &[key, val] : vals)
  {
    std::cout << key << " --> " << val << '\n';
  }

  stew::pause();

  return 0;
}
