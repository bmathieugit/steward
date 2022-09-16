#include <ui-widget.hpp>

namespace stew::ui
{
  input::input(std::string_view lbl)
      : _label(lbl) {}

  const std::string &input::label() const
  {
    return _label;
  }

  const std::string &input::value() const
  {
    return _value;
  }

  widget_drawing input::paint() const
  {
    widget_drawing wd;
    wd.drawln("----------------");
    wd.drawln(" + ", _label, " : %");
    wd.mark(_label);
    return wd;
  }

  void input::notify(
      const std::string &id,
      const std::string &value)
  {
    if (id == _label)
    {
      _value = value;
    }
  }

  form::form(std::string_view name)
      : _name(name) {}

  void form::push(std::unique_ptr<widget> w)
  {
    _inputs.push_back(std::move(w));
  }

  std::vector<std::unique_ptr<widget>> &form::inputs()
  {
    return _inputs;
  }

  widget_drawing form::paint() const
  {
    widget_drawing wd;

    wd.drawln("------------------------");
    wd.drawln("--- ", _name);

    for (const auto &w : _inputs)
    {
      auto &&wdi = w->paint();
      wd.drawing.insert(
          wd.drawing.end(),
          wdi.drawing.begin(),
          wdi.drawing.end());
      wd.markers.insert(
          wd.markers.end(),
          wdi.markers.begin(),
          wdi.markers.end());
    }

    return wd;
  }

  void form::notify(
      const std::string &id,
      const std::string &value)
  {
    for (const auto &i : _inputs)
    {
      i->notify(id, value);
    }
  }

  void vlayout::push(std::unique_ptr<widget> w)
  {
    _widgets.push_back(std::move(w));
  }

  widget_drawing vlayout::paint() const
  {
    widget_drawing wd;

    for (const auto &w : _widgets)
    {
      auto &&wdw = w->paint();

      wd.drawing.insert(
          wd.drawing.end(),
          wdw.drawing.begin(),
          wdw.drawing.end());

      wd.markers.insert(
          wd.markers.end(),
          wdw.markers.begin(),
          wdw.markers.end());
    }

    return wd;
  }

  void vlayout::notify(
      const std::string &id,
      const std::string &value)
  {
    for (auto &w : _widgets)
    {
      w->notify(id, value);
    }
  }

  void hlayout::push(std::unique_ptr<widget> w)
  {
    _widgets.push_back(std::move(w));
  }

  widget_drawing hlayout::paint() const
  {
    std::vector<widget_drawing> wds;

    for (auto &w : _widgets)
    {
      wds.push_back(std::move(w->paint()));
    }

    std::size_t maxs(0);

    for (auto &wd : wds)
    {
      if (maxs < wd.drawing.size())
      {
        maxs = wd.drawing.size();
      }
    }

    for (auto &wd : wds)
    {
      std::size_t max(0);

      for (const std::string &s : wd.drawing)
      {
        if (max < s.size())
        {
          max = s.size();
        }
      }

      for (std::string &s : wd.drawing)
      {
        s += std::string(max - s.size(), ' ');
      }

      while (wd.drawing.size() < maxs)
      {
        wd.drawing.push_back(std::string(max, ' '));
      }
    }

    widget_drawing wdr;

    for (std::size_t i(0); i < maxs; ++i)
    {
      wdr.drawing.push_back("");

      for (const auto &wd : wds)
      {
        wdr.drawing.back() += wd.drawing[i];
      }
    }

    for (const auto &wd : wds)
    {
      wdr.markers.insert(
          wdr.markers.end(),
          wd.markers.begin(),
          wd.markers.end());
    }

    return wdr;
  }

  void hlayout::notify(
      const std::string &id,
      const std::string &value)
  {
    for (auto &w : _widgets)
    {
      w->notify(id, value);
    }
  }
}
