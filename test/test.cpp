#include <string.h>

#include <core/algorithm.hpp>
#include <core/span.hpp>
#include <core/string.hpp>
#include <core/vector.hpp>
#include <initializer_list>
#include <tests.hpp>

void test_vector_default_constructor() {
  vector<int> v;
  N_TEST_ASSERT_TRUE(v.empty());
  N_TEST_ASSERT_EQUALS(v.len(), 0);
}

void test_vector_size_constructor() {
  vector<int> v(5);
  N_TEST_ASSERT_EQUALS(v.max(), 5);
  N_TEST_ASSERT_TRUE(v.empty());
}

void test_vector_copy_constructor() {
  vector<int> v1(5);
  v1.add(1);
  vector<int> v2(v1);
  N_TEST_ASSERT_EQUALS(v2.len(), v1.len());
  N_TEST_ASSERT_EQUALS(v2.at(0), 1);
}

void test_vector_move_constructor() {
  vector<int> v1(5);
  v1.add(1);
  vector<int> v2(move(v1));
  N_TEST_ASSERT_EQUALS(v2.len(), 1);
  N_TEST_ASSERT_TRUE(v1.empty());
}

void test_vector_copy_assignment() {
  vector<int> v1(5);
  v1.add(1);
  vector<int> v2;
  v2 = v1;
  N_TEST_ASSERT_EQUALS(v2.len(), v1.len());
}

void test_vector_move_assignment() {
  vector<int> v1(5);
  v1.add(1);
  vector<int> v2;
  v2 = move(v1);
  N_TEST_ASSERT_EQUALS(v2.len(), 1);
  N_TEST_ASSERT_TRUE(v1.empty());
}

void test_vector_properties() {
  vector<int> v(5);
  N_TEST_ASSERT_TRUE(v.empty());
  N_TEST_ASSERT_FALSE(v.full());
  N_TEST_ASSERT_EQUALS(v.max(), 5);
  N_TEST_ASSERT_FALSE(v.has(0));
  v.add(1);
  N_TEST_ASSERT_TRUE(v.has(0));
}

void test_vector_clear() {
  vector<int> v(5);
  v.add(1);
  v.clear();
  N_TEST_ASSERT_TRUE(v.empty());
}

void test_vector_at() {
  vector<int> v(5);
  v.add(1);
  N_TEST_ASSERT_EQUALS(v.at(0), 1);
  try {
    v.at(1);
    N_TEST_ASSERT_TRUE(false);  // ne devrait pas atteindre cette ligne
  } catch (out_of_range&) {
    N_TEST_ASSERT_TRUE(true);  // exception attendue
  }
}

void test_vector_add() {
  vector<int> v(2);
  v.add(1);
  v.add(2);
  N_TEST_ASSERT_EQUALS(v.at(0), 1);
  N_TEST_ASSERT_EQUALS(v.at(1), 2);
  v.add(3, 1);  // Insérer 3 à la position 1
  N_TEST_ASSERT_EQUALS(v.at(1), 3);
  N_TEST_ASSERT_EQUALS(v.at(2), 2);
}

void test_vector_exchange() {
  vector<int> v(3);
  v.add(1);
  v.add(2);
  v.exchange(0, 1);
  N_TEST_ASSERT_EQUALS(v.at(0), 2);
  N_TEST_ASSERT_EQUALS(v.at(1), 1);
}

void test_vector_modify() {
  vector<int> v(2);
  v.add(1);
  v.modify(2, 0);  // Modifier l'élément à la position 0
  N_TEST_ASSERT_EQUALS(v.at(0), 2);
}

void test_vector_remove() {
  vector<int> v(3);
  v.add(1);
  v.add(2);
  v.remove(0);
  N_TEST_ASSERT_EQUALS(v.len(), 1);
  N_TEST_ASSERT_EQUALS(v.at(0), 2);
}

void test_count_iterators() {
  vector<int> v;
  for (int i = 0; i < 5; ++i)
    v.add(i);
  auto cnt = count(begin(v), end(v));
  N_TEST_ASSERT_EQUALS(cnt, 5);
}

void test_count_value() {
  vector<int> v;
  for (int i : {1, 2, 1, 4, 1})
    v.add(i);
  auto cnt = count(begin(v), end(v), 1);
  N_TEST_ASSERT_EQUALS(cnt, 3);
}

