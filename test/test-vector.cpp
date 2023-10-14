#include <tests.hpp>
#include <core/vector.hpp>


// Test du constructeur par défaut
void test_default_constructor() {
    vector<int> v;
    N_TEST_ASSERT_TRUE(v.empty());
    N_TEST_ASSERT_EQUALS(v.len(), 0);
    N_TEST_ASSERT_EQUALS(v.max(), 0);
}

// Test du constructeur avec taille initiale
void test_constructor_with_size() {
    vector<int> v2(20);
    N_TEST_ASSERT_TRUE(v2.empty());
    N_TEST_ASSERT_EQUALS(v2.len(), 0);
    N_TEST_ASSERT_EQUALS(v2.max(), 20);
}

// Test de la méthode add()
void test_add_method() {
    vector<int> v;
    v.add(42);
    N_TEST_ASSERT_FALSE(v.empty());
    N_TEST_ASSERT_EQUALS(v.len(), 1);
    N_TEST_ASSERT_EQUALS(v.at(typename vector<int>::position{0}), 42);
}

// Test de la méthode modify()
void test_modify_method() {
    vector<int> v;
    v.add(42);
    v.modify(typename vector<int>::position{0}, 99);
    N_TEST_ASSERT_EQUALS(v.at(typename vector<int>::position{0}), 99);
}

// Test de la méthode remove()
void test_remove_method() {
    vector<int> v;
    v.add(42);
    v.remove(typename vector<int>::position{0});
    N_TEST_ASSERT_TRUE(v.empty());
    N_TEST_ASSERT_EQUALS(v.len(), 0);
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

