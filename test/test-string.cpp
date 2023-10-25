#include <core/string.hpp>
#include <tests.hpp>
template <size_t N>
constexpr bool operator==(const stringlike auto& s1, const char* s2) {
  auto is1 = istream(s1);
  auto is2 = istream(s2);
  return equals(is1, is2);
}

constexpr void test_string_create() {
  string s = istream("Hello");
  N_TEST_ASSERT_EQUALS(s.len(), 5);
}

constexpr void test_string_copy_constructor() {
  string s = istream("Hello");
  string s2 = s;
  ::printf("%lu", s2.len());
  N_TEST_ASSERT_EQUALS(s2.len(), 5);
}

constexpr void test_string_move_constructor() {
  string s = istream("Hello");
  string s3 = move(s);
  N_TEST_ASSERT_EQUALS(s3.len(), 5);
}

constexpr void test_string_copy_assignment() {
  string s = istream("Hello");
  string s2;
  s2 = s;
  N_TEST_ASSERT_EQUALS(s2.len(), 5);
}

constexpr void test_string_move_assignment() {
  string s = istream("Hello");
  string s3;
  s3 = move(s);
  N_TEST_ASSERT_EQUALS(s3.len(), 5);
}

constexpr void test_string_len() {
  string s = istream("Hello");
  N_TEST_ASSERT_EQUALS(s.len(), 5);
}

constexpr void test_string_empty() {
  string s;
  N_TEST_ASSERT_TRUE(s.empty());
}

constexpr void test_string_full() {
  string s = istream("Hello");
  N_TEST_ASSERT_FALSE(s.full());
}

constexpr void test_string_add() {
  string s;
  s.add('H');
  s.add('i');
  N_TEST_ASSERT_EQUALS(s.len(), 2);
}

constexpr void test_string_remove() {
  string s = istream("Hi");
  s.remove(typename string::position{s.len() - 1});
  N_TEST_ASSERT_EQUALS(s.len(), 1);
  N_TEST_ASSERT_FALSE(s.full());
}

constexpr void test_string_clear() {
  string s = istream("Clear me");
  s.clear();
  N_TEST_ASSERT_TRUE(s.empty());
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
  N_TEST_RUN_SUITE
  return 0;
}
