#ifndef __stew_core_random_hpp__
#define __stew_core_random_hpp__

#include <core/core.hpp>

constexpr size_t prand(size_t a, size_t b, size_t seed) {
  seed = (a * seed + b) & max_of<unsigned>;
  return static_cast<size_t>(seed);
}

constexpr size_t rand(size_t seed) {
  return prand(1103515245, 12345, seed);
}

#endif