#include <string.h>
#include <core/algorithm.hpp>
#include <core/array.hpp>
#include <core/exception.hpp>
#include <core/file.hpp>
#include <core/hash.hpp>
#include <core/list.hpp>
#include <core/span.hpp>
#include <core/string.hpp>
#include <core/tuple.hpp>
#include <core/vector.hpp>
#include <initializer_list>
#include <tests.hpp>

constexpr void test_vector_default_constructor() {
  vector<int> v;
  N_TEST_ASSERT_TRUE(v.empty());
  N_TEST_ASSERT_EQUALS(v.len(), 0);
}

constexpr void test_vector_size_constructor() {
  vector<int> v(5);
  N_TEST_ASSERT_EQUALS(v.max(), 5);
  N_TEST_ASSERT_TRUE(v.empty());
}

constexpr void test_vector_copy_constructor() {
  vector<int> v1(5);
  v1.add(1);
  vector<int> v2(v1);
  N_TEST_ASSERT_EQUALS(v2.len(), v1.len());
  N_TEST_ASSERT_EQUALS(v2.at(0), 1);
}

constexpr void test_vector_move_constructor() {
  vector<int> v1(5);
  v1.add(1);
  vector<int> v2(move(v1));
  N_TEST_ASSERT_EQUALS(v2.len(), 1);
  N_TEST_ASSERT_TRUE(v1.empty());
}

constexpr void test_vector_copy_assignment() {
  vector<int> v1(5);
  v1.add(1);
  vector<int> v2;
  v2 = v1;
  N_TEST_ASSERT_EQUALS(v2.len(), v1.len());
}

constexpr void test_vector_move_assignment() {
  vector<int> v1(5);
  v1.add(1);
  vector<int> v2;
  v2 = move(v1);
  N_TEST_ASSERT_EQUALS(v2.len(), 1);
  N_TEST_ASSERT_TRUE(v1.empty());
}

constexpr void test_vector_properties() {
  vector<int> v(5);
  N_TEST_ASSERT_TRUE(v.empty());
  N_TEST_ASSERT_FALSE(v.full());
  N_TEST_ASSERT_EQUALS(v.max(), 5);
  N_TEST_ASSERT_FALSE(v.has(0));
  v.add(1);
  N_TEST_ASSERT_TRUE(v.has(0));
}

