#ifndef __stew_pencil_hpp__
#define __stew_pencil_hpp__

#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <string>
#include <string_view>

#include <ui-screen-content.hpp>

namespace stew::ui
{
  class widget;

  class sketch_content
  {
    widget *_w = nullptr;

  public:
    virtual ~sketch_content() = default;
    sketch_content(widget *w) : _w(w) {}
    virtual void push_to(screen_content &content) = 0;
    widget *source() { return _w; }
  };

  struct sketch
  {
    std::vector<std::unique_ptr<sketch_content>> _contents;
  };

  class marker_sktcontent : public sketch_content
  {
    std::string _id;

  public:
    virtual ~marker_sktcontent() = default;
    marker_sktcontent(widget *w, const std::string &id) : sketch_content(w), _id(id) {}
    marker_sktcontent(widget *w, std::string &&id) : sketch_content(w), _id(id) {}

    virtual void push_to(screen_content &content) override
    {
    }
  };

  class text_sktcontent : public sketch_content
  {
    std::string _text;

  public:
    virtual ~text_sktcontent() = default;
    text_sktcontent(widget *w, const std::string &text) : sketch_content(w), _text(text) {}
    text_sktcontent(widget *w, std::string &&text) : sketch_content(w), _text(text) {}

    virtual void push_to(screen_content &content) override
    {
      for (char c : _text)
      {
        if (content._table.empty())
        {
          content._table.push_back({});
        }

        content._table.back().push_back(screen_cell{source(), c});
      }
    }
  };

  class style_sktcontent : public sketch_content
  {
    std::string _style;

  public:
    virtual ~style_sktcontent() = default;
    style_sktcontent(widget *w, const std::string &style) : sketch_content(w), _style(style) {}
    style_sktcontent(widget *w, std::string &&style) : sketch_content(w), _style(style) {}
    virtual void push_to(screen_content &content) override
    {
    }
  };

  class pencil
  {
    widget *_widget;
    std::vector<std::unique_ptr<sketch_content>> _contents;

  public:
    pencil(widget *w) : _widget(w) {}

  public:
    pencil &merge(pencil &&other)
    {
      for (auto &&content : other._contents)
      {
        _contents.push_back(std::move(content));
      }

      other._contents.clear();
      return *this;
    }

    sketch draw_sktech()
    {
      sketch skt = sketch{std::move(_contents)};
      _contents.clear();
      return std::move(skt);
    }

    pencil &mark(const std::string &id)
    {
      _contents.push_back(std::make_unique<marker_sktcontent>(_widget, id));
      return *this;
    }

    pencil &mark(std::string &&id)
    {
      _contents.push_back(std::make_unique<marker_sktcontent>(_widget, id));
      return *this;
    }

    template <typename... T>
    pencil &text(const T &...t)
    {
      std::stringstream ss;

      if (sizeof...(T) > 0)
      {
        (ss << ... << t);
      }

      _contents.push_back(std::make_unique<text_sktcontent>(_widget, std::move(ss.str())));
      return *this;
    }

    template <typename... T>
    pencil &textln(const T &...t)
    {
      std::stringstream ss;

      if (sizeof...(T) > 0)
      {
        (ss << ... << t);
      }

      ss << '\n';
      _contents.push_back(std::make_unique<text_sktcontent>(_widget, std::move(ss.str())));
      return *this;
    }

    pencil &style(const std::string &style)
    {
      _contents.push_back(std::make_unique<style_sktcontent>(_widget, style));
      return *this;
    }

    pencil &style(std::string &&style)
    {
      _contents.push_back(std::make_unique<style_sktcontent>(_widget, style));
      return *this;
    }

    pencil &back_red_mode()
    {
      return style("\033[41m");
    }

    pencil &back_green_mode()
    {
      return style("\033[42m");
    }

    pencil &back_yellow_mode()
    {
      return style("\033[43m");
    }

    pencil &back_blue_mode()
    {
      return style("\033[44m");
    }

    pencil &back_magenta_mode()
    {
      return style("\033[45m");
    }

    pencil &back_cyan_mode()
    {
      return style("\033[46m");
    }

    pencil &back_white_mode()
    {
      return style("\033[47m");
    }

    pencil &back_default_mode()
    {
      return style("\033[49m");
    }

    pencil &fore_red_mode()
    {
      return style("\033[31m");
    }

    pencil &fore_green_mode()
    {
      return style("\033[32m");
    }

    pencil &fore_yellow_mode()
    {
      return style("\033[33m");
    }

    pencil &fore_blue_mode()
    {
      return style("\033[34m");
    }

    pencil &fore_magenta_mode()
    {
      return style("\033[35m");
    }

    pencil &fore_cyan_mode()
    {
      return style("\033[36m");
    }

    pencil &fore_white_mode()
    {
      return style("\033[37m");
    }

    pencil &fore_default_mode()
    {
      return style("\033[39m");
    }

    pencil &reset_mode()
    {
      return style("\033[0m");
    }
  };
}

#endif