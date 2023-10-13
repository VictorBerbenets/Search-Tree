#include <iostream>
#include <stdexcept>
#include <string>

#include "compare.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        throw std::invalid_argument{"invalid number of command arguments: expected 1, got " + std::to_string(argc)};
    }

    compare::comporator<int> comporator {argv[1]};
    comporator.dump();
}

