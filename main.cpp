#include "factorial.hpp"
#include "queen.hpp"

// 八皇后问题
static void test_queen() {
    using answer = queen<number<5>>;
    display<queen<number<5>>>();
}

// 测试符号
static void test_symbol() {
    using lst = list<list<symbol("Hello"), symbol("World")>, list<number<1>, number<2>>>;
    display<lst>();
}

// 测试阶乘
static void test_factorial() {
    display<factorial<number<10>>>();
    display<factorial_2<number<10>>>();
}

int main(int argc, char **argv) {
    test_queen();
    test_symbol();
    test_factorial();
    return 0;
}
