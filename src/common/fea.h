#ifndef FEA_H_INCLUDE_GUARD
#define FEA_H_INCLUDE_GUARD

#include "common.h"

#include "FEA_Library/FEA.h"
#include "FEA_Library/FEA_Transform.h"

struct FEAContext {//   cnhernehf  FEA sdv 
    u32 type;
    size_t key_size, block_size;

    std::mt19937 gen;

    fpe_transform_ctx transform_ctx[1];
    fpe_encrypt_ctx encrypt_ctx[1];
    std::vector<u8> key;

    fpe_tweak tweak[1]; 
    fpe_bits_data data[2] { 0,0 };

    u64 input[2] { 0,0 };
    u64 output[2] { 0,0 };

    bool odd_nb_rounds = false;

    // Initialize with random key
    FEAContext(u32 type, size_t key_size, size_t block_size, size_t nb_rounds,
               std::mt19937& rng)
        : type(type), block_size(block_size), key_size(key_size),
          gen(rng), key(key_size / 8, 0)
    {
        FEA_SetupLength(transform_ctx, FEA_DATA_BITS, block_size, nullptr);
        fillRandom(key.data(), key.size(), gen);
        FEA_Keyschedule(encrypt_ctx, transform_ctx, type, key.data(), key_size);
        encrypt_ctx->e_ctx.rounds = nb_rounds;
        odd_nb_rounds = nb_rounds % 2;
    }
};

#endif
