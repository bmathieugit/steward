#include <cstdio>
#include <core/format.hpp>
#include <core/io.hpp>
#include <core/result.hpp>
#include <core/string.hpp>
#include <tests.hpp>
#include <core/vector.hpp>

#include "core/iterator.hpp"

bool operator==(const string<char>& s, const char* s1) {
  return equal(s.iter(), pointer_iterator(s1, strlen(s1)));
}

bool operator==(const string<char>& s, const string<char>& s1) {
  return equal(s.iter(), s1.iter());
}

// Test file_iterator (readable mode)
void test_file_iterator_readable_mode() {
  const char* filename = "test_file.txt";
  FILE* file = fopen(filename, "w+");
  fputs("Hello\nWorld\n", file);
  rewind(file);

  file<char, mode::rp> f(file);
  file_iterator<char, mode::rp> it(&f);

  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), 'H');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), 'e');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), 'l');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), 'l');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), 'o');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), '\n');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), 'W');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), 'o');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), 'r');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), 'l');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), 'd');
  N_TEST_ASSERT_TRUE(it.has_next());
  N_TEST_ASSERT_EQUALS(it.next(), '\n');
  N_TEST_ASSERT_FALSE(it.has_next());

  remove(filename);
}

// Test file_oterator (writable mode)
void test_file_oterator_writable_mode() {
  const char* filename = "test_file.txt";

  {
    file<char, mode::wp> f(filename);
    auto ot = f.oter();

    ot.sext('H');
    ot.sext('e');
    ot.sext('l');
    ot.sext('l');
    ot.sext('o');
    ot.sext('\n');
    ot.sext('W');
    ot.sext('o');
    ot.sext('r');
    ot.sext('l');
    ot.sext('d');
    ot.sext('\n');
  }

  {
    file<char, mode::r> f(filename);
    string<char> s(100);
    
    copy<char>(f.iter(), s.oter());

    N_TEST_ASSERT_EQUALS(s, str("Hello\nWorld\n"));
  }

  remove(filename);
}

// Test file (pathable mode)
void test_file_pathable_mode() {
  const char* filename = "test_file.txt";

  // Write to file
  {
    file<char, mode::w> f(filename);
    f.push('H');
    f.push('e');
    f.push('l');
    f.push('l');
    f.push('o');
    f.push('\n');
    f.push('W');
    f.push('o');
    f.push('r');
    f.push('l');
    f.push('d');
    f.push('\n');
  }

  // Read from file
  {
    file<char, mode::r> f(filename);
    N_TEST_ASSERT_TRUE(f.opened());

    maybe<char> c;

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), 'H');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), 'e');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), 'l');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), 'l');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), 'o');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), '\n');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), 'W');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), 'o');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), 'r');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), 'l');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), 'd');

    c = f.pop();
    N_TEST_ASSERT_TRUE(c.has());
    N_TEST_ASSERT_EQUALS(c.get(), '\n');

    c = f.pop();
    N_TEST_ASSERT_FALSE(c.has());
  }

  remove(filename);
}

// Test file (stdin mode)
void test_file_stdin_mode() {
  file<char, mode::std_in> f;
  N_TEST_ASSERT_FALSE(f.opened());
  N_TEST_ASSERT_TRUE(stdr.opened());
}

// Test file (stdout mode)
void test_file_stdout_mode() {
  file<char, mode::std_out> f;
  N_TEST_ASSERT_FALSE(f.opened());
  N_TEST_ASSERT_TRUE(stdw.opened());
}


// Main function to run the tests
int main() {
  N_TEST_SUITE("IO file test suite")

  N_TEST_REGISTER(test_file_iterator_readable_mode);
  N_TEST_REGISTER(test_file_oterator_writable_mode);
  N_TEST_REGISTER(test_file_pathable_mode);
  N_TEST_REGISTER(test_file_stdin_mode);
  N_TEST_REGISTER(test_file_stdout_mode);

  N_TEST_RUN_SUITE;

  return 0;
}
