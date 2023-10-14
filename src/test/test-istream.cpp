#include <core/string-istream.hpp>
#include <core/string.hpp>
#include <core/tests.hpp>

/*bool operator==(const string<char>& s, const char* s1) {
  return equal(s.iter(), pointer_iterator(s1, strlen(s1)));
}*/

void test_extract_specific_pattern3() {
  auto ss = iter("cba12Y");

  maybe<char> c, b, a;
  ss >> c >> b >> a;

  if (c.has() and b.has() and a.has()) {
    printf("%c, %c, %c\n", c.get(), b.get(), a.get());
  }

  maybe<int> i;
  ss >> i;

  if (i.has()) {
    printf("%d \n", i.get());
  }

  maybe<bool> bl = false;
  ss >> bl;

  if (bl.has()) {
    printf("%c\n", bl.get() ? 'Y' : 'N');
  }

  maybe<expected_char<char>> exc = expected_char<char>{'B'};
  ss >> exc;

  if (exc.has()) {
    printf("il y bien B dans la chaine\n");
  }
}

int main() {
  N_TEST_SUITE("istream Tests")
  N_TEST_REGISTER(test_extract_specific_pattern3)
  N_TEST_RUN_SUITE
}
