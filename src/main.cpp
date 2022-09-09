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
    std::stringstream ss;
    ss << "----------------\n";
    ss << " + " << _label << " : % ";
    ss << " + %\n";
    return stew::ui::draw::widget_drawing{
        ss.str(), {_label, _label + "2"}};
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

int main(int argc, char **argv)
{
  namespace widget = stew::ui::widget;

  stew::ui::widget::form_builder fb;

  auto f = fb.name("enterprise identity")
               .input("name")
               .input("firstname")
               .input("email")

               .build();
   input i("test");
 
  stew::ui::screen::screen scr;
  stew::ui::screen::screen_painter sp;
  stew::ui::screen::screen_user_input sui;

  sp.paint(scr, f, i);
  sui.notify_user_inputs(scr, f, i);

 

  return EXIT_SUCCESS;
}
