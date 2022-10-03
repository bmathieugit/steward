#include <ui.hpp>

#include <iostream>

namespace ui = stew::ui;

int main(int argc, char **argv)
{
  ui::bus bus;
  ui::loop loop(bus);

  ui::consumer cons(bus, [](const ui::message &mess)
                    { std::cout << mess._data << '\n'; });
  ui::producer prod(bus);

  prod.produce("hello", ui::message{._data = "1 - Hello World !!"});
  prod.produce("hello2", ui::message{._data = "2 - Hello World !!"});
  prod.produce("hello3", ui::message{._data = "3 - Hello World !!"});
  prod.produce("hello4", ui::message{._data = "4 - Hello World !!"});

  loop.subscribe("hello", cons);
  loop.subscribe("hello", cons);
  loop.subscribe("hello", cons);
  loop.subscribe("hello2", cons);
  loop.subscribe("hello3", cons);
  loop.subscribe("hello4", cons);

  loop.run();
  
  return EXIT_SUCCESS;
}
