#ifndef __HUFFMAN_HPP__
#define __HUFFMAN_HPP__

#include "MetaLisp.hpp"

// huffman 编码
// 相应的 Scheme 代码见 https://github.com/hjcapple/reading-sicp/blob/master/chapter_2/exercise_2_68.scm

template <typename symbol_, typename weight>
struct make_leaf : public list<symbol("leaf"), symbol_, weight> {};

template <typename object>
struct is_leaf : public is_equal<car<object>, symbol("leaf")> {};

template <typename x>
struct symbol_leaf : public cadr<x> {};

template <typename x>
struct weight_leaf : public caddr<x> {};

template <typename tree>
struct symbol_ : public if_else<is_leaf<tree>, list<symbol_leaf<tree>>, caddr<tree>> {};

template <typename tree>
struct weight : public if_else<is_leaf<tree>, weight_leaf<tree>, cadddr<tree>> {};

template <typename left, typename right>
struct make_code_tree {
    using type = typename list<left, right, append<symbol_<left>, symbol_<right>>, add<weight<left>, weight<right>>>::type;
    using tag = typename type::tag;
};

template <typename tree>
struct left_branch : public car<tree> {};

template <typename tree>
struct right_branch : public cadr<tree> {};

template <typename bit, typename branch>
struct choose_branch {
    using type = typename cond<is_equal<bit, number<0>>, left_branch<branch>,
                               is_equal<bit, number<1>>, right_branch<branch>,
                               null>::type;
    using tag = typename type::tag;
};

template <typename bits, typename tree>
struct decode {
    template <typename bits_2, typename current_branch>
    struct decode_1 {
        struct Else {
            using next_branch = choose_branch<car<bits_2>, current_branch>;
            using type = typename if_else<is_leaf<next_branch>,
                                          cons<symbol_leaf<next_branch>, decode_1<cdr<bits_2>, tree>>,
                                          decode_1<cdr<bits_2>, next_branch>>::type;
        };
        using type = typename if_else<is_null<bits_2>, null, Else>::type;
    };
    using type = typename decode_1<bits, tree>::type;
    using tag = typename type::tag;
};

template <typename sym, typename tree>
struct encode_symbol {
    struct Else {
        struct Else2 {
            using right_result = encode_symbol<sym, right_branch<tree>>;
            using type = typename if_else<right_result, cons<number<1>, right_result>, boolean<false>>::type;
        };
        using left_right = encode_symbol<sym, left_branch<tree>>;
        using type = typename if_else<left_right, cons<number<0>, left_right>, Else2>::type;
    };
    using type = typename if_else<is_leaf<tree>, if_else<is_equal<sym, symbol_leaf<tree>>, null, boolean<false>>, Else>::type;
    using tag = typename type::tag;
};

template <typename message, typename tree>
struct encode {
    using type = typename if_else<is_null<message>, null, append<encode_symbol<car<message>, tree>, encode<cdr<message>, tree>>>::type;
    using tag = typename type::tag;
};


using sample_tree = make_code_tree<make_leaf<symbol("A"), number<4>>,
                                   make_code_tree<make_leaf<symbol("B"), number<2>>,
                                                  make_code_tree<make_leaf<symbol("D"), number<1>>,
                                                                 make_leaf<symbol("C"), number<1>>>>>;
using sample_message = number_list<0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0>;

using decode_result = decode<sample_message, sample_tree>;  // (A D A B B C A)
using encode_result = encode<decode_result, sample_tree>;   // (0 1 1 0 0 1 0 1 0 1 1 1 0)

#endif
