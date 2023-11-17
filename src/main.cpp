#include <core/chars.hpp>
#include <core/file.hpp>
#include <core/list.hpp>

int main(int argc, char** argv) {
  list<int> l;
  l.add(12);
  l.add(10);

  auto i = iter(l);

  while (i.has()) {
    write(sout, i.next(), '\n');
  }

  return 0;
}
