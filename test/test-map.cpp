#include <core/map.hpp>
#include <core/string.hpp>
#include <tests.hpp>

void test_map_empty() {
  map<string, int> m;
  N_TEST_ASSERT_TRUE(m.empty());
}

void test_map_with_one_item() {
  map<int, int> m;
  m.add(12,10);
  N_TEST_ASSERT_EQUALS(m.at(10), 12);
}

void test_map_with_more_items() {
  map<string, int> m;
  m.add(12, iter("Georges"));
  m.add(7, iter("Bob"));
  N_TEST_ASSERT_EQUALS(m.at(iter("Bob")), 7);
  N_TEST_ASSERT_EQUALS(m.at(iter("Georges")), 12);

  m.modify(14, iter("Georges"));
  N_TEST_ASSERT_EQUALS(m.at(iter("Georges")), 14);

  m.modify(11, iter("Bob"));
  N_TEST_ASSERT_EQUALS(m.at(iter("Bob")), 11);

  m.remove(iter("Bob"));
  N_TEST_ASSERT_EQUALS(m.len(), 1);
}


int main() {
  N_TEST_SUITE("Tests pour la classe map");

  N_TEST_REGISTER(test_map_empty);
  N_TEST_REGISTER(test_map_with_one_item);
  N_TEST_REGISTER(test_map_with_more_items);
  
  N_TEST_RUN_SUITE;

  return 0;
}
