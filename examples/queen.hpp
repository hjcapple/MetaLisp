#ifndef __QUEEN_HPP__
#define __QUEEN_HPP__

#include "MetaLisp.hpp"

// 求解八皇后问题
// 相应的 Scheme 代码见 https://github.com/hjcapple/reading-sicp/blob/master/chapter_2/exercise_2_42.scm

template <typename low, typename high>
struct enumerate_interval {
    using type =
        typename if_else<is_greater<low, high>, null, cons<low, enumerate_interval<add<low, number<1>>, high>>>::type;
    using tag = typename type::tag;
};

using empty_board = null;

template <typename k, typename positions>
struct is_safe {
    template <typename v0, typename v1>
    using is_same_row = is_equal<car<v0>, car<v1>>;

    template <typename v0, typename v1>
    struct is_same_diag {
        using tmp0 = typename abs_<sub<car<v0>, car<v1>>>::type;
        using tmp1 = typename abs_<sub<cdr<v0>, cdr<v1>>>::type;
        using type = typename is_equal<tmp0, tmp1>::type;
    };

    using pair_list = typename map2<cons, positions, enumerate_interval<number<1>, k>>::type;
    using val = typename car<pair_list>::type;

    template <typename x>
    struct filter_op : public or_<is_same_row<x, val>, is_same_diag<x, val>> {};

    using type = typename is_equal<length<filter<filter_op, cdr<pair_list>>>, number<0>>::type;
    using tag = typename type::tag;
};

template <typename new_row, typename k, typename rest_of_queens>
using adjoin_position = cons<new_row, rest_of_queens>;

template <typename board_size>
struct queen {
    template <typename k>
    struct queen_cols {
        template <typename positions>
        using filter_lambda = is_safe<k, positions>;

        template <typename rest_of_queens>
        struct flatmap_lambda {
            template <typename new_row>
            using map_lambda = adjoin_position<new_row, k, rest_of_queens>;
            using type = typename map<map_lambda, enumerate_interval<number<1>, board_size>>::type;
        };

        using type =
            typename if_else<is_equal<k, number<0>>,
                             list<empty_board>,
                             filter<filter_lambda, flatmap<flatmap_lambda, queen_cols<sub<k, number<1>>>>>>::type;
        using tag = typename type::tag;
    };
    using type = typename queen_cols<board_size>::type;
    using tag = typename type::tag;
};

static inline void test_queen() {
    printf("queen: \n");
    /*
        ((4 2 5 3 1)
        (3 5 2 4 1)
        (5 3 1 4 2)
        (4 1 3 5 2)
        (5 2 4 1 3)
        (1 4 2 5 3)
        (2 5 3 1 4)
        (1 3 5 2 4)
        (3 1 4 2 5)
        (2 4 1 3 5)) */
    display<queen<number<5>>>();
    printf("\n");
}

#endif
