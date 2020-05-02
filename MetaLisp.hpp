#ifndef __METALISP_H__
#define __METALISP_H__

#include <iostream>

struct number_tag {
    static const bool is_pair = false;
    static const bool is_null = false;
    static const bool is_number = true;
    static const bool is_boolean = false;
    static const bool is_symbol = false;
};

struct null_tag {
    static const bool is_pair = false;
    static const bool is_null = true;
    static const bool is_number = false;
    static const bool is_boolean = false;
    static const bool is_symbol = false;
};

struct pair_tag {
    static const bool is_pair = true;
    static const bool is_null = false;
    static const bool is_number = false;
    static const bool is_boolean = false;
    static const bool is_symbol = false;
};

struct boolean_tag {
    static const bool is_pair = false;
    static const bool is_null = false;
    static const bool is_number = false;
    static const bool is_boolean = true;
    static const bool is_symbol = false;
};

struct symbol_tag {
    static const bool is_pair = false;
    static const bool is_null = false;
    static const bool is_number = false;
    static const bool is_boolean = false;
    static const bool is_symbol = true;
};

namespace impl {
    template <int64_t x, int64_t y>
    struct gcd {
        static const int64_t value = gcd<y, x % y>::value;
    };

    template <int64_t x>
    struct gcd<x, 0> {
        static const int64_t value = x;
    };

    template <bool isNegN, bool isNegD, int64_t N, int64_t D>
    struct rat_gcd;

    template <int64_t N, int64_t D>
    struct rat_gcd<false, false, N, D> {
        static const int64_t value = gcd<N, D>::value;
    };

    template <int64_t N, int64_t D>
    struct rat_gcd<true, true, N, D> {
        static const int64_t value = -gcd<-N, -D>::value;
    };

    template <int64_t N, int64_t D>
    struct rat_gcd<true, false, N, D> {
        static const int64_t value = gcd<-N, D>::value;
    };

    template <int64_t N, int64_t D>
    struct rat_gcd<false, true, N, D> {
        static const int64_t value = -gcd<N, -D>::value;
    };

    template <bool isZero, int64_t N, int64_t D>
    struct rat_reduce {
        static const int64_t numer = 0;
        static const int64_t denom = 1;
    };

    template <int64_t N, int64_t D>
    struct rat_reduce<false, N, D> {
        static const int64_t numer = N / rat_gcd<(N < 0), (D < 0), N, D>::value;
        static const int64_t denom = D / rat_gcd<(N < 0), (D < 0), N, D>::value;
    };
} // namespace impl

template <int64_t N, int64_t D = 1>
struct number {
    using type = number<N, D>;
    using tag = number_tag;
    static const int64_t numer = impl::rat_reduce<(N == 0 || D == 0), N, D>::numer;
    static const int64_t denom = impl::rat_reduce<(N == 0 || D == 0), N, D>::denom;
};

// from https://github.com/irrequietus/typestring
namespace impl {
    template <char... C>
    struct Symbol {
        using type = Symbol<C...>;
        using tag = symbol_tag;
        static constexpr char const value[sizeof...(C) + 1] = {C..., '\0'};
        
    };

    template <int N, int M>
    constexpr char tygrab(char const (&c)[M]) noexcept {
        return c[N < M ? N : M - 1];
    }

    template <char... X>
    auto typoke(Symbol<X...>) -> Symbol<X...>;

    template <char... X, char... Y>
    auto typoke(Symbol<X...>, Symbol<'\0'>, Symbol<Y>...) -> Symbol<X...>;

    template <char A, char... X, char... Y>
    auto typoke(Symbol<X...>, Symbol<A>, Symbol<Y>...) -> decltype(typoke(Symbol<X..., A>(), Symbol<Y>()...));

    template <char... C>
    auto typeek(Symbol<C...>) -> decltype(typoke(Symbol<C>()...));
}

#define TYPESTRING16(n, x)                                                                                      \
    impl::tygrab<0x##n##0>(x), impl::tygrab<0x##n##1>(x), impl::tygrab<0x##n##2>(x), impl::tygrab<0x##n##3>(x), \
    impl::tygrab<0x##n##4>(x), impl::tygrab<0x##n##5>(x), impl::tygrab<0x##n##6>(x), impl::tygrab<0x##n##7>(x), \
    impl::tygrab<0x##n##8>(x), impl::tygrab<0x##n##9>(x), impl::tygrab<0x##n##A>(x), impl::tygrab<0x##n##B>(x), \
    impl::tygrab<0x##n##C>(x), impl::tygrab<0x##n##D>(x), impl::tygrab<0x##n##E>(x), impl::tygrab<0x##n##F>(x)

