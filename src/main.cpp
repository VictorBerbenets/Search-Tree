#include <iostream>

#include "search_tree.hpp"

int main() {
    yLAB::AVL_Tree<int> avl {};
    
    avl.insert(0);
    avl.insert(10);
    avl.insert(40);

    avl.graph_dump("main"); 
}

