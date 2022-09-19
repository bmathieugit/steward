#include <logger.hpp>
#include <csv.hpp>
#include <vault.hpp>
#include <ui.hpp>

#include <string>
#include <vector>

#include <array>
#include <sstream>
#include <iostream>

namespace ui = stew::ui;

std::unique_ptr<ui::form> make_form(const std::string& name)
{
  return std::make_unique<ui::form>(name);
}

std::unique_ptr<ui::input> make_input(const std::string& label)
{
  return std::make_unique<ui::input>(label);
}

int main(int argc, char **argv)
{
  namespace ui = stew::ui;

  ui::menu main_menu("steward main menu", 
    {"vault", "agenda", "note", "wiki"});

  auto f = make_form("login/password");
  auto l1 = make_input("login");
  auto p1 = make_input("password");
  
  f->push(std::move(l1));
  f->push(std::move(p1));

  auto f2 = make_form("login/password2");

  auto l2 = make_input("login2");
  auto p2 = make_input("password2");
  
  f2->push(std::move(l2));
  f2->push(std::move(p2));

  ui::cursor curs(std::cout);
  ui::hlayout v;
  v.push(std::move(f));
  v.push(std::move(f2));

  ui::screen scr;

  scr.paint({&v, &main_menu});
  scr.notify({&v, &main_menu});

  return EXIT_SUCCESS;
}
