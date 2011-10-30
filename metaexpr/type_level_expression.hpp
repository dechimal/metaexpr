#if !defined ARBITAL_TYPE_LEVEL_EXPRESSION_HPP_INCLUDED_
#define      ARBITAL_TYPE_LEVEL_EXPRESSION_HPP_INCLUDED_

#include <cstddef>

namespace arbital { namespace typelevel {

template<typename T> struct type_t;
template<typename T> struct fun_t;
template<typename T> struct p;
template<typename T> struct a;
struct universal;
template<typename ...Ts> struct seq;
template<std::size_t I> struct size_t;
struct false_t;
struct true_t;
template<typename P, typename T> struct is_t;

template<typename T> auto val() -> T&;
template<template<typename...> class F, typename ...Ts> auto deref(F<Ts&...>&) -> F<Ts...>&;

// seq operations
template<typename ...Ts> auto seq_(Ts&...) -> seq<Ts...>&;
template<typename T, typename ...Ts> auto push_front(T&, seq<Ts...>&) -> seq<T, Ts...>&;
template<typename T, typename ...Ts> auto push_back(T&, seq<Ts...>&) -> seq<Ts..., T>&;
template<typename T, typename ...Ts> auto pop_front(seq<T, Ts...>&) -> seq<Ts...>&;
template<typename T, typename ...Ts> auto at(size_t<0>&, seq<T, Ts...>&) -> T&;
template<std::size_t I, typename T, typename ...Ts> auto at(size_t<I>&, seq<T, Ts...>&)
    -> decltype(at(val<size_t<I-1> >(), val<seq<Ts...> >()));

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
    -> decltype(replace1(i, pop_front(ps), pop_front(as)));
template<template<typename...> class Con, typename ...Ts,
         typename ...Params,
         typename ...Args>
auto replace1(Con<Ts...>&, seq<Params...>& ps, seq<Args...>& as)
    -> decltype(deref(val<Con<decltype(replace1(val<Ts>(), ps, as))...> >()));
template<typename P, typename T,
         typename ...Params,
         typename ...Args>
auto replace1(is_t<P, T>&, seq<Params...>& ps, seq<Args...>& as)
    -> decltype(is1(bind(val<seq<P> >(), val<seq<decltype(replace1(val<T>(), ps, as)))> >()));
template<typename Def,
         typename ...Params,
         typename ...Args>
auto replace1(Def&, seq<Params...>&, seq<Args...>&)
    -> Def&;

template<typename Def, typename ...Params, typename ...Args>
auto replace(Def& d, seq<seq<Params...>, seq<Args...> >&)
    -> decltype(replace1(d, val<seq<Params...> >(), val<seq<Args...> >()))&;

// destruct values and bind variables
template<typename ...R1, typename ...R2>
auto bind1(seq<seq<R1, R2>...>& r,
           seq<>&, seq<>&)
    -> decltype(r);
template<typename ...R1, typename ...R2,
         typename I, typename ...Params,
         typename Arg, typename ...Args>
auto bind1(seq<seq<R1, R2>...>& r,
           seq<p<I>, Params...>& ps,
           seq<Arg, Args...>& as)
    -> decltype(bind1(push_back(val<seq<p<I>, Arg> >(), r),
                      pop_front(ps),
                      pop_front(as)));
template<typename ...R1, typename ...R2,
         typename Con, typename ...Params,
         typename ...Args>
auto bind1(seq<seq<R1, R2>...>& r,
           seq<type_t<Con>, Params...>& ps,
           seq<type_t<Con>, Args...>& as)
    -> decltype(bind1(r,
                      pop_front(ps),
                      pop_front(as)));
template<typename ...R1, typename ...R2,
         typename ...Params,
         typename Arg, typename ...Args>
auto bind1(seq<seq<R1, R2>...>& r,
           seq<p<universal>, Params...>& ps,
           seq<Arg, Args...>& as)
    -> decltype(bind1(r,
                      pop_front(ps),
                      pop_front(as)));
template<typename ...R1, typename ...R2,
         template<typename ...> class Con, typename ...Ts, typename ...Params,
         typename ...Us, typename ...Args>
auto bind1(seq<seq<R1, R2>...>& r,
           seq<Con<Ts...>, Params...>&,
           seq<Con<Us...>, Args...>&)
    -> decltype(bind1(r,
                      val<seq<Ts..., Params...> >(),
                      val<seq<Us..., Args...> >()));
template<typename ...R1, typename ...R2,
         typename ...Params,
         typename ...Args>
auto bind1(seq<seq<R1, R2>...>& r,
           seq<R2...>&,
           seq<Params...>&,
           seq<Args...>&)
    -> false_t&;

template<typename ...Params, typename ...Args>
auto bind(seq<Params...>& ps, seq<Args...>& as)
    -> decltype(bind1(seq_(), ps, as));

template<typename P, typename T> auto is(T& x) -> is_t<P, T>&;
template<typename ...Params, typename ...Args> auto is1(seq<Params..., Args...>&) -> true_t;
auto is1(false_t&) -> false_t&;

template<typename Def, typename ...Params>
struct fun_t<Def(Params...)> {
    template<typename ...Args>
    auto operator()(Args& ...) -> decltype(replace(val<Def>(), bind(val<seq<Params...> >(), val<seq<Args...> >())));
};


typedef p<universal> _;
typedef p<size_t<0> > p0;
typedef p<size_t<1> > p1;
typedef p<size_t<2> > p2;
typedef p<size_t<3> > p3;
typedef a<size_t<0> > a0;
typedef a<size_t<1> > a1;
typedef a<size_t<2> > a2;
typedef a<size_t<3> > a3;

template<typename T> auto type() -> type_t<T>&;
template<typename T> auto type(T&) -> type_t<T>&;
template<typename T> auto type(type_t<T>&) -> type_t<T>&;
template<typename T> auto run(type_t<T>&) -> T;

}}

#define ARBITAL_TYPE_LEVEL_VALUE_OF(...) ::arbital::typelevel::val<__VA_ARGS__>()
#define ARBITAL_TYPE_LEVEL_RUN(...) decltype(::arbital::typelevel::run(__VA_ARGS__))

#endif
