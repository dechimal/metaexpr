#include "arbital/type_level_expression/type_level_expression.hpp"
#include "boost/type_traits/is_same.hpp"
#include "boost/preprocessor/stringize.hpp"
#include "boost/mpl/print.hpp"

#define val(...) ARBITAL_TYPE_LEVEL_VALUE_OF(__VA_ARGS__)
#define TEST(expr, ...) static_assert(boost::is_same<decltype(expr), __VA_ARGS__>::value, __FILE__ ": " BOOST_PP_STRINGIZE(__LINE__))

struct foo;
struct bar;
struct baz;
template<typename T, typename U> struct f;

using boost::is_same;
using boost::mpl::print;
namespace t = arbital::typelevel;

typedef t::seq<foo, f<foo, bar>, baz> args1;
typedef t::seq<foo, foo, baz> args2;
typedef t::seq<t::p0, t::p1, t::p2> params1;
typedef t::seq<t::p0, f<t::p1, t::_>, t::p2> params2;
typedef f<f<t::p1, t::p0>, t::p2> def;

TEST(t::seq_(val(foo), val(bar)), t::seq<foo, bar>&);
TEST(t::push_front(val(foo), t::seq_(val(bar))), t::seq<foo, bar>&);
TEST(t::push_back(val(bar), t::seq_(val(foo))), t::seq<foo, bar>&);
TEST(t::pop_front(t::seq_(val(foo))), t::seq<>&);
TEST(t::at(val(t::ulong<1>), t::seq_(val(foo), val(bar), val(foo))), bar&);
TEST(t::bind(val(params1), val(args1)), t::seq<params1, args1>&);
// TEST(t::bind(val(params2), val(args1)), t::seq<params1, args2>&);
// TEST(t::replace(val(foo), t::seq_(val(params1), val(args1))), foo&);
// TEST(t::replace(val(t::p0), t::seq_(val(params1), val(args1))), foo&);
// TEST(t::replace(val(t::p1), t::seq_(val(params1), val(args1))), f<foo, bar>&);
// TEST(t::replace(val(t::p0), t::seq_(val(params2), val(args1))), foo&);
// TEST(t::replace(val(t::p1), t::bind(val(params2), val(args1))), foo&);
// TEST(t::replace(val(def), t::bind(val(params2), val(args1))), f<f<foo, foo>, baz>&);
// TEST(val(t::type<t::p0(t::p0)>)(val(foo)), foo&);

// template<typename T, typename Ts> struct list;
// auto head(list<t::p0, _>) -> t::p0;
// template<typename T, typename ...Ts> auto head(list<T, Ts...>) -> T;
// template<typename T, typename ...Ts> auto tail(list<T, Ts...>) -> list<Ts...>;

// auto id(t::p0 x) -> decltype(x);
// auto apply(t::p0 f, t::p1 x) -> decltype(f(x));

int main() {}
