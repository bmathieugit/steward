#ifndef __stew_ui_grid_hpp__
#define __stew_ui_grid_hpp__

#include <memory>
#include <string>
#include <string_view>
#include <optional>
#include <vector>

#include <ui-screen.hpp>
#include <ui-position.hpp>
#include <ui-event.hpp>
#include <ui-config.hpp>

namespace stew::ui
{
  class grid_cell
  {
  public:
    virtual ~grid_cell() = default;
    virtual void to_screen(screen &scr) = 0;
    virtual std::optional<message> from_screen(position pos, screen &scr) = 0;
  };

  class grid
  {
    std::vector<std::vector<std::ptr<grid_cell>>> _table;

  public:
    grid() = default;

  public:
    void push_back(std::ptr<grid_cell> &&cell);
    void push_back_row();

    void to_screen(screen &scr);
    void from_screen(screen &scr, bus &bs);
  };

  class text_grid_cell : public grid_cell
  {
    char _c;

  public:
    virtual ~text_grid_cell() = default;
    text_grid_cell(char c);
    virtual void to_screen(screen &scr) override;
    virtual std::optional<message> from_screen(position pos, screen &scr) override;
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
    virtual std::optional<message> from_screen(position pos, screen &scr) override;
  };

  class message_grid_cell : public text_grid_cell
  {
    std::string _id;
    std::string _value;

  public:
    message_grid_cell(std::string_view id, char c);
    virtual ~message_grid_cell() = default;
    virtual void to_screen(screen &scr) override;
    virtual std::optional<message> from_screen(position pos, screen &scr) override;
  };

  class hidden_marker_grid_cell : public marker_grid_cell
  {
  public:
    hidden_marker_grid_cell(std::string_view id, char c);
    virtual ~hidden_marker_grid_cell() = default;
    virtual std::optional<message> from_screen(position pos, screen &scr) override;
  };
}

#endif