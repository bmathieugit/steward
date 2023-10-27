#include <core/result.hpp>
#include <core/string.hpp>
#include <tests.hpp>

void test_variant_set_int() {
  variant<int, double, string> v = 42;
  N_TEST_ASSERT_EQUALS(v.index(), 0);
  N_TEST_ASSERT_EQUALS(v.get<int>(), 42);
}

void test_variant_set_double() {
  variant<int, double, string> v = 42.5;
  N_TEST_ASSERT_EQUALS(v.index(), 1);
  N_TEST_ASSERT_EQUALS(v.get<double>(), 42.5);
}

void test_variant_set_string_view() {
  variant<int, double, string> v = string(iter("test"));
  N_TEST_ASSERT_EQUALS(v.index(), 2);
}

void test_variant_copy_constructor() {
  variant<int, double, string> v1(42);
  variant<int, double, string> v2(v1);
  N_TEST_ASSERT_EQUALS(v2.get<int>(), 42);
}

void test_variant_move_constructor() {
  variant<int, double, string> v1 = 42;
  variant<int, double, string> v2(move(v1));
  N_TEST_ASSERT_EQUALS(v2.get<int>(), 42);
}

int main() {
  N_TEST_SUITE("Variant Tests")

  N_TEST_REGISTER(test_variant_set_int);
  N_TEST_REGISTER(test_variant_set_double);
  N_TEST_REGISTER(test_variant_set_string_view);
  N_TEST_REGISTER(test_variant_copy_constructor);
  N_TEST_REGISTER(test_variant_move_constructor);

  N_TEST_RUN_SUITE
}
