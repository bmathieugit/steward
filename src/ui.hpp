#ifndef __stew_ui_hpp__
#define __stew_ui_hpp__

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <map>

namespace stew::ui
{
  struct position
  {
    std::size_t x = 0;
    std::size_t y = 0;
  };

  using marker_position = position;
  using cursor_position = position;

  struct cursor
  {
    std::ostream &os;
    cursor_position pos;
    std::map<std::string, marker_position> markers;

    void erase()
    {
      std::cout << "\033[2J";
    }

    void up(std::size_t n)
    {
      os << "\033[" << n << 'A';
      pos.y = pos.y - n >= 0 ? pos.y - n : 0;
    }

    void down(std::size_t n)
    {
      os << "\033[" << n << 'B';
      pos.y = pos.y + n;
    }

    void left(std::size_t n)
    {
      os << "\033[" << n << 'D';
      pos.x = pos.x - n >= 0 ? pos.x - n : 0;
    }

    void right(std::size_t n)
    {
      os << "\033[" << n << 'C';
      pos.x = pos.x + n;
    }

    void origin()
    {
      pos.x = 0;
      pos.y = 0;
      os << "\033[H";
    }

    void at_marker(const std::string &id)
    {
      if (markers.contains(id))
      {
        marker_position mpos = markers.at(id);

        origin();

        if (mpos.y != 0)
        {
          down(mpos.y);
        }

        if (mpos.x != 0)
        {
          right(mpos.x);
        }
      }
    }
  };

  class screen
  {
  public:
    class screen_painter
    {
      screen *_scr;

    public:
      ~screen_painter() = default;
      explicit screen_painter(screen &scr)
          : _scr(&scr) {}

      screen_painter(const screen_painter &) = default;
      screen_painter(screen_painter &&) = default;
      screen_painter &operator=(const screen_painter &) = default;
      screen_painter &operator=(screen_painter &&) = default;

    public:
      void append(std::string_view s)
      {
        _scr->append(s);
      }

      void append_marker(const std::string &id)
      {
        _scr->append_marker(id);
      }
    };

  private:
    std::vector<std::string> data;
    cursor curs{std::cout};

  public:
    ~screen() = default;
    screen() { clear(); }
    screen(const screen &) = default;
    screen(screen &&) = default;
    screen &operator=(const screen &) = default;
    screen &operator=(screen &&) = default;

  public:
    void append_marker(const std::string &id)
    {
      if (data.empty())
      {
        data.push_back("");
      }

      curs.markers[id] =
          marker_position{
              data.back().size(),
              data.size() - 1};

      data.back().push_back('%');
    }

    void append(std::string_view s)
    {
      if (data.empty())
      {
        data.push_back("");
      }

      for (char c : s)
      {
        if (c == '\n')
        {
          data.push_back("");
        }
        else
        {
          data.back().push_back(c);
        }
      }
    }

    screen_painter painter()
    {
      return screen_painter(*this);
    }

    void clear()
    {
      data.clear();
    }

  public:
    void at_origin()
    {
      curs.origin();
    }

    void at_marker(const std::string &id)
    {
      curs.at_marker(id);
    }

  public:
    void refresh()
    {
      curs.origin();
      curs.erase();

      for (const std::string &row : data)
      {
        std::cout << row << '\n';
      }
    }

    std::string capture()
    {
      std::string resp;
      std::cin >> resp;
      return resp;
    }
  };

  namespace widget
  {
    template <typename W>
    concept widget = requires(const W &w, screen::screen_painter &sp)
    {
      w.paint(sp);
    };

    class input
    {
      std::string _label;

    public:
      ~input() = default;
      explicit input(std::string_view lbl)
          : _label(lbl) {}
      input(const input &) = default;
      input(input &&) = default;
      input &operator=(const input &) = default;
      input &operator=(input &&) = default;

    public:
      const std::string &label() const
      {
        return _label;
      }

    public:
      void paint(screen::screen_painter &sp) const
      {
        sp.append("----------------\n");
        sp.append(" + ");
        sp.append(_label);
        sp.append(" : ");
        sp.append_marker(_label);
        sp.append("\n");
      }
    };

    class form
    {
      std::string _name;
      std::vector<input> _inputs;

    public:
      ~form() = default;
      explicit form(std::string_view name)
          : _name(name) {}
      form(const form &) = default;
      form(form &&) = default;
      form &operator=(const form &) = default;
      form &operator=(form &&) = default;

    public:
      void push(const input &i)
      {
        _inputs.push_back(i);
      }

      std::vector<input> &inputs()
      {
        return _inputs;
      }

      void paint(screen::screen_painter &sp) const
      {
        sp.append("------------------------\n");
        sp.append("--- ");
        sp.append(_name);
        sp.append(" ---\n");

        for (const widget::input &i : _inputs)
        {
          i.paint(sp);
        }
      }

      std::map<std::string, std::string> submit(screen &scr)
      {
        std::map<std::string, std::string> values;

        for (const auto &i : _inputs)
        {
          scr.at_marker(i.label());
          values[i.label()] = scr.capture();
        }

        return values;
      }
    };

    class form_builder
    {
      std::string _name;
      std::vector<std::string> _inputs_label;

    public:
      form_builder &name(std::string_view n)
      {
        _name = n;
        return *this;
      }

      form_builder &input(std::string_view lbl)
      {
        _inputs_label.emplace_back(lbl);
        return *this;
      }

      widget::form build() const
      {
        widget::form f(_name);

        for (const std::string &lbl : _inputs_label)
        {
          f.push(widget::input(lbl));
        }

        return f;
      }
    };
  }
}

#endif
