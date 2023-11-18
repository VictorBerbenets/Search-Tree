#pragma once

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
                std::chrono::duration<double> avl_dur = avl_end - avl_start;

                auto std_dist_start = std::chrono::high_resolution_clock::now();
                auto std_dist       = std::distance(lower_iter, upper_iter);
                auto std_dist_end   = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> std_dist_dur = std_dist_end - std_dist_start;

                if (std_dist != avl_dist) {
                    throw std::runtime_error{"error in distance method"};
                }

                data_.emplace_back(avl_dur.count(), std_dist_dur.count(), std_dist, range{lower_bound, upper_bound});
            }
        }
    }

    void dump() {
        distance_comparing();

        std::string comp_file = "../compare/compare.txt";
        std::ofstream dump_file(comp_file, std::ios::app);
        if (!dump_file.is_open()) {
            throw std::runtime_error{"error while opening compare.txt\n"};
        }
        std::string clean_file_name = get_clean_file_name(file_name_);

        dump_file << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        dump_file << "                         file name:      " << clean_file_name     << std::endl;
        for (size_type count {1}; auto val : data_) {
            dump_file << "------------------------------------------------------------" << std::endl;
            dump_file << "    range " << count << ": [ " << std::get<3>(val).first << "; "
                      << std::get<3>(val).second << "]" << std::endl;
            dump_file << "distance: " << std::get<2>(val) << std::endl;
            dump_file << "avl tree time: " << std::get<0>(val)      << std::endl;
            dump_file << "std::distance time: " << std::get<1>(val) << std::endl;
            ++count;
        }
        dump_file << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
        if (!dump_file.good()) {
            throw std::runtime_error{"writing file error\n!"};
        }
    }

private:
    std::string file_name_;
    std::vector<std::tuple<double, double, size_type, range>> data_;
}; // <--- class comporator

} // <--- namespace compare

