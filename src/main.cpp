#include <ui.hpp>

#include <iostream>
#include <exception>

namespace ui = stew::ui;
using stm = ui::style_text_mode;

int main(int argc, char **argv)
try
{
  ui::screen scr;
  ui::grid grd;
  ui::pencil pen(grd);

  pen.style_text("-------------------------\n", {stm::back_blue, stm::fore_white})
      .style_text("-- email : ", {stm::back_blue, stm::fore_white})
      .style_text("*********\n", {stm::back_white, stm::fore_black})
      
      .style_text("-- login : ", {stm::back_blue, stm::fore_white})
      .style_text("*********\n", {stm::back_white, stm::fore_black})
      
      .style_text("-- password : ", {stm::back_blue, stm::fore_white})
      .style_text("*********\n", {stm::back_white, stm::fore_black});

  grd.to_screen(scr);

  return EXIT_SUCCESS;
}
catch (const std::exception &e)
{
  std::cout << e.what() << '\n';
}