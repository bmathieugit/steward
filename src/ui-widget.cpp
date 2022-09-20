#include <ui-widget.hpp>

namespace stew::ui
{
  void widget_drawing::mark(const std::string &id)
  {
    markers.push_back(id);
  }

  widget::widget(widget *parent)
      : _parent(parent)
  {
    if (parent != nullptr)
    {
      parent->child(this);
    }
  }

  std::list<widget *> &widget::childs()
  {
    return _childs;
  }

  const std::list<widget *> &widget::childs() const
  {
    return _childs;
  }

  void widget::child(widget *ch)
  {
    _childs.push_back(ch);
  }

  input::input(widget *parent,
               std::string_view lbl)
      : widget(parent), _label(lbl) {}

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
      std::string_view id,
      std::string_view value)
  {
    if (id == _label)
    {
      _value = value;
    }
  }

  form::form(widget *parent, std::string_view name)
      : widget(parent), _name(name) {}

  widget_drawing form::paint() const
  {
    widget_drawing wd;

    wd.drawln("------------------------");
    wd.drawln("--- ", _name);

    for (const auto &w : childs())
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
      std::string_view id,
      std::string_view value)
  {
    for (const auto &i : childs())
    {
      i->notify(id, value);
    }
  }

  vlayout::vlayout(widget *parent)
      : widget(parent) {}

  widget_drawing vlayout::paint() const
  {
    widget_drawing wd;

    for (const auto &w : childs())
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
      std::string_view id,
      std::string_view value)
  {
    for (auto &w : childs())
    {
      w->notify(id, value);
    }
  }

  hlayout::hlayout(widget *parent)
      : widget(parent) {}

  widget_drawing hlayout::paint() const
  {
    std::vector<widget_drawing> wds;

    for (auto &w : childs())
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
      std::string_view id,
      std::string_view value)
  {
    for (auto &w : childs())
    {
      w->notify(id, value);
    }
  }

  menu::menu(
      widget *parent,
      std::string_view name,
      const std::vector<std::string> &items)
      : widget(parent), _name(name), _items(items) {}

  widget_drawing menu::paint() const
  {
    widget_drawing wd;
    wd.drawln("########################");
    wd.drawln("# ", _name);

    for (std::size_t i(1); i <= _items.size(); ++i)
    {
      wd.drawln(i, ": -- ", _items[i - 1]);
    }

    wd.drawln(" ---> ? %");
    wd.mark(_name);

    return wd;
  }

  void menu::notify(
      std::string_view id,
      std::string_view value)
  {
    if (id == _name)
    {
      _choice = std::stoull(std::string(value));
    }
  }
}
