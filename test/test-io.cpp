#include <core/io.hpp>
#include <tests.hpp>

void test_file_input_stream() {
  const char* filename = "test_file.txt";
  FILE* _file = fopen(filename, "w");
  fputs("Hello\n", _file);
  fclose(_file);

  file<char, mode::r> f(filename);
  auto fs = iter(f);
  maybe<char> c;
  extract(fs, c);
  N_TEST_ASSERT_TRUE(c.has());
  N_TEST_ASSERT_EQUALS(c.get(), 'H');
  extract(fs, c);
  N_TEST_ASSERT_TRUE(c.has());
  N_TEST_ASSERT_EQUALS(c.get(), 'e');
  extract(fs, c);
  N_TEST_ASSERT_TRUE(c.has());
  N_TEST_ASSERT_EQUALS(c.get(), 'l');
  extract(fs, c);
  N_TEST_ASSERT_TRUE(c.has());
  N_TEST_ASSERT_EQUALS(c.get(), 'l');
  extract(fs, c);
  N_TEST_ASSERT_TRUE(c.has());
  N_TEST_ASSERT_EQUALS(c.get(), 'o');
  extract(fs, c);
  N_TEST_ASSERT_TRUE(c.has());
  N_TEST_ASSERT_EQUALS(c.get(), '\n');

  remove(filename);
}

void test_file_output_stream() {
  const char* filename = "test_file.txt";
  file<char, mode::w> f(filename);
  auto fo = oter(f);
  insert(fo, "coucou");
  N_TEST_ASSERT_EQUALS(f.len(), 6);
  remove(filename);
}

// Main function to run the tests
int main() {
  N_TEST_SUITE("IO file test suite")

  N_TEST_REGISTER(test_file_input_stream);
  N_TEST_REGISTER(test_file_output_stream);

  N_TEST_RUN_SUITE;

  return 0;
}
