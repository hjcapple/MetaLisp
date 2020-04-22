#ifndef __HUFFMAN_HPP__
#define __HUFFMAN_HPP__

#include "MetaLisp.hpp"

// huffman 编码
// 相应的 Scheme 代码见 https://github.com/hjcapple/reading-sicp/blob/master/chapter_2/exercise_2_69.scm

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
    using type =
        typename list<left, right, append<symbol_<left>, symbol_<right>>, add<weight<left>, weight<right>>>::type;
    using tag = typename type::tag;
};

template <typename tree>
struct left_branch : public car<tree> {};

template <typename tree>
struct right_branch : public cadr<tree> {};

template <typename bit, typename branch>
struct choose_branch {
    using type = typename cond<is_equal<bit, number<0>>,
                               left_branch<branch>,
                               is_equal<bit, number<1>>,
                               right_branch<branch>,
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
    using type =
        typename if_else<is_leaf<tree>, if_else<is_equal<sym, symbol_leaf<tree>>, null, boolean<false>>, Else>::type;
    using tag = typename type::tag;
};

template <typename message, typename tree>
struct encode {
    using type = typename if_else<is_null<message>,
                                  null,
                                  append<encode_symbol<car<message>, tree>, encode<cdr<message>, tree>>>::type;
    using tag = typename type::tag;
};

template <typename x, typename set>
struct adjoin_set {
    using type = typename cond<is_null<set>,
                               list<x>,
                               is_less<weight<x>, weight<car<set>>>,
                               cons<x, set>,
                               cons<car<set>, adjoin_set<x, cdr<set>>>>::type;
    using tag = typename type::tag;
};

template <typename pairs>
struct make_leaf_set {
    struct Else {
        using pair_ = car<pairs>;
        using type = typename adjoin_set<make_leaf<car<pair_>, cadr<pair_>>, make_leaf_set<cdr<pairs>>>::type;
    };
    using type = typename if_else<is_null<pairs>, null, Else>::type;
    using tag = typename type::tag;
};

template <typename set>
struct successive_merge {
    struct Else {
        using left = car<set>;
        using right = cadr<set>;
        using remained = cddr<set>;
        using type = typename successive_merge<adjoin_set<make_code_tree<left, right>, remained>>::type;
    };
    using type = typename if_else<is_equal<length<set>, number<1>>, car<set>, Else>::type;
    using tag = typename type::tag;
};

template <typename pairs>
struct generate_huffman_tree : public successive_merge<typename make_leaf_set<pairs>::type> {};

//////////////////////////////////////////////

static inline void test_huffman() {
    printf("huffman: \n");
    using huffman_pair = typename list<list<symbol("A"), number<4>>,
                                       list<symbol("B"), number<2>>,
                                       list<symbol("C"), number<1>>,
                                       list<symbol("D"), number<1>>>::type;

    using huffman_tree = generate_huffman_tree<huffman_pair>;
    using symbols = list<symbol("A"), symbol("D"), symbol("A"), symbol("B"), symbol("B"), symbol("C"), symbol("A")>;
    using message = encode<symbols, huffman_tree>;

    display<huffman_tree>(); // ((leaf A 4) ((leaf B 2) ((leaf D 1) (leaf C 1) (D C) 2) (B D C) 4) (A B D C) 8)
    display<symbols>();      // (A D A B B C A)
    display<message>();      // (0 1 1 0 0 1 0 1 0 1 1 1 0)
    display<decode<message, huffman_tree>>(); // (A D A B B C A)
    printf("\n");
}

#endif
