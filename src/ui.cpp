#include <ui.hpp>
#include <iostream>

namespace stew::ui
{
  screen::screen()
  {
    savec();
    save();
  }

  screen::~screen()
  {
    restore();
    restorec();
  }

  void screen::origin()
  {
    std::cout << "\033[H";
  }

  void screen::at(position pos)
  {
    origin();

    if (pos._row)
    {
      std::cout << "\033[" << pos._row << 'B';
    }

    if (pos._col)
    {
      std::cout << "\033[" << pos._col << 'C';
    }
  }

  void screen::erase()
  {
    std::cout << "\033[2J";
  }

  void screen::save()
  {
    std::cout << "\033[?47h";
  }

  void screen::restore()
  {
    std::cout << "\033[?47l";
  }

  void screen::savec()
  {
    std::cout << "\033[s";
  }

  void screen::restorec()
  {
    std::cout << "\033[u";
  }

  void screen::write(char c)
  {
    std::cout << c;
  }

  void screen::write(std::string_view txt)
  {
    std::cout << txt;
  }

  void screen::read(std::string &resp)
  {
    std::getline(std::cin, resp);
  }
}

namespace stew::ui
{
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

  void grid::from_screen(screen &scr, topic &tpc)
  {
    scr.origin();

    for (std::size_t row(0); row < _table.size(); ++row)
    {
      for (std::size_t col(0); col < _table[row].size(); ++col)
      {
        if (_table[row][col])
        {
          std::optional<message> mess = _table[row][col]->from_screen({row, col}, scr);

          if (mess.has_value())
          {
            tpc.post(mess.value());
          }
        }
      }
    }
  }

  void grid::clear()
  {
    _table.clear();
  }

  void empty_cell::to_screen(screen &scr)
  {
  }

  std::optional<message> empty_cell::from_screen(position pos, screen &scr)
  {
    return std::nullopt;
  }

  text_grid_cell::text_grid_cell(char c)
      : _c(c) {}

  void text_grid_cell::to_screen(screen &scr)
  {
    scr.write(_c);
  }

  std::optional<message> text_grid_cell::from_screen(position pos, screen &scr)
  {
    return std::nullopt;
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
    default:
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

  std::optional<message> marker_grid_cell::from_screen(position pos, screen &scr)
  {
    scr.at(pos);
    scr.read(_value);
    return message{_id + ":" + _value};
  }

  message_grid_cell::message_grid_cell(std::string_view id, char c)
      : text_grid_cell(c), _id(id)
  {
  }

  void message_grid_cell::to_screen(screen &scr)
  {
    text_grid_cell::to_screen(scr);
  }

  std::optional<message> message_grid_cell::from_screen(position pos, screen &scr)
  {
    return std::nullopt;
  }

  hidden_marker_grid_cell::hidden_marker_grid_cell(std::string_view id, char c)
      : marker_grid_cell(id, c) {}

  std::optional<message> hidden_marker_grid_cell::from_screen(position pos, screen &scr)
  {
    scr.write("\033[8m");
    auto &&res = marker_grid_cell::from_screen(pos, scr);
    scr.write("\033[28m");
    return res;
  }
}

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

namespace stew::ui
{
  void view::show(screen &scr)
  {
    if (!_showing)
    {
      _grd.to_screen(scr);
      _showing = true;
    }
  }

  void view::emit(screen &scr, topic &tpc)
  {
    if (_showing)
    {
      _grd.from_screen(scr, tpc);
    }
  }

  void view::hide(screen &scr)
  {
    if (_showing)
    {
      scr.erase();
      scr.origin();
      _showing = false;
    }
  }

  pencil &view::pen()
  {
    return _pen;
  }

  text_field::text_field(const std::string &label)
      : _label(label) {}

  void text_field::draw(pencil &pen)
  {
    pen.text("----- ").text(_label).text(" : ").marker(_label, '%').text("\n");
  }
}