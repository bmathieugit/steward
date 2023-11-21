#include <core/map.hpp>
#include <core/string.hpp>
#include <tests.hpp>

void test_map_empty() {
  map<string, int> m;
  N_TEST_ASSERT_TRUE(m.empty());
}

void test_map_with_one_item() {
  map<int, int> n;
  n.add(12,10);
  N_TEST_ASSERT_EQUALS(n.at(10), 12);
}

void test_map_with_more_items() {
  map<string, int> h;
  h.add(12, iter("Georges"));
  h.add(7, iter("Bob"));
  N_TEST_ASSERT_EQUALS(h.at(iter("Bob")), 7);
  N_TEST_ASSERT_EQUALS(h.at(iter("Georges")), 12);
}


int main() {
  N_TEST_SUITE("Tests pour la classe map");

  N_TEST_REGISTER(test_map_empty);
  N_TEST_REGISTER(test_map_with_one_item);
  N_TEST_REGISTER(test_map_with_one_item);
  N_TEST_REGISTER(test_map_with_more_items);
  N_TEST_REGISTER(test_map_with_more_items);
  
  N_TEST_RUN_SUITE;

  return 0;
}
