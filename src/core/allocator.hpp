#ifndef __stew_core_allocator_hpp__
#define __stew_core_allocator_hpp__

template <typename T>
struct allocator {
  constexpr T* allocate(size_t n) const {
    return n == 0 ? nullptr : static_cast<T*>(::operator new(n * sizeof(T)));
  }

  constexpr void deallocate(T* p) const noexcept {
    if (p != nullptr) {
      delete p;
    }
  }

  template <typename... A>
  constexpr void construct(T* p, A&&... args) const {
    new (p) T(relay<A>(args)...);
  }

  constexpr void destroy(T* p) const noexcept {
    if (p != nullptr) {
      p->~T();
    }
  }

  constexpr void destroy(T* p, size_t n) const noexcept {
    if (p != nullptr) {
      for (size_t i = 0; i < n; ++i) {
        p[i].~T();
      }
    }
  }
};

#endif