#define TYPESTRING32(n, x) TYPESTRING16(n##0, x), TYPESTRING16(n##1, x)

#define symbol(x) decltype(impl::typeek(impl::Symbol<TYPESTRING32(, x)>()))

template <bool flag>
struct boolean {
    using tag = boolean_tag;
    using type = boolean<flag>;
    static const bool value = flag;
};

struct null {
    using tag = null_tag;
    using type = null;
};

template <typename T, typename U>
struct pair {
    using tag = pair_tag;
    using type = pair<T, U>;
    using car_ = typename T::type;
    using cdr_ = typename U::type;
};

template <typename T>
using is_null = boolean<T::tag::is_null>;

template <typename T>
using is_pair = boolean<T::tag::is_pair>;

template <typename T>
using is_number = boolean<T::tag::is_number>;

template <typename T>
using is_boolean = boolean<T::tag::is_boolean>;

template <typename T>
using is_symbol = boolean<T::tag::is_symbol>;

///////////////////////////////////////////////////////////////
template <typename... T>
struct list;

template <typename T>
struct list<T> : public pair<T, null> {};

template <typename T, typename... X>
struct list<T, X...> : public pair<T, list<X...>> {};

template <int ... N>
struct number_list;

template <int N>
struct number_list<N> : public pair<number<N>, null> {};

template <int M, int... N>
struct number_list<M, N...> : public pair<number<M>, number_list<N...>> {};

template <typename x>
struct car {
    using type = typename x::type::car_;
    using tag = typename type::tag;
};

template <typename x>
struct cdr {
    using type = typename x::type::cdr_;
    using tag = typename type::tag;
};

template <typename T, typename U>
struct cons : public pair<T, U> {};

template <typename x>
using cadr = car<cdr<x>>;

template <typename x>
using cddr = cdr<cdr<x>>;

template <typename x>
using caddr = car<cdr<cdr<x>>>;

template <typename x>
using cadddr = car<cdr<cdr<cdr<x>>>>;

///////////////////////////////////////////////////////////////
template <typename Flag, typename True, typename False>
struct if_else_impl : public True {};

template <typename True, typename False>
struct if_else_impl<boolean<false>, True, False> : public False {};

template <typename Flag, typename True, typename False>
struct if_else : public if_else_impl<typename Flag::type, True, False> {};

template <typename... x>
struct cond;

template <typename Flag, typename S, typename... X>
struct cond<Flag, S, X...> : public if_else<Flag, S, cond<X...>> {};

template <typename Else>
struct cond<Else> : public Else {};

//////////////////////////////////////////////////////////////
template <typename a, typename b>
struct or_ : public boolean<(a::type::value || b::type::value)> {};

template <typename a, typename b>
struct and_ : public boolean<(a::type::value && b::type::value)> {};

template <typename a>
struct not_ : public boolean<(!a::type::value)> {};

/////////////////////////////////////////
namespace impl {
    template <int64_t n0, int64_t d0, int64_t n1, int64_t d1>
    using add = number<n0 * d1 + n1 * d0, d0 * d1>;

    template <int64_t n0, int64_t d0, int64_t n1, int64_t d1>
    using sub = number<n0 * d1 - n1 * d0, d0 * d1>;

    template <int64_t n0, int64_t d0, int64_t n1, int64_t d1>
    using is_greater = boolean<(n0 * d1 > d0 * n1)>;

    template <int64_t n0, int64_t d0, int64_t n1, int64_t d1>
    using is_less = boolean<(n0 * d1 < d0 * n1)>;
}; // namespace impl

template <typename a, typename b>
struct add : public impl::add<a::type::numer, a::type::denom, b::type::numer, b::type::denom> {};

template <typename a, typename b>
struct sub : public impl::sub<a::type::numer, a::type::denom, b::type::numer, b::type::denom> {};

template <typename a, typename b>
struct mul : public number<a::type::numer * b::type::numer, a::type::denom * b::type::denom> {};

template <typename a, typename b>
struct div_ : public number<a::type::numer * b::type::denom, a::type::denom * b::type::numer> {};

template <typename a, typename b>
struct is_greater : public impl::is_greater<a::type::numer, a::type::denom, b::type::numer, b::type::denom> {};

