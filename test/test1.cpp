#include "metaexpr/metaexpr.hpp"
#include "boost/type_traits/is_same.hpp"
#include "boost/preprocessor/stringize.hpp"
#include "boost/mpl/print.hpp"

#define val(...) METAEXPR_VALUE_OF(__VA_ARGS__)
#define fun(...) METAEXPR_FUN(__VA_ARGS__)
#define run(...) METAEXPR_RUN(__VA_ARGS__)
#define TEST(expr, ...) static_assert(boost::is_same<decltype(expr), __VA_ARGS__>::value, __FILE__ ": " BOOST_PP_STRINGIZE(__LINE__))

struct foo;
struct bar;
struct baz;
template<typename T, typename U> struct f;

using boost::is_same;
using boost::mpl::print;
namespace m = metaexpr;

template<typename Head, typename Tail>
struct list;
struct nil;

template<typename T> struct left_t;
template<typename T> struct right_t;

int main() {

    // seqの操作
    {
        TEST(m::seq_(val(foo), val(bar)), m::seq<foo, bar>&);
        TEST(m::push_front(val(foo), m::seq_(val(bar))), m::seq<foo, bar>&);
        TEST(m::push_back(val(bar), m::seq_(val(foo))), m::seq<foo, bar>&);
        TEST(m::pop_front(m::seq_(val(foo))), m::seq<>&);
        TEST(m::at(val(m::size_t<1>), m::seq_(val(foo), val(bar), val(foo))), bar&);
        TEST(m::append(m::seq_(val(foo), val(bar)), m::seq_(val(baz), val(baz))), m::seq<foo, bar, baz, baz>&);
    }

    // bind, replace, 関数適用の呼び出し
    {
        typedef m::seq<m::p0, m::p1, m::p2, m::p3> params1;
        typedef m::seq<foo, f<foo, bar>, m::type_t<foo>, baz> args1;
        typedef m::seq<m::seq<m::p0, foo>, m::seq<m::p1, f<foo, bar> >, m::seq<m::p2, m::type_t<foo> >, m::seq<m::p3, baz> > res1;

        typedef m::seq<m::p0, f<m::p1, m::_>, m::p2, m::p3> params2;
        typedef args1 args2;
        typedef m::seq<m::seq<m::p0, foo>, m::seq<m::p1, foo>, m::seq<m::p2, m::type_t<foo> >, m::seq<m::p3, baz> > res2;

        typedef m::seq<m::p0, f<m::p1, m::_>, m::type_t<foo>, m::p2> params3;
        typedef args1 args3;
        typedef m::seq<m::seq<m::p0, foo>, m::seq<m::p1, foo>, m::seq<m::p2, baz> > res3;

        typedef f<f<m::p1, m::p0>, m::p2> def;

        TEST(m::bind(val(params1), val(args1)), res1&);
        TEST(m::bind(val(params2), val(args2)), res2&);
        TEST(m::bind(val(params3), val(args3)), res3&);
        TEST(m::replace1(val(foo), val(res1)), foo&);
        TEST(m::replace(val(foo), m::bind(val(params1), val(args1))), foo&);
        TEST(m::replace(val(m::p0), m::bind(val(params1), val(args1))), foo&);
        TEST(m::replace(val(m::p1), m::bind(val(params1), val(args1))), f<foo, bar>&);
        TEST(m::replace(val(m::p0), m::bind(val(params2), val(args2))), foo&);
        TEST(m::replace(val(m::p1), m::bind(val(params2), val(args2))), foo&);
        TEST(m::replace(val(m::p0), m::bind(val(params3), val(args3))), foo&);
        TEST(m::replace(val(m::p1), m::bind(val(params3), val(args3))), foo&);
        TEST(m::replace(val(m::p2), m::bind(val(params3), val(args3))), baz&);
        TEST(m::replace(val(def), m::bind(val(params3), val(args3))), f<f<foo, foo>, baz>&);
        TEST(val(m::fun_t<m::p0(m::p0)>)(val(foo)), foo&);
    }

    // is構文
    {
        typedef f<m::_, m::type_t<bar> > pat1;
        typedef f<m::type_t<foo>, m::type_t<bar> > val1;
        typedef m::is_t<pat1, val1> isres1;

        typedef pat1 pat2;
        typedef f<m::type_t<foo>, m::p0> val2;
        typedef m::seq<m::seq<m::p0, m::type_t<bar> > > bind2;

        typedef pat2 pat3;
        typedef val2 val3;
        typedef m::is_t<pat3, val3> isres3;
        typedef m::seq<m::seq<m::p0, m::type_t<foo> > > bind3;

        TEST(m::is<pat1>(val(val1)), isres1&);
        TEST(replace(val(val1), m::seq_()), val1&);
        TEST(m::bind(val(m::seq<pat1>), val(m::seq<val1>)), m::seq<>&);
        TEST(m::bind(val(m::seq<pat1>), m::deref(val(m::seq<decltype(m::replace(val(val1), m::seq_()))>))), m::seq<>&);
        TEST(m::is2(m::bind(val(m::seq<pat1>), m::deref(val(m::seq<decltype(m::replace(val(val1), m::seq_()))>)))), m::true_t&);
        TEST(m::is1(val(isres1), m::seq_()), m::true_t&);
        TEST(m::replace(m::is<pat1>(val(val1)), m::seq_()), m::true_t&);
        TEST(m::replace(m::is<pat2>(val(val2)), val(bind2)), m::true_t&);
        TEST(m::bind1(m::seq_(), val(m::seq<pat3>), val(m::seq<f<m::type_t<foo>, m::type_t<foo> > >)), m::false_t&);
        TEST(m::bind1(m::seq_(), val(m::seq<pat3>), m::deref(val(m::seq<decltype(m::replace(val(val3), val(bind3)))>))), m::false_t&);
        TEST(m::bind(val(m::seq<pat3>), m::deref(val(m::seq<decltype(m::replace(val(val3), val(bind3)))>))), m::false_t&);
        TEST(m::is2(m::bind(val(m::seq<pat3>), m::deref(val(m::seq<decltype(m::replace(val(val3), val(bind3)))>)))), m::false_t&);
        TEST(m::is1(val(isres3), val(bind3)), m::false_t&);
        TEST(m::is1(m::is<pat3>(val(val3)), val(bind3)), m::false_t&);
        TEST(m::replace(m::is<pat3>(val(val3)), val(bind3)), m::false_t&);
    }

    // match 〜 when構文
    {
        typedef f<m::type_t<foo>, m::type_t<bar> > val1;
        typedef f<m::_, m::p0> pat1;
        typedef f<m::p0, m::type_t<bar> > def1;
        typedef f<m::type_t<bar>, m::_> pat2;
        typedef f<m::type_t<foo>, m::type_t<foo> > def2;
        typedef m::match_t<val1, m::seq<m::case_t<pat1, def1> > > res1;
        TEST(m::match(val(val1)), m::match_t<val1, m::seq<> >&);
        TEST(m::match(val(val1)) | m::when<pat1>(val(def1)), res1&);
        TEST(m::match3(m::bind(val(m::seq<pat1>), m::deref(val(m::seq<decltype(m::replace(val(val1), val(m::seq<>)))>))),
                       val(val1),
                       val(def1),
                       val(m::seq<>),
                       val(m::seq<>)),
             f<m::type_t<bar>, m::type_t<bar> >&);
        TEST(m::match2(val(val1),
                       val(pat1),
                       val(def1),
                       val(m::seq<>),
                       val(m::seq<>)),
             f<m::type_t<bar>, m::type_t<bar> >&);
        TEST(m::match1(m::match(val(val1)) | m::when<pat1>(val(def1)),
                       val(m::seq<>)),
             f<m::type_t<bar>, m::type_t<bar> >&);
        TEST(m::replace(m::match(val(val1)) | m::when<pat1>(val(def1)),
                        val(m::seq<>)),
             f<m::type_t<bar>, m::type_t<bar> >&);
        TEST(m::replace(m::match(val(val1))
                            | m::when<pat2>(val(def2))
                            | m::when<pat1>(val(def1)),
                        val(m::seq<>)),
             f<m::type_t<bar>, m::type_t<bar> >&);

    }

    // それぽいコード
    // TODO 高階関数、再帰関数
    {
        auto cons = fun((m::p0, m::p1) -> list<m::p0, m::p1>&);
        auto head = fun((list<m::p0, m::_>) -> m::p0);
        auto tail = fun((list<m::_, m::p0>) -> m::p0);
        TEST(cons(m::type<int>(), m::type<nil>()), list<m::type_t<int>, m::type_t<nil> >&);
        // auto map(m::p0 f, m::p1 xs) -> decltype(match(xs) | when<type_t<nil> >(val(type_t<nil>))
        //                                                   | when<list<m::p2, m::p3> >(cons(f(_2)), rec(f, val(m::p1)))));
    }
}
