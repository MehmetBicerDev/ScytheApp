#pragma once
#include <random>
#include <string>
#include <ctime>

namespace random_generator {

    int generate_random_int(int min, int max);
    std::string generate_random_string(size_t length);
}