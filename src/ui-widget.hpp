#ifndef __steward_ui_widget_hpp__
#define __steward_ui_widget_hpp__

#include <string>
#include <sstream>
#include <string_view>
#include <vector>

namespace stew::ui::draw
{
  struct widget_drawing
  {
    std::string drawing;
    std::vector<std::string> markers;
  };
};

namespace stew::ui::widget
{
  template <typename W>
  concept widget = requires(const W &w, W& w2)
  {
    {
      w.paint()
      } -> std::same_as<draw::widget_drawing>;

    w2.notify_value(std::string(), std::string());
  };

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

    const std::string& value() const
    {
      return _value;
    }

  public:
    draw::widget_drawing paint() const
    {
      std::stringstream ss;
      ss << "----------------\n";
      ss << " + \033[1m" << _label << "\033[0m : %\n";
      return draw::widget_drawing{
          ss.str(), {_label}};
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

  class form
  {
    std::string _name;
    std::vector<input> _inputs;

  public:
    ~form() = default;
    explicit form(std::string_view name)
        : _name(name) {}
    form(const form &) = default;
    form(form &&) = default;
    form &operator=(const form &) = default;
    form &operator=(form &&) = default;

  public:
    void push(const input &i)
    {
      _inputs.push_back(i);
    }

    std::vector<input> &inputs()
    {
      return _inputs;
    }

    draw::widget_drawing paint() const
    {
      draw::widget_drawing wdf;

      std::stringstream ss;
      ss << "------------------------\n";
      ss << "--- " << _name << " ---\n";

      for (const widget::input &i : _inputs)
      {
        draw::widget_drawing wdi = i.paint();

        ss << wdi.drawing;
        wdf.markers.insert(
            wdf.markers.end(),
            wdi.markers.begin(),
            wdi.markers.end());
      }

      wdf.drawing = ss.str();

      return wdf;
    }

    void notify_value(
        const std::string &id,
        const std::string &value)
    {
      for (input &i : _inputs)
      {
        i.notify_value(id, value);
      }
    }
  };

  class form_builder
  {
    std::string _name;
    std::vector<std::string> _inputs_label;

  public:
    form_builder &name(std::string_view n)
    {
      _name = n;
      return *this;
    }

    form_builder &input(std::string_view lbl)
    {
      _inputs_label.emplace_back(lbl);
      return *this;
    }

    widget::form build() const
    {
      widget::form f(_name);

      for (const std::string &lbl : _inputs_label)
      {
        f.push(widget::input(lbl));
      }

      return f;
    }
  };
}

#endif