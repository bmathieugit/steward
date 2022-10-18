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
      std::string val;

      scr.move().at(_vpos);
      position cur = _vpos;

      bool is_newline = false;

      while (!is_newline)
      {
        keyevent ev = getkey();

        switch (ev.index())
        {
        case case_of<arrow_event, keyevent>:

          switch (std::get<arrow_event>(ev))
          {
          case arrow_event::LEFT:
            if (cur._col > _vpos._col)
            {
              scr.move().left();
              --cur._col;
            }
            break;
          case arrow_event::RIGHT:
            if (cur._col < val.size() && cur._col < C)
            {
              scr.move().right();
              ++cur._col;
            }
            break;
          case arrow_event::DOWN:
          case arrow_event::UP:
          default:
            break;
          };

          break;

        case case_of<char, keyevent>:

          switch (std::get<char>(ev))
          {
          case '\n':
            is_newline = true;
            break;
          case 127:
            // Ici on doit retirer le caractère courant de la string.
            if (cur._col > _vpos._col && !val.empty())
            {
              val.erase(scr.pos()._col - cur._col, 1);
              --cur._col;
            }
            break;
          default:
            if (cur._col < C)
            {
              ++cur._col;
              val.insert(cur._col - _vpos._col, std::get<char>(ev), 1);
            }
          }

          break;
        }

        scr.move().at(_vpos);

        for (char c : val)
        {
          scr.writer().write(' ');
        }

        scr.move().at(_vpos);
        
        for (char c : val)
        {
          scr.writer().write(c);
        }

        scr.move().at(cur);
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