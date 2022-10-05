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
  ui::bus_guard bguard(bs);

  ui::screen scr;
  copy_view cpv;

  std::jthread prod([&bguard]
                    {
    for (int i=0; i<10000; i++)
    {
      bguard.push("fcopy", ui::message{._data=std::to_string(i)+"/file"});
    } });

  std::jthread cons([&bguard, &scr, &cpv]
                    {
    for (int i=0; i<1000; i++)
    {
      std::optional<ui::message> mess =  bguard.pop("fcopy");
      if (mess.has_value())
      {
        cpv._filename = mess.value()._data;
    cpv.draw();
    cpv.show(scr);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    cpv.hide(scr);
      }
    } });

  prod.join();
  cons.join();

  std::string dodo;
  std::cin >> dodo;

  return EXIT_SUCCESS;
}
