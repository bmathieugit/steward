//#include <ui.hpp>
#include <event.hpp>
#include <format.hpp>

#include <thread>
#include <iostream>

/*namespace ui = stew::ui;

constexpr int maxi = 1000000;

namespace stew
{
  void pause()
  {
    std::string str;
    std::getline(std::cin, str);
  }
}

namespace stew::jewel
{
  void draw_term()
  {
    ui::screen scr;

    int c[3];

    while (true)
    {

      c[0] = getch();
      c[1] = getch();
      c[2] = getch();

      using kb = ui::keyboard;

      if (c[0] == int(kb::ESC) && c[1] == '[')
      {
        scr.write("\033[40m \033[0m");
        scr.left();

        switch (c[2])
        {
        case 'A':
          scr.up();
          break;
        case 'B':
          scr.down();
          break;
        case 'C':
          scr.right();
          break;
        case 'D':
          scr.left();
          break;
        }
      }
    }
  }
}

void copyfile(std::size_t nbfile)
{
  for (std::size_t i(0); i < nbfile; ++i)
  {   
    std::string file = std::string("/tmp/") + std::to_string(i) + ".txt";
  }
}
*/
int main(int argc, char **argv)
{
  std::cout << std::format("{} {} !\n", "helloooo", "world");
  return EXIT_SUCCESS;
}
