#ifndef __stew_ui_view_hpp__
#define __stew_ui_view_hpp__

#include <ui-grid.hpp>
#include <ui-pencil.hpp>
#include <ui-event.hpp>
#include <ui-screen.hpp>

namespace stew::ui
{
  class view
  {
    bus &_bs;
    screen &_scr;

    grid _grd;
    bool _showing = false;

  public:
    view(screen &scr, bus &bs);
    virtual ~view() = default;

  public:
    virtual void draw() = 0;

    void show();
    void emit();

  protected:
    pencil pen();
  };

  view::view(screen &scr, bus &bs)
      : _scr(scr), _bs(bs) {}

  void view::show()
  {
    if (!_showing)
    {
      _grd.to_screen(_scr);
      _showing = true;
    }
  }

  void view::emit()
  {
    if (_showing)
    {
      _grd.from_screen(_scr, _bs);
    }
  }

  pencil view::pen()
  {
    return pencil(_grd);
  }
}

namespace stew::ui
{
  class sso_view : public view
  {
  public:
    sso_view(screen &scr, bus &bs);
    virtual ~sso_view() = default;

  public:
    virtual void draw() override;
  };

  sso_view::sso_view(screen &scr, bus &bs)
      : view(scr, bs)
  {
  }

  void sso_view::draw()
  {
    using stm = style_text_mode;
    auto &&p = pen();
    p.style_text("-------------------------\n", {stm::back_blue, stm::fore_white})
        .style_text("-- email : ", {stm::back_blue, stm::fore_white})
        .marker("email", '%')
        .text("\n")
        .style_text("-- login : ", {stm::back_blue, stm::fore_white})
        .marker("login", '%')
        .text("\n")
        .style_text("-- password : ", {stm::back_blue, stm::fore_white})
        .hidden("password", '%')
        .text("\n");
  }
}

#endif