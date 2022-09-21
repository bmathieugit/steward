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
  stew::ui::widget_factory wf;
  ui::menu *main_menu =
      wf.build<stew::ui::menu>(
          nullptr,
          std::string("steward main menu"),
          std::vector<std::string>{"vault", "agenda", "note", "wiki"});

  ui::form *f = wf.build<stew::ui::form>(
    nullptr, 
    std::string("Enedis SSO"));
  
  wf.build<ui::input>(f, std::string("login"), [](std::string_view){return true;});
  wf.build<ui::input>(f, std::string("password"), [](std::string_view){return true;});
  wf.build<ui::input>(f, std::string("email"), [](std::string_view){return true;});

  ui::screen scr;

  scr.paint(std::vector<const stew::ui::widget *>{f, main_menu});
  auto res = scr.get_inputs(std::vector<stew::ui::widget *>{f, main_menu});

  return EXIT_SUCCESS;
}
catch (const std::exception &e)
{
  std::cout << e.what() << '\n';
}