#if !defined ARBITAL_TYPE_LEVEL_EXPRESSION_HPP_INCLUDED_
#define      ARBITAL_TYPE_LEVEL_EXPRESSION_HPP_INCLUDED_

#include <cstddef>

namespace arbital { namespace typelevel {

template<typename T> struct type;
template<typename T> struct p;
struct universal;
template<typename ...Ts> struct seq;
template<std::size_t I> struct size_t;

namespace adl_barrier {
namespace here = adl_barrier;

template<typename T> auto val() -> T&;
template<template<typename...> class F, typename ...Ts> auto deref(F<Ts&...>&) -> F<Ts...>&;

// seq operations
template<typename ...Ts> auto seq_(Ts&...) -> seq<Ts...>&;
template<typename T, typename ...Ts> auto push_front(T&, seq<Ts...>&) -> seq<T, Ts...>&;
template<typename T, typename ...Ts> auto push_back(T&, seq<Ts...>&) -> seq<Ts..., T>&;
template<typename T, typename ...Ts> auto pop_front(seq<T, Ts...>&) -> seq<Ts...>&;
template<typename T, typename ...Ts> auto at(size_t<0>&, seq<T, Ts...>&) -> T&;
template<std::size_t I, typename T, typename ...Ts> auto at(size_t<I>&, seq<T, Ts...>&)
    -> decltype(here::at(val<size_t<I-1> >(), val<seq<Ts...> >()));

// replace placeholders in body with arguments
template<typename I,
         typename ...Params,
         typename Arg, typename ...Args>
auto replace1(p<I>&, seq<p<I>, Params...>&, seq<Arg, Args...>&)
    -> Arg&;
template<typename I,
         typename J, typename ...Params,
         typename Arg, typename ...Args>
auto replace1(p<I>& i, seq<p<J>, Params...>& ps, seq<Arg, Args...>& as)
    -> decltype(here::replace1(i, pop_front(ps), pop_front(as)));
template<template<typename...> class Con, typename ...Ts,
         typename ...Params,
         typename ...Args>
auto replace1(Con<Ts...>&, seq<Params...>& ps, seq<Args...>& as)
    -> decltype(here::deref(val<Con<decltype(here::replace1(val<Ts>(), ps, as))...> >()));
template<typename Def,
         typename ...Params,
         typename ...Args>
auto replace1(Def&, seq<Params...>&, seq<Args...>&)
    -> Def&;

template<typename Def, typename ...Params, typename ...Args>
auto replace(Def& d, seq<seq<Params...>, seq<Args...> >&)
    -> decltype(here::replace1(d, val<seq<Params...> >(), val<seq<Args...> >()))&;

// destruct values and bind variables
template<typename ...R1,
         typename ...R2>
auto bind1(seq<R1...>& r1, seq<R2...>& r2, seq<>&, seq<>&)
    -> decltype(here::seq_(r1, r2));
template<typename ...R1,
         typename ...R2,
         typename I, typename ...Params,
         typename Arg, typename ...Args>
auto bind1(seq<R1...>& r1,
           seq<R2...>& r2,
           seq<p<I>, Params...>& ps,
           seq<Arg, Args...>& as)
    -> decltype(here::bind1(here::push_back(val<p<I> >(), r1),
                            here::push_back(val<Arg>(), r2),
                            here::pop_front(ps),
                            here::pop_front(as)));
template<typename ...R1,
         typename ...R2,
         typename Con, typename ...Params,
         typename ...Args>
auto bind1(seq<R1...>& r1,
           seq<R2...>& r2,
           seq<type_t<Con>, Params...>& ps,
           seq<type_t<Con>, Args...>& as)
    -> decltype(here::bind1(r1,
                            r2,
                            here::pop_front(ps),
                            here::pop_front(as)));
template<typename ...R1,
         typename ...R2,
         typename ...Params,
         typename Arg, typename ...Args>
auto bind1(seq<R1...>& r1,
           seq<R2...>& r2,
           seq<p<universal>, Params...>& ps,
           seq<Arg, Args...>& as)
    -> decltype(here::bind1(r1,
                            r2,
                            here::pop_front(ps),
                            here::pop_front(as)));
template<typename ...R1,
         typename ...R2,
         template<typename ...> class Con, typename ...Ts, typename ...Params,
         typename ...Us, typename ...Args>
auto bind1(seq<R1...>& r1,
           seq<R2...>& r2,
           seq<Con<Ts...>, Params...>&,
           seq<Con<Us...>, Args...>&)
    -> decltype(here::bind1(r1,
                            r2,
                            val<seq<Ts..., Params...> >(),
                            val<seq<Us..., Args...> >()));

template<typename ...Params, typename ...Args>
auto bind(seq<Params...>& ps, seq<Args...>& as)
    -> decltype(here::bind1(seq_(), seq_(), ps, as));

}

using namespace adl_barrier;

template<typename Def, typename ...Params>
struct type<Def(Params...)> {
    template<typename ...Args>
    auto operator()(Args& ...) -> decltype(typelevel::replace(typelevel::val<Def>(), typelevel::bind(typelevel::val<seq<Params...> >(), typelevel::val<seq<Args...> >())));
};


typedef p<universal> _;
typedef p<size_t<0> > p0;
typedef p<size_t<1> > p1;
typedef p<size_t<2> > p2;
typedef p<size_t<3> > p3;

template<typename T> auto ty(T&) -> type<T>&;
template<typename T> auto ty(type<T>&) -> type<T>&;

}}

#define ARBITAL_TYPE_LEVEL_VALUE_OF(...) ::arbital::typelevel::val<__VA_ARGS__>()

#endif
