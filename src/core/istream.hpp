#ifndef __n_istream_hpp__
#define __n_istream_hpp__

#include <core/utils.hpp>



template <typename I>
concept istream = requires(I i) {
  typename I::type;
  { i.get() } -> same_as<typename I::type>;
  { i.has() } -> same_as<bool>;
  { i.next() } -> same_as<bool>;
};



#endif
