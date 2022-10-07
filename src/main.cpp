#include <ui.hpp>

#include <thread>
#include <iostream>

namespace ui = stew::ui;

constexpr int maxi = 100000;

int main(int argc, char **argv)
{
  ui::topic fcopy;

  auto &s1 = fcopy.subscribe();
  auto &s2 = fcopy.subscribe();

  fcopy.post(ui::message{._data = "Hello World"});

  std::jthread jt1([&fcopy]
                   {
                     for (std::size_t i(0); i < maxi; ++i)
                     {
                       fcopy.post(ui::message{._data = "Hello World"});
                     }
                   });

  std::jthread jt2([&s1]
                   {
    for (std::size_t i(0); i<maxi; ++i)
    {
      auto&& mess = s1.consume();
      if (mess.has_value())
      {
      std::cout << std::string("s1 consume : ") + mess.value()._data  + std::to_string(i)+'\n';
    }}
    
    ; });

  std::jthread jt3([&s2]
                   {
    for (std::size_t i(0); i<maxi; ++i)
    {
      auto&& mess = s2.consume();
      if (mess.has_value())
      {
      std::cout << std::string("s2 consume : ") + mess.value()._data  +  std::to_string(i)+'\n';
    }} });

  return EXIT_SUCCESS;
}
