#include <core/string.hpp>
#include <tests.hpp>

template <size_t N>
constexpr bool operator==(const char_collection auto& s1, const char* s2) {
  auto is1 = iter(s1);
  auto is2 = iter(s2);
  return equals(is1, is2);
}

constexpr void test_string_create() {
  string s = iter("Hello");
  N_TEST_ASSERT_EQUALS(s.len(), 5);
}

constexpr void test_string_copy_constructor() {
  string s = iter("Hello");
  string s2 = s;
  ::printf("%lu", s2.len());
  N_TEST_ASSERT_EQUALS(s2.len(), 5);
}

constexpr void test_string_move_constructor() {
  string s = iter("Hello");
  string s3 = move(s);
  N_TEST_ASSERT_EQUALS(s3.len(), 5);
}

constexpr void test_string_copy_assignment() {
  string s = iter("Hello");
  string s2;
  s2 = s;
  N_TEST_ASSERT_EQUALS(s2.len(), 5);
}

constexpr void test_string_move_assignment() {
  string s = iter("Hello");
  string s3;
  s3 = move(s);
  N_TEST_ASSERT_EQUALS(s3.len(), 5);
}

constexpr void test_string_len() {
  string s = iter("Hello");
  N_TEST_ASSERT_EQUALS(s.len(), 5);
}

constexpr void test_string_empty() {
  string s;
  N_TEST_ASSERT_TRUE(s.empty());
}

constexpr void test_string_full() {
  string s = iter("Hello");
  N_TEST_ASSERT_FALSE(s.full());
}

constexpr void test_string_add() {
  string s;
  s.add('H');
  s.add('i');
  N_TEST_ASSERT_EQUALS(s.len(), 2);
}

constexpr void test_string_remove() {
  string s = iter("Hi");
  s.remove(typename string::position{s.len() - 1});
  N_TEST_ASSERT_EQUALS(s.len(), 1);
  N_TEST_ASSERT_FALSE(s.full());
}

constexpr void test_string_clear() {
  string s = iter("Clear me");
  s.clear();
  N_TEST_ASSERT_TRUE(s.empty());
}

void test_input_stream_unsigned_integer() {
  auto is = iter("12");
  maybe<size_t> i;
  from_chars(is, i);
  N_TEST_ASSERT_TRUE(i.has());
  N_TEST_ASSERT_EQUALS(i.get(), 12);
}

void test_input_stream_signed_integer() {
  auto is = iter("12");
  maybe<int> i;
  from_chars(is, i);
  N_TEST_ASSERT_TRUE(i.has());
  N_TEST_ASSERT_EQUALS(i.get(), 12);
}

void test_input_stream_signed_integer2() {
  auto is = iter("-12");
  maybe<int> i;
  from_chars(is, i);
  N_TEST_ASSERT_TRUE(i.has());
  N_TEST_ASSERT_EQUALS(i.get(), -12);
}

void test_input_stream_bool_true() {
  auto is = iter("0");
  maybe<bool> b;
  from_chars(is, b);
  N_TEST_ASSERT_TRUE(b.has());
  N_TEST_ASSERT_EQUALS(b.get(), true);
}

void test_input_stream_bool_false() {
  auto is = iter("1");
  maybe<bool> b;
  from_chars(is, b);
  N_TEST_ASSERT_TRUE(b.has());
  N_TEST_ASSERT_EQUALS(b.get(), false);
}

using char_static_vector_stream = char_static_vector<100>;

template <size_t N>
bool operator==(const char_static_vector<N>& s1, const char* s2) {
  auto is1 = iter(s1);
  auto is2 = iter(s2);
  return equals(is1, is2);
}

void test_ostream_string() {
  char_static_vector_stream result;

  to_chars(result, "Hello, ");
  to_chars(result, "world");
  to_chars(result, "!");
  N_TEST_ASSERT_EQUALS(result, "Hello, world!");
  result.clear();

  to_chars(result, "The answer is ");
  to_chars(result, 42);
  N_TEST_ASSERT_EQUALS(result, "The answer is 42");
  result.clear();
  to_chars(result, "John");
  to_chars(result, " is ");
  to_chars(result, 30);
  to_chars(result, " years old");
  N_TEST_ASSERT_EQUALS(result, "John is 30 years old");
}

void test_ostream_signed_integral() {
  char_static_vector_stream result;

  to_chars(result, -42);
  N_TEST_ASSERT_EQUALS(result, "-42");
  result.clear();
  to_chars(result, 0);
  N_TEST_ASSERT_EQUALS(result, "0");
  result.clear();
  to_chars(result, 123456789);
  N_TEST_ASSERT_EQUALS(result, "123456789");
}

void test_ostream_unsigned_integral() {
  char_static_vector_stream result;
  to_chars(result, 42u);
  N_TEST_ASSERT_EQUALS(result, "42");
  result.clear();
  to_chars(result, 0u);
  N_TEST_ASSERT_EQUALS(result, "0");
  result.clear();
  to_chars(result, 123456789u);
  N_TEST_ASSERT_EQUALS(result, "123456789");
}

void test_ostream_bool() {
  char_static_vector_stream result;
  to_chars(result, true);
  N_TEST_ASSERT_EQUALS(result, "true");
  result.clear();
  to_chars(result, false);
  N_TEST_ASSERT_EQUALS(result, "false");
}

int main() {
  N_TEST_SUITE("N String Test Suite")
  N_TEST_REGISTER(test_string_create)
  N_TEST_REGISTER(test_string_copy_constructor)
  N_TEST_REGISTER(test_string_move_constructor)
  N_TEST_REGISTER(test_string_copy_assignment)
  N_TEST_REGISTER(test_string_move_assignment)
  N_TEST_REGISTER(test_string_len)
  N_TEST_REGISTER(test_string_empty)
  N_TEST_REGISTER(test_string_full)
  N_TEST_REGISTER(test_string_add)
  N_TEST_REGISTER(test_string_remove)
  N_TEST_REGISTER(test_string_clear)

  N_TEST_REGISTER(test_input_stream_unsigned_integer)
  N_TEST_REGISTER(test_input_stream_signed_integer)
  N_TEST_REGISTER(test_input_stream_signed_integer2)
  N_TEST_REGISTER(test_input_stream_bool_true)
  N_TEST_REGISTER(test_input_stream_bool_false)

  N_TEST_REGISTER(test_ostream_string)
  N_TEST_REGISTER(test_ostream_signed_integral)
  N_TEST_REGISTER(test_ostream_unsigned_integral)
  N_TEST_REGISTER(test_ostream_bool)
  N_TEST_RUN_SUITE
  return 0;
}
