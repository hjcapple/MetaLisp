#include "factorial.hpp"
#include "queen.hpp"
#include "sqrt.hpp"
#include "huffman.hpp"

// 八皇后问题
static void test_queen() {
    printf("queen: \n");
    using answer = queen<number<5>>;
    display<queen<number<5>>>();
    printf("\n");
}

// 测试符号
static void test_symbol() {
    printf("symbol: \n");
    using lst = list<list<symbol("Hello"), symbol("World")>, list<number<1>, number<2>>>;
    display<lst>();
    printf("\n");
}

// 阶乘
static void test_factorial() {
    printf("factorial: \n");
    display<factorial<number<10>>>();
    display<factorial_2<number<10>>>();
    printf("\n");
}

static void test_sqrt() {
    printf("sqrt: \n");
    display<sqrt<number<1>>>();
    display<sqrt<number<2>>>();
    display<sqrt<number<3>>>();
    display<sqrt<number<4>>>();
    display<sqrt<number<5>>>();
    display<sqrt<number<6>>>();
    printf("\n");
}

static void test_huffman() {
    printf("huffman: \n");
    display<sample_tree>();
    display<sample_message>();
    display<decode_result>();
    display<encode_result>();
    printf("\n");
}

int main(int argc, char **argv) {
    test_queen();
    test_symbol();
    test_factorial();
    test_sqrt();
    test_huffman();
    return 0;
}
