#include <ui-widget.hpp>
#include <sstream>
#include <iostream>

namespace stew::ui
{
  void widget_drawing::mark(
      const std::string &id,
      std::function<bool(std::string_view)> validator)
  {
    markers.push_back(marker{id, validator});
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
               std::string_view lbl,
               std::function<bool(std::string_view)> validator)
      : widget(parent), _label(lbl), _validator(validator) {}

  const std::string &input::label() const
  {
    return _label;
  }

  widget_drawing input::paint() const
  {
    widget_drawing wd;
    wd.drawln("----------------");
    wd.drawln(" + ", _label, " : %");
    wd.mark(_label, _validator);
    return wd;
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

    auto is_valid_choice = [nb = _items.size()](std::string_view v)
    {
      bool is_valid = true;

      for (auto c : v)
      {
        if (!('0' <= c && c <= '9'))
        {
          is_valid = false;
          break;
        }
      }

      if (is_valid)
      {
        std::size_t tmp;
        std::stringstream ss = std::stringstream(std::string(v));
        ss >> tmp;
        return 0 < tmp && tmp <= nb;
      }
      else
      {
        return false;
      }
    };

    wd.drawln(" ---> ? %");
    wd.mark(_name, is_valid_choice);

    return wd;
  }
}
