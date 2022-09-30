#include <ui.hpp>

namespace ui = stew::ui;
using stm = ui::style_text_mode;

int main(int argc, char **argv)
{
  ui::application app;
  
  app.add_view("sso", std::ptr<ui::view>(new ui::sso_view()));
  app.add_view("sso2", std::ptr<ui::view>(new ui::sso_view2()));
  app.show("sso");
  app.emit("sso");
  app.show("sso2");
  app.emit("sso2");
  
  return EXIT_SUCCESS;
}
