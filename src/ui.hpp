#ifndef __stew_ui_hpp__
#define __stew_ui_hpp__

#include <cstddef>
#include <string_view>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <optional>
#include <tuple>

#include <event.hpp>

namespace std
{
  template <typename T>
  using ptr = std::unique_ptr<T>;
}

namespace stew
{
  struct position
  {
    std::size_t _row = 0;
    std::size_t _col = 0;
  };

  using xy = position;
}

namespace stew::ui
{
  class screen
  {
  public:
    screen();
    ~screen();

  public:
    void origin();
    void at(position pos);
    void erase();
    void save();
    void restore();
    void savec();
    void restorec();
    void write(char c);
    void write(std::string_view txt);
    void read(std::string &resp);
  };
}

namespace stew
{
  template <
      typename T,
      std::size_t rows,
      std::size_t cols>
  class matrix
  {
    std::array<std::array<T, cols>, rows> _data;

  public:
    matrix() = default;
    matrix(const matrix &) = default;
    matrix(matrix &&) = default;
    matrix &operator=(const matrix &) = default;
    matrix &operator=(matrix &&) = default;

  public:
    T &operator[](position pos);
    const T &operator[](position pos) const;
  };

  template <typename T, std::size_t rows, std::size_t cols>
  T &matrix<T, rows, cols>::operator[](position pos)
  {
    return _data[pos._row][pos._col];
  }

  template <typename T, std::size_t rows, std::size_t cols>
  const T &matrix<T, rows, cols>::operator[](position pos) const
  {
    return _data[pos._row][pos._col];
  }
}

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
    void from_screen(screen &scr, topic &tpc);

    void clear();
  };

  class empty_cell : public grid_cell
  {
  public:
    virtual ~empty_cell() = default;
    virtual void to_screen(screen &scr) override;
    virtual std::optional<message> from_screen(position pos, screen &scr) override;
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
    pencil &message(std::string_view id, char c);
    pencil &hidden(std::string_view id, char c);
  };
}

namespace stew::ui
{
  class view
  {
  protected:
    grid _grd;
    pencil _pen{_grd};
    bool _showing = false;

  public:
    virtual ~view() = default;

  public:
    virtual void draw() = 0;

    void show(screen &scr);
    void emit(screen &scr, topic &tpc);
    void hide(screen &scr);

  protected:
    pencil &pen();
  };

}

namespace stew::ui
{
  template <typename T>
  concept component = requires(T &t, pencil &pen)
  {
    t.draw(pen);
  };

  class text_field
  {
    std::string _label;

  public:
    text_field(const std::string &label);
    text_field(const text_field &) = default;
    text_field(text_field &&) = default;
    text_field &operator=(const text_field &) = default;
    text_field &operator=(text_field &&) = default;
    ~text_field() = default;

  public:
    void draw(pencil &pen);
  };

  template <typename... CMP>
  class composed_view : public view
  {
    std::tuple<CMP...> _components;

  public:
    template <typename... CMPO>
    composed_view(CMPO &&...cmpo)
        : view(), _components(std::forward<CMPO>(cmpo)...) {}

    virtual ~composed_view() = default;

    virtual void draw()
    {
      std::apply([this](CMP &...cmp)
                 { (cmp.draw(this->pen()), ...); },
                 _components);
    }
  };

  template <typename... CMP>
  composed_view<std::decay_t<CMP>...> make_view(CMP &&...cmp)
  {
    return composed_view<std::decay_t<CMP>...>(std::forward<CMP>(cmp)...);
  }
}

#endif
