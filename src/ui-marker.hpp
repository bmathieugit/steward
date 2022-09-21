#ifndef __stew_ui_marker_hpp__
#define __stew_ui_marker_hpp__

#include <functional>
#include <string>
#include <string_view>

namespace stew::ui
{
  struct marker
  {
    std::string _id;
    std::function<bool(std::string_view)> _validator;
  };
}

#endif