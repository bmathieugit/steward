#include <logger.hpp>
#include <csv.hpp>
#include <vault.hpp>
#include <ui.hpp>

#include <string>
#include <vector>

#include <array>
#include <sstream>
#include <iostream>

std::unique_ptr<stew::ui::widget::form> make_form(const std::string& name)
{
  return std::make_unique<stew::ui::widget::form>(name);
}

std::unique_ptr<stew::ui::widget::input> make_input(const std::string& label)
{
  return std::make_unique<stew::ui::widget::input>(label);
}

int main(int argc, char **argv)
{
  namespace widget = stew::ui::widget;

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


  stew::ui::widget::hlayout v;
  v.push(std::move(f));
  v.push(std::move(f2));

  stew::ui::screen::screen scr;

  scr.paint(scr, v);
  scr.notify_user_inputs(scr, v);

  return EXIT_SUCCESS;
}
