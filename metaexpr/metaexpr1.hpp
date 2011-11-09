#if !defined METAEXPR_METAEXPR1_HPP_INCLUDED_
#define      METAEXPR_METAEXPR1_HPP_INCLUDED_

#include "metaexpr/compiler.hpp"
#include <cstddef>
#include "boost/preprocessor/cat.hpp"
#include "boost/preprocessor/control/if.hpp"
#include "boost/preprocessor/logical/not.hpp"
#include "boost/preprocessor/comparison/equal.hpp"
#include "boost/preprocessor/repetition/for.hpp"
#include "boost/preprocessor/repetition/enum_binary_params.hpp"
#include "boost/preprocessor/repetition/enum_params.hpp"
#include "boost/preprocessor/facilities/is_empty.hpp"
#include "boost/preprocessor/facilities/intercept.hpp"
#include "boost/preprocessor/variadic/to_seq.hpp"
#include "boost/preprocessor/variadic/size.hpp"
#include "boost/preprocessor/tuple/elem.hpp"
#include "boost/preprocessor/tuple/rem.hpp"
#include "boost/preprocessor/seq/for_each.hpp"
#include "boost/preprocessor/seq/for_each_i.hpp"
#include "boost/preprocessor/punctuation/comma_if.hpp"

// ADLでの誤爆はせめてambiguousにしようという意図から
// 全ての関数の引数の型パターンには具体的なクラステンプレートが入っている

