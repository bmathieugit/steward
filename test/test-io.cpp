
#include <core/io.hpp>
#include <core/result.hpp>
#include <core/string.hpp>
#include <core/vector.hpp>
#include <tests.hpp>

void test_file_iterator_readable_mode() {
  const char* filename = "test_file.txt";
  FILE* _file = fopen(filename, "w");
  fputs("Hello\n", _file);
  fclose(_file);

  file_istream<char> fs(filename);
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

// Main function to run the tests
int main() {
  N_TEST_SUITE("IO file test suite")

  N_TEST_REGISTER(test_file_iterator_readable_mode);
  // N_TEST_REGISTER(test_file_oterator_writable_mode);
  // N_TEST_REGISTER(test_file_pathable_mode);
  // N_TEST_REGISTER(test_file_stdin_mode);
  // N_TEST_REGISTER(test_file_stdout_mode);

  N_TEST_RUN_SUITE;

  return 0;
}
