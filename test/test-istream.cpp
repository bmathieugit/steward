#include <core/char-istream.hpp>
#include <core/cstring.hpp>
#include <tests.hpp>

void test_input_stream_unsigned_integer() {
  auto is = istream("12");
  maybe<size_t> i;
  is >> i;
  N_TEST_ASSERT_TRUE(i.has());
  N_TEST_ASSERT_EQUALS(i.get(), 12);
}

void test_input_stream_signed_integer() {
  auto is = istream("12");
  maybe<int> i;
  is >> i;
  N_TEST_ASSERT_TRUE(i.has());
  N_TEST_ASSERT_EQUALS(i.get(), 12);
}

void test_input_stream_signed_integer2() {
  auto is = istream("-12");
  maybe<int> i;
  is >> i;
  N_TEST_ASSERT_TRUE(i.has());
  N_TEST_ASSERT_EQUALS(i.get(), -12);
}

void test_input_stream_bool_true() {
  auto is = istream("0");
  maybe<bool> b;
  is >> b;
  N_TEST_ASSERT_TRUE(b.has());
  N_TEST_ASSERT_EQUALS(b.get(), true);
}

void test_input_stream_bool_false() {
  auto is = istream("1");
  maybe<bool> b;
  is >> b;
  N_TEST_ASSERT_TRUE(b.has());
  N_TEST_ASSERT_EQUALS(b.get(), false);
}

int main() {
  N_TEST_SUITE("istream Tests")
  N_TEST_REGISTER(test_input_stream_unsigned_integer)
  N_TEST_REGISTER(test_input_stream_signed_integer)
  N_TEST_REGISTER(test_input_stream_signed_integer2)
  N_TEST_REGISTER(test_input_stream_bool_true)
  N_TEST_REGISTER(test_input_stream_bool_false)
  N_TEST_RUN_SUITE
}
