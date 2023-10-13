#include <iostream>
#include <stdexcept>
#include <string>

#include "compare.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        throw std::invalid_argument{"invalid number of command arguments: expected 1, got " + std::to_string(argc)};
    }

    std::string file_name = argv[1];
    compare::comporator<int> comporator {file_name};
    comporator.dump();
}

