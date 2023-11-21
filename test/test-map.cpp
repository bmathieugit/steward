#include <core/map.hpp>
#include <core/string.hpp>
#include <tests.hpp>

void test_map_empty() {
  map<string, int> m;
  N_TEST_ASSERT_TRUE(m.empty());
}

void test_map_with_one_item() {
  map<string, int> m;
  m.add(12, iter("Georges"));
  N_TEST_ASSERT_EQUALS(m.at(iter("Georges")), 12);
}

void test_map_with_more_items() {
  map<string, int> m;
  m.add(12, iter("Georges"));
  N_TEST_ASSERT_EQUALS(m.at(iter("Georges")), 12);
}

int main() {
  N_TEST_SUITE("Tests pour la classe vector");

  N_TEST_REGISTER(test_map_empty);
  N_TEST_REGISTER(test_map_with_one_item);

  N_TEST_RUN_SUITE;

  return 0;
}
