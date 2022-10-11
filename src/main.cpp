#include <ui.hpp>
#include <event.hpp>

#include <thread>
#include <iostream>

namespace ui = stew::ui;

constexpr int maxi = 1000000;

namespace stew
{
  void pause()
  {
    std::string str;
    std::getline(std::cin, str);
  }
}

int main(int argc, char **argv)
{

  stew::matrix<int, 10, 20> mat;
  std::size_t i = 0;

  for (std::size_t row(0); row<10; ++row)
  {
    for (std::size_t col(0); col<20; ++col)
    {
      mat[stew::xy{row, col}] = i;
      ++i;
    }
  }

  for (std::size_t row(0); row<10; ++row)
  {
    for (std::size_t col(0); col<20; ++col)
    {
      std::cout << mat[stew::xy{row, col}];
    }

    std::cout << '\n';
  }



  // ui::screen scr;

  // ui::view &&v = ui::make_view(ui::text_field("login"), ui::text_field("password"));
  // v.draw();
  // v.show(scr);

  stew::pause();

  return EXIT_SUCCESS;
}
