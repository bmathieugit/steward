#include <core/string.hpp>
#include <core/tests.hpp>
#include <core/variant.hpp>

void test_variant_default_constructor() {
  variant<int, double, string_iterator<const char>> v;
  N_TEST_ASSERT_TRUE(v.empty());
}

void test_variant_set_int() {
  variant<int, double, string_iterator<const char>> v;
  v.set(42);
  N_TEST_ASSERT_FALSE(v.empty());
  N_TEST_ASSERT_EQUALS(v.get<int>(), 42);
}

void test_variant_set_double() {
  variant<int, double, string_iterator<const char>> v;
  v.set(42.5);
  N_TEST_ASSERT_FALSE(v.empty());
  N_TEST_ASSERT_EQUALS(v.get<double>(), 42.5);
}

void test_variant_set_string_view() {
  variant<int, double, string_iterator<const char>> v;
  v.set(string_iterator<const char>("test"));
  N_TEST_ASSERT_FALSE(v.empty());
  // N_TEST_ASSERT_EQUALS(v.get<string_iterator<const char>>(),
  // string_iterator<const char>("test"));
}

void test_variant_copy_constructor() {
  variant<int, double, string_iterator<const char>> v1;
  v1.set(42);
  variant<int, double, string_iterator<const char>> v2(v1);
  N_TEST_ASSERT_FALSE(v2.empty());
  N_TEST_ASSERT_EQUALS(v2.get<int>(), 42);
}

void test_variant_copy_assignment() {
  variant<int, double, string_iterator<const char>> v1;
  v1.set(42);
  variant<int, double, string_iterator<const char>> v2;
  v2 = v1;
  N_TEST_ASSERT_FALSE(v2.empty());
  N_TEST_ASSERT_EQUALS(v2.get<int>(), 42);
}

void test_variant_move_constructor() {
  variant<int, double, string_iterator<const char>> v1;
  v1.set(42);
  variant<int, double, string_iterator<const char>> v2(transfer(v1));
  N_TEST_ASSERT_FALSE(v2.empty());
  N_TEST_ASSERT_EQUALS(v2.get<int>(), 42);
}

void test_variant_move_assignment() {
  variant<int, double, string_iterator<const char>> v1;
  v1.set(42);
  variant<int, double, string_iterator<const char>> v2;
  v2 = transfer(v1);
  N_TEST_ASSERT_FALSE(v2.empty());
  N_TEST_ASSERT_EQUALS(v2.get<int>(), 42);
}

void test_variant_index() {
  variant<int, double, string_iterator<const char>> v;
  v.set(42.5);
  N_TEST_ASSERT_EQUALS(v.index(), 1);
}

int main() {
  N_TEST_SUITE("Variant Tests") {
    N_TEST_REGISTER(test_variant_default_constructor);
    N_TEST_REGISTER(test_variant_set_int);
    N_TEST_REGISTER(test_variant_set_double);
    N_TEST_REGISTER(test_variant_set_string_view);
    N_TEST_REGISTER(test_variant_copy_constructor);
    N_TEST_REGISTER(test_variant_copy_assignment);
    N_TEST_REGISTER(test_variant_move_constructor);
    N_TEST_REGISTER(test_variant_move_assignment);
    N_TEST_REGISTER(test_variant_index);
  }
  N_TEST_RUN_SUITE
}
