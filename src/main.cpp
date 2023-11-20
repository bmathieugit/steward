#include <core/chars.hpp>
#include <core/file.hpp>
#include <core/list.hpp>

int main(int argc, char** argv) {
  list<int> l;
  
  l.add(0);
  l.add(1);
  l.add(2);
  l.add(3);
  l.add(4);
  l.add(5);

  auto i = iter(l);

  while (i.has()) {
    write(sout, i.next(), '\n');
  }

  l.remove(2);

  write(sout, '\n');  
  auto i2 = iter(l);

  while (i2.has()) {
    write(sout, i2.next(), '\n');
  }


  return 0;
}
