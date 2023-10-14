#ifndef __n_tests_hpp__
#define __n_tests_hpp__

#include <stdio.h>
#include <string.h>

#define N_TEST_ASSERT_TRUE(condition)    \
  if (!(condition)) {                    \
    throw "expected true : " #condition; \
  }

#define N_TEST_ASSERT_FALSE(condition)    \
  if (condition) {                        \
    throw "expected false : " #condition; \
  }

#define N_TEST_ASSERT_EQUALS(v0, v1)                                        \
  if (!((v0) == (v1))) {                                                    \
    throw "expected equal : expected value : " #v0 ", actual value : " #v1; \
  }

#define N_TEST_SUITE(name) \
  {                        \
    test_suite suite(name);

#define N_TEST_REGISTER(t) suite.push(test(#t, (t)));

#define N_TEST_RUN_SUITE \
  suite.run_all();       \
  }


using test_function = void (*)();
enum class test_result { success, failure };

class test {
 private:
  const char* _name = nullptr;
  test_function _func = nullptr;

 public:
  constexpr test() = default;
  constexpr test(const char* name, test_function func)
      : _name(name), _func(func) {}

  // copy
  constexpr test(const test&) = default;
  constexpr test& operator=(const test&) = default;

  // move
  constexpr test(test&&) = default;
  constexpr test& operator=(test&&) = default;

 public:
  constexpr const char* name() const { return _name; }
  constexpr test_result run() const try {
    _func();
    return test_result::success;
  } catch (const char* e) {
    ::printf("\033[1;31merror : %s\033[0m\n", e);
    return test_result::failure;
  }
};

class test_vector {
 private:
  test* _tests = nullptr;
  int _size = 0;
  int _max = 0;

 public:
  constexpr ~test_vector() {
    if (_tests != nullptr) delete[] _tests;
    _size = 0;
    _max = 0;
  }

  constexpr test_vector(int max) : _tests(new test[max]), _size(0), _max(max) {}

  // copy
  constexpr test_vector(const test_vector&) = delete;
  constexpr test_vector& operator=(const test_vector&) = delete;

  // move
  constexpr test_vector(test_vector&&) = default;
  constexpr test_vector& operator=(test_vector&&) = default;

  constexpr int size() const { return _size; }
  constexpr bool empty() const { return _size == 0; }
  constexpr int max() const { return _max; }

  constexpr void push(const test& t) {
    if (_size != _max) {
      _tests[_size] = t;
      ++_size;
    }
  }

  constexpr test& at(int i) { return _tests[i]; }
};

namespace str {

template <int N>
void padln(char (&buffer)[N], char c) {
  int len = ::strlen(buffer);

  for (int i = len; i < N - 1; ++i) {
    buffer[i] = c;
  }

  buffer[N - 2] = '\n';
  buffer[N - 1] = '\0';
}
}  // namespace str

class test_suite {
 private:
  test_vector _tests{100};
  const char* _name = nullptr;

 public:
  constexpr test_suite() = default;
  constexpr test_suite(const char* name) : _name(name) {}

 public:
  constexpr void push(const test& t) { _tests.push(t); }

 public:
  void run_all() {
    constexpr int MAX = 80;
    char buffer[MAX];

    int succeed = 0;
    int failed = 0;

    ::snprintf(buffer, MAX - 1, "\nSUITE [%s] ", _name);
    str::padln(buffer, '*');
    ::printf("%s", buffer);

    for (int i = 0; i < _tests.size(); ++i) {
      ::snprintf(buffer, MAX - 1, "\nTEST [%s] ", _tests.at(i).name());
      str::padln(buffer, '*');
      ::printf("%s", buffer);
      test_result res = _tests.at(i).run();

      if (res == test_result::success) {
        ++succeed;
        ::printf(" - Result : \033[1;32mPASSED\033[0m\n");
      } else {
        ++failed;
        ::printf(" - Result : \033[1;31mFAILED\033[0m\n");
      }
    }
    ::snprintf(buffer, MAX - 1, "\nSUITE RECAP ");
    str::padln(buffer, '*');
    ::printf("%s", buffer);
    ::printf(" - Passed : \033[1;32m%d\033[0m\n", succeed);
    ::printf(" - Failed : \033[1;31m%d\033[0m\n", failed);

    ::printf("\n");
  }
};


#endif
