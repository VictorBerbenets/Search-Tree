#include <iostream>

#include "end2end.hpp"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        throw std::runtime_error {"invalid numbers of arguments"};
    }
    std::size_t tests_number   = std::stol(argv[1]);
    std::size_t keys_number    = std::stol(argv[2]);
    std::size_t queries_number = std::stol(argv[3]);

    testing::generator<int> gen(tests_number, keys_number, queries_number);
    gen.generate_tests();
}

