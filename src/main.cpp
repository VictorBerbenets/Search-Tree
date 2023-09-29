#include <iostream>

#include "search_tree.hpp"
#include <filesystem>

int main() {
    yLAB::AVL_Tree<int> avl {};
#if 0
    for (int i = 0; i < 10000; ++i) {
        avl.insert(i % 10);
    }
#endif
    //avl.print(avl.ptr());
    std::cout << std::filesystem::current_path() << std::endl;
}

