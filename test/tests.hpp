#ifndef __n_tests_hpp__
#define __n_tests_hpp__

#include <core/array.hpp>
#include <core/char-ostream.hpp>
#include <core/io.hpp>
#include <core/string.hpp>

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

#define N_TEST_REGISTER(t) suite.add(test(#t, (t)));

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

 public:
  constexpr const char* name() const { return _name; }
  constexpr test_result run() const try {
    _func();
    return test_result::success;
  } catch (const char* e) {
    sout << "\033[1;31merror : " << e << "\033[0m\n";
    return test_result::failure;
  }
};

class test_suite {
 private:
  array<test, 100> _tests;
  const char* _name = nullptr;

 public:
  constexpr test_suite() = default;
  constexpr test_suite(const char* name) : _name(name) {}

 public:
  constexpr void add(const test& t) { _tests.add(t); }

 public:
  constexpr void run_all() {
    int succeed = 0;
    int failed = 0;

    sout << "\nSUITE [" << _name << "] ";

    auto tests_os = istream(_tests);

    while (tests_os.has()) {
      auto&& t = tests_os.next();
      sout << "\nTEST[" << t.name() << "]\n";
      auto res = t.run();

      sout << (res == test_result::success
                   ? " - Result : \033[1;32mPASSED\033[0m\n"
                   : " - Result : \033[1;31mFAILED\033[0m\n");

      if (res == test_result::success) {
        ++succeed;
      } else {
        ++failed;
      }
    }

    sout << "\nSUITE RECAP "
         << " - Passed : \033[1;32m" << succeed << "\033[0m\n"
         << " - Failed : \033[1;31m" << failed << "\033[0m\n"
         << '\n';
  }
};

#endif
