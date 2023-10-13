#ifndef DISTANCE_COMPARING
#define DISTANCE_COMPARING

#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <iterator>
#include <chrono>
#include <tuple>

#include "search_tree.hpp"

namespace compare {

class comporator final {
    using size_type = std::size_t;
    
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

    template<typename KeyT>
    void distance_comparing() const {
        std::ifstream data_file {file_name};
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
                std::chrono
                data.push_back( avl_tree.distance( avl_tree.lower_bound(lower_bound), avl_tree.upper_bound(upper_bound) ) );
            }
        }

    }
    
    void dump() const {
        std::string comp_file = "compare.txt";
        std::ofstream dump_file(comp_file, std::ios::app);
        if (!dump_file.is_open()) {
            throw std::runtime_error{"error while opening compare.txt\n"};
        }
        std::string clean_file_name = get_clean_file_name(file_name);

        dump_file << "----------------------------------------------------------------\n";
        dump_file << "                         file name:      " << clean_file_name << '\n';
        dump_file << "                         cache capacity: " << capacity_ << '\n';
        dump_file << "                         data_size:      " << data_size_ << '\n';
        dump_file << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \n";
        dump_file << "                              Hits                              \n";
        dump_file << "lfu: " << get_lfu_hits() << "\nperfect: " << get_perfect_hits() << '\n';
        dump_file << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \n";
        dump_file << "                              Time                              \n";
        dump_file << "lfu: " << get_lfu_time() << "\nperfect: " << get_perfect_time() << '\n';
        dump_file << "----------------------------------------------------------------\n\n";
        if (!dump_file.good()) {
            throw std::runtime_error{"writing file error\n!"};
        }

    }

private:
    std::string file_name_;
    std::vector<std::tuple<double, double, size_type>>
}; // <--- class comporator

} // <--- namespace compare

#endif
