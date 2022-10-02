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
    for(const auto& e : list) {
        std::cout << e;
    }
    std::cout << "\n";
    auto it = list.rbegin();
    std::cout << *it;
    return 0;
}
