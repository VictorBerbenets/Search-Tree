#include <iostream>
#include <iterator>

#include "search_tree.hpp"

int main() {
    yLAB::AVL_Tree<int> avl{-1, 2, 45, -435, 0, 254};

    avl.graph_dump("main");
    for (auto begin = avl.cbegin(); begin != avl.cend(); ++begin) {
        std::cout << "BEGIN = " << *begin << std::endl;
        std::cout << "END   = " << *avl.end() << std::endl;
//        std::cout << *begin << std::endl;
    }
    std::cout << "BOOL = " << (nullptr == nullptr) << std::endl;
}

