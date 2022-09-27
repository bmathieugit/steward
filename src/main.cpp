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

constexpr auto no_valid = [](std::string_view)
{ return true; };

void pencil_test_code()
{
  ui::pencil pen{nullptr};
  auto skt = pen.style("\033[41m")
                 .text("login : ")
                 .style("\033[0m")
                 .mark()
                 .draw_sktech();
}

int main(int argc, char **argv)
try
{
  namespace ui = stew::ui;

  ui::widget_factory wf;
  ui::vlayout *vl = wf.build<ui::vlayout>(nullptr);

  ui::menu *main_menu = wf.build<ui::menu>(
      vl, std::string("steward main menu"),
      std::vector<std::string>{"vault", "agenda", "note", "wiki"});

  ui::form *f = wf.build<ui::form>(vl, std::string("Enedis SSO"));

  wf.build<ui::input>(f, std::string("login"), no_valid);
  wf.build<ui::input>(f, std::string("password"), no_valid);
  wf.build<ui::input>(f, std::string("email"), no_valid);

  ui::screen scr;

  scr.paint(*vl);
  auto res = scr.get_inputs();

  return EXIT_SUCCESS;
}
catch (const std::exception &e)
{
  std::cout << e.what() << '\n';
}