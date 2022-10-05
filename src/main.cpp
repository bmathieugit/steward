#include <ui.hpp>

#include <thread>
#include <iostream>

namespace ui = stew::ui;

class copy_view : public ui::view
{
public:
  std::string _filename;
  // std::string _ratio;
public:
  virtual ~copy_view() = default;
  virtual void draw() override;
};

void copy_view::draw()
{
  auto p = pen();
  p.text("------------- copy file ------------\n")
      .text("--- copying : ")
      .text(_filename)
      .text("\n")
      .text("--- progressing : ")
      .message("progress", '%')
      .text("\n");
}

int main(int argc, char **argv)
{

  ui::bus bs;
  ui::producer prod(bs, "fcopy");
  ui::consumer cons(bs, "fcopy");

  ui::screen scr;
  copy_view cpv;

  

  for (int i = 0; i < 1000000; ++i)
  {
    std::string fname = "file";
    fname += std::to_string(i);
    prod.produce(ui::message{._data = fname});
    cpv._filename = cons.consume().value()._data;
    cpv.draw();
    cpv.show(scr);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    cpv.hide(scr);
  }

  std::string dodo;
  std::cin >> dodo;

  return EXIT_SUCCESS;
}
