#ifndef __steward_ui_widget_hpp__
#define __steward_ui_widget_hpp__

#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <list>
#include <memory>

namespace stew::ui
{

  struct widget_drawing
  {
    std::vector<std::string> drawing;
    std::vector<std::string> markers;

  public:
    template <typename... T>
    void drawln(T &&...t);

    void mark(const std::string &id);
  };

  template <typename... T>
  void widget_drawing::drawln(T &&...t)
  {
    std::stringstream ss;
    (ss << ... << std::forward<T>(t));
    drawing.push_back(ss.str());
  }

  class widget;

  class widget_factory
  {
    std::list<std::unique_ptr<widget>> ws;

  public:
    template <typename W, typename... ARGS>
    W *build(ARGS &&...args) requires std::derived_from<W, widget>
    {
      ws.push_back(std::make_unique<W>(std::forward<ARGS>(args)...));
      return static_cast<W *>(ws.back().get());
    }
  };

  class widget
  {
    widget *_parent = nullptr;
    std::list<widget *> _childs;

  public:
    widget(widget *parent);
    virtual ~widget() = default;

  public:
    virtual widget_drawing paint() const = 0;

    virtual void notify(
        std::string_view id,
        std::string_view value) = 0;

  protected:
    std::list<widget *> &childs();
    const std::list<widget *> &childs() const;

  private:
    void child(widget *ch);
  };

  class input : public widget
  {
    std::string _label;
    std::string _value;

  public:
    input(widget *parent,
          std::string_view label);
    virtual ~input() = default;

  public:
    const std::string &label() const;
    const std::string &value() const;

  public:
    virtual widget_drawing paint() const override;

    virtual void notify(
        std::string_view id,
        std::string_view value) override;
  };

  class form : public widget
  {
    std::string _name;

  public:
    form(widget *parent, std::string_view name);
    virtual ~form() = default;

  public:
    virtual widget_drawing paint() const override;

    virtual void notify(
        std::string_view id,
        std::string_view value) override;
  };

  class vlayout : public widget
  {

  public:
    vlayout(widget *parent);
    virtual ~vlayout() = default;

    void push(std::unique_ptr<widget> w);

    virtual widget_drawing paint() const override;
    virtual void notify(
        std::string_view id,
        std::string_view value) override;
  };

  class hlayout : public widget
  {
  public:
    hlayout(widget *parent);
    virtual ~hlayout() = default;

  public:
    virtual widget_drawing paint() const override;

    virtual void notify(
        std::string_view id,
        std::string_view value) override;
  };

  class menu : public stew::ui::widget
  {
    std::string _name;
    std::vector<std::string> _items;
    std::size_t _choice = 0;

  public:
    menu(widget *parent,
         std::string_view name,
         const std::vector<std::string> &items);
    virtual ~menu() = default;

  public:
    virtual widget_drawing paint() const override;

    virtual void notify(
        std::string_view id,
        std::string_view value) override;
  };
}

#endif