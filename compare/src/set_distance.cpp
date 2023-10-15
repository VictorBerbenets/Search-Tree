#include <iostream>
#include <iterator>
#include <vector>
#include <chrono>
#include <set>

#define Green "\033[1;32m"
#define Usual "\033[0m"

int main() {
    static constexpr char KEY   = 'k';
    static constexpr char QUERY = 'q';

    std::set<int> set {};
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

    auto start = std::chrono::high_resolution_clock::now();
    for (auto iter = data.begin(); iter != data.end(); ++iter) {
        if (*iter == KEY) {
            ++iter;
            set.insert(*iter);
        } else if (*iter == QUERY) {
            int lower_bound = *(iter + 1);
            int upper_bound = *(iter + 2);
            iter += 2;

            ans.push_back(std::distance( set.lower_bound(lower_bound), set.upper_bound(upper_bound)));
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dur = end - start;

    for (auto val : ans) {
        std::cout << val << ' ';
    }
    std::cout << std::endl;

    std::cout << Green "Time: " Usual << dur.count() << std::endl;
}