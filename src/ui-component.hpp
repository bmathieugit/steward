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
      std::list<char> head;
      std::list<char> tail;

      position cur = _vpos;

      scr.move().at(_vpos);

      bool there_is_newline = false;

      while (!there_is_newline)
      {
        keyevent ev = getkey();

        scr.move().at(_vpos);
        scr.writer().write_n(head.size() + tail.size());

        if (ev.index() == case_of<arrow_event, keyevent>)
        {
          arrow_event ae = std::get<arrow_event>(ev);

          if (ae == arrow_event::LEFT)
          {
            if (!head.empty())
            {
              scr.move().left();
              tail.push_front(head.back());
              head.pop_back();
            }
          }
          else if (ae == arrow_event::RIGHT)
          {
            if (!tail.empty())
            {
              scr.move().right();
              head.push_back(tail.front());
              tail.pop_front();
            }
          }
        }
        else if (ev.index() == case_of<char, keyevent>)
        {
          char c = std::get<char>(ev);

          if (c == '\n')
          {
            there_is_newline = true;
          }
          else if (c == 127)
          {
            head.pop_back();
          }
          else if (std::isprint(c))
          {
            head.push_back(c);
          }
        }

        scr.move().at(_vpos);
        
        for (char c : head)
        {
          scr.writer().write(c);
        }

        for (char c : tail)
        {
          scr.writer().write(c);
        }

        scr.move().at(position{_vpos._row, _vpos._col + head.size()});
      }

      scr.writer().write('\n');
      vals[_label] = std::string(head.begin(), head.end()) + std::string(tail.begin(), tail.end());
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