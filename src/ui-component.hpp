#ifndef __stew_ui_component_hpp__
#define __stew_ui_component_hpp__

#include <ui.hpp>
#include <format.hpp>
#include <cstddef>
#include <string>
#include <map>

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
    position _vpos;

  public:
    ~text_field() = default;
    text_field() = default;
    text_field(const std::string &label);
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
    void collect(
        screen<R, C> &scr,
        std::map<std::string, std::string> &vals)
    {
      scr.move().at(_vpos);

      std::string val;

      while (true)
      {
        keyevent ev = getkey();

        switch (ev.index())
        {
        case case_of<arrow_event, keyevent>:

          switch (std::get<arrow_event>(ev))
          {
          case arrow_event::LEFT:
            scr.move().left();
            break;
          case arrow_event::RIGHT:
            scr.move().right();
            break;
          case arrow_event::DOWN:
          case arrow_event::UP:
          default:
            break;
          };

          break;

        case case_of<char, keyevent>:

          if (std::get<char>(ev) == 127) // BACKSPACE
          {
            if (!val.empty())
            {
              scr.move().left();
              scr.writer().write(' ');
              scr.move().left();
              val.pop_back();
            }
          }
          else if (std::isprint(std::get<char>(ev)))
          {
            scr.writer().write(std::get<char>(ev));
            val += std::get<char>(ev);
          }

          break;
        }
      }

      scr.writer().write('\n');
      vals[_label] = val;
    }

    template <std::size_t R, std::size_t C>
    void notify(screen<R, C> &scr, std::map<std::string, std::string> &v)
    {
      scr.move().at(_vpos);

      for (char c : v[_label])
      {
        scr.writer().write(c);
      }
    }
  };

  class hidden_text_field
  {
    std::string _label;
    position _vpos;

  public:
    ~hidden_text_field() = default;
    hidden_text_field() = default;
    hidden_text_field(const std::string &labesl);
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

      std::string val;

      scr.writer().write('\n');
      vals[_label] = val;
    }

    template <std::size_t R, std::size_t C>
    void notify(screen<R, C> &scr, std::map<std::string, std::string> &v)
    {
      scr.move().at(_vpos);

      for (char c : v[_label])
      {
        scr.writer().write('*');
      }
    }
  };

}

#endif