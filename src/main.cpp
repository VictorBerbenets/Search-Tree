#include <iostream>
#include <iterator>
#include <string>

#include "search_tree.hpp"

int main() {
    yLAB::AVL_Tree<double> avl {5.32, 2,523, 5232, 1341, -51.532};
    //auto it21 = std::prev(avl.end());
    auto find = avl.find(5.32);
    std::cout << avl << std::endl;
    if (find != avl.end()) {
        std::cout << "FIND = " << *find << std::endl;
    }

    for (auto it = avl.end(); it != avl.cbegin(); --it) {
        std::cout << "DECREMENT IT = " << *it << std::endl;
    }
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

