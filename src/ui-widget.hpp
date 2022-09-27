#ifndef __steward_ui_widget_hpp__
#define __steward_ui_widget_hpp__

#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <list>
#include <memory>
#include <functional>

#include <ui-pencil.hpp>
//#include <ui-marker.hpp>

namespace stew::ui
{
  class widget;

  class widget_factory
  {
    std::list<std::unique_ptr<widget>> ws;

  public:
    template <typename W, typename... ARGS>
    W *build(widget *parent, ARGS &&...args) requires std::derived_from<W, widget>
    {
      ws.push_back(std::make_unique<W>(parent, std::forward<ARGS>(args)...));
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
    virtual pencil paint() = 0;

  protected:
    std::list<widget *> &childs();
    const std::list<widget *> &childs() const;

  private:
    void child(widget *ch);
  };

  class input : public widget
  {
    std::string _label;
    std::function<bool(std::string_view)> _validator;

  public:
    input(widget *parent,
          std::string_view label,
          std::function<bool(std::string_view)> validator);
    virtual ~input() = default;

  public:
    const std::string &label() const;

  public:
    virtual pencil paint() override;
  };

  class form : public widget
  {
    std::string _name;

  public:
    form(widget *parent, std::string_view name);
    virtual ~form() = default;

  public:
    virtual pencil paint() override;
  };

  class vlayout : public widget
  {

  public:
    vlayout(widget *parent);
    virtual ~vlayout() = default;

    void push(std::unique_ptr<widget> w);

    virtual pencil paint() override;
  };

  class menu : public stew::ui::widget
  {
    std::string _name;
    std::vector<std::string> _items;

  public:
    menu(widget *parent,
         std::string_view name,
         const std::vector<std::string> &items);
    virtual ~menu() = default;

  public:
    virtual pencil paint() override;
  };

}

#endif