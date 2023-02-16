#include <random>
#include <chrono>
#include <unistd.h>
#include <ctime>

int generate()
{
    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 r(seed1);
    return r();
}

std::string random_string() {
    auto randchar = []() -> char
    {
        const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(12,0);
    std::generate_n( str.begin(), 12, randchar);
    return str;
}