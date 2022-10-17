#ifndef __stew_ui_component_hpp__
#define __stew_ui_component_hpp__

#include <ui.hpp>
#include <format.hpp>
#include <cstddef>
#include <string>

namespace stew::ui
{
  class text_field
  {
    std::string _label;
    position _vpos;

  public:
    ~text_field() = default;
    text_field() = default;
    text_field(const std::string& label);
    text_field(const text_field &) = default;
    text_field(text_field &&) = default;
    text_field &operator=(const text_field &) = default;
    text_field &operator=(text_field &&) = default;

  public:
    template <std::size_t R, std::size_t C>
    void render(screen_writer<R, C> &scr);
    // void notify(const std::pair<std::string, std::string> &v);
    // std::pair<std::string, std::string> collect(screen &scr);
  };

  template <std::size_t R, std::size_t C>
  void text_field::render(screen_writer<R, C> &scr)
  {
    for (auto c : std::format("+ {} : ", _label))
    {
      scr.write(c);
    }

    _vpos = scr.pos();
    scr.write('\n');
  }
}

#endif