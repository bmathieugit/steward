#include <ui-pencil.hpp>

namespace stew::ui
{
  pencil::pencil(grid &grd)
      : _grd(grd)
  {
    _grd.clear();
  }

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

  pencil &pencil::style_text(std::string_view txt, const std::vector<style_text_mode> &modes)
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

  pencil &pencil::marker(std::string_view id, char c)
  {
    _grd.push_back(std::ptr<grid_cell>(new marker_grid_cell(id, c)));
    return *this;
  }

  pencil &pencil::message(std::string_view id, char c)
  {
    _grd.push_back(std::ptr<grid_cell>(new message_grid_cell(id, c)));
    return *this;
  }

  pencil &pencil::hidden(std::string_view id, char c)
  {
    _grd.push_back(std::ptr<grid_cell>(new hidden_marker_grid_cell(id, c)));
    return *this;
  }
}
