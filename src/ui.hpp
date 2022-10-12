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
      std::size_t R,
      std::size_t C>
  class matrix
  {
    std::array<std::array<T, C>, R> _data;

  public:
    matrix() = default;
    matrix(const matrix &) = default;
    matrix(matrix &&) = default;
    matrix &operator=(const matrix &) = default;
    matrix &operator=(matrix &&) = default;

  public:
    std::size_t rows() const;
    std::size_t cols() const;

    T &operator[](position pos);
    const T &operator[](position pos) const;
  };

  template <typename T, std::size_t R, std::size_t C>
  std::size_t matrix<T, R, C>::rows() const
  {
    return R;
  }

  template <typename T, std::size_t R, std::size_t C>
  std::size_t matrix<T, R, C>::cols() const
  {
    return C;
  }

  template <typename T, std::size_t R, std::size_t C>
  T &matrix<T, R, C>::operator[](position pos)
  {
    return _data[pos._row][pos._col];
  }

  template <typename T, std::size_t R, std::size_t C>
  const T &matrix<T, R, C>::operator[](position pos) const
  {
    return _data[pos._row][pos._col];
  }
}

namespace stew::ui
{
  class grid_cell
  {
  public:
    std::size_t _row = 0;
    std::size_t _col = 0;

  public:
    virtual ~grid_cell() = default;
    virtual void to_screen(screen &scr) = 0;
    virtual std::optional<message> from_screen(position pos, screen &scr) = 0;
  };

  template <std::size_t R, std::size_t C>
  class grid
  {
    stew::matrix<std::ptr<grid_cell>, R, C> _table;
    stew::position _cur{0, 0};

  public:
    grid() = default;
    grid(const grid &) = delete;
    grid(grid &&) = default;
    grid &operator=(const grid &) = delete;
    grid &operator=(grid &&) = default;

  public:
    void push_back(std::ptr<grid_cell> &&cell);
    void push_back_row();

    void to_screen(screen &scr);
    void from_screen(screen &scr, topic &tpc);

    void clear();
  };

  template <std::size_t R, std::size_t C>
  void grid<R, C>::push_back(std::ptr<grid_cell> &&cell)
  {
    if (_cur._row < _table.rows() && _cur._col < _table.cols())
    {
      _table[_cur] = std::move(cell);
      _cur._col++;
    }
  }

  template <std::size_t R, std::size_t C>
  void grid<R, C>::push_back_row()
  {
    if (_cur._row < _table.rows())
    {
      _cur._col = 0;
      _cur._row++;
    }
  }

  template <std::size_t R, std::size_t C>
  void grid<R, C>::to_screen(screen &scr)
  {
    scr.erase();
    scr.origin();

    for (std::size_t row(0); row < _table.rows(); ++row)
    {
      for (std::size_t col(0); col < _table.cols(); ++col)
      {
        if (_table[xy{row, col}])
        {
          _table[xy{row, col}]->to_screen(scr);
        }
      }

      scr.write('\n');
    }
  }

  template <std::size_t R, std::size_t C>
  void grid<R, C>::from_screen(screen &scr, topic &tpc)
  {
    scr.origin();

    for (std::size_t row(0); row < _table.rows(); ++row)
    {
      for (std::size_t col(0); col < _table.cols(); ++col)
      {
        auto pos = xy{row, col};

        if (_table[pos])
        {
          std::optional<message> mess = _table[pos]->from_screen(pos, scr);

          if (mess.has_value())
          {
            tpc.post(mess.value());
          }
        }
      }
    }
  }

  template <std::size_t R, std::size_t C>
  void grid<R, C>::clear()
  {
    for (std::size_t row(0); row < _table.rows(); ++row)
    {
      for (std::size_t col(0); col < _table.cols(); ++col)
      {
        _table[xy{row, col}] = nullptr;
      }
    }

    _cur = xy{0, 0};
  }

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

  template <std::size_t R, std::size_t C>
  class pencil
  {
    grid<R, C> &_grd;

  public:
    pencil(grid<R, C> &grd);

