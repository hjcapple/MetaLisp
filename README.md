## MetaQueen

使用 C++ 模板元编程解决八皇后问题。

思路是使用 C++ 模板来模拟 Scheme 的 car、cdr、cons 等语法。之后将 Scheme 的代码用 C++ 模板语法来改写。需要 C++ 17 支持。

### 模拟 Scheme 函数

[MetaLisp.hpp](./MetaLisp.hpp) 用于模拟 Scheme 的一些常用函数。比如下面 Scheme 函数

```Scheme
(define (append a b)
  (if (null? a)
      b
      (cons (car a) (append (cdr a) b))))

(define (reverse items)
  (define (iter items result)
    (if (null? items)
        result
        (iter (cdr items) (cons (car items) result))))
  (iter items null))
```

C++ 相应写成

```C++
template <typename a, typename b>
struct append : public if_else<is_null<a>, b, cons<car<a>, append<cdr<a>, b>>> {};

template <typename items>
struct reverse {
    template <typename items2, typename result>
    struct iter : public if_else<is_null<items2>, result, iter<cdr<items2>, cons<car<items2>, result>>> {};

    using type = typename iter<items, null>::type;
    using tag = typename type::tag;
};
```

C++ 每个模板类，相当于 Scheme 的一个函数。结果存放在模板类的 type 中。



### 八皇后的解法

八皇后的 Scheme 解法可以参考 [SICP 的练习 2.42](https://github.com/hjcapple/reading-sicp/blob/master/chapter_2/exercise_2_42.scm)

[queen.cpp](./queen.cpp) 使用 C++ 模板语法改写了这些代码。

我在 Xcode 中尝试，当 `board_size = 6`时，就会编译错误，告诉模板嵌套太多

```
Recursive template instantiation exceeded maximum depth of 1024
```

假如使用 `-ftemplate-depth=N`编译选项，来增加嵌套层次，还是会编译出错

```
clang: error: unable to execute command: Illegal instruction: 4
```

这里的模板编程，也至多只能解决五皇后问题。说是解决八皇后问题，有点名不符实。

