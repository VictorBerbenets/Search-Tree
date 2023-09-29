#include <iostream>

#include "search_tree.hpp"

int main() {
    yLAB::AVL_Tree<int> avl {};
    for (int i = 0; i < 5; ++i) {
        avl.insert(i % 10);
    }
    avl.graph_dump(); 
}

