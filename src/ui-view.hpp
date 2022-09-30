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
    grid _grd;
    bool _showing = false;

  public:
    virtual ~view() = default;

  public:
    virtual void draw() = 0;

    void show(screen& scr);
    void emit(screen& scr, bus& bs);
    void hide(screen& scr);

  protected:
    pencil pen();
  };

}

namespace stew::ui
{
  class sso_view : public view
  {
  public:;
    virtual ~sso_view() = default;

  public:
    virtual void draw() override;
  };

  class sso_view2 : public view
  {
  public:;
    virtual ~sso_view2() = default;

  public:
    virtual void draw() override;
  };
}

#endif