void test_count_predicate() {
  vector<int> v;
  for (int i = 1; i <= 5; ++i)
    v.add(i);
  auto cnt = count(begin(v), end(v), [](int i) { return i % 2 == 0; });
  N_TEST_ASSERT_EQUALS(cnt, 2);
}

void test_find_value() {
  vector<int> v;
  for (int i = 1; i <= 5; ++i)
    v.add(i);
  auto it = find(begin(v), end(v), 3);
  N_TEST_ASSERT_TRUE(it != end(v) && *it == 3);
}

void test_find_predicate() {
  vector<int> v;
  for (int i = 1; i <= 5; ++i)
    v.add(i);
  auto it = find(begin(v), end(v), [](int i) { return i > 3; });
  N_TEST_ASSERT_TRUE(it != end(v) && *it == 4);
}

void test_find_not() {
  vector<int> v;
  for (int i : {2, 4, 6, 7, 8})
    v.add(i);
  auto it = find_not(begin(v), end(v), [](int i) { return i % 2 == 0; });
  N_TEST_ASSERT_TRUE(it != end(v) && *it == 7);
}
void test_all_of() {
  vector<int> v;
  for (int i : {2, 4, 6, 8})
    v.add(i);
  bool result = all_of(begin(v), end(v), [](int i) { return i % 2 == 0; });
  N_TEST_ASSERT_TRUE(result);
}
void test_any_of() {
  vector<int> v;
  for (int i : {1, 3, 5, 6})
    v.add(i);
  bool result = any_of(begin(v), end(v), [](int i) { return i % 2 == 0; });
  N_TEST_ASSERT_TRUE(result);
}

void test_none_of() {
  vector<int> v;
  for (int i : {1, 3, 5, 7})
    v.add(i);
  bool result = none_of(begin(v), end(v), [](int i) { return i % 2 == 0; });
  N_TEST_ASSERT_TRUE(result);
}

void test_starts_with() {
  vector<int> v1, v2;
  for (int i : {1, 2, 3})
    v1.add(i);
  for (int i : {1, 2})
    v2.add(i);
  bool result = starts_with(begin(v1), end(v1), begin(v2), end(v2));
  N_TEST_ASSERT_TRUE(result);
}

void test_equals() {
  vector<int> v1, v2;
  for (int i : {1, 2, 3})
    v1.add(i);
  for (int i : {1, 2, 3})
    v2.add(i);
  bool result = equals(begin(v1), end(v1), begin(v2), end(v2));
  N_TEST_ASSERT_TRUE(result);
}

void test_string_size_constructor() {
  basic_string<char> s(10);
  N_TEST_ASSERT_EQUALS(s.max(), 10);
  N_TEST_ASSERT_TRUE(s.empty());
}

void test_string_default_constructor() {
  basic_string<char> s;
  N_TEST_ASSERT_TRUE(s.empty());
  N_TEST_ASSERT_EQUALS(s.len(), 0);
}

void test_string_copy_constructor() {
  basic_string<char> s1(10);
  s1.add('a');
  basic_string<char> s2(s1);
  N_TEST_ASSERT_EQUALS(s2.len(), s1.len());
  N_TEST_ASSERT_EQUALS(s2.at(0), 'a');
}

void test_string_move_constructor() {
  basic_string<char> s1(10);
  s1.add('a');
  basic_string<char> s2(move(s1));
  N_TEST_ASSERT_EQUALS(s2.len(), 1);
  N_TEST_ASSERT_TRUE(s1.empty());
}

void test_string_copy_assignment() {
  basic_string<char> s1(10);
  s1.add('a');
  basic_string<char> s2;
  s2 = s1;
  N_TEST_ASSERT_EQUALS(s2.len(), s1.len());
}

void test_string_move_assignment() {
  basic_string<char> s1(10);
  s1.add('a');
  basic_string<char> s2;
  s2 = move(s1);
  N_TEST_ASSERT_EQUALS(s2.len(), 1);
  N_TEST_ASSERT_TRUE(s1.empty());
}

void test_string_properties() {
  basic_string<char> s(10);
  N_TEST_ASSERT_TRUE(s.empty());
  N_TEST_ASSERT_FALSE(s.full());
  N_TEST_ASSERT_EQUALS(s.max(), 10);
  N_TEST_ASSERT_FALSE(s.has(0));
  s.add('a');
  N_TEST_ASSERT_TRUE(s.has(0));
}

