#include <logger.hpp>
#include <csv.hpp>
#include <vault.hpp>
#include <ui.hpp>

#include <string>
#include <vector>
#include <bit>

#include <array>
#include <sstream>
#include <iostream>

int main(int argc, char **argv)
{
  namespace widget = stew::ui::widget;

  stew::ui::widget::form_builder fb;

  auto f = fb.name("enterprise identity")
               .input("name")
               .input("firstname")
               .input("email")
              
               .build();

  stew::ui::screen scr;
  auto sp = scr.painter();
  
  f.paint(sp);

  scr.refresh();
  scr.at_origin();

  f.submit(scr);

  return EXIT_SUCCESS;
}
