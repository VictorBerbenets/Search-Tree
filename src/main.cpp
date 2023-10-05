#include <iostream>
#include <iterator>

#include "search_tree.hpp"

int main() {
    yLAB::AVL_Tree<int> avl{0, -123, 43, 53245, -542352, -532, 100235};
    auto it = std::prev(avl.end());
    for (; it != avl.cbegin(); --it) {
        std::cout << "DECREMENT IT = " << *it << std::endl;
    }
        std::cout << "DECREMENT IT = " << *it << std::endl;
    //std::advance(it, 5);
    avl.graph_dump("main");
    for (auto it = avl.cbegin(); it != avl.cend(); ++it) {
        std::cout << "INCREMENT IT = " << *it << std::endl;
    }
#if 0
    for (auto begin = avl.cbegin(); begin != avl.cend(); ++begin) {
        std::cout << "BEGIN = " << *begin << std::endl;
        std::cout << "END   = " << *avl.end() << std::endl;
//        std::cout << *begin << std::endl;
    }
#endif
}

