#ifndef __steward_ui_widget_hpp__
#define __steward_ui_widget_hpp__

#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <memory>

namespace stew::ui
{
  struct widget_drawing
  {
    std::vector<std::string> drawing;
    std::vector<std::string> markers;

  public:
    template <typename... T>
    void drawln(T &&...t)
    {
      std::stringstream ss;
      (ss << ... << t);
      drawing.push_back(ss.str());
    }

    void mark(const std::string &id)
    {
      markers.push_back(id);
    }
  };

  class widget
  {
  public:
    virtual ~widget() = default;
    virtual widget_drawing paint() const = 0;
    virtual void notify(
        const std::string &id,
        const std::string &value) = 0;
  };

  class input : public widget
  {
    std::string _label;
    std::string _value;

  public:
    virtual ~input() = default;
    input(std::string_view lbl = "");
    input(const input &) = default;
    input(input &&) = default;
    input &operator=(const input &) = default;
    input &operator=(input &&) = default;
    const std::string &label() const;
    const std::string &value() const;
    virtual widget_drawing paint() const override;
    virtual void notify(
        const std::string &id,
        const std::string &value) override;
  };

  class form : public widget
  {
    std::string _name;
    std::vector<std::unique_ptr<widget>> _inputs;

  public:
    virtual ~form() = default;
    explicit form(std::string_view name);
    form(const form &) = default;
    form(form &&) = default;
    form &operator=(const form &) = default;
    form &operator=(form &&) = default;
    void push(std::unique_ptr<widget> w);
    std::vector<std::unique_ptr<widget>> &inputs();
    virtual widget_drawing paint() const override;
    virtual void notify(
        const std::string &id,
        const std::string &value) override;
  };

  class vlayout : public widget
  {
    std::vector<std::unique_ptr<widget>> _widgets;

  public:
    virtual ~vlayout() = default;
    void push(std::unique_ptr<widget> w);
    virtual widget_drawing paint() const override;
    virtual void notify(
        const std::string &id,
        const std::string &value) override;
  };

  class hlayout : public widget
  {
    std::vector<std::unique_ptr<widget>> _widgets;

  public:
    virtual ~hlayout() = default;

    void push(std::unique_ptr<widget> w);
    virtual widget_drawing paint() const override;
    virtual void notify(
        const std::string &id,
        const std::string &value);
  };
}

#endif