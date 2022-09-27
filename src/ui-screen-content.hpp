#ifndef __stew_ui_screen_content_hpp__
#define __stew_ui_screen_content_hpp__

#include <vector>

namespace stew::ui
{
  class widget;

  struct screen_cell
  {
    widget *_w;
    char _c = ' ';
  };

  struct screen_content
  {
    std::vector<std::vector<screen_cell>> _table;
  };
}

#endif