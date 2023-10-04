#include <iostream>
#include <iterator>

#include "search_tree.hpp"

int main() {
    yLAB::AVL_Tree<int> avl{-1, 2, 45, -435, 0, 254};

    avl.graph_dump("main");
    for (auto begin = avl.begin(); begin != avl.end(); ++begin) {
        std::cout << "BEGIN = " << begin->key_ << std::endl;
        std::cout << "END   = " << avl.end()->key_ << std::endl;
        std::cout << *begin << std::endl;
    }
    std::cout << "BOOL = " << (nullptr == nullptr) << std::endl;
}

