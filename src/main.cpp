#include <iostream>
#include <iterator>
#include <string>

#include "search_tree.hpp"

int main() {
    yLAB::AVL_Tree<double> avl {1, 2, 3, 35, 54};

    for (auto val : avl) {
        std::cout << val << std::endl;
    }
    avl.graph_dump("main");
   // std::cout << "BEGIN KEY = " << *avl.cbegin() << std::endl;
   std::cout << "PREV END KEY = " << *std::prev(avl.cend()) << std::endl;
    for (auto it = std::prev(avl.cend()); it != avl.cbegin(); --it) {
        std::cout << *it << std::endl;
    }
}

