#ifndef __SQRT_HPP__
#define __SQRT_HPP__

#include "MetaLisp.hpp"

// 平方根
// 相应的 Scheme 代码见 https://github.com/hjcapple/reading-sicp/blob/master/chapter_1/newton_sqrt.scm
template <typename x, typename y>
struct average : public div_<add<x, y>, number<2>> {};

template <typename x>
struct square : public mul<x, x> {};

template <typename guess, typename x>
struct is_good_enough {
    using type = typename is_less<abs_<sub<square<guess>, x>>, number<1, 1000>>::type;
    using tag = typename type::tag;
};

template <typename guess, typename x>
struct improve : public average<guess, div_<x, guess>> {};

template <typename guess, typename x>
struct sqrt_iter {
    using type = typename if_else<is_good_enough<guess, x>, guess, sqrt_iter<improve<guess, x>, x>>::type;
    using tag = typename type::tag;
};

template <typename x>
struct sqrt : public sqrt_iter<number<1>, x> {};

static void test_sqrt() {
    printf("sqrt: \n");
    display<sqrt<number<1>>>(); // 1
    display<sqrt<number<2>>>(); // 1.41422
    display<sqrt<number<3>>>(); // 1.73214
    display<sqrt<number<4>>>(); // 2
    display<sqrt<number<5>>>(); // 2.23607
    display<sqrt<number<6>>>(); // 2.44949
    printf("\n");
}

#endif
