
#include <core/io.hpp>
#include <core/result.hpp>
#include <core/string.hpp>
#include <core/vector.hpp>
#include <tests.hpp>

void test_file_iterator_readable_mode() {
  const char* filename = "test_file.txt";
  FILE* _file = fopen(filename, "w+");
  fputs("Hello\n", _file);
  fclose(_file);

  file_reader<char> f(filename);
  auto it = iter(f);

  N_TEST_ASSERT_TRUE(it.has());
  N_TEST_ASSERT_EQUALS(it.get(), 'H');
  it.next();
  N_TEST_ASSERT_TRUE(it.has());
  N_TEST_ASSERT_EQUALS(it.get(), 'e');
  it.next();
  N_TEST_ASSERT_TRUE(it.has());
  N_TEST_ASSERT_EQUALS(it.get(), 'l');
  it.next();
  N_TEST_ASSERT_TRUE(it.has());
  N_TEST_ASSERT_EQUALS(it.get(), 'l');
  it.next();
  N_TEST_ASSERT_TRUE(it.has());
  N_TEST_ASSERT_EQUALS(it.get(), 'o');
  it.next();
  N_TEST_ASSERT_TRUE(it.has());
  N_TEST_ASSERT_EQUALS(it.get(), '\n');

  remove(filename);
}

// Test file_oterator (writable mode)
// void test_file_oterator_writable_mode() {
//   const char* filename = "test_file.txt";

//   {
//     file<char, mode::wp> f(filename);
//     auto ot = f.oter();

//     ot.sext('H');
//     ot.sext('e');
//     ot.sext('l');
//     ot.sext('l');
//     ot.sext('o');
//     ot.sext('\n');
//     ot.sext('W');
//     ot.sext('o');
//     ot.sext('r');
//     ot.sext('l');
//     ot.sext('d');
//     ot.sext('\n');
//   }

//   {
//     file<char, mode::r> f(filename);
//     string<char> s(100);

//     copy<char>(f.iter(), s.oter());

//     N_TEST_ASSERT_EQUALS(s, str("Hello\nWorld\n"));
//   }

//   remove(filename);
// }

// // Test file (pathable mode)
// void test_file_pathable_mode() {
//   const char* filename = "test_file.txt";

//   // Write to file
//   {
//     file<char, mode::w> f(filename);
//     f.push('H');
//     f.push('e');
//     f.push('l');
//     f.push('l');
//     f.push('o');
//     f.push('\n');
//     f.push('W');
//     f.push('o');
//     f.push('r');
//     f.push('l');
//     f.push('d');
//     f.push('\n');
//   }

//   // Read from file
//   {
//     file<char, mode::r> f(filename);
//     N_TEST_ASSERT_TRUE(f.opened());

//     maybe<char> c;

//     c = f.pop();
//     N_TEST_ASSERT_TRUE(c.has());
//     N_TEST_ASSERT_EQUALS(c.get(), 'H');

//     c = f.pop();
//     N_TEST_ASSERT_TRUE(c.has());
//     N_TEST_ASSERT_EQUALS(c.get(), 'e');

//     c = f.pop();
//     N_TEST_ASSERT_TRUE(c.has());
//     N_TEST_ASSERT_EQUALS(c.get(), 'l');

//     c = f.pop();
//     N_TEST_ASSERT_TRUE(c.has());
//     N_TEST_ASSERT_EQUALS(c.get(), 'l');

//     c = f.pop();
//     N_TEST_ASSERT_TRUE(c.has());
//     N_TEST_ASSERT_EQUALS(c.get(), 'o');

//     c = f.pop();
//     N_TEST_ASSERT_TRUE(c.has());
//     N_TEST_ASSERT_EQUALS(c.get(), '\n');

//     c = f.pop();
//     N_TEST_ASSERT_TRUE(c.has());
//     N_TEST_ASSERT_EQUALS(c.get(), 'W');

//     c = f.pop();
//     N_TEST_ASSERT_TRUE(c.has());
//     N_TEST_ASSERT_EQUALS(c.get(), 'o');

//     c = f.pop();
//     N_TEST_ASSERT_TRUE(c.has());
//     N_TEST_ASSERT_EQUALS(c.get(), 'r');

//     c = f.pop();
//     N_TEST_ASSERT_TRUE(c.has());
//     N_TEST_ASSERT_EQUALS(c.get(), 'l');

//     c = f.pop();
//     N_TEST_ASSERT_TRUE(c.has());
//     N_TEST_ASSERT_EQUALS(c.get(), 'd');

//     c = f.pop();
//     N_TEST_ASSERT_TRUE(c.has());
//     N_TEST_ASSERT_EQUALS(c.get(), '\n');

//     c = f.pop();
//     N_TEST_ASSERT_FALSE(c.has());
//   }

//   remove(filename);
// }

// // Test file (stdin mode)
// void test_file_stdin_mode() {
//   file<char, mode::std_in> f;
//   N_TEST_ASSERT_FALSE(f.opened());
//   N_TEST_ASSERT_TRUE(stdr.opened());
// }

// // Test file (stdout mode)
// void test_file_stdout_mode() {
//   file<char, mode::std_out> f;
//   N_TEST_ASSERT_FALSE(f.opened());
//   N_TEST_ASSERT_TRUE(stdw.opened());
// }

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
