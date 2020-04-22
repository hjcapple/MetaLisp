#include "factorial.hpp"
#include "queen.hpp"
#include "sqrt.hpp"
#include "huffman.hpp"

// 测试符号
static void test_symbol() {
    printf("symbol: \n");
    using lst = list<list<symbol("Hello"), symbol("World")>, list<number<1>, number<2>>>;
    display<lst>();
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
