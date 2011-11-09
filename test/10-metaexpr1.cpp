
#include "metaexpr/metaexpr1.hpp"
#include "boost/type_traits/is_same.hpp"
#include "boost/preprocessor/stringize.hpp"
#include "boost/mpl/print.hpp"

#define TEST(x, ...) static_assert(boost::is_same<decltype(x), __VA_ARGS__>::value, __FILE__ ": " BOOST_PP_STRINGIZE(__LINE__))
#define EXPR_TEST(x, y) TEST(x, decltype(y))
using boost::mpl::print;

namespace m = metaexpr;

// bool
TEST(m::and_(m::false_, m::false_), m::false_t&);
TEST(m::and_(m::true_, m::false_), m::false_t&);
TEST(m::and_(m::false_, m::true_), m::false_t&);
TEST(m::and_(m::true_, m::true_), m::true_t&);
TEST(m::or_(m::false_, m::false_), m::false_t&);
TEST(m::or_(m::true_, m::false_), m::true_t&);
TEST(m::or_(m::false_, m::true_), m::true_t&);
TEST(m::or_(m::true_, m::true_), m::true_t&);

// nat
TEST(m::succ(m::zero), m::s<m::z>&);
TEST(m::pred(m::succ(m::succ(m::zero))), m::s<m::z>&);

template<typename> struct ns_test;
#define METAEXPR_ADL_BARRIER_TEMPLATE ns_test

// seq
METAEXPR_TYPE(foo_t, (foo));
METAEXPR_TYPE(bar_t, (bar));
METAEXPR_TYPE(baz_t, (baz));

#undef METAEXPR_ADL_BARRIER_TEMPLATE

extern m::seq<foo, bar, baz>& seq1;
extern m::seq<bar, baz, foo>& seq2;
extern m::seq<m::seq<foo, bar>, m::seq<baz, foo>, m::seq<bar, baz> > assoc1;

typedef m::seq<m::seq<foo, bar>, m::seq<bar, baz>, m::seq<baz, foo> > zip1;

TEST(m::cons(mk_foo(), m::nil), m::seq<foo>&);
EXPR_TEST(m::mk_seq(mk_foo(), mk_bar(), mk_baz()), seq1);
TEST(m::at(m::succ(m::succ(m::zero)), seq1), baz&);
TEST(m::append(seq1, seq1), m::seq<foo, bar, baz, foo, bar, baz>&);
TEST(m::from_just(m::lookup(mk_baz(), assoc1)), foo&);
TEST(m::zip(seq1, seq2), zip1&);
EXPR_TEST(m::unzip(m::zip(seq1, seq2)), m::mk_seq(seq1, seq2));
TEST(m::reverse(seq1), m::seq<baz, bar, foo>&);
EXPR_TEST(m::reverse(m::reverse(seq1)), seq1);
TEST(m::remove(mk_bar(), seq1), m::seq<foo, baz>&);

int main() {
}
