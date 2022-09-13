#include <logger.hpp>
#include <csv.hpp>
#include <vault.hpp>
#include <ui.hpp>

#include <string>
#include <vector>
#include <bit>

#include <array>
#include <sstream>
#include <iostream>

class input
{
  std::string _label;
  std::string _value;

public:
  ~input() = default;
  explicit input(std::string_view lbl)
      : _label(lbl) {}
  input(const input &) = default;
  input(input &&) = default;
  input &operator=(const input &) = default;
  input &operator=(input &&) = default;

public:
  const std::string &label() const
  {
    return _label;
  }

  const std::string &value() const
  {
    return _value;
  }

public:
  stew::ui::draw::widget_drawing paint() const
  {
    stew::ui::draw::widget_drawing wd;
    wd.drawing.push_back("----------------");
    wd.drawing.push_back(" + ");
    wd.drawing.back() += _label + " : % ";
    wd.markers.push_back(_label);
    wd.markers.push_back(_label + "2");
    return wd;
  }

  void notify_value(
      const std::string &id,
      const std::string &value)
  {
    if (id == _label)
    {
      _value = value;
    }
  }
};

struct vlayout
{
  stew::ui::widget::form l = stew::ui::widget::form("");
  stew::ui::widget::form r = stew::ui::widget::form("");

public:
  stew::ui::draw::widget_drawing paint() const
  {
    stew::ui::draw::widget_drawing wd;

    auto &&wdl = l.paint();
    auto &&wdr = r.paint();

    wd.drawing.insert(
        wd.drawing.end(),
        wdl.drawing.begin(),
        wdl.drawing.end());

    wd.drawing.insert(
        wd.drawing.end(),
        wdr.drawing.begin(),
        wdr.drawing.end());

    wd.markers.insert(
        wd.markers.end(),
        wdl.markers.begin(),
        wdl.markers.end());

    wd.markers.insert(
        wd.markers.end(),
        wdr.markers.begin(),
        wdr.markers.end());

    return wd;
  }

  void notify_value(
      const std::string &id,
      const std::string &value)
  {
    l.notify_value(id, value);
    r.notify_value(id, value);
  }
};

struct hlayout
{
  stew::ui::widget::form l = stew::ui::widget::form("");
  stew::ui::widget::form r = stew::ui::widget::form("");

public:
  stew::ui::draw::widget_drawing paint() const
  {
    stew::ui::draw::widget_drawing wd;

    auto &&wdl = l.paint();
    auto &&wdr = r.paint();

    std::size_t maxl =
        (*std::max_element(
             wdl.drawing.begin(),
             wdl.drawing.end(),
             [](const std::string &s0,
                const std::string &s1)
             {
               return s0.size() < s1.size();
             }))
            .size();

    for (auto &s : wdl.drawing)
    {
      s += std::string(maxl - s.size(), ' ');
    }

    std::size_t max =
        std::max(wdl.drawing.size(),
                 wdr.drawing.size());

    while (wdl.drawing.size() < max)
    {
      wdl.drawing.push_back(std::string(maxl, ' '));
    }

    while (wdr.drawing.size() < max)
    {
      wdr.drawing.push_back("");
    }

    for (std::size_t i(0); i < max; ++i)
    {
      wd.drawing.push_back(wdl.drawing[i] + wdr.drawing[i]);
    }

    wd.markers.insert(
        wd.markers.end(),
        wdl.markers.begin(),
        wdl.markers.end());

    wd.markers.insert(
        wd.markers.end(),
        wdr.markers.begin(),
        wdr.markers.end());

    return wd;
  }

  void notify_value(
      const std::string &id,
      const std::string &value)
  {
    l.notify_value(id, value);
    r.notify_value(id, value);
  }
};

int main(int argc, char **argv)
{
  namespace widget = stew::ui::widget;

  stew::ui::widget::form_builder fb;

  hlayout vl;
  vl.l = fb.name("enterprise identity")
             .input("name")
             .input("firstname")
             .input("email")
             .build();
  vl.r = fb.name("enterprise identity")
             .input("name2")
             .input("firstname2")
             .input("email2")
             .build();

  stew::ui::screen::screen scr;

  scr.paint(scr, vl);
  scr.notify_user_inputs(scr, vl);

  return EXIT_SUCCESS;
}
