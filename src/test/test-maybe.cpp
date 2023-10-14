#include <core/maybe.hpp>
#include <core/tests.hpp>

void test_default_constructor() {
  maybe<int> m;
  N_TEST_ASSERT_FALSE(m.has());
}

void test_value_constructor() {
  maybe<int> m(5);
  N_TEST_ASSERT_TRUE(m.has());
  N_TEST_ASSERT_EQUALS(m.get(), 5);
}
/*TODO fixer ce test
void test_move_constructor() {
  maybe<std::unique_ptr<int>> m(std::make_unique<int>(5));
  N_TEST_ASSERT_TRUE(m.has());
  N_TEST_ASSERT_EQUALS(*m.get(), 5);
}
*/
void test_copy_constructor() {
  maybe<int> m1(5);
  maybe<int> m2(m1);
  N_TEST_ASSERT_TRUE(m2.has());
  N_TEST_ASSERT_EQUALS(m2.get(), 5);
}

void test_assignment_operator() {
  maybe<int> m;
  m = 5;
  N_TEST_ASSERT_TRUE(m.has());
  N_TEST_ASSERT_EQUALS(m.get(), 5);
}
/*TODO fixer ce test
void test_move_assignment_operator() {
  maybe<std::unique_ptr<int>> m;
  m = std::make_unique<int>(5);
  N_TEST_ASSERT_TRUE(m.has());
  N_TEST_ASSERT_EQUALS(*m.get(), 5);
}
*/
void test_copy_assignment_operator() {
  maybe<int> m1(5);
  maybe<int> m2;
  m2 = m1;
  N_TEST_ASSERT_TRUE(m2.has());
  N_TEST_ASSERT_EQUALS(m2.get(), 5);
}

void test_map() {
  maybe<int> m(5);
  auto m2 = m.map([](int i) { return i * 2; });
  N_TEST_ASSERT_TRUE(m2.has());
  N_TEST_ASSERT_EQUALS(m2.get(), 10);
}


int main() {
  N_TEST_SUITE("Maybe Tests")
  N_TEST_REGISTER(test_default_constructor);
  N_TEST_REGISTER(test_value_constructor);
  //TODO fixer ce test N_TEST_REGISTER(test_move_constructor);
  N_TEST_REGISTER(test_copy_constructor);
  N_TEST_REGISTER(test_assignment_operator);
  //TODO fixer ce test N_TEST_REGISTER(test_move_assignment_operator);
  N_TEST_REGISTER(test_copy_assignment_operator);
  N_TEST_REGISTER(test_map);
  N_TEST_RUN_SUITE
}
