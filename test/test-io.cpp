
#include <core/char-istream.hpp>
#include <core/char-ostream.hpp>
#include <core/io.hpp>
#include <tests.hpp>

void test_file_input_stream() {
  const char* filename = "test_file.txt";
  FILE* _file = fopen(filename, "w");
  fputs("Hello\n", _file);
  fclose(_file);

  file<char, mode::r> f(filename);
  text_file_input_stream fs(f);
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
  file<char, mode::w> f(filename);
  text_file_output_stream fo(f);
  fo << "coucou";
  N_TEST_ASSERT_EQUALS(flength(f), 6);
  remove(filename);
}

// Main function to run the tests
int main() {
  N_TEST_SUITE("IO file test suite")

  N_TEST_REGISTER(test_file_input_stream);
  N_TEST_REGISTER(test_file_output_stream);

  N_TEST_RUN_SUITE;

  {
    file<char, mode::w> out("tmp");
    text_file_output_stream f(out);
    f << "line1\n"
      << "line2\n"
      << "line3\n"
      << "line4";
  }

  {
    file<char, mode::r> in("tmp");
    text_file_input_stream f(in);
    string s;
    while (getline(s, f)) {
      sout << s << "\n";
    }
  }

  return 0;
}
