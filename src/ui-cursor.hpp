#ifndef __stew_ui_cursor_hpp__
#define __stew_ui_cursor_hpp__

#include <iostream>
#include <ui-position.hpp>

namespace stew::ui
{
  class cursor
  {
    std::ostream &_os;
    position _pos;

  public:
    ~cursor() = default;
    cursor(std::ostream &os);
    cursor(const cursor &) = default;
    cursor(cursor &&) = default;

  public:
    void erase();
    void up(std::size_t n);
    void down(std::size_t n);
    void left(std::size_t n);
    void right(std::size_t n);
    void origin();
    void at(position mpos);

  public:
    void backred_mode();
    void reset_mode();

  public:
    void pushc(char c);
    void pushln();

  public: 
    position pos() const;
  };

}

#endif