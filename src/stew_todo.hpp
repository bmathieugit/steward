#ifndef __stew_todo_hpp__
#define __stew_todo_hpp__

namespace stew
{
  template <typename T0, typename... Tn>
  consteval size_t sizeofmax()
  {
    if constexpr (sizeof...(Tn) == 0)
    {
      return sizeof(T0);
    }
    else
    {
      if (sizeof(T0) > sizeofmax<Tn...>())
      {
        return sizeof(T0);
      }
      else
      {
        return sizeofmax<Tn...>();
      }
    }
  }

  namespace impl
  {
    template <size_t I, typename H, typename... T>
    struct indexof
    {
      static_assert(sizeof...(T) > 0, "indexof : search type not in types pack");
    };

    template <size_t I, typename H, typename T0, typename... Tn>
    struct indexof<I, H, T0, Tn...>
    {
      static constexpr size_t value = indexof<I + 1, H, Tn...>::value;
    };

    template <size_t I, typename T, typename... Tn>
    struct indexof<I, T, T, Tn...>
    {
      static constexpr size_t value = I;
    };
  }

  template <typename H, typename... T>
  constexpr size_t indexof = impl::indexof<0, H, T...>::value;

  namespace impl
  {
    template <size_t I, typename T0, typename... Tn>
    struct typeat : struct_type<typeat<I - 1, Tn...>>
    {
    };

    template <typename T0, typename... Tn>
    struct typeat<0, T0, Tn...> : struct_type<T0>
    {
    };
  }

  template <size_t I, typename... T>
  using typeat = type<impl::typeat<I, T...>>;

  namespace todo
  {
    template <class T>
    struct type_identity
    {
      using type = T;
    };

    template <typename T>
    using type_identity_t = typename type_identity<T>::type;

    template <size_t... I>
    struct isequence
    {
    };

    namespace impl
    {
      template <size_t I0, size_t... In>
      consteval auto make_isequence()
      {
        if constexpr (I0 == 0)
        {
          return isequence<I0, In...>{};
        }
        else
        {
          return make_isequence<I0 - 1, I0, In...>();
        }
      }
    }

    template <size_t N>
      requires(N > 0)
    constexpr auto make_isequence()
    {
      return impl::make_isequence<N - 1>();
    }

    template <typename T>
    constexpr size_t array_size = 0;

    template <typename... T>
    constexpr size_t array_size<tuple<T...>> = sizeof...(T);

    template <typename T, typename F, size_t... I>
    consteval auto apply(T &&t, F &&f, isequence<I...>) -> decltype(auto)
    {
      return forward<F>(f)(get<I>(forward<T>(t))...);
    }

    template <typename T, typename F>
    consteval auto apply(T &&t, F &&f) -> decltype(auto)
    {
      return apply(forward<T>(t), forward<F>(f), make_isequence<array_size<rm_cvref<T>>>());
    }
  }

}

#endif