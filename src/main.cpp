#include <iostream>

#include "search_tree.hpp"

int main() {
    yLAB::AVL_Tree<int> avl {};
    
    avl.insert(0);
    avl.insert(-1);
    avl.insert(10);
    avl.insert(40);
    avl.insert(60);
    avl.insert(500);
    avl.insert(700);
    avl.insert(1000);

    avl.graph_dump("main"); 
}

