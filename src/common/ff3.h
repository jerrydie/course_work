#ifndef FF3_H_INCLUDE_GUARD
#define FF3_H_INCLUDE_GUARD

#include "common.h"
#include <array>
#include <openssl/aes.h>
#include <iostream>

typedef unsigned __int128 u128;

template<class T>
void fillRandom_fillZero(u8* array, size_t len, T& gen)
{
    fillRandom(array, len, gen);
    *(array + len - 1) &= 0xf0;
}

template<class T>
T bytes_to_uint(u8* bytes, size_t len)
{
    T result = 0;
    for(size_t j = 0; j < len; ++j)
        result |= T(bytes[j]) << 8 * (len - j - 1);
    return result;
}

void u64_to_bytes(u64 value, size_t len, u8* bytes)
{
    for(size_t j = 0; j < len; ++j) {
        bytes[len - j - 1] = value & 0xff;
        value >>= 8;
    }
}



struct FF3 {
    size_t nb_rounds;
    size_t input_size;
    size_t domain_size;
    std::mt19937 gen;
    AES_KEY key;

    // input_size is maximum number of input bytes
    FF3(size_t nb_rounds, size_t input_size, size_t domain_size, u8* keybytes, std::mt19937& rng)
        : nb_rounds(nb_rounds), input_size(input_size), domain_size(domain_size),
          gen(rng)
    {
        AES_set_encrypt_key(keybytes, 128, &key);
    }

    // Tweak must be 56 bits
    // of the form T_L || T_R
    void encrypt(u8* output, u8* input, u8* tweak)
    {
        std::copy(input, input + 2 * input_size, output);

        std::array<u8, 16> buffer {};
        for(size_t i = 0; i < nb_rounds; ++i) {
            const size_t offset = (i % 2 == 0) ? 4 : 0; // T_R in even rounds

            // Copy tweak and right branch to buffer
            std::copy(tweak + offset, tweak + offset + 4, buffer.begin());
            std::fill(buffer.begin() + 4, buffer.begin() + 16 - input_size, 0);
            buffer[3] ^= i;
            std::copy(output + input_size, output + 2 * input_size, buffer.begin() + 16 - input_size);

            // Encrypt buffer
            std::reverse(buffer.begin(), buffer.end());
            AES_encrypt(buffer.data(), buffer.data(), &key);
            std::reverse(buffer.begin(), buffer.end());

            // Add buffer to left branch modulo domain size
            // TODO: avoid u128
            const u128 aes_out = bytes_to_uint<u128>(buffer.data(), 16) % domain_size;
            u64 left = bytes_to_uint<u64>(output, input_size) % domain_size;
            left = (left + aes_out) % domain_size;

            // Copy right half to left half
            std::copy(output + input_size, output + 2 * input_size, output);
            // New right value to byte array
            u64_to_bytes(left, input_size, output + input_size);
        } 
    }
};

#endif
