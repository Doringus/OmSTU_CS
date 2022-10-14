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
    list.push_back(8);
    list.push_back(4);
    list.push_back(6);
    list.push_back(1);
    list.push_back(2);
    list.push_back(8);
    list.sort();
    for(const auto& e : list) {
        std::cout << e;
    }
    return 0;
}
