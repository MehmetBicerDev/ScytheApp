#include "rand_util.h"

int random_generator::generate_random_int(int min, int max)
{
    static std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr))); // Seed the generator with current time
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

std::string random_generator::generate_random_string(size_t length)
{
    static const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr))); // Seed the generator with current time
    std::uniform_int_distribution<size_t> distribution(0, characters.size() - 1);

    std::string random_string;
    for (size_t i = 0; i < length; ++i) {
        random_string += characters[distribution(generator)];
    }

    return random_string;
}
