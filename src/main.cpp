#include <ui.hpp>
#include <ui-component.hpp>

#include <event.hpp>
#include <format.hpp>

#include <thread>
#include <iostream>

constexpr int maxi = 1000000;

namespace stew
{
  void pause()
  {
    std::string str;
    std::getline(std::cin, str);
  }
}

void copy(stew::topic &copytp, std::size_t n)
{
  using namespace std::chrono_literals;

  for (std::size_t i(0); i < n; ++i)
  {
    stew::message mess;
    mess.append("file.name", std::format("/tmp/file-to-copy-{}.txt", i));
    mess.append("file.index", std::to_string(i));
    copytp.post(mess);
  }

  copytp.close();
}

void view(
    stew::subscriber &copysub,
    stew::ui::text_message &fname,
    stew::ui::text_message &findex,
    stew::ui::screen<10, 80> &scr)
{
  std::optional<stew::message> mess;

  while (!copysub.closed() || (mess = copysub.consume()).has_value())
  {
    if (mess.has_value())
    {
      fname.notify(mess.value(), scr);
      findex.notify(mess.value(), scr);
    }
  }
}

int main()
{
  namespace ui = stew::ui;

  ui::screen<10, 80> scr;

  scr.writer().write_s("\033[?25l");

  scr.move().origin();

  ui::text_message fname("file.name");
  ui::text_message findex("file.index");

  fname.render(scr);
  findex.render(scr);

  stew::topic copytp;
  stew::subscriber &copysub = copytp.subscribe();

  std::jthread copyth(copy, std::ref(copytp), 1000000);
  std::jthread viewth(view, std::ref(copysub), std::ref(fname),
                      std::ref(findex), std::ref(scr));

  stew::pause();

  return 0;
}
