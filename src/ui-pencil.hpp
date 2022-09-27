#ifndef __stew_pencil_hpp__
#define __stew_pencil_hpp__

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <string_view>

namespace stew::ui
{
  class widget;

  class sketch_content
  {
    widget *_w = nullptr;

  public:
    virtual ~sketch_content() = default;
    sketch_content(widget *w) : _w(w) {}
  };

  struct sketch
  {
    std::vector<std::unique_ptr<sketch_content>> _contents;
  };

  class marker_sktcontent : public sketch_content
  {
    char _mark;

  public:
    virtual ~marker_sktcontent() = default;
    marker_sktcontent(widget *w, char mark) : sketch_content(w), _mark(mark) {}
  };

  class text_sktcontent : public sketch_content
  {
    std::string _text;

  public:
    virtual ~text_sktcontent() = default;
    text_sktcontent(widget *w, const std::string &text) : sketch_content(w), _text(text) {}
    text_sktcontent(widget *w, std::string &&text) : sketch_content(w), _text(text) {}
  };

  class style_sktcontent : public sketch_content
  {
    std::string _style;

  public:
    virtual ~style_sktcontent() = default;
    style_sktcontent(widget *w, const std::string &style) : sketch_content(w), _style(style) {}
    style_sktcontent(widget *w, std::string &&style) : sketch_content(w), _style(style) {}
  };

  struct pencil
  {
    widget *_widget;
    std::vector<std::unique_ptr<sketch_content>> _contents;

    sketch draw_sktech()
    {
      sketch skt = sketch{std::move(_contents)};
      _contents.clear();
      return std::move(skt);
    }

    pencil &mark(char c = '%')
    {
      _contents.push_back(std::make_unique<marker_sktcontent>(_widget, c));
      return *this;
    }

    pencil &text(const std::string &text)
    {
      _contents.push_back(std::make_unique<text_sktcontent>(_widget, text));
      return *this;
    }

    pencil &text(std::string &&text)
    {
      _contents.push_back(std::make_unique<text_sktcontent>(_widget, text));
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