constexpr void test_vector_clear() {
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

constexpr void test_vector_add() {
  vector<int> v(2);
  v.add(1);
  v.add(2);
  N_TEST_ASSERT_EQUALS(v.at(0), 1);
  N_TEST_ASSERT_EQUALS(v.at(1), 2);
  v.add(3, 1);  // Insérer 3 à la position 1
  N_TEST_ASSERT_EQUALS(v.at(1), 3);
  N_TEST_ASSERT_EQUALS(v.at(2), 2);
}

constexpr void test_vector_exchange() {
  vector<int> v(3);
  v.add(1);
  v.add(2);
  v.exchange(0, 1);
  N_TEST_ASSERT_EQUALS(v.at(0), 2);
  N_TEST_ASSERT_EQUALS(v.at(1), 1);
}

constexpr void test_vector_modify() {
  vector<int> v(2);
  v.add(1);
  v.modify(2, 0);  // Modifier l'élément à la position 0
  N_TEST_ASSERT_EQUALS(v.at(0), 2);
}

constexpr void test_vector_remove() {
  vector<int> v(3);
  v.add(1);
  v.add(2);
  v.remove(0);
  N_TEST_ASSERT_EQUALS(v.len(), 1);
  N_TEST_ASSERT_EQUALS(v.at(0), 2);
}

constexpr void test_count_iterators() {
  array<int, 5> a = {1, 2, 3, 4, 5};
  auto cnt = count()(iter(a));
  N_TEST_ASSERT_EQUALS(cnt, 5);
}

constexpr void test_count_value() {
  array<int, 5> a = {1, 2, 1, 4, 1};
  auto cnt = count(1)(iter(a));
  N_TEST_ASSERT_EQUALS(cnt, 3);
}

constexpr void test_count_predicate() {
  array<int, 5> a = {1, 2, 3, 4, 5};
  auto cnt = count_if([](int i) { return i % 2 == 0; })(iter(a));
  N_TEST_ASSERT_EQUALS(cnt, 2);
}

constexpr void test_find_value() {
  array<int, 5> a = {1, 2, 3, 4, 5};
  auto it = find(3)(iter(a));
  N_TEST_ASSERT_TRUE(it.has_next() and it.next() == 3);
}

constexpr void test_find_predicate() {
  array<int, 5> a = {1, 2, 3, 4, 5};
  auto it = find_if([](int i) { return i > 3; })(iter(a));
  N_TEST_ASSERT_TRUE(it.has_next() and it.next() == 4);
}

constexpr void test_find_not() {
  array<int, 5> a = {1, 2, 3, 4, 5};
  auto it = find_if_not([](int i) { return i % 2 == 0; })(iter(a));
  N_TEST_ASSERT_TRUE(it.has_next() and it.next() == 1);
}
constexpr void test_all_of() {
  array<int, 5> a = {2, 4, 6, 8, 10};
  bool b = all_of([](int i) { return i % 2 == 0; })(iter(a));
  N_TEST_ASSERT_TRUE(b);
}
constexpr void test_any_of() {
  array<int, 5> a = {1, 2, 3, 4, 5};
  bool b = any_of([](int i) { return i % 2 == 0; })(iter(a));
  N_TEST_ASSERT_TRUE(b);
}

constexpr void test_none_of() {
  array<int, 5> a = {1, 2, 3, 4, 5};
  bool b = none_of([](int i) { return i == 0; })(iter(a));
  N_TEST_ASSERT_TRUE(b);
}

constexpr void test_starts_with() {
  array<int, 5> a1 = {1, 2, 3, 4, 5};
  array<int, 3> a2 = {1, 2, 3};
  bool b = starts_with(iter(a2))(iter(a1));
  N_TEST_ASSERT_TRUE(b);
}

constexpr void test_equals() {
  array<int, 5> a1 = {1, 2, 3, 4, 5};
  array<int, 5> a2 = {1, 2, 3, 4, 5};
  bool b = equals(iter(a2))(iter(a1));
  N_TEST_ASSERT_TRUE(b);
}

constexpr void test_string_size_constructor() {
  basic_string<char> s(10);
  N_TEST_ASSERT_EQUALS(s.max(), 10);
  N_TEST_ASSERT_TRUE(s.empty());
}

constexpr void test_string_default_constructor() {
  basic_string<char> s;
  N_TEST_ASSERT_TRUE(s.empty());
  N_TEST_ASSERT_EQUALS(s.len(), 0);
}

constexpr void test_string_copy_constructor() {
  basic_string<char> s1(10);
  s1.add('a');
  basic_string<char> s2(s1);
  N_TEST_ASSERT_EQUALS(s2.len(), s1.len());
  N_TEST_ASSERT_EQUALS(s2.at(0), 'a');
}

constexpr void test_string_move_constructor() {
  basic_string<char> s1(10);
  s1.add('a');
  basic_string<char> s2(move(s1));
  N_TEST_ASSERT_EQUALS(s2.len(), 1);
  N_TEST_ASSERT_TRUE(s1.empty());
}

constexpr void test_string_copy_assignment() {
  basic_string<char> s1(10);
  s1.add('a');
  basic_string<char> s2;
  s2 = s1;
  N_TEST_ASSERT_EQUALS(s2.len(), s1.len());
}

constexpr void test_string_move_assignment() {
  basic_string<char> s1(10);
  s1.add('a');
  basic_string<char> s2;
  s2 = move(s1);
  N_TEST_ASSERT_EQUALS(s2.len(), 1);
  N_TEST_ASSERT_TRUE(s1.empty());
}

constexpr void test_string_properties() {
  basic_string<char> s(10);
  N_TEST_ASSERT_TRUE(s.empty());
  N_TEST_ASSERT_FALSE(s.full());
  N_TEST_ASSERT_EQUALS(s.max(), 10);
  N_TEST_ASSERT_FALSE(s.has(0));
  s.add('a');
  N_TEST_ASSERT_TRUE(s.has(0));
}

constexpr void test_string_clear() {
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
    N_TEST_ASSERT_TRUE(false);
  } catch (out_of_range&) {
    N_TEST_ASSERT_TRUE(true);
  }
}

