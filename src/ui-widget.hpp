#ifndef __steward_ui_widget_hpp__
#define __steward_ui_widget_hpp__

#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <memory>

namespace stew::ui::draw
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
};

namespace stew::ui::widget
{
  class widget
  {
  public:
    virtual ~widget() {}
    virtual draw::widget_drawing paint() const = 0;
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

    input(std::string_view lbl = "")
        : _label(lbl) {}

    input(const input &) = default;

    input(input &&) = default;

    input &operator=(const input &) = default;

    input &operator=(input &&) = default;

    const std::string &label() const
    {
      return _label;
    }

    const std::string &value() const
    {
      return _value;
    }

    virtual draw::widget_drawing paint() const override
    {
      draw::widget_drawing wd;
      wd.drawln("----------------");
      wd.drawln(" + ", _label, " : %");
      wd.mark(_label);
      return wd;
    }

    virtual void notify(
        const std::string &id,
        const std::string &value) override
    {
      if (id == _label)
      {
        _value = value;
      }
    }
  };

  class form : public widget
  {
    std::string _name;
    std::vector<std::unique_ptr<widget>> _inputs;

  public:
    virtual ~form() = default;
    explicit form(std::string_view name)
        : _name(name) {}
    form(const form &) = default;
    form(form &&) = default;
    form &operator=(const form &) = default;
    form &operator=(form &&) = default;

    void push(std::unique_ptr<widget> w)
    {
      _inputs.push_back(std::move(w));
    }

    std::vector<std::unique_ptr<widget>> &inputs()
    {
      return _inputs;
    }

    draw::widget_drawing paint() const
    {
      draw::widget_drawing wd;

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

    void notify(
        const std::string &id,
        const std::string &value)
    {
      for (const auto &i : _inputs)
      {
        i->notify(id, value);
      }
    }
  };

  class vlayout : public widget
  {
    std::vector<std::unique_ptr<widget>> _widgets;

  public:
    virtual ~vlayout() = default;

    void push(std::unique_ptr<widget> w)
    {
      _widgets.push_back(std::move(w));
    }

    virtual stew::ui::draw::widget_drawing paint() const override
    {
      stew::ui::draw::widget_drawing wd;

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

    void notify(
        const std::string &id,
        const std::string &value)
    {
      for (auto &w : _widgets)
      {
        w->notify(id, value);
      }
    }
  };

  class hlayout : public widget
  {
    std::vector<std::unique_ptr<widget>> _widgets;

  public:
    virtual ~hlayout() = default;

    void push(std::unique_ptr<widget> w)
    {
      _widgets.push_back(std::move(w));
    }

    virtual stew::ui::draw::widget_drawing paint() const override
    {
      std::vector<draw::widget_drawing> wds;

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

      draw::widget_drawing wdr;

      for (std::size_t i(0); i<maxs; ++i)
      {
        wdr.drawing.push_back("");

        for (const auto& wd : wds)
        {
          wdr.drawing.back() += wd.drawing[i];
        }
      }

      for (const auto & wd : wds)
      {
        wdr.markers.insert(
          wdr.markers.end(),
          wd.markers.begin(), 
          wd.markers.end());
      }

      return wdr;
    }

    virtual void notify(
        const std::string &id,
        const std::string &value)
    {
      for (auto &w : _widgets)
      {
        w->notify(id, value);
      }
    }
  };
}

#endif