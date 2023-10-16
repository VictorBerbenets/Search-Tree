#include <iostream>
#include <vector>
#include <chrono>

#include "search_tree.hpp"

int main() {
    static constexpr char KEY   = 'k';
    static constexpr char QUERY = 'q';

    std::vector<int> data {};

    char marker = 0;
    int key     = 0;
    for (; std::cin.good(); ) {
        std::cin >> marker;
        if (marker == KEY) {
            std::cin >> key;
            data.push_back(KEY);
            data.push_back(key);
        } else if (marker == QUERY) {
            data.push_back(QUERY);

            int lower_bound {0};
            int upper_bound {0};

            std::cin >> lower_bound;
            std::cin >> upper_bound;

            data.push_back(lower_bound);
            data.push_back(upper_bound);
        }
    }

    static constexpr std::size_t CAPACITY = 50000;
    std::vector<int> ans {};
    ans.reserve(CAPACITY);

    yLAB::AVL_Tree<int> avl_tree {};
    auto start = std::chrono::high_resolution_clock::now();
    for (auto iter = data.begin(); iter != data.end(); ++iter) {
        if (*iter == KEY) {
            ++iter;
            avl_tree.insert(*iter);
        } else if (*iter == QUERY) {
            int lower_bound = *(iter + 1);
            int upper_bound = *(iter + 2);
            iter += 2;

            ans.push_back(avl_tree.distance( avl_tree.lower_bound(lower_bound), avl_tree.upper_bound(upper_bound)));
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dur = end - start;

    std::cout << dur.count() << std::endl;
    for (auto val : ans) {
        std::cout << val << ' ';
    }
    std::cout << std::endl;

}