## MetaLisp

使用 C++ 模板元编程来模拟 Scheme 的 car、cdr、cons、lisp  等语法。需要 C++ 17 支持。

我是在 Mac 系统，使用 clang 编译器测试。它默认的模板嵌套是 1024 层。也可以用编译选项

```
-ftemplate-depth=N
```

修改嵌套层数。

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



### 元编程例子

* [求解八皇后问题](./examples/queen.hpp)
* [计算阶乘](./examples/factorial.hpp)
* [平方根](./examples/sqrt.hpp)
* [huffman 编码](./examples/huffman.hpp)


