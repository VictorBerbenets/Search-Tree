#include <iostream>
#include <iterator>

#include "search_tree.hpp"

int main() {
    yLAB::AVL_Tree<int> avl{-1, 2, 45, -435, 0, 254};
    auto it = avl.begin();

    std::advance(it, 5);
    avl.graph_dump("main");
    for ( ; it != avl.begin(); --it) {
        std::cout << "ITER = " << *it << std::endl;
    }
#if 0
    for (auto begin = avl.cbegin(); begin != avl.cend(); ++begin) {
        std::cout << "BEGIN = " << *begin << std::endl;
        std::cout << "END   = " << *avl.end() << std::endl;
//        std::cout << *begin << std::endl;
    }
#endif
}