constexpr void test_string_add() {
  basic_string<char> s(2);
  s.add('a');
  s.add('b');
  N_TEST_ASSERT_EQUALS(s.at(0), 'a');
  N_TEST_ASSERT_EQUALS(s.at(1), 'b');
  s.add('c', 1);
  N_TEST_ASSERT_EQUALS(s.at(1), 'c');
  N_TEST_ASSERT_EQUALS(s.at(2), 'b');
}

constexpr void test_string_exchange() {
  basic_string<char> s(3);
  s.add('a');
  s.add('b');
  s.exchange(0, 1);
  N_TEST_ASSERT_EQUALS(s.at(0), 'b');
  N_TEST_ASSERT_EQUALS(s.at(1), 'a');
}

constexpr void test_string_modify() {
  basic_string<char> s(2);
  s.add('a');
  s.modify('b', 0);
  N_TEST_ASSERT_EQUALS(s.at(0), 'b');
}

constexpr void test_string_remove() {
  basic_string<char> s(3);
  s.add('a');
  s.add('b');
  s.remove(0);
  N_TEST_ASSERT_EQUALS(s.len(), 1);
  N_TEST_ASSERT_EQUALS(s.at(0), 'b');
}

constexpr void test_null_terminating_string() {
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

constexpr void test_string_view_constructors() {
  const char* str = "hello";
  basic_string_view<char> sv1(str, str + 5);
  N_TEST_ASSERT_EQUALS(sv1.len(), 5);

  basic_string_view<char> sv2(str, 5);
  N_TEST_ASSERT_EQUALS(sv2.len(), 5);

  basic_string_view<char> sv3(str);
  N_TEST_ASSERT_EQUALS(sv3.len(), 5);
}

constexpr void test_string_view_assignment() {
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
  N_TEST_ASSERT_TRUE(equals(iter(sv))(iter(string_view("hello"))));

  N_TEST_ASSERT_EQUALS(sv.at(0), 'h');
  try {
    sv.at(5);
    N_TEST_ASSERT_TRUE(false);
  } catch (out_of_range&) {
    N_TEST_ASSERT_TRUE(true);
  }
}

constexpr void test_span_constructors() {
  int arr[] = {1, 2, 3, 4, 5};
  span<int> sp1(arr, arr + 5);
  N_TEST_ASSERT_EQUALS(sp1.len(), 5);

  span<int> sp2(arr, 5);
  N_TEST_ASSERT_EQUALS(sp2.len(), 5);
}

constexpr void test_span_assignment() {
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
    N_TEST_ASSERT_TRUE(false);
  } catch (out_of_range& e) {
    N_TEST_ASSERT_TRUE(true);
  }
}

