#include "arbital/type_level_expression/type_level_expression.hpp"
#include "boost/type_traits/is_same.hpp"
#include "boost/preprocessor/stringize.hpp"
#include "boost/mpl/print.hpp"

#define val(...) ARBITAL_TYPE_LEVEL_VALUE_OF(__VA_ARGS__)
#define run(...) ARBITAL_TYPE_LEVEL_RUN(__VA_ARGS__)
#define TEST(expr, ...) static_assert(boost::is_same<decltype(expr), __VA_ARGS__>::value, __FILE__ ": " BOOST_PP_STRINGIZE(__LINE__))

struct foo;
struct bar;
struct baz;
template<typename T, typename U> struct f;

using boost::is_same;
using boost::mpl::print;
namespace t = arbital::typelevel;

typedef t::seq<t::p0, t::p1, t::p2, t::p3> params1;
typedef t::seq<foo, f<foo, bar>, t::type_t<foo>, baz> args1;
typedef t::seq<t::seq<t::p0, foo>, t::seq<t::p1, f<foo, bar> >, t::seq<t::p2, t::type_t<foo> >, t::seq<t::p3, baz> > res1;

typedef t::seq<t::p0, f<t::p1, t::_>, t::p2, t::p3> params2;
typedef args1 args2;
typedef t::seq<t::seq<t::p0, foo>, t::seq<t::p1, foo>, t::seq<t::p2, t::type_t<foo> >, t::seq<t::p3, baz> > res2;

typedef t::seq<t::p0, f<t::p1, t::_>, t::type_t<foo>, t::p2> params3;
typedef args1 args3;
typedef t::seq<t::seq<t::p0, foo>, t::seq<t::p1, foo>, t::seq<t::p2, baz> > res3;

typedef f<f<t::p1, t::p0>, t::p2> def;

TEST(t::seq_(val(foo), val(bar)), t::seq<foo, bar>&);
TEST(t::push_front(val(foo), t::seq_(val(bar))), t::seq<foo, bar>&);
TEST(t::push_back(val(bar), t::seq_(val(foo))), t::seq<foo, bar>&);
TEST(t::pop_front(t::seq_(val(foo))), t::seq<>&);
TEST(t::at(val(t::size_t<1>), t::seq_(val(foo), val(bar), val(foo))), bar&);
TEST(t::bind(val(params1), val(args1)), res1&);
TEST(t::bind(val(params2), val(args2)), res2&);
TEST(t::bind(val(params3), val(args3)), res3&);
TEST(t::replace1(val(foo), val(res1)), foo&);
TEST(t::replace(val(foo), t::bind(val(params1), val(args1))), foo&);
TEST(t::replace(val(t::p0), t::bind(val(params1), val(args1))), foo&);
TEST(t::replace(val(t::p1), t::bind(val(params1), val(args1))), f<foo, bar>&);
TEST(t::replace(val(t::p0), t::bind(val(params2), val(args2))), foo&);
TEST(t::replace(val(t::p1), t::bind(val(params2), val(args2))), foo&);
TEST(t::replace(val(t::p0), t::bind(val(params3), val(args3))), foo&);
TEST(t::replace(val(t::p1), t::bind(val(params3), val(args3))), foo&);
TEST(t::replace(val(t::p2), t::bind(val(params3), val(args3))), baz&);
TEST(t::replace(val(def), t::bind(val(params3), val(args3))), f<f<foo, foo>, baz>&);
TEST(val(t::fun_t<t::p0(t::p0)>)(val(foo)), foo&);

// auto g(t::p0 x)->decltype(t::is<f<f<t::_> > >(x))

// template<typename T, typename Ts> struct cons_t {};
// struct nil {};
// auto cons(t::p0, t::p1) -> cons_t<t::p0, t::p1>;

// auto add_const(t::p0) -> t::p0;

// auto map(t::p0, t::p1) -> 

// TEST(t::run(cons(t::type<int>(), t::type<nil>())), list<int, nil>)

int main() {}