namespace metaexpr {

namespace m = metaexpr;

#define METAEXPR_TYPE(name, ctors) \
    template<typename> struct name \
    BOOST_PP_FOR((name, ctors), METAEXPR_VALUE_CTOR_P, METAEXPR_VALUE_CTOR_OP, METAEXPR_VALUE_CTOR_M)

#define METAEXPR_VALUE_CTOR_OP(r, state) \
    METAEXPR_VALUE_CTOR_OP_I(BOOST_PP_TUPLE_REM(2) state)
#define METAEXPR_VALUE_CTOR_OP_I(state) \
    METAEXPR_VALUE_CTOR_OP_II(state)
#define METAEXPR_VALUE_CTOR_OP_II(type, ctors) \
    (type, BOOST_PP_TUPLE_EAT() ctors)

#define METAEXPR_VALUE_CTOR_P(r, state) \
    BOOST_PP_NOT(METAEXPR_TUPLE_SEQ_IS_EMPTY(BOOST_PP_TUPLE_ELEM(2, 1, state)))

#define METAEXPR_VALUE_CTOR_M(r, state) \
    METAEXPR_VALUE_CTOR_M_I(BOOST_PP_TUPLE_ELEM(2, 0, state), BOOST_PP_TUPLE_ELEM(2, 1, state))
#define METAEXPR_VALUE_CTOR_M_I(type, ctors) \
    METAEXPR_VALUE_CTOR_M_II(type, METAEXPR_TUPLE_SEQ_HEAD(ctors))
#define METAEXPR_VALUE_CTOR_M_II(type, ctor) \
    METAEXPR_VALUE_CTOR_M_III(type, BOOST_PP_TUPLE_REM() ctor BOOST_PP_COMMA_IF(BOOST_PP_EQUAL(BOOST_PP_VARIADIC_SIZE ctor, 1)))
#define METAEXPR_VALUE_CTOR_M_III(type, ctor) \
    METAEXPR_VALUE_CTOR(type, ctor)

// value constructor generator
#define METAEXPR_VALUE_CTOR(type, tag, ...) \
    METAEXPR_VALUE_CTOR_I(type, \
                          tag, \
                          BOOST_PP_VARIADIC_TO_SEQ (__VA_ARGS__), \
                          BOOST_PP_VARIADIC_SIZE (__VA_ARGS__), \
                          BOOST_PP_IS_EMPTY (__VA_ARGS__))
#define METAEXPR_VALUE_CTOR_I(type, tag, fields, size, empty) \
    METAEXPR_VALUE_CTOR_II(type, tag, fields, size, empty)
#define METAEXPR_VALUE_CTOR_II(type, tag, fields, size, empty) \
    BOOST_PP_IF(empty, METAEXPR_VALUE_CTOR_0, METAEXPR_VALUE_CTOR_N) (\
        type, tag, BOOST_PP_CAT(tag, _impl), fields, size)

#define METAEXPR_VALUE_CTOR_0(type, tag, tag_impl, fields, size) ;\
    struct tag_impl; \
    using tag = type<tag_impl>; \
    auto BOOST_PP_CAT(mk_, tag)() -> tag&

#define METAEXPR_VALUE_CTOR_N(type, tag, tag_impl, fields, size) \
    METAEXPR_VALUE_CTOR_N_I(type, tag, tag_impl, fields, size)
#define METAEXPR_VALUE_CTOR_N_I(type, tag, tag_impl, fields, size) \
    METAEXPR_VALUE_CTOR_N_II(type, tag, tag_impl, fields, size)
#define METAEXPR_VALUE_CTOR_N_II(type, tag, tag_impl, fields, size) ;\
    template<BOOST_PP_ENUM_PARAMS(size, typename BOOST_PP_INTERCEPT)> struct tag_impl; \
    template<BOOST_PP_ENUM_PARAMS(size, typename T)> \
    using tag = type<tag_impl<BOOST_PP_ENUM_PARAMS(size, T)> >; \
    template<BOOST_PP_ENUM_PARAMS(size, typename T)> \
    auto BOOST_PP_CAT(mk_, tag)(BOOST_PP_ENUM_BINARY_PARAMS(size, T ,& BOOST_PP_INTERCEPT)) \
        -> tag<BOOST_PP_ENUM_PARAMS(size, T)>&; \
    METAEXPR_VALUE_CTOR_N_FIELD_ACCESSORS(tag, size, fields)
#define METAEXPR_VALUE_CTOR_N_FIELD_ACCESSORS(tag, size, fields) \
    BOOST_PP_SEQ_FOR_EACH_I(METAEXPR_VALUE_CTOR_N_FIELD_ACCOESSORS_M, (tag, size), fields)
#define METAEXPR_VALUE_CTOR_N_FIELD_ACCOESSORS_M(r, data, i, field) \
    METAEXPR_VALUE_CTOR_N_FIELD_ACCOESSORS_M_I(r, BOOST_PP_TUPLE_REM(2) data, i, field)
#define METAEXPR_VALUE_CTOR_N_FIELD_ACCOESSORS_M_I(r, data, i, field) \
    METAEXPR_VALUE_CTOR_N_FIELD_ACCOESSORS_M_II(r, data, i, field)
#define METAEXPR_VALUE_CTOR_N_FIELD_ACCOESSORS_M_II(r, tag, size, i, field) \
    template<BOOST_PP_ENUM_PARAMS(size, typename T)> auto \
    BOOST_PP_CAT(tag, BOOST_PP_CAT(_, field)) \
        (tag<BOOST_PP_ENUM_PARAMS(size, T)>&) -> BOOST_PP_CAT(T, i)&

#define METAEXPR_TUPLE_SEQ_IS_EMPTY(ts) \
    METAEXPR_TUPLE_SEQ_I(BOOST_PP_CAT(METAEXPR_TUPLE_SEQ_CHECK_, METAEXPR_TUPLE_SEQ_IS_EMPTY_HELPER ts))
#define METAEXPR_TUPLE_SEQ_I(x) \
    METAEXPR_TUPLE_SEQ_II(x)
#define METAEXPR_TUPLE_SEQ_II(res, x) \
    res

#define METAEXPR_TUPLE_SEQ_IS_EMPTY_HELPER(...) METAEXPR_TUPLE_SEQ_IS_EMPTY_HELPER_I
#define METAEXPR_TUPLE_SEQ_CHECK_METAEXPR_TUPLE_SEQ_IS_EMPTY_HELPER 1,
#define METAEXPR_TUPLE_SEQ_CHECK_METAEXPR_TUPLE_SEQ_IS_EMPTY_HELPER_I 0,

#define METAEXPR_TUPLE_SEQ_HEAD(ts) \
    METAEXPR_TUPLE_SEQ_HEAD_I(METAEXPR_TUPLE_SEQ_HEAD_HELPER ts)
#define METAEXPR_TUPLE_SEQ_HEAD_I(ts) \
    METAEXPR_TUPLE_SEQ_HEAD_II(ts)
#define METAEXPR_TUPLE_SEQ_HEAD_II(t, ...) \
    t
#define METAEXPR_TUPLE_SEQ_HEAD_HELPER(...) (__VA_ARGS__),

// bool
METAEXPR_TYPE(bool_,
              (true_t)
              (false_t));

extern true_t& true_;
extern false_t& false_;

// and_
auto and_(false_t&, false_t&) -> decltype(false_);
auto and_(true_t&, false_t&) -> decltype(false_);
auto and_(false_t&, true_t&) -> decltype(false_);
auto and_(true_t&, true_t&) -> decltype(true_);
// or_
auto or_(false_t&, false_t&) -> decltype(false_);
auto or_(true_t&, false_t&) -> decltype(true_);
auto or_(false_t&, true_t&) -> decltype(true_);
auto or_(true_t&, true_t&) -> decltype(true_);
auto not_(true_t&) -> decltype(false_);
auto not_(false_t&) -> decltype(true_);
template<typename Q> // implies
auto implies(true_t& p, Q& q) -> decltype(m::or_(m::not_(p), q));
template<typename Q>
auto implies(false_t& p, Q& q) -> decltype(m::or_(m::not_(p), q));

// nat
METAEXPR_TYPE(nat,
              (z)(s, n));
extern z& zero;

template<typename N> // succ
auto succ(N& n) -> decltype(mk_s(n));
template<typename N> // pred
auto pred(s<N>& n) -> decltype(s_n(n));

// maybe
METAEXPR_TYPE(maybe,
              (just, value)
              (nothing));

template<typename T> // from_just
auto from_just(just<T>& t) -> decltype(just_value(t));

// sequence
template<typename ...Ts> struct seq;
template<typename T, typename ...Ts> // cons
auto cons(T&, seq<Ts...>&) -> seq<T, Ts...>&;
template<typename T, typename ...Ts> // head
auto head(seq<T, Ts...>& ts) -> T&;
template<typename T, typename ...Ts> // tail
auto tail(seq<T, Ts...>&) -> seq<Ts...>&;

template<typename ...Ts> // mk_seq
auto mk_seq(Ts&...) -> seq<Ts...>&;

extern seq<>& nil;

template<typename T, typename ...Ts> // push_front
auto push_front(T& x, seq<Ts...>& xs) -> decltype(cons(x, xs));
template<typename T, typename ...Ts> // push_back
auto push_back(T&, seq<Ts...>&) -> seq<Ts..., T>&;
template<typename ...Ts> // pop_front
auto pop_front(seq<Ts...>& xs) -> decltype(tail(xs));
template<typename Ts> // at
auto at(z&, Ts& xs) -> decltype(head(xs));
template<typename N, typename Ts> 
auto at(s<N>& n, Ts& xs) -> decltype(at(pred(n), tail(xs)));
template<typename T, typename U> // fst
auto fst(seq<T, U>& xy) -> decltype(head(xy));
template<typename T, typename U> // snd
auto snd(seq<T, U>& xy) -> decltype(head(tail(xy)));
template<typename ...Ts, typename ...Us> // append
auto append(seq<Ts...>&, seq<Us...>&) -> seq<Ts..., Us...>&;
template<typename K, typename V, typename ...Ks, typename ...Vs> // lookup
auto lookup(K& k,
            seq<seq<K, V>, seq<Ks, Vs>...>& as) -> decltype(mk_just(snd(head(as))));
template<typename K> 
auto lookup(K& k,
            seq<>&) -> decltype(mk_nothing());
template<typename K1, typename K2, typename V, typename ...Ks, typename ...Vs> 
auto lookup(K1& x,
            seq<seq<K2, V>, seq<Ks, Vs>...>& as) -> decltype(lookup(x, tail(as)));
template<typename ...Ts, typename ...Us> // zip
auto zip(seq<Ts...>&, seq<Us...>&) -> seq<seq<Ts, Us>...>&;
template<typename ...Ts, typename ...Us> // unzip
auto unzip(seq<seq<Ts, Us>...>&) -> seq<seq<Ts...>, seq<Us...> >&;
template<typename ...Us> // reverse
auto reverse1(seq<>&, seq<Us...>& ys) -> decltype(ys);
template<typename ...Ts, typename ...Us>
auto reverse1(seq<Ts...>& xs, seq<Us...>& ys)
    -> decltype(reverse1(tail(xs), cons(head(xs), ys)));
template<typename ...Ts>
auto reverse(seq<Ts...>& xs) -> decltype(reverse1(xs, nil));
auto length(seq<>&) -> decltype(zero); // length
template<typename T, typename ...Ts>
auto length(seq<Ts...>& xs) -> decltype(succ(length(tail(xs))));
template<typename T, typename Rs> // remove
auto remove1(T&, seq<>&, Rs&) -> Rs&;
template<typename T, typename ...Ts, typename Rs> 
auto remove1(T&, seq<T, Ts...>& xs, Rs& rs) -> decltype(append(rs, tail(xs)));
template<typename T1, typename T2, typename ...Ts, typename Rs> 
auto remove1(T1& x, seq<T2, Ts...>& xs, Rs& rs) -> decltype(remove1(x, tail(xs), push_back(head(xs), rs)));
template<typename T, typename ...Ts>
auto remove(T& x, seq<Ts...>& xs) -> decltype(remove1(x, xs, nil));

#undef METAEXPR_ADL_BARRIER_TEMPLATE

}

#endif
