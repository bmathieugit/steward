#include <core/vector.hpp>
#include <tests.hpp>

// Test du constructeur par défaut
void test_default_constructor() {
  vector<int> v;
  N_TEST_ASSERT_TRUE(empty(v));
  N_TEST_ASSERT_EQUALS(len(v), 0);
}

// Test du constructeur avec taille initiale
void test_constructor_with_size() {
  vector<int> v2(20);
  N_TEST_ASSERT_TRUE(empty(v2));
  N_TEST_ASSERT_EQUALS(len(v2), 0);
}

// Test de la méthode add()
void test_add_method() {
  vector<int> v;
  v.add(42);
  N_TEST_ASSERT_FALSE(empty(v));
  N_TEST_ASSERT_EQUALS(len(v), 1);
  N_TEST_ASSERT_EQUALS(at(v, 0), 42);
}

// Test de la méthode modify()
void test_modify_method() {
  vector<int> v;
  add(v, 42, len(v));
  modify(v, 99, 0);
  N_TEST_ASSERT_EQUALS(at(v, 0), 99);
}

// Test de la méthode remove()
void test_remove_method() {
  vector<int> v;
  add(v, 42, len(v));
  remove(v, 0);
  N_TEST_ASSERT_TRUE(empty(v));
  N_TEST_ASSERT_EQUALS(len(v), 0);
}

int main() {
  N_TEST_SUITE("Tests pour la classe vector");

  N_TEST_REGISTER(test_default_constructor);
  N_TEST_REGISTER(test_constructor_with_size);
  N_TEST_REGISTER(test_add_method);
  N_TEST_REGISTER(test_modify_method);
  N_TEST_REGISTER(test_remove_method);

  N_TEST_RUN_SUITE;

  return 0;
}
