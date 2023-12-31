#include <iostream>
#include <iterator>
#include <vector>
#include <optional>
#include <ranges>

#include "search_tree.hpp"

std::optional<std::vector<int>> get_data() {
    enum class Data : char {Key = 'k', N = 'n', M = 'm'};

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
        if (marker == static_cast<char>(Data::N)) {
            std::cin >> key;
            data.push_back(avl_tree.lower_than_one(key));
        } else if (marker == static_cast<char>(Data::M)) {
            std::cin >> key;
            if (auto it = avl_tree.min_account_elem(key); it != avl_tree.cend()) {
                data.push_back(*it);
            } else {
                std::cerr << "Invalid m-query: " << key << "\ntree size: " << avl_tree.size() << std::endl;
                return {};
            }
        }
    }
    return {data};
}

int main() {
    auto data = get_data();
    if (!data) {
        return -1; // input error
    }

    std::ranges::copy(*data, std::ostream_iterator<int>{std::cout, " "});
    std::cout << std::endl;
}
