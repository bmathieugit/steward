#ifndef __stew_ui_grid_hpp__
#define __stew_ui_grid_hpp__

#include <vector>
#include <map>
#include <memory>
#include <exception>
#include <string>
#include <string_view>

#include <ui-screen.hpp>
#include <ui-position.hpp>

namespace std
{
  template <typename T>
  using ptr = unique_ptr<T>;
}

namespace stew::ui
{
  class grid_cell
  {
  public:
    virtual ~grid_cell() = default;
    virtual void to_screen(screen &scr) = 0;
    virtual void from_screen(position pos, screen &scr) = 0;
  };

  class grid
  {
    std::vector<std::vector<std::ptr<grid_cell>>> _table;

  public:
    grid() = default;

  public:
    void push_back(std::ptr<grid_cell> &&cell);
    void push_back_row();

    grid_cell &at(std::size_t row, std::size_t col);
    const grid_cell &at(std::size_t row, std::size_t col) const;

    void to_screen(screen &scr);
    void from_screen(screen &scr);
  };

  class text_grid_cell : public grid_cell
  {
    char _c;

  public:
    virtual ~text_grid_cell() = default;
    text_grid_cell(char c);
    virtual void to_screen(screen &scr) override;
    virtual void from_screen(position pos, screen &scr) override;
  };

  enum class style_text_mode
  {
    back_black,
    back_red,
    back_green,
    back_yellow,
    back_blue,
    back_magenta,
    back_cyan,
    back_white,
    back_default,
    fore_black,
    fore_red,
    fore_green,
    fore_yellow,
    fore_blue,
    fore_magenta,
    fore_cyan,
    fore_white,
    fore_default,
    reset
  };

  class style_text_grid_cell : public text_grid_cell
  {
    std::vector<style_text_mode> _modes;

  public:
    style_text_grid_cell(char c, const std::vector<style_text_mode> &modes);
    virtual ~style_text_grid_cell() = default;
    virtual void to_screen(screen &scr) override;
  };

  class marker_grid_cell : public text_grid_cell
  {
    std::string _id;
    std::string _value;

  public:
    marker_grid_cell(std::string_view id, char c);
    virtual ~marker_grid_cell() = default;
    virtual void to_screen(screen &scr) override;
    virtual void from_screen(position pos, screen &scr) override;
  };

  text_grid_cell::text_grid_cell(char c)
      : _c(c) {}

  void text_grid_cell::to_screen(screen &scr)
  {
    scr.write(_c);
  }

  void text_grid_cell::from_screen(position pos, screen &scr)
  {
  }

  style_text_grid_cell::style_text_grid_cell(
      char c, const std::vector<style_text_mode> &modes)
      : text_grid_cell(c), _modes(modes) {}

  std::string_view convert(style_text_mode mode)
  {
    switch (mode)
    {
    case style_text_mode::back_black:
      return "\033[40m";
    case style_text_mode::back_red:
      return "\033[41m";
    case style_text_mode::back_green:
      return "\033[42m";
    case style_text_mode::back_yellow:
      return "\033[43m";
    case style_text_mode::back_blue:
      return "\033[44m";
    case style_text_mode::back_magenta:
      return "\033[45m";
    case style_text_mode::back_cyan:
      return "\033[46m";
    case style_text_mode::back_white:
      return "\033[47m";
    case style_text_mode::back_default:
      return "\033[49m";

    case style_text_mode::fore_black:
      return "\033[30m";
    case style_text_mode::fore_red:
      return "\033[31m";
    case style_text_mode::fore_green:
      return "\033[32m";
    case style_text_mode::fore_yellow:
      return "\033[33m";
    case style_text_mode::fore_blue:
      return "\033[34m";
    case style_text_mode::fore_magenta:
      return "\033[35m";
    case style_text_mode::fore_cyan:
      return "\033[36m";
    case style_text_mode::fore_white:
      return "\033[37m";
    case style_text_mode::fore_default:
      return "\033[39m";

    case style_text_mode::reset:
      return "\033[0m";
    }
  }

  void style_text_grid_cell::to_screen(screen &scr)
  {
    for (auto mode : _modes)
    {
      scr.write(convert(mode));
    }

    text_grid_cell::to_screen(scr);
    scr.write(convert(style_text_mode::reset));
  }

  marker_grid_cell::marker_grid_cell(std::string_view id, char c)
      : text_grid_cell(c), _id(id)
  {
  }

  void marker_grid_cell::to_screen(screen &scr)
  {
    text_grid_cell::to_screen(scr);
  }

  void marker_grid_cell::from_screen(position pos, screen &scr)
  {
    scr.at(pos);
    scr.read(_value);
  }

  void grid::push_back(std::ptr<grid_cell> &&cell)
  {
    if (_table.empty())
    {
      push_back_row();
    }

    _table.back().push_back(std::move(cell));
  }

  void grid::push_back_row()
  {
    _table.push_back(std::vector<std::ptr<grid_cell>>());
  }

  grid_cell &grid::at(std::size_t row, std::size_t col)
  {
    if (row >= _table.size() || col >= _table[row].size())
      throw int(12); // TODO EXCEPTION;

    if (!_table[row][col])
      throw int(12); // TODO EXCEPTION

    return *_table[row][col];
  }

  const grid_cell &grid::at(std::size_t row, std::size_t col) const
  {
    if (row >= _table.size() || col >= _table[row].size())
      throw int(12); // TODO EXCEPTION;

    if (!_table[row][col])
      throw int(12); // TODO EXCEPTION

    return *_table[row][col];
  }

  void grid::to_screen(screen &scr)
  {
    scr.erase();
    scr.origin();

    for (auto &&row : _table)
    {
      for (auto &&cell : row)
      {
        if (cell)
        {
          cell->to_screen(scr);
        }
      }

      scr.write('\n');
    }
  }

  void grid::from_screen(screen &scr)
  {
    scr.origin();

    for (std::size_t row(0); row < _table.size(); ++row)
    {
      for (std::size_t col(0); col < _table[row].size(); ++col)
      {
        if (_table[row][col])
        {
          _table[row][col]->from_screen({row, col}, scr);
        }
      }
    }
  }
}

#endif