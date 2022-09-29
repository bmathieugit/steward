#include <ui-screen.hpp>

namespace stew::ui
{
  void screen::origin()
  {
    std::cout << "\033[H";
  }

  void screen::at(position pos)
  {
    origin();

    if (pos._row)
    {
      std::cout << "\033[" << pos._row << 'B';
    }

    if (pos._col)
    {
      std::cout << "\033[" << pos._col << 'C';
    }
  }

  void screen::erase()
  {
    std::cout << "\033[2J";
  }

  void screen::write(char c)
  {
    std::cout << c;
  }

  void screen::write(std::string_view txt)
  {
    std::cout << txt;
  }

  void screen::read(std::string &resp)
  {
    std::getline(std::cin, resp);
  }
}