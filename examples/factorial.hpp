#ifndef __FACTORIAL_HPP__
#define __FACTORIAL_HPP__

#include "MetaLisp.hpp"

// 计算阶乘
// 相应的 Scheme 代码见 https://github.com/hjcapple/reading-sicp/blob/master/chapter_1/factorial.scm

template <typename n>
struct factorial {
    using one = number<1>;
    using type = typename if_else<is_equal<n, one>, one, mul<n, factorial<sub<n, one>>>>::type;
    using tag = typename type::tag;
};

template <typename n>
struct factorial_2 {
    using one = number<1>;
    template <typename product, typename counter, typename max_count>
    struct fact_iter {
        using type = typename if_else<is_greater<counter, max_count>,
                                      product,
                                      fact_iter<mul<counter, product>, add<counter, one>, max_count>>::type;
        using tag = typename type::tag;
    };
    using type = typename fact_iter<one, one, n>::type;
    using tag = typename type::tag;
};

static inline void test_factorial() {
    printf("factorial: \n");
    display<factorial<number<10>>>();   // 3628800
    display<factorial_2<number<10>>>(); // 3628800
    printf("\n");
}

#endif
