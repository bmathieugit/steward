#include <ui-widget.hpp>
#include <sstream>
#include <iostream>

namespace stew::ui
{

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

  pencil input::paint()
  {
    return std::move(pencil(this)
        .textln("----------------")
        .text(" + ", _label, " : ").mark("login").textln());
  }

  form::form(widget *parent, std::string_view name)
      : widget(parent), _name(name) {}

  pencil form::paint()
  {
    pencil pen(this);
    pen.textln("------------------------");
    pen.textln("--- ", _name);

    for (auto &child : childs())
    {
      pen.merge(child->paint());
    }

    return std::move(pen);
  }

  vlayout::vlayout(widget *parent)
      : widget(parent) {}

  pencil vlayout::paint()
  {
    pencil pen(this);

    for (auto &child : childs())
    {
      pen.merge(child->paint());
    }

    return std::move(pen);
  }

  
  menu::menu(
      widget *parent,
      std::string_view name,
      const std::vector<std::string> &items)
      : widget(parent), _name(name), _items(items) {}

  pencil menu::paint()
  {
    pencil pen(this);
    pen.textln("########################");
    pen.textln("# ", _name);

    for (std::size_t i(1); i <= _items.size(); ++i)
    {
      pen.textln(i, ": --", _items[i-1]);
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

    pen.text(" ---> ? ").mark(_name).textln();

    return std::move(pen);
  }
}
