#include <ui.hpp>

#include <iostream>

#ifdef _WIN32
#include <conio.h>
#endif
#ifdef __unix__
#include <termios.h>
int getch()
{
  termios oldt, newt;
  int ch;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return ch;
}
#endif

namespace stew::ui
{
  keyevent getkey()
  {
    keyevent k;

    int c = getch();

    if (c == '\033')
    {
      getch();

      switch (getch())
      {
      case 'A':
        return arrow_event::UP;
      case 'B':
        return arrow_event::DOWN;
      case 'C':
        return arrow_event::RIGHT;
      case 'D':
        return arrow_event::LEFT;
      }
    }

    return (char)c;
  }
  // std::string_view convert(style_text_mode mode)
  // {
  //   switch (mode)
  //   {
  //   case style_text_mode::back_black:
  //     return "\033[40m";
  //   case style_text_mode::back_red:
  //     return "\033[41m";
  //   case style_text_mode::back_green:
  //     return "\033[42m";
  //   case style_text_mode::back_yellow:
  //     return "\033[43m";
  //   case style_text_mode::back_blue:
  //     return "\033[44m";
  //   case style_text_mode::back_magenta:
  //     return "\033[45m";
  //   case style_text_mode::back_cyan:
  //     return "\033[46m";
  //   case style_text_mode::back_white:
  //     return "\033[47m";
  //   case style_text_mode::back_default:
  //     return "\033[49m";

  //   case style_text_mode::fore_black:
  //     return "\033[30m";
  //   case style_text_mode::fore_red:
  //     return "\033[31m";
  //   case style_text_mode::fore_green:
  //     return "\033[32m";
  //   case style_text_mode::fore_yellow:
  //     return "\033[33m";
  //   case style_text_mode::fore_blue:
  //     return "\033[34m";
  //   case style_text_mode::fore_magenta:
  //     return "\033[35m";
  //   case style_text_mode::fore_cyan:
  //     return "\033[36m";
  //   case style_text_mode::fore_white:
  //     return "\033[37m";
  //   case style_text_mode::fore_default:
  //     return "\033[39m";

  //   case style_text_mode::reset:
  //   default:
  //     return "\033[0m";
  //   }
  // }

}