  public:
    pencil &text(std::string_view txt);
    pencil &style_text(std::string_view txt, const std::vector<style_text_mode> &mode);
    pencil &marker(std::string_view id, char c);
    pencil &message(std::string_view id, char c);
    pencil &hidden(std::string_view id, char c);
  };

  template <std::size_t R, std::size_t C>
  pencil<R, C>::pencil(grid<R, C> &grd)
      : _grd(grd)
  {
    _grd.clear();
  }

  template <std::size_t R, std::size_t C>
  pencil<R, C> &pencil<R, C>::text(std::string_view txt)
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

  template <std::size_t R, std::size_t C>
  pencil<R, C> &pencil<R, C>::style_text(std::string_view txt, const std::vector<style_text_mode> &modes)
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

  template <std::size_t R, std::size_t C>
  pencil<R, C> &pencil<R, C>::marker(std::string_view id, char c)
  {
    _grd.push_back(std::ptr<grid_cell>(new marker_grid_cell(id, c)));
    return *this;
  }

  template <std::size_t R, std::size_t C>
  pencil<R, C> &pencil<R, C>::message(std::string_view id, char c)
  {
    _grd.push_back(std::ptr<grid_cell>(new message_grid_cell(id, c)));
    return *this;
  }

  template <std::size_t R, std::size_t C>
  pencil<R, C> &pencil<R, C>::hidden(std::string_view id, char c)
  {
    _grd.push_back(std::ptr<grid_cell>(new hidden_marker_grid_cell(id, c)));
    return *this;
  }
}

namespace stew::ui
{
  template <std::size_t R, std::size_t C>
  class view
  {
  protected:
    grid<R, C> _grd;
    pencil<R, C> _pen{_grd};
    bool _showing = false;

  public:
    virtual ~view() = default;

  public:
    virtual void draw() = 0;

    void show(screen &scr);
    void emit(screen &scr, topic &tpc);
    void hide(screen &scr);

  protected:
    pencil<R, C> &pen();
  };

  template <std::size_t R, std::size_t C>
  void view<R, C>::show(screen &scr)
  {
    if (!_showing)
    {
      _grd.to_screen(scr);
      _showing = true;
    }
  }

  template <std::size_t R, std::size_t C>
  void view<R, C>::emit(screen &scr, topic &tpc)
  {
    if (_showing)
    {
      _grd.from_screen(scr, tpc);
    }
  }

  template <std::size_t R, std::size_t C>
  void view<R, C>::hide(screen &scr)
  {
    if (_showing)
    {
      scr.erase();
      scr.origin();
      _showing = false;
    }
  }

  template <std::size_t R, std::size_t C>
  pencil<R, C> &view<R, C>::pen()
  {
    return _pen;
  }

}

namespace stew::ui
{
  template <typename T, std::size_t R, std::size_t C>
  concept component = requires(T &t, pencil<R, C> &pen)
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
    template <std::size_t R, std::size_t C>
    void draw(pencil<R, C> &pen);
  };

  template <std::size_t R, std::size_t C>
  void text_field::draw(pencil<R, C> &pen)
  {
    pen.text("----- ").text(_label).text(" : ").marker(_label, '%').text("\n");
  }

  template <std::size_t R, std::size_t C, typename... CMP>
  class composed_view : public view<R, C>
  {
    std::tuple<CMP...> _components;

  public:
    template <typename... CMPO>
    composed_view(CMPO &&...cmpo)
        : view<R, C>(), _components(std::forward<CMPO>(cmpo)...) {}

    virtual ~composed_view() = default;

    virtual void draw()
    {
      std::apply([this](CMP &...cmp)
                 { (cmp.draw(this->pen()), ...); },
                 _components);
    }
  };

  template <std::size_t R, std::size_t C, typename... CMP>
  composed_view<R, C, std::decay_t<CMP>...> make_view(CMP &&...cmp)
  {
    return composed_view<R, C, std::decay_t<CMP>...>(std::forward<CMP>(cmp)...);
  }
}

#endif
