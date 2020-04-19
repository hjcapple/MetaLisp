#include "queen.hpp"

static void test_queen() {
    using answer = queen<number<5>>;
    display<queen<number<5>>>();
}

static void test_symbol() {
    using lst = list<list<symbol("Hello"), symbol("World")>, list<number<1>, number<2>>>;
    display<lst>();
}

int main(int argc, char **argv) {
    test_queen();
    test_symbol();
    return 0;
}
