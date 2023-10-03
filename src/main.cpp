#include <iostream>

#include "search_tree.hpp"

int main() {
    yLAB::AVL_Tree<int> avl({1, 2, 3});

    avl.insert(0);
    avl.graph_dump("main");
    for (auto begin = avl.begin(); begin != avl.end(); begin++) {
        std::cout << begin->key_ << std::endl;
    }
    std::cout << "BOOL = " << (nullptr == nullptr) << std::endl;
    /*
    avl.insert(-1);
    avl.insert(10);
    avl.insert(40);
    avl.insert(60);
    avl.insert(500);
    avl.insert(700);
    avl.insert(1000);
*/
}

