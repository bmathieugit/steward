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

  text_field::text_field(const std::string &label)
      : _label(label) {}

}
