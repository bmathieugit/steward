#ifndef __stew_ui_component_hpp__
#define __stew_ui_component_hpp__

#include <cstddef>
#include <functional>
#include <list>
#include <map>
#include <string>
#include <variant>

#include <ui.hpp>
#include <format.hpp>

namespace stew::ui
{
  template <typename T, typename V, std::size_t I = 0>
  constexpr std::size_t case_of_impl()
  {
    static_assert(std::variant_size_v<V> > I, "Type not found in variant");

    if constexpr (I == std::variant_size_v<V>)
    {
      return I;
    }
    else if constexpr (std::is_same_v<std::variant_alternative_t<i, V>, T>)
    {
      return I;
    }
    else
    {
      return case_of_impl<T, V, i + 1>();
    }
  }

  template <typename T, typename V>
  constexpr int case_of = case_of_impl<T, V>();

  template <typename T>
  constexpr int event_from = case_of<T, keyboard_event>;

  using component = subject;

  class text_field : public component
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
    void notify(keyboard_event ev, screen<R, C> &scr)
    {
      scr.move().at(_vpos);
      scr.writer().write_n(_value.size(), ' ');

      switch (ev.index())
      {
      case event_from<backspace_event>:

        if (!_value.empty())
        {
          _value.pop();
        }

        break;

      case event_from<enter_event>:
      {
        message mess;
        mess.append(std::format("{}.value", _label), _value);
        update(mess);
      }
      break;

      case event_from<char>:

        char c = std::get<char>(ev);

        if (std::isprint(c) && _value.size() < _max_length)
        {
          _value.push_back(c);
        }

        break;
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
  };

  class text_message : public component
  {
    std::string _key;
    std::string _previous;
    position _vpos;

  public:
    ~text_message() = default;
    text_message(const std::string &key);
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

      scr.writer().write('\n');
    }

    template <std::size_t R, std::size_t C>
    void notify(const message &mess, screen<R, C> &scr)
    {
      scr.move().at(_vpos);
      scr.writer().write_n(_previous.size(), ' ');
      scr.move().at(_vpos);
      _previous = mess.get(_key).value_or("");
      scr.writer().write_s(_previous);
    }
  };

  class gauge_field : public component
  {
    std::size_t _value = 0;
    position _vpos;

  public:
    ~gauge_field() = default;
    gauge_field() = default;
    gauge_field(const gauge_field &) = default;
    gauge_field(gauge_field &&) = default;
    gauge_field &operator=(const gauge_field &) = default;
    gauge_field &operator=(gauge_field &&) = default;

  public:
    template <std::size_t R, std::size_t C>
    void render(screen<R, C> &scr)
    {
      for (char c : std::string_view("\u2206\u2207"))
      {
        scr.writer().write(c);
      }

      _vpos = scr.pos();
      
      for (char c : std::to_string(_value))
      {
        scr.writer().write(' ');
      }


      scr.writer().write('\n');
    }

    template <std::size_t R, std::size_t C>
    void notify(keyboard_event ev, screen<R, C> &scr)
    {
      if (ev.index() == event_from<arrow_event>)
      {

        scr.move().at(_vpos);

        for (char c : std::to_string(_value))
        {
          scr.writer().write(' ');
        }

        arrow_event a = std::get<arrow_event>(ev);

        if (a == arrow_event::UP)
        {
          ++_value;
        }
        else if (a == arrow_event::DOWN)
        {
          if (_value > 0)
          {
            --_value;
          }
        }

        scr.move().at(_vpos);

        for (char c : std::to_string(_value))
        {
          scr.writer().write(c);
        }
      }
    }
  };

}

#endif