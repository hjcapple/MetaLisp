#ifndef __METALISP_H__
#define __METALISP_H__

#include <iostream>

struct number_tag {
    static const bool is_pair = false;
    static const bool is_null = false;
    static const bool is_number = true;
    static const bool is_boolean = false;
};

struct null_tag {
    static const bool is_pair = false;
    static const bool is_null = true;
    static const bool is_number = true;
    static const bool is_boolean = false;
};

struct pair_tag {
    static const bool is_pair = true;
    static const bool is_null = false;
    static const bool is_number = false;
    static const bool is_boolean = false;
};

struct boolean_tag {
    static const bool is_pair = false;
    static const bool is_null = false;
    static const bool is_number = true;
    static const bool is_boolean = true;
};

template <int N>
struct number {
    using type = number<N>;
    using tag = number_tag;
    static const int value = N;
};

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

///////////////////////////////////////////////////////////////
template <typename... x>
struct list;

template <typename T>
struct list<T> : public pair<T, null> {};

template <typename T, typename... X>
struct list<T, X...> : public pair<T, list<X...>> {};

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

///////////////////////////////////////////////////////////////
template <bool flag, typename True, typename False>
struct if_else_impl : public True {};

template <typename True, typename False>
struct if_else_impl<false, True, False> : public False {};

template <typename Flag, typename True, typename False>
struct if_else : public if_else_impl<Flag::type::value, True, False> {};

template <typename... x>
struct cond;

template <typename Flag, typename S, typename... X>
struct cond<Flag, S, X...> : public if_else<Flag, S, cond<X...>> {};

template <typename Else>
struct cond<Else> : public Else {};

//////////////////////////////////////////////////////////////
template <typename a, typename b>
struct add : public number<a::type::value + b::type::value> {};

template <typename a, typename b>
struct sub : public number<a::type::value - b::type::value> {};

template <typename a, typename b>
struct not_equal : public boolean<a::type::value != b::type::value> {};

template <typename a, typename b>
struct is_greater : public boolean<(a::type::value > b::type::value)> {};

template <typename a, typename b>
struct is_equal : public boolean<(a::type::value == b::type::value)> {};

template <typename a, typename b>
struct or_ : public boolean<(a::type::value || b::type::value)> {};

template <typename a, typename b>
struct and_ : public boolean<(a::type::value && b::type::value)> {};

template <typename a>
struct abs_ : public if_else<is_greater<a, number<0>>, a, number<-a::type::value>> {};

//////////////////////////////////////////////////////////////

template <typename T>
struct display_impl {
    static std::ostream &display(std::ostream &os, bool showBracket, number_tag) {
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

template <typename seq>
struct length : public if_else<is_null<seq>, number<0>, add<number<1>, length<cdr<seq>>>> {};

template <template <typename> typename op, typename seq>
struct map : public if_else<is_null<seq>, null, cons<op<car<seq>>, map<op, cdr<seq>>>> {};

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
    using type =
        typename cond<is_null<seq>, null, op<car<seq>>, cons<car<seq>, filter<op, cdr<seq>>>, filter<op, cdr<seq>>>::
            type;
    using tag = typename type::tag;
};

template <template <typename> typename op, typename seq>
struct flatmap : public accumulate<append, null, map<op, seq>> {};

#endif
