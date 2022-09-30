#include <ui.hpp>

#include <iostream>

#include <exception>

namespace ui = stew::ui;
using stm = ui::style_text_mode;

int main(int argc, char **argv)
try
{
  ui::dispatcher disp;
  ui::bus bs(&disp);

  ui::screen scr;

  ui::sso_view ssov(scr, bs);

  disp.connect("user_input", [](const ui::message &mess)
               { std::cout << mess._topic << " : " << mess._data << '\n'; });

  ssov.draw();
  ssov.show();
  ssov.emit();

  disp.consume();

  std::string str;
  std::cin >> str;

  return EXIT_SUCCESS;
}
catch (const std::exception &e)
{
  std::cout << e.what() << '\n';
}