constexpr void test_span_exchange_modify() {
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

constexpr void test_array_len_max() {
  array<int, 5> arr;
  N_TEST_ASSERT_EQUALS(arr.len(), 5);
  N_TEST_ASSERT_EQUALS(arr.max(), 5);
}

constexpr void test_array_has() {
  array<int, 5> arr;
  N_TEST_ASSERT_TRUE(arr.has(4));
  N_TEST_ASSERT_FALSE(arr.has(5));
}

constexpr void test_array_data() {
  array<int, 5> arr;
  N_TEST_ASSERT_EQUALS(arr.data(), &arr.at(0));
}

void test_array_at() {
  array<int, 5> arr;
  arr.at(0) = 1;
  N_TEST_ASSERT_EQUALS(arr.at(0), 1);
  try {
    arr.at(5);
    N_TEST_ASSERT_TRUE(false);
  } catch (out_of_range&) {
    N_TEST_ASSERT_TRUE(true);
  }
}

constexpr void test_array_exchange() {
  array<int, 5> arr;
  arr.at(0) = 1;
  arr.at(1) = 2;
  arr.exchange(0, 1);
  N_TEST_ASSERT_EQUALS(arr.at(0), 2);
  N_TEST_ASSERT_EQUALS(arr.at(1), 1);
}

constexpr void test_array_modify() {
  array<int, 5> arr;
  arr.modify(3, 0);
  N_TEST_ASSERT_EQUALS(arr.at(0), 3);
}


void test_from_chars_istream_ostream() {
  string_view ss("hello");
  string so;
  from_chars(iter(ss), oter(so));
  N_TEST_ASSERT_TRUE(equals(iter(so))(iter(ss)));
}

void test_from_chars_istream_maybe_char() {
  string ss("a");
  maybe<char> c;
  from_chars(iter(ss), c);
  N_TEST_ASSERT_TRUE(c.has() and c.get() == 'a');
}

void test_from_chars_istream_maybe_uint() {
  string ss("123");
  maybe<unsigned int> i;
  from_chars(iter(ss), i);
  N_TEST_ASSERT_TRUE(i.has() and i.get() == 123);
}

void test_from_chars_istream_maybe_int() {
  string ss("-123");
  maybe<int> i;
  from_chars(iter(ss), i);
  N_TEST_ASSERT_TRUE(i.has() and i.get() == -123);
}

void test_from_chars_istream_maybe_bool() {
  string ss("1");
  maybe<bool> b;
  from_chars(iter(ss), b);
  N_TEST_ASSERT_TRUE(b.has() and b.get() == false);
}

void test_to_chars_ostream_istream() {
  string ss("hello");
  string so;
  to_chars(oter(so), iter(ss));
  N_TEST_ASSERT_TRUE(equals(iter(so))(iter(string_view("hello"))));
}

void test_to_chars_ostream_char() {
  string so;
  to_chars(oter(so), 'a');
  N_TEST_ASSERT_TRUE(equals(iter(so))(iter(string_view("a"))));
}

void test_to_chars_ostream_int() {
  string so;
  to_chars(oter(so), -123);
  N_TEST_ASSERT_TRUE(equals(iter(so))(iter(string_view("-123"))));
}

void test_to_chars_ostream_uint() {
  string so;
  to_chars(oter(so), 123u);
  N_TEST_ASSERT_TRUE(equals(iter(so))(iter(string_view("123"))));
}

void test_to_chars_ostream_bool() {
  string so;
  to_chars(oter(so), true);
  N_TEST_ASSERT_TRUE(equals(iter(so))(iter(string_view("true"))));
}

void test_to_hash_byte() {
  byte_t b = 'a';
  auto hash = to_hash<32>(b);
  N_TEST_ASSERT_EQUALS(hash, 3826002220);
}

void test_to_hash_byte_array() {
  const byte_t bs[] = {'a', 'b', 'c'};
  auto hash = to_hash<32>(bs);
  N_TEST_ASSERT_EQUALS(hash, 440920331);
}

void test_to_hash_wchar_t() {
  wchar_t c = L'a';
  auto hash = to_hash<32>(c);
  N_TEST_ASSERT_EQUALS(hash, 4125217764);
}

void test_to_hash_integral() {
  int i = 123;
  auto hash = to_hash<32>(i);
  N_TEST_ASSERT_EQUALS(hash, 442514334);
}

void test_to_hash_iterator() {
  string_view ss("test");
  auto hash = to_hash<32>(iter(ss));
  N_TEST_ASSERT_EQUALS(hash, 2949673445);
}

void test_list_constructors_assignments() {
  list<int> l1(10);
  N_TEST_ASSERT_EQUALS(l1.max(), 10);

  list<int> l2(l1);
  N_TEST_ASSERT_EQUALS(l2.max(), 10);

  list<int> l3;
  l3 = l2;
  N_TEST_ASSERT_EQUALS(l3.max(), 10);
}

void test_list_properties() {
  list<int> l(5);
  N_TEST_ASSERT_TRUE(l.empty());
  N_TEST_ASSERT_EQUALS(l.max(), 5);
  N_TEST_ASSERT_FALSE(l.full());
  N_TEST_ASSERT_FALSE(l.has(0));
}

void test_list_clear() {
  list<int> l(5);
  l.add(1);
  l.clear();
  N_TEST_ASSERT_TRUE(l.empty());
}

void test_list_at() {
  list<int> l(5);
  l.add(1);
  N_TEST_ASSERT_EQUALS(l.at(0), 1);
  try {
    l.at(1);
    N_TEST_ASSERT_TRUE(false);
  } catch (out_of_range&) {
    N_TEST_ASSERT_TRUE(true);
  }
}

void test_list_modifications() {
  list<int> l(5);
  l.add(1);
  l.add(2);
  l.modify(3, 1);
  N_TEST_ASSERT_EQUALS(l.at(1), 3);

  l.exchange(0, 1);
  N_TEST_ASSERT_EQUALS(l.at(0), 3);
  N_TEST_ASSERT_EQUALS(l.at(1), 1);

  l.remove(0);
  N_TEST_ASSERT_EQUALS(l.at(0), 1);
  N_TEST_ASSERT_EQUALS(l.len(), 1);
}

#include <core/map.hpp>

void test_map_constructors_assignments() {
  map<int, string> m1(10);
  N_TEST_ASSERT_EQUALS(m1.len(), 0);

  map<int, string> m2(m1);
  N_TEST_ASSERT_EQUALS(m2.len(), 0);

  map<int, string> m3;
  m3 = m2;
  N_TEST_ASSERT_EQUALS(m3.len(), 0);
}

void test_map_properties() {
  map<int, string> m;
  N_TEST_ASSERT_TRUE(m.empty());
  N_TEST_ASSERT_EQUALS(m.len(), 0);
  N_TEST_ASSERT_FALSE(m.has(1));
}

void test_map_modifications() {
  map<int, string> m;
  m.add(string_view("Hello"), 1);

  N_TEST_ASSERT_TRUE(m.has(1));
  N_TEST_ASSERT_TRUE(equals(iter(m.at(1)))(iter(string_view("Hello"))));

  m.modify(string_view("World"), 1);
  N_TEST_ASSERT_TRUE(equals(iter(m.at(1)))(iter(string_view("World"))));

  m.remove(1);
  N_TEST_ASSERT_FALSE(m.has(1));
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

  N_TEST_SUITE(array_tests);

  N_TEST_RUN(test_array_len_max);
  N_TEST_RUN(test_array_has);
  N_TEST_RUN(test_array_data);
  N_TEST_RUN(test_array_at);
  N_TEST_RUN(test_array_exchange);
  N_TEST_RUN(test_array_modify);

  N_TEST_RESULTS;

  N_TEST_SUITE(char_conversion_tests);

  N_TEST_RUN(test_from_chars_istream_ostream);
  N_TEST_RUN(test_from_chars_istream_maybe_char);
  N_TEST_RUN(test_from_chars_istream_maybe_uint);
  N_TEST_RUN(test_from_chars_istream_maybe_int);
  N_TEST_RUN(test_from_chars_istream_maybe_bool);
  N_TEST_RUN(test_to_chars_ostream_istream);
  N_TEST_RUN(test_to_chars_ostream_char);
  N_TEST_RUN(test_to_chars_ostream_int);
  N_TEST_RUN(test_to_chars_ostream_uint);
  N_TEST_RUN(test_to_chars_ostream_bool);

  N_TEST_RESULTS;

  N_TEST_SUITE(fnv_hash_tests);

  N_TEST_RUN(test_to_hash_byte);
  N_TEST_RUN(test_to_hash_byte_array);
  N_TEST_RUN(test_to_hash_wchar_t);
  N_TEST_RUN(test_to_hash_integral);
  N_TEST_RUN(test_to_hash_iterator);

  N_TEST_RESULTS

  N_TEST_SUITE(list_tests);

  N_TEST_RUN(test_list_constructors_assignments);
  N_TEST_RUN(test_list_properties);
  N_TEST_RUN(test_list_clear);
  N_TEST_RUN(test_list_at);
  N_TEST_RUN(test_list_modifications);

  N_TEST_RESULTS;

  N_TEST_SUITE(map_tests);

  N_TEST_RUN(test_map_constructors_assignments);
  N_TEST_RUN(test_map_properties);
  N_TEST_RUN(test_map_modifications);

  N_TEST_RESULTS;
}
