#include <iostream>

#include "omstl/list.h"

class Foo {
public:
    Foo(int l) {
        std::cout << "ctor";
    }
    Foo(Foo&& other) {
        std::cout << "move ctor";
    }
};

int main() {
    omstl::List<int> list;
    list.push_front(0);
    list.push_back(1);
    list.push_front(2);
    list.push_back(3);
    list.push_front(4);
    list.test();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
