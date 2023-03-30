#ifndef __steward_container_hpp__
#define __steward_container_hpp__

namespace stew {
template <typename I>
concept iterable = requires(I i) { i.iter(); };

template <typename C, typename T>
concept push_container = requires(C c, T t) { c.push(t); };

template <typename C>
concept pop_container = requires(C c) { c.pop(); };

template <typename C>
concept size_container = requires(C c) { c.size(); };

template <typename C, typename T>
concept container = iterable<C> && push_container<C, T> && pop_container<C> &&
                    size_container<C>;

}  // namespace stew

#endif
