#ifndef __steward_meta_iterator_hpp__
#define __steward_meta_iterator_hpp__

#include <stew/meta.hpp>

namespace stew {

template <typename I>
concept iterator = requires(I i) {
                     i.has_next();
                     i.next();
                   };

template <typename I>
concept sizeable_iterator = iterator<I> && requires(I i) { i.size(); };

template <typename I, typename T>
concept iterator_of = iterator<I> && requires(I i) {
                                       { i.next() } -> same_as<T>;
                                     };

}  // namespace stew

#endif
