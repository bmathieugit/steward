#ifndef __stew_ui_application_hpp__
#define __stew_ui_application_hpp__

#include <event.hpp>

#include <memory>
#include <map>
#include <string>
#include <string_view>

namespace std
{
  template <typename T>
  using ptr = unique_ptr<T>;
}

namespace stew::ui
{
  class application
  {
    std::vector<producer> _prods;
    std::vector<consumer> _conss;

    bus& _bus;

  public:
    
  };
}

#endif