#include <logger.hpp>
#include <csv.hpp>
#include <vault.hpp>
#include <ui.hpp>

#include <string>
#include <vector>

#include <array>
#include <sstream>
#include <iostream>
#include <exception>

namespace ui = stew::ui;

int main(int argc, char **argv)
try
{
  namespace ui = stew::ui;
  stew::ui::widget_factory wstore;
  ui::menu *main_menu = wstore.build<stew::ui::menu>(
      nullptr,
      std::string("steward main menu"),
      std::vector<std::string>{"vault", "agenda", "note", "wiki"});

  ui::screen scr;

  scr.paint(std::vector<const stew::ui::widget*>{main_menu});
  scr.notify(std::vector<stew::ui::widget*>{main_menu});

  return EXIT_SUCCESS;
}
catch (const std::exception &e)
{
  std::cout << e.what() << '\n';
}