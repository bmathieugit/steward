#include <ui-view.hpp>

namespace stew::ui
{
  void view::show(screen &scr)
  {
    if (!_showing)
    {
      _grd.to_screen(scr);
      _showing = true;
    }
  }

  void view::emit(screen &scr, topic &tpc)
  {
    if (_showing)
    {
      _grd.from_screen(scr, tpc);
    }
  }

  void view::hide(screen &scr)
  {
    if (_showing)
    {
      scr.erase();
      scr.origin();
      _showing = false;
    }
  }

  pencil view::pen()
  {
    return pencil(_grd);
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


  void sso_view2::draw()
  {
    using stm = style_text_mode;

    auto &&p = pen();
    p.style_text("-------------------------\n", {stm::back_blue, stm::fore_white})
        .style_text("-- email2 : ", {stm::back_blue, stm::fore_white})
        .marker("email", '%')
        .text("\n")
        .style_text("-- login2 : ", {stm::back_blue, stm::fore_white})
        .marker("login", '%')
        .text("\n")
        .style_text("-- password2 : ", {stm::back_blue, stm::fore_white})
        .hidden("password", '%')
        .text("\n");
  }
}