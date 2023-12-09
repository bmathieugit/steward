#ifndef __n_tests_hpp__
#define __n_tests_hpp__

#include <stdio.h>

#define ___STRINGIZE_DETAIL(x) #x
#define ___STRINGIZE(x) ___STRINGIZE_DETAIL(x)
#define __SLINE__ ___STRINGIZE(__LINE__)

#define N_TEST_ASSERT_TRUE(condition)                              \
  if (!(condition)) {                                              \
    throw __FILE__ ":" __SLINE__ " : expected true : " #condition; \
  }

#define N_TEST_ASSERT_FALSE(condition)                              \
  if (condition) {                                                  \
    throw __FILE__ ":" __SLINE__ " : expected false : " #condition; \
  }

#define N_TEST_ASSERT_EQUALS(v0, v1)                     \
  if (!((v0) == (v1))) {                                 \
    throw __FILE__ ":" __SLINE__                         \
                   " : expected equal : expected value " \
                   ": " #v0 ", actual value : " #v1;     \
  }

#define N_TEST_SUITE(name) \
  {                        \
    int success = 0;       \
    int failure = 0;       \
    printf("\nSUITE [\033[1m%s\033[0m]\n\n", #name);

#define N_TEST_RUN(t)                                    \
  try {                                                  \
    printf("TEST [\033[1m%s\033[0m]\n\n", #t);                         \
    t();                                                 \
    ++success;                                           \
    printf("---> Result : \033[1;32mPASSED\033[0m\n\n"); \
  } catch (const char* e) {                              \
    printf("\033[1;31merror : %s\033[0m\n\n", e);        \
    ++failure;                                           \
    printf("---> Result : \033[1;31mFAILED\033[0m\n\n"); \
  }

#define N_TEST_RESULTS                                            \
  printf(                                                         \
      "\nSUITE RECAP - Passed : \033[1;32m%d \033[0m - Failed : " \
      "\033[1;31m%d\033[0m\n\n",                                  \
      success, failure);                                          \
  }
;

#endif
