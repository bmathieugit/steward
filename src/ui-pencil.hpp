#ifndef __stew_ui_pencil_hpp__
#define __stew_ui_pencil_hpp__

#include <string_view>
#include <vector>

#include <ui-grid.hpp>

namespace stew::ui
{
  class pencil
  {
    grid &_grd;

  public:
    pencil(grid &grd);

  public:
    pencil &text(std::string_view txt);
    pencil &style_text(std::string_view txt, const std::vector<style_text_mode> &mode);
    pencil &marker(std::string_view id, char c);
  };
}

#endif