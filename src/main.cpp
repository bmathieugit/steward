// #include <dbfile.hpp>

#include <stew.hpp>
#include <threads.h>
#include <any>
#include <tuple>

template <class... T>
void task(T... args)
{
  stew::cout.printfln("{} {} coucou {} ! comment vas-tu ? {}", args...);
}

// namespace
// {
//   template <typename F, typename... A>
//   concept thread_runnable =
//       requires(F &&f, A &&...a) {
//         std::apply(f, stew::tuple<stew::rm_ref<A>...>(stew::move(a)...));
//       };

//   template <typename F, typename... A>
//   int run_thread(void *ctx)
//   {
//     auto &pctx = *static_cast<stew::tuple<F, stew::tuple<A...>> *>(ctx);
//     std::apply(stew::get<0>(pctx), stew::get<1>(pctx));
//     return 0;
//   }

//   template <typename F, typename... A>
//   void spawn_thread(F &&f, A &&...a)
//   {
//     thrd_t t;
//     stew::tuple<stew::rm_ref<A>...> args(stew::move(a)...);
//     stew::tuple<stew::rm_ref<F>, stew::tuple<stew::rm_ref<A>...>> ctx(f, args);
//     thrd_create(&t, run_thread<stew::rm_ref<F>, stew::rm_ref<A>...>, &ctx);

//     int res;
//     thrd_join(t, &res);
//   }
// }

int main()
{
  // spawn_thread([](auto &&...args)
  //              { task(args...); },
  //              stew::string_view("Benjamin"), 12, 24, true);

  stew::tuple<int, int> i2(1, 2);
  stew::get<1>(i2) = 31;
  // stew::cout.printfln("{} {}", stew::get<0>(i2), stew::get<1>(i2));
  stew::tuple<int&, int&> i22(stew::get<0>(i2), stew::get<1>(i2));
  stew::cout.printfln("{} {}", stew::get<0>(i22), stew::get<1>(i22));

  return EXIT_SUCCESS;
}