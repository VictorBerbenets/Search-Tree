#include <iostream>
#include <iterator>
#include <vector>

#include "search_tree.hpp"

std::vector<int> get_data() {
    enum class Data : char {Query= 'q', Key = 'k', N = 'n', M = 'm'};

    yLAB::AVL_Tree<int> avl_tree;
    std::vector<int> data;

    char marker = 0;
    int key     = 0;
    for (; std::cin.good(); ) {
        std::cin >> marker;
        if (std::cin.fail()) { break;}

        if (marker == static_cast<char>(Data::Key)) {
            std::cin >> key;
            avl_tree.insert(key);
        }
#ifndef HWT_SECOND_LEVEL
        if (marker == static_cast<char>(Data::Query)) {
            int lower_bound {0};
            int upper_bound {0};

            std::cin >> lower_bound;
            std::cin >> upper_bound;
            if (!std::cin.good()) {
                break;
            }
            if (lower_bound >= upper_bound) {
                data.push_back(0);
            } else {
                data.push_back( avl_tree.distance( avl_tree.lower_bound(lower_bound), avl_tree.upper_bound(upper_bound) ) );
            }
        }
#else
        if (marker == static_cast<char>(Data::N)) {
            std::cin >> key;
            data.push_back(avl_tree.lower_than_one(key));
        } else if (marker == static_cast<char>(Data::M)) {
            std::cin >> key;
            data.push_back(*avl_tree.min_account_elem(key));
        }
#endif
    }
    return data;
}

int main() {
    auto data = get_data();

    for (auto val : data) {
        std::cout << val << ' ';
    }
    std::cout << std::endl;

}