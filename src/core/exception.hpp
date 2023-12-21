#ifndef __core_exception_hpp__
#define __core_exception_hpp__

#include <core/core.hpp>

template <typename E>
concept exception = requires(E e) {
  { e.message() } -> same_as<const char*>;
};

class out_of_range {
 public:
  const char* message() { return "out_of_range exception"; }
};

class bad_alloc {
 public:
  const char* message() { return "bad_alloc exception"; }
};
#endif