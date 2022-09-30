#include <ui-application.hpp>

namespace stew::ui
{
  void application::add_view(std::string_view id, std::ptr<view> &&v)
  {
    if (v)
    {
      std::ptr<view> &vin = _views[std::string(id)];
      vin = std::move(v);
      vin->draw();
    }
  }

  void application::show(std::string_view id)
  {
    if (_showing != nullptr)
    {
      _showing->hide(_scr);
    }
    
    std::ptr<view> &v = _views[std::string(id)];

    if (v)
    {
      v->show(_scr);
    }
  }

  void application::emit(std::string_view id)
  {
    std::ptr<view> &v = _views[std::string(id)];

    if (v)
    {
      ui::bus bs(_disp);
      v->emit(_scr, bs);
    }
  }
}
