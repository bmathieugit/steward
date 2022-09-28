#ifndef __stew_ui_position_hpp__
#define __stew_ui_position_hpp__

#include <cstddef>

namespace stew::ui
{
  struct position
  {
    std::size_t _row = 0;
    std::size_t _col = 0;
  };
}

#endif