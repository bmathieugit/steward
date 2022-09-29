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
  ui::dispatcher disp;
  ui::bus bs(&disp);

  disp.connect("user_input", [](const ui::message &mess)
               { std::cout << mess._topic << " : " << mess._data << '\n'; });

  pen.style_text("-------------------------\n", {stm::back_blue, stm::fore_white})
      .style_text("-- email : ", {stm::back_blue, stm::fore_white})
      .marker("email", '%')
      .text("\n")

      .style_text("-- login : ", {stm::back_blue, stm::fore_white})
      .marker("login", '%')
      .text("\n")

      .style_text("-- password : ", {stm::back_blue, stm::fore_white})
      .marker("password", '%')
      .text("\n");

  grd.to_screen(scr);
  grd.from_screen(scr, bs);

  disp.consume();

  return EXIT_SUCCESS;
}
catch (const std::exception &e)
{
  std::cout << e.what() << '\n';
}