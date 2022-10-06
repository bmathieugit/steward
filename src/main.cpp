#include <ui.hpp>

#include <thread>
#include <iostream>

namespace ui = stew::ui;

int main(int argc, char **argv)
{
  ui::topic fcopy;

  auto& s1 = fcopy.subscribe();
  auto& s2 = fcopy.subscribe();
  
  fcopy.post(ui::message{._data = "Hello World"});

  std::cout << s1.consume().value()._data << '\n';
  std::cout << s2.consume().value()._data << '\n';

  std::string dodo;
  std::cin >> dodo;

  return EXIT_SUCCESS;
}
