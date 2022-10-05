#include <ui.hpp>

#include <thread>
#include <iostream>

namespace ui = stew::ui;

class copy_view : public ui::view
{
public:
  virtual ~copy_view() = default;
  virtual void draw() override;
};

void copy_view::draw()
{
  auto p = pen();
  p.text("------------- copy file ------------\n")
      .text("--- copying : ")
      //.message("current", '%')
      .text("\n")
      .text("--- progressing : ")
      //.message("progress", '%')
      .text("\n");
}

int main(int argc, char **argv)
{
  ui::screen scr;

  /*std::thread tcopy([&prod]
                    {
    for (int i=0; i < 100000; ++i)
    {
      prod.produce(ui::message{._data = std::string("file:/tmp/file_")+std::to_string(i)+".txt"});
    } });
*/


  copy_view cp;
  cp.show(scr);

  std::string dodo;
  std::cin >> dodo;
  // tcopy.join();

  return EXIT_SUCCESS;
}
