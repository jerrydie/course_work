#ifndef COMMON_H_INCLUDE_GUARD
#define COMMON_H_INCLUDE_GUARD

#include <cstdint>
#include <functional>
#include <numeric>
#include <random>
#include <algorithm>
#include <vector>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

std::random_device rd;
std::uniform_int_distribution<> distribution(0, 65535);

template<class T>
u64 randomU64(T& gen)
{
    return (((u64) distribution(gen)) << 48) ^
           (((u64) distribution(gen)) << 32) ^
           (((u64) distribution(gen)) << 16) ^
           distribution(gen);
}

template<class T>
u8 randomU8(T& gen)
{
    return distribution(gen) % 256;
}

template<class T>
void fillRandom(u8* array, size_t len, T& gen)
{
    std::uniform_int_distribution<> distribution(0, 255);
    std::generate(array, array + len, [&] () { return distribution(gen); });
}

int parity(u64 c)
{
    return (__builtin_parity(c) ^ __builtin_parity(c >> 32));
}


#endif
