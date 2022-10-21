#ifndef __stew_ui_component_hpp__
#define __stew_ui_component_hpp__

#include <ui.hpp>
#include <format.hpp>
#include <cstddef>
#include <string>
#include <map>
#include <list>

namespace stew::ui
{
  template <typename T, typename V, std::size_t i = 0>
  constexpr std::size_t case_of_impl()
  {
    static_assert(std::variant_size_v<V> > i, "Type not found in variant");

    if constexpr (i == std::variant_size_v<V>)
    {
      return i;
    }
    else if constexpr (std::is_same_v<std::variant_alternative_t<i, V>, T>)
    {
      return i;
    }
    else
    {
      return case_of_impl<T, V, i + 1>();
    }
  }

  template <typename T, typename V>
  constexpr int case_of = case_of_impl<T, V>();

  class text_field
  {
    std::string _label;
    std::size_t _max_length;
    std::string _value;

    position _vpos;

  public:
    ~text_field() = default;
    text_field() = default;
    text_field(const std::string &label, std::size_t max_length = 10);
    text_field(const text_field &) = default;
    text_field(text_field &&) = default;
    text_field &operator=(const text_field &) = default;
    text_field &operator=(text_field &&) = default;

  public:
    template <std::size_t R, std::size_t C>
    void render(screen<R, C> &scr)
    {
      for (auto c : std::format("+ {} : ", _label))
      {
        scr.writer().write(c);
      }

      _vpos = scr.pos();
      scr.writer().write('\n');
    }

    template <std::size_t R, std::size_t C>
    void notify(keyevent ev, screen<R, C> &scr, topic &tpc)
    {
      scr.move().at(_vpos);
      scr.writer().write_n(_value.size(), ' ');

      if (ev.index() == case_of<char, keyevent>)
      {
        char c = std::get<char>(ev);

        if (c == '\n')
        {
          tpc.post(message{std::format("{}:{}", _label, _value)});
        }
        else if (c == 127)
        {
          if (!_value.empty())
          {
            _value.pop_back();
          }
        }
        else if (std::isprint(c) && _value.size() < _max_length)
        {
          _value.push_back(c);
        }
      }

      scr.move().at(_vpos);

      std::size_t total = _value.size();
      std::size_t width = std::min(total, C - _vpos._col);
      std::size_t start = 0;

      if (total > width)
      {
        start = total - width;
      }
      else
      {
        start = 0;
      }

      std::string_view tmp = std::string_view(_value).substr(start, width);

      for (char c : tmp)
      {
        scr.writer().write(c);
      }

      scr.move().at(position{_vpos._row, _vpos._col + tmp.size()});
    }

    template <std::size_t R, std::size_t C>
    void collect(
        screen<R, C> &scr,
        std::map<std::string, std::string> &vals)
    {
      scr.move().at(_vpos);

      std::string value;

      bool there_is_newline = false;

      while (!there_is_newline)
      {
        keyevent ev = getkey();

        scr.move().at(_vpos);
        scr.writer().write_n(value.size(), ' ');

        if (ev.index() == case_of<char, keyevent>)
        {
          char c = std::get<char>(ev);

          if (c == '\n')
          {
            there_is_newline = true;
          }
          else if (c == 127)
          {
            if (!value.empty())
            {
              value.pop_back();
            }
          }
          else if (std::isprint(c) && value.size() < _max_length)
          {
            value.push_back(c);
          }
        }

        scr.move().at(_vpos);

        std::size_t total = value.size();
        std::size_t width = std::min(value.size(), C - _vpos._col);
        std::size_t start = 0;

        if (total > width)
        {
          start = total - width;
        }
        else
        {
          start = 0;
        }

        std::string_view tmp = std::string_view(value).substr(start, width);

        for (char c : tmp)
        {
          scr.writer().write(c);
        }

        scr.move().at(position{_vpos._row, _vpos._col + tmp.size()});
      }

      vals[_label] = value;
    }

    template <std::size_t R, std::size_t C>
    void notify(screen<R, C> &scr, std::map<std::string, std::string> &v)
    {
      scr.move().at(_vpos);
      std::string &value = v[_label];
      std::size_t width = std::min(value.size(), C - _vpos._col);

      for (char c : std::string_view(value).substr(0, width))
      {
        scr.writer().write(c);
      }
    }
  };

  class hidden_text_field
  {
    std::string _label;
    std::size_t _max_length;
    char _mask;

    position _vpos;

  public:
    ~hidden_text_field() = default;
    hidden_text_field() = default;
    hidden_text_field(const std::string &label, std::size_t max_length = 10, char mask = '*');
    hidden_text_field(const hidden_text_field &) = default;
    hidden_text_field(hidden_text_field &&) = default;
    hidden_text_field &operator=(const hidden_text_field &) = default;
    hidden_text_field &operator=(hidden_text_field &&) = default;

  public:
    template <std::size_t R, std::size_t C>
    void render(screen<R, C> &scr)
    {
      for (auto c : std::format("+ {} : ", _label))
      {
        scr.writer().write(c);
      }

      _vpos = scr.pos();
      scr.writer().write('\n');
    }

    template <std::size_t R, std::size_t C>
    void collect(
        screen<R, C> &scr,
        std::map<std::string, std::string> &vals)
    {
      scr.move().at(_vpos);

      std::string value;

      bool there_is_newline = false;

      while (!there_is_newline)
      {
        keyevent ev = getkey();

        scr.move().at(_vpos);
        scr.writer().write_n(value.size(), ' ');

        if (ev.index() == case_of<char, keyevent>)
        {
          char c = std::get<char>(ev);

          if (c == '\n')
          {
            there_is_newline = true;
          }
          else if (c == 127)
          {
            if (!value.empty())
            {
              value.pop_back();
            }
          }
          else if (std::isprint(c) && value.size() < _max_length)
          {
            value.push_back(c);
          }
        }

        scr.move().at(_vpos);

        std::size_t total = value.size();
        std::size_t width = std::min(value.size(), C - _vpos._col);
        std::size_t start = 0;

        if (total > width)
        {
          start = total - width;
        }
        else
        {
          start = 0;
        }

        std::string_view tmp = std::string_view(value).substr(start, width);

        for (char c : tmp)
        {
          scr.writer().write(_mask);
        }

        scr.move().at(position{_vpos._row, _vpos._col + tmp.size()});
      }

      vals[_label] = value;
    }

    template <std::size_t R, std::size_t C>
    void notify(screen<R, C> &scr, std::map<std::string, std::string> &v)
    {
      scr.move().at(_vpos);

      std::string &value = v[_label];
      std::size_t width = std::min(value.size(), C - _vpos._col);

      for (char c : std::string_view(value).substr(0, width))
      {
        scr.writer().write(_mask);
      }
    }
  };

  class text_message
  {
    position _vpos;

  public:
    ~text_message() = default;
    text_message() = default;
    text_message(const text_message &) = default;
    text_message(text_message &&) = default;
    text_message &operator=(const text_message &) = default;
    text_message &operator=(text_message &&) = default;

  public:
    template <std::size_t R, std::size_t C>
    void render(screen<R, C> &scr)
    {
      for (char c : std::string_view("(I) : "))
      {
        scr.writer().write(c);
      }

      _vpos = scr.pos();
    }

    template <std::size_t R, std::size_t C>
    void notify(const message &mess, screen<R, C> &scr)
    {
      scr.move().at(_vpos);

      for (char c : mess._data)
      {
        scr.writer().write(c);
      }
    }
  };

}

#endif