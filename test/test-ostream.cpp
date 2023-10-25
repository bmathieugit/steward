#include <core/char-ostream.hpp>
#include <tests.hpp>
#include <core/core.hpp>

using char_array_stream = char_array<100>;

template <size_t N>
bool operator==(const char_array<N>& s1, const char* s2) {
  auto is1 = istream(s1);
  auto is2 = istream(s2);
  return equals(is1, is2);
}

void test_ostream_string() {
  char_array_stream result;

  result << "Hello, "
         << "world"
         << "!";
  N_TEST_ASSERT_EQUALS(result, "Hello, world!");
  result.clear();

  result << "The answer is " << 42;
  N_TEST_ASSERT_EQUALS(result, "The answer is 42");
  result.clear();
  result << "John"
         << " is " << 30 << " years old";
  N_TEST_ASSERT_EQUALS(result, "John is 30 years old");
}

void test_ostream_to_string() {
  char_array_stream result;
  result << "Hello, "
      << "world"
      << "!";
  N_TEST_ASSERT_EQUALS(result, "Hello, world!");
  result.clear();
  result << "The answer is " << 42;
  N_TEST_ASSERT_EQUALS(result, "The answer is 42");
  result.clear();
  result << "John"
      << " is " << 30 << " years old";
  N_TEST_ASSERT_EQUALS(result, "John is 30 years old");
}

void test_ostream_signed_integral() {
  char_array_stream result;

  result << -42;
  N_TEST_ASSERT_EQUALS(result, "-42");
  result.clear();
  result << 0;
  N_TEST_ASSERT_EQUALS(result, "0");
  result.clear();
  result << 123456789;
  N_TEST_ASSERT_EQUALS(result, "123456789");
}

void test_ostream_unsigned_integral() {
  char_array_stream result;
  result << 42u;
  N_TEST_ASSERT_EQUALS(result, "42");
  result.clear();
  result << 0u;
  N_TEST_ASSERT_EQUALS(result, "0");
  result.clear();
  result << 123456789u;
  N_TEST_ASSERT_EQUALS(result, "123456789");
}

void test_ostream_bool() {
  char_array_stream result;
  result << true;
  N_TEST_ASSERT_EQUALS(result, "true");
  result.clear();
  result << false;
  N_TEST_ASSERT_EQUALS(result, "false");
}

int main() {
  N_TEST_SUITE("ostream Library Tests")
  N_TEST_REGISTER(test_ostream_string)
  N_TEST_REGISTER(test_ostream_to_string)
  N_TEST_REGISTER(test_ostream_signed_integral)
  N_TEST_REGISTER(test_ostream_unsigned_integral)
  N_TEST_REGISTER(test_ostream_bool)
  N_TEST_RUN_SUITE
}
