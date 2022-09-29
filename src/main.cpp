#include <ui.hpp>

#include <iostream>
#include <exception>

namespace ui = stew::ui;
using stm = ui::style_text_mode;

struct person
{
  std::string login;
  std::string email;
  std::string passwd;
};

int main(int argc, char **argv)
try
{
  ui::screen scr;
  ui::grid grd;
  ui::pencil pen(grd);
  ui::dispatcher disp;
  ui::bus bs(&disp);

  scr.savec();
  scr.save();

  person p;

  disp.connect("user_input",
               [&p](const std::vector<ui::message> &messs)
               {
                 for (const ui::message &mess : messs)
                 {
                   if (mess._data.starts_with("login"))
                   {
                     std::string_view sv = mess._data;
                     sv.remove_prefix(6);
                     p.login = sv;
                   }
                   else if (mess._data.starts_with("email"))
                   {
                     std::string_view sv = mess._data;
                     sv.remove_prefix(6);
                     p.email = sv;
                   }
                   else if (mess._data.starts_with("password"))
                   {
                     std::string_view sv = mess._data;
                     sv.remove_prefix(9);
                     p.passwd = sv;
                   }
                 }
               });

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
      .hidden("password", '%')
      .text("\n");

  grd.to_screen(scr);
  grd.from_screen(scr, bs);

  scr.origin();

  disp.consume();

  scr.restore();
  scr.restorec();

  return EXIT_SUCCESS;
}
catch (const std::exception &e)
{
  std::cout << e.what() << '\n';
}