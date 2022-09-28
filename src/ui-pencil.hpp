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
    pencil &style_text(std::string_view txt, const std::vector<style_text_mode>& mode);
  };

  pencil::pencil(grid &grd)
      : _grd(grd) {}

  pencil &pencil::text(std::string_view txt)
  {
    for (char c : txt)
    {
      if (c == '\n')
      {
        _grd.push_back_row();
      }
      else
      {
        _grd.push_back(std::ptr<grid_cell>(new text_grid_cell(c)));
      }
    }

    return *this;
  }

  pencil& pencil::style_text(std::string_view txt, const std::vector<style_text_mode>& modes)
  {
    for (char c : txt)
    {
      if (c == '\n')
      {
        _grd.push_back_row();
      }
      else
      {
        _grd.push_back(std::ptr<grid_cell>(new style_text_grid_cell(c, modes)));
      }
    }

    return *this;
  }
}

#endif