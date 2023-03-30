#ifndef __steward_meta_iterable_hpp__
#define __steward_meta_iterable_hpp__

#include <stew/meta/iterator.hpp>

namespace stew {
template <typename I>
concept iterable = requires(I i) {
                     { i.iter() } -> iterator;
                   };

template <typename I, typename T>
concept iterable_of = requires(I i) {
                        { i.iter() } -> iterator_of<T>;
                      };

}  // namespace stew

#endif
