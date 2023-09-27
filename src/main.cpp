#include <iostream>

#include "search_tree.hpp"

int main() {
    yLAB::AVL_Tree<int> avl {};

    for (int i = 0; i < 10000; ++i) {
        avl.insert(i);
    }
    //avl.print(avl.ptr());
}

