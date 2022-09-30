#ifndef __stew_ui_application_hpp__
#define __stew_ui_application_hpp__

#include <ui-event.hpp>
#include <ui-screen.hpp>
#include <ui-view.hpp>

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
    std::map<std::string, std::ptr<view>> _views;
    dispatcher _disp;
    screen _scr;
    view *_showing = nullptr;

  public:
    void add_view(std::string_view id, std::ptr<view> &&v);
    void show(std::string_view id);
    void emit(std::string_view id);
  };
}

#endif