void test_string_clear() {
  basic_string<char> s(10);
  s.add('a');
  s.clear();
  N_TEST_ASSERT_TRUE(s.empty());
}

void test_string_at() {
  basic_string<char> s(10);
  s.add('a');
  N_TEST_ASSERT_EQUALS(s.at(0), 'a');
  try {
    s.at(1);
    N_TEST_ASSERT_TRUE(false);  // ne devrait pas atteindre cette ligne
  } catch (out_of_range&) {
    N_TEST_ASSERT_TRUE(true);  // exception attendue
  }
}

void test_string_add() {
  basic_string<char> s(2);
  s.add('a');
  s.add('b');
  N_TEST_ASSERT_EQUALS(s.at(0), 'a');
  N_TEST_ASSERT_EQUALS(s.at(1), 'b');
  s.add('c', 1);  // Insérer 'c' à la position 1
  N_TEST_ASSERT_EQUALS(s.at(1), 'c');
  N_TEST_ASSERT_EQUALS(s.at(2), 'b');
}

void test_string_exchange() {
  basic_string<char> s(3);
  s.add('a');
  s.add('b');
  s.exchange(0, 1);
  N_TEST_ASSERT_EQUALS(s.at(0), 'b');
  N_TEST_ASSERT_EQUALS(s.at(1), 'a');
}

void test_string_modify() {
  basic_string<char> s(2);
  s.add('a');
  s.modify('b', 0);  // Modifier l'élément à la position 0
  N_TEST_ASSERT_EQUALS(s.at(0), 'b');
}

void test_string_remove() {
  basic_string<char> s(3);
  s.add('a');
  s.add('b');
  s.remove(0);
  N_TEST_ASSERT_EQUALS(s.len(), 1);
  N_TEST_ASSERT_EQUALS(s.at(0), 'b');
}

void test_null_terminating_string() {
  basic_string<char> s(3);
  s.add('a');
  s.add('b');
  s.add('c');
  N_TEST_ASSERT_EQUALS(s.len(), 3);
  N_TEST_ASSERT_EQUALS(s.len(), strlen(s.data()));

  s.modify(0, 1);
  N_TEST_ASSERT_EQUALS(s.len(), 1);
  N_TEST_ASSERT_EQUALS(s.len(), strlen(s.data()));
}

void test_string_view_constructors() {
  const char* str = "hello";
  basic_string_view<char> sv1(str, str + 5);
  N_TEST_ASSERT_EQUALS(sv1.len(), 5);

  basic_string_view<char> sv2(str, 5);
  N_TEST_ASSERT_EQUALS(sv2.len(), 5);

  basic_string_view<char> sv3(str);
  N_TEST_ASSERT_EQUALS(sv3.len(), 5);
}

void test_string_view_assignment() {
  const char* str = "hello";
  basic_string_view<char> sv;
  sv = str;
  N_TEST_ASSERT_EQUALS(sv.len(), 5);

  basic_string<char> s;
  sv = s;
  N_TEST_ASSERT_EQUALS(sv.len(), 0);
}

void test_string_view_methods() {
  basic_string_view<char> sv("hello", 5);
  N_TEST_ASSERT_FALSE(sv.empty());
  N_TEST_ASSERT_TRUE(sv.has(4));
  N_TEST_ASSERT_FALSE(sv.has(5));
  // N_TEST_ASSERT_TRUE(equals(sv.data(), basic_string_view<char>("hello")));

  N_TEST_ASSERT_EQUALS(sv.at(0), 'h');
  try {
    sv.at(5);
    N_TEST_ASSERT_TRUE(false);  // Ne devrait pas atteindre cette ligne
  } catch (out_of_range&) {
    N_TEST_ASSERT_TRUE(true);  // Exception attendue
  }
}

void test_span_constructors() {
  int arr[] = {1, 2, 3, 4, 5};
  span<int> sp1(arr, arr + 5);
  N_TEST_ASSERT_EQUALS(sp1.len(), 5);

  span<int> sp2(arr, 5);
  N_TEST_ASSERT_EQUALS(sp2.len(), 5);
}

void test_span_assignment() {
  int arr[] = {1, 2, 3, 4, 5};
  span<int> sp1(arr, 5);
  span<int> sp2 = sp1;
  N_TEST_ASSERT_EQUALS(sp2.len(), 5);

  span<int> sp3;
  sp3 = move(sp1);
  N_TEST_ASSERT_EQUALS(sp3.len(), 5);
}

void test_span_methods() {
  int arr[] = {1, 2, 3, 4, 5};
  span<int> sp(arr, 5);
  N_TEST_ASSERT_FALSE(sp.empty());
  N_TEST_ASSERT_TRUE(sp.has(4));
  N_TEST_ASSERT_FALSE(sp.has(5));
  N_TEST_ASSERT_EQUALS(sp.data(), arr);

  N_TEST_ASSERT_EQUALS(sp.at(0), 1);
  try {
    sp.at(5);
    N_TEST_ASSERT_TRUE(false);  // Ne devrait pas atteindre cette ligne
  } catch (out_of_range&) {
    N_TEST_ASSERT_TRUE(true);  // Exception attendue
  }
}

void test_span_exchange_modify() {
  int arr[] = {1, 2, 3};
  span<int> sp(arr, 3);

  // Test de exchange
  sp.exchange(0, 2);
  N_TEST_ASSERT_EQUALS(sp.at(0), 3);
  N_TEST_ASSERT_EQUALS(sp.at(2), 1);

  // Test de modify
  sp.modify(4, 1);
  N_TEST_ASSERT_EQUALS(sp.at(1), 4);
}

int main() {
  N_TEST_SUITE(vector test suite);
  N_TEST_RUN(test_vector_default_constructor);
  N_TEST_RUN(test_vector_size_constructor);
  N_TEST_RUN(test_vector_copy_constructor);
  N_TEST_RUN(test_vector_move_constructor);
  N_TEST_RUN(test_vector_copy_assignment);
  N_TEST_RUN(test_vector_move_assignment);
  N_TEST_RUN(test_vector_properties);
  N_TEST_RUN(test_vector_clear);
  N_TEST_RUN(test_vector_at);
  N_TEST_RUN(test_vector_add);
  N_TEST_RUN(test_vector_exchange);
  N_TEST_RUN(test_vector_modify);
  N_TEST_RUN(test_vector_remove);

  N_TEST_RESULTS;

  N_TEST_SUITE(algorithms test suite);

  N_TEST_RUN(test_count_iterators);
  N_TEST_RUN(test_count_value);
  N_TEST_RUN(test_count_predicate);
  N_TEST_RUN(test_find_value);
  N_TEST_RUN(test_find_predicate);
  N_TEST_RUN(test_find_not);
  N_TEST_RUN(test_all_of);
  N_TEST_RUN(test_any_of);
  N_TEST_RUN(test_none_of);
  N_TEST_RUN(test_starts_with);
  N_TEST_RUN(test_equals);

  N_TEST_RESULTS;

  N_TEST_SUITE(string_tests);

  N_TEST_RUN(test_string_default_constructor);
  N_TEST_RUN(test_string_size_constructor);
  N_TEST_RUN(test_string_copy_constructor);
  N_TEST_RUN(test_string_move_constructor);
  N_TEST_RUN(test_string_copy_assignment);
  N_TEST_RUN(test_string_move_assignment);
  N_TEST_RUN(test_string_properties);
  N_TEST_RUN(test_string_clear);
  N_TEST_RUN(test_string_at);
  N_TEST_RUN(test_string_add);
  N_TEST_RUN(test_string_exchange);
  N_TEST_RUN(test_string_modify);
  N_TEST_RUN(test_string_remove);
  N_TEST_RUN(test_null_terminating_string);

  N_TEST_RESULTS;

  N_TEST_SUITE(string_view_tests);

  N_TEST_RUN(test_string_view_constructors);
  N_TEST_RUN(test_string_view_assignment);
  N_TEST_RUN(test_string_view_methods);

  N_TEST_RESULTS;

  N_TEST_SUITE(span_tests);

  N_TEST_RUN(test_span_constructors);
  N_TEST_RUN(test_span_assignment);
  N_TEST_RUN(test_span_methods);
  N_TEST_RUN(test_span_exchange_modify);

  N_TEST_RESULTS;
}