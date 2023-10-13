#ifndef DISTANCE_COMPARING
#define DISTANCE_COMPARING

#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <iterator>
#include <chrono>
#include <vector>
#include <utility>
#include <tuple>

#include "search_tree.hpp"

namespace compare {

template<typename KeyT>
class comporator final {
    using size_type = std::size_t;
    using range     = std::pair<KeyT, KeyT>;
    static constexpr char KEY   = 'k';
    static constexpr char QUERY = 'q';

    std::string get_clean_file_name(const std::string& full_file_name) const {
        auto file_name = full_file_name;
        for (auto r_it = file_name.rbegin(); r_it != file_name.rend(); ++r_it) {
            if (*r_it == '/' || *r_it == '\\') {
                file_name.erase(file_name.begin(), r_it.base());
                break;
            }
        }
        return file_name;
    }

public:
    comporator(const std::string& file_name)
    : file_name_ {file_name} {}

    void distance_comparing() {
        std::ifstream data_file {file_name_};
        if (!data_file.is_open()) {
            throw std::runtime_error{"error while opening compare.txt\n"};
        }
        
        yLAB::AVL_Tree<KeyT> avl_tree{};

        char marker = 0;
        int key     = 0;
        for (; data_file.good(); ) {
            data_file >> marker;
            if (marker == KEY) {
                data_file >> key;
                avl_tree.insert(key);
            } else if (marker == QUERY) {
                int lower_bound {0};
                int upper_bound {0};

                data_file >> lower_bound;
                data_file >> upper_bound;
                if (!data_file.good()) {
                    break;
                }
                
                auto lower_iter = avl_tree.lower_bound(lower_bound);
                auto upper_iter = avl_tree.upper_bound(upper_bound);

                auto avl_start = std::chrono::high_resolution_clock::now();
                auto avl_dist  = avl_tree.distance(lower_iter, upper_iter);
                auto avl_end   = std::chrono::high_resolution_clock::now();
                auto avl_time  = (avl_end - avl_start).count();
                
                auto std_dist_start = std::chrono::high_resolution_clock::now();
                auto std_dist       = std::distance(lower_iter, upper_iter);
                auto std_dist_end   = std::chrono::high_resolution_clock::now();
                auto std_dist_time  = (avl_end - avl_start).count();

                if (std_dist != avl_dist) {
                    throw std::runtime_error{"error in distance method"};
                }

                data_.emplace_back(avl_time, std_dist_time, range{lower_bound, upper_bound});
            }
        }

    }
    
    void dump() {
        distance_comparing();

        std::string comp_file = "compare.txt";
        std::ofstream dump_file(comp_file, std::ios::app);
        if (!dump_file.is_open()) {
            throw std::runtime_error{"error while opening compare.txt\n"};
        }
        std::string clean_file_name = get_clean_file_name(file_name_);

        dump_file << "----------------------------------------------------------------\n";
        dump_file << "                         file name:      " << clean_file_name << '\n';
  
        if (!dump_file.good()) {
            throw std::runtime_error{"writing file error\n!"};
        }

    }

private:
    std::string file_name_;
    std::vector<std::tuple<double, double, range>> data_;
}; // <--- class comporator

} // <--- namespace compare

#endif
