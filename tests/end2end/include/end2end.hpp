#ifndef END2END_GENERATOR_
#define END2END_GENERATOR_

#include <iostream>
#include <string>
#include <iterator>
#include <set>
#include <random>
#include <fstream>
#include <filesystem>
#include <concepts>
#include <cmath>

namespace testing {

namespace dirs {
    const std::string resource_dir = "../tests/end2end/resources";
    const std::string tests_dir    = "../tests/end2end/resources/tests/";
    const std::string ans_dir      = "../tests/end2end/resources/answers/";
}

template<std::integral T>
class generator final {
    using size_type         = std::size_t;
    using value_type        = T;
    using generator_type    = std::mt19937;
    using distribution_type = std::uniform_int_distribution<T>;

    enum class Data : char {Key = 1, Query = 2};

    static constexpr size_type MAX_KEYS_NUMBER    = 1000000;
    static constexpr size_type MAX_QUERIES_NUMBER = 100000;
    static constexpr T MAX_KEY_VALUE = std::pow(2, sizeof(T) * 8 - 1) - 1; // max T value
    static constexpr T MIN_KEY_VALUE = -MAX_KEY_VALUE - 1;

    void create_source_directory() {
        using namespace std::filesystem;
        directory_entry resource_dir(dirs::resource_dir);

        if (!resource_dir.is_directory()) {
            create_directory(dirs::resource_dir);
            create_directory(dirs::tests_dir);
            create_directory(dirs::ans_dir);
        } else {
            const path tests_path{dirs::tests_dir};
            const path answs_path{dirs::ans_dir};

            for (auto& dir_iter : directory_iterator{tests_path}) {
                remove(dir_iter.path());
            }
            for (auto& dir_iter : directory_iterator{answs_path}) {
                remove(dir_iter.path());
            }
        }
    }

    value_type random_value(value_type min_val = MIN_KEY_VALUE, value_type max_val = MAX_KEY_VALUE) {
        distribution_type distr(min_val, max_val);
        return distr(generator_);
    }

    Data key_or_query() {
        switch( static_cast<Data>(random_value(static_cast<T>(Data::Key), static_cast<T>(Data::Query))) ) {
            case Data::Query: return Data::Query;
            default: return Data::Key;
        }
    }

    void generate_range_of_keys_and_queries(size_type test_number) {
        std::string test_name = "test" + std::to_string(test_number) + ".txt";
        std::string ans_name  = "ans" + std::to_string(test_number) + ".txt";
        std::ofstream test_file {dirs::tests_dir + test_name};
        std::ofstream ans_file  {dirs::ans_dir + ans_name};

        std::set<T> set {};
        for (size_type keys = 0, queries = 0; (keys + queries) != (keys_number_ + queries_number_); ) {
            auto data_type = key_or_query();
            if (data_type == Data::Key && keys != keys_number_) {
                test_file << 'k' << ' ';
                auto key = random_value();
                test_file << key << ' ';
                set.insert(key);
                ++keys;
            }
            if (data_type == Data::Query && queries != queries_number_) {
                test_file << 'q' << ' ';
                auto lower = random_value();
                if (lower == MAX_KEY_VALUE) {
                    --lower;
                }
                auto upper = random_value();
                test_file << lower << ' ';
                test_file << upper << ' ';

                auto lower_it = set.lower_bound(lower);
                auto upper_it = set.upper_bound(upper);

                if (lower >= upper) {
                    ans_file << 0 << ' ';
                } else {
                    ans_file << std::distance(lower_it, upper_it) << ' ';
                }

                ++queries;
            }
        }
        ans_file << std::endl;
    }

public:
    generator(size_type tests_number, size_type keys_number, size_type queries_number)
    :   tests_number_ {tests_number},
        keys_number_ {keys_number},
        queries_number_ {queries_number} {
            if (keys_number_ > MAX_KEYS_NUMBER) { keys_number_ = MAX_KEYS_NUMBER; }
            if (queries_number > MAX_QUERIES_NUMBER) { queries_number = MAX_QUERIES_NUMBER; }

            generator_.seed( static_cast<size_type>(std::time(nullptr)) );
    }

    void generate_tests() {
        create_source_directory();

        for (size_type counter = 1; counter <= tests_number_; ++counter) {
            generate_range_of_keys_and_queries(counter);
        }
    }

private:
    size_type tests_number_;
    size_type keys_number_;
    size_type queries_number_;
    generator_type generator_;
};

} // <--- namespace testing

#endif
