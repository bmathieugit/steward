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
}

#endif