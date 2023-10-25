
#include <core/char-istream.hpp>
#include <core/char-ostream.hpp>
#include <core/io.hpp>
#include <tests.hpp>

void test_file_input_stream() {
  const char* filename = "test_file.txt";
  FILE* _file = fopen(filename, "w");
  fputs("Hello\n", _file);
  fclose(_file);

  typed_file<char, mode::r> f(filename);
  auto fs = istream(f);
  maybe<char> c;
  fs >> c;
  N_TEST_ASSERT_TRUE(c.has());
  N_TEST_ASSERT_EQUALS(c.get(), 'H');
  fs >> c;
  N_TEST_ASSERT_TRUE(c.has());
  N_TEST_ASSERT_EQUALS(c.get(), 'e');
  fs >> c;
  N_TEST_ASSERT_TRUE(c.has());
  N_TEST_ASSERT_EQUALS(c.get(), 'l');
  fs >> c;
  N_TEST_ASSERT_TRUE(c.has());
  N_TEST_ASSERT_EQUALS(c.get(), 'l');
  fs >> c;
  N_TEST_ASSERT_TRUE(c.has());
  N_TEST_ASSERT_EQUALS(c.get(), 'o');
  fs >> c;
  N_TEST_ASSERT_TRUE(c.has());
  N_TEST_ASSERT_EQUALS(c.get(), '\n');

  remove(filename);
}

void test_file_output_stream() {
  const char* filename = "test_file.txt";
  typed_file<char, mode::w> f(filename);
  auto fo = ostream(f);
  fo << "coucou";
  N_TEST_ASSERT_EQUALS(f.len(), 6);
  remove(filename);
}

// Main function to run the tests
int main() {
  N_TEST_SUITE("IO file test suite")

  N_TEST_REGISTER(test_file_input_stream);
  N_TEST_REGISTER(test_file_output_stream);

  N_TEST_RUN_SUITE;

  raw_file<mode::w> w("tmp.txt");
  w.open("tmp.txt");
  w.writeall(istream("coucou"));
  w.close();
  raw_file<mode::r> r("tmp.txt");
  r.open("tmp.txt");
  r.readall(sout);
  r.close();

  return 0;
}