template <typename a, typename b>
struct is_less : public impl::is_less<a::type::numer, a::type::denom, b::type::numer, b::type::denom> {};

template <typename a>
struct abs_ : public if_else<is_greater<a, number<0>>, a, number<-a::type::numer, a::type::denom>> {};

template <typename a, typename b, typename tagA, typename tagB>
struct is_equal_impl : public boolean<false> {};

template <typename a, typename b>
struct is_equal_impl<a, b, number_tag, number_tag> : public boolean<(a::type::numer == b::type::numer && a::type::denom == b::type::denom)> {};

template <typename a, typename b>
struct is_equal_impl<a, b, symbol_tag, symbol_tag> : public boolean<(a::type::value == b::type::value)> {};

template <typename a, typename b>
struct is_equal : public is_equal_impl<a, b, typename a::tag, typename b::tag> {};

template <typename a, typename b>
struct not_equal : public not_<is_equal<a, b>> {};

/////////////////////////////////////////////////////////////////////
template <typename T>
struct display_impl {
    static std::ostream &display(std::ostream &os, bool showBracket, number_tag) {
        if (T::type::denom == 1) {
            return os << T::type::numer;
        } else {
            return os << static_cast<double>(T::type::numer) / static_cast<double>(T::type::denom);
        }
    }
    
    static std::ostream &display(std::ostream &os, bool showBracket, symbol_tag) {
        return os << T::type::value;
    }

    static std::ostream &display(std::ostream &os, bool showBracket, boolean_tag) {
        if (T::type::value) {
            os << "#t";
        } else {
            os << "#f";
        }
        return os;
    }

    static std::ostream &display(std::ostream &os, bool showBracket, null_tag) {
        os << "()";
        return os;
    }

    static std::ostream &display(std::ostream &os, bool showBracket, pair_tag) {
        if (showBracket) {
            os << "(";
        }
        display_impl<car<T>>::display(os, true);
        using cdrT = cdr<T>;
        if (is_null<cdrT>::value) {
            // nothing
        } else if (is_pair<cdrT>::value) {
            os << " ";
            display_impl<cdr<T>>::display(os, false);
        } else {
            os << " . ";
            display_impl<cdr<T>>::display(os, false);
        }
        if (showBracket) {
            os << ")";
        }
        return os;
    }

    static std::ostream &display(std::ostream &os, bool showBracket) {
        return display(os, showBracket, typename T::tag());
    }
};

template <typename T>
void display(std::ostream &os = std::cout) {
    display_impl<T>::display(os, true) << std::endl;
};

//////////////////////////////////////////////

template <typename items>
struct length : public if_else<is_null<items>, number<0>, add<number<1>, length<cdr<items>>>> {};

template <typename items, typename n>
struct list_ref : public if_else<is_equal<n, number<0>>, car<items>, list_ref<cdr<items>, sub<n, number<1>>>> {};

template <template <typename> typename op, typename seq>
struct map : public if_else<is_null<seq>, null, cons<op<car<seq>>, map<op, cdr<seq>>>> {};

template <template <typename, typename> typename op, typename seq0, typename seq1>
struct map2 {
    using type = typename if_else<or_<is_null<seq0>, is_null<seq1>>,
                                  null,
                                  cons<op<car<seq0>, car<seq1>>, map2<op, cdr<seq0>, cdr<seq1>>>>::type;
    using tag = typename type::tag;
};

template <template <typename, typename> typename op, typename initial, typename seq>
struct accumulate : public if_else<is_null<seq>, initial, op<car<seq>, accumulate<op, initial, cdr<seq>>>> {};

template <typename a, typename b>
struct append : public if_else<is_null<a>, b, cons<car<a>, append<cdr<a>, b>>> {};

template <typename items>
struct reverse {
    template <typename items2, typename result>
    struct iter : public if_else<is_null<items2>, result, iter<cdr<items2>, cons<car<items2>, result>>> {};

    using type = typename iter<items, null>::type;
    using tag = typename type::tag;
};

template <template <typename> typename op, typename seq>
struct filter {
    using type = typename cond<is_null<seq>, null, op<car<seq>>, cons<car<seq>, filter<op, cdr<seq>>>, filter<op, cdr<seq>>>::type;
    using tag = typename type::tag;
};

template <template <typename> typename op, typename seq>
struct flatmap : public accumulate<append, null, map<op, seq>> {};

#endif
