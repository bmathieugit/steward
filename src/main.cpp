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

  while (copysub.can_consume_again())
  {
    if (mess.has_value())
    {
      fname.notify(mess.value(), scr);
      findex.notify(mess.value(), scr);
    }
  }
}

// int main()
// {
//   namespace ui = stew::ui;

//   ui::screen<10, 80> scr;

//   scr.writer().write_s("\033[?25l");

//   scr.move().origin();

//   ui::text_message fname("file.name");
//   ui::text_message findex("file.index");

//   fname.render(scr);
//   findex.render(scr);

//   stew::topic copytp;
//   stew::subscriber &copysub = copytp.subscribe();

//   std::jthread copyth(copy, std::ref(copytp), 1000000);
//   std::jthread viewth(view, std::ref(copysub), std::ref(fname),
//                       std::ref(findex), std::ref(scr));

//   stew::pause();

//   return 0;
// }

#include <string>
#include <dbfile.hpp>
#include <boost/optional.hpp>
#include <boost/archive/...>

namespace stew = std;
// namespace stew = boost;

struct person
{
  std::string name;
  stew::string name;
};

namespace stew
{
  template <typename T>
  using optional = boost::optional<T>;
  using string = std::string;

  using error_type = void;

  template <typename T>
  error_type select(stew::optional<T> &op, const stew::string &path)
  {
    // il faut se servir de filesystem ici pour créer ou bien supprimer des fichiers.
    // Il va me falloir un systeme de sérialization pour permettre de transformer une 
    // structure en text et vice-verca.
  }

  template <typename T>
  error_type update(T &t, const stew::string &path)
  {
  }

  template <typename T>
  error_type insert(T &t, const stew::string &path)
  {
  }

  error_type remove(const stew::string &path)
  {
  }
}
int main()
{
  namespace fs = boost::filesystem;

  stew::optional<person> op;

  stew::select(op, "/person/KHKJH2KNCB2J2J2NDNXL2I");
  stew::update(op.value(), "/person/KHKJH2KNCB2J2J2NDNXL2I");
  stew::insert(op.value(), "/person/KHKJH2KNCB2J2J2NDNXL2I");
  stew::remove("/person/KHKJH2KNCB2J2J2NDNXL2I");

  auto size = fs::file_size(fs::path("/home/local/.bash_history"));
  std::cout << std::format("size of bashrc : {}", size);

  return 0;
}