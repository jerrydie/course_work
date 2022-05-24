#pragma once
#ifndef BLOCK_ENC_FEA_BLK_H
#define BLOCK_ENC_FEA_BLK_H

/**
 * @file block_enc/block_local.h
 * 
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "../FEA.h"
#include "../FEA_Local.h"

/** @brief Encryption function for FEA-A */
void FEA_A_Enc_U64ToU64(const u64 *rk, const u64 *TW, u64 *XX, int nBits, int nRounds);
/** @brief Decryption function for FEA-A */
void FEA_A_Dec_U64ToU64(const u64 *rk, const u64 *TW, u64 *XX, int nBits, int nRounds);
/** @brief Encryption function for FEA-B */
void FEA_B_Enc_U64ToU64(const u64 *rk, const u64 *TW, u64 *XX, int nBits, int nRounds);
/** @brief Decryption function for FEA-B */
void FEA_B_Dec_U64ToU64(const u64 *rk, const u64 *TW, u64 *XX, int nBits, int nRounds);

fpe_err_t _FEA_Enc_Decimal(fpe_decimal_data* ct, const fpe_decimal_data* pt, const fpe_tweak* tw, const fpe_encrypt_ctx *ctx);
fpe_err_t _FEA_Dec_Decimal(fpe_decimal_data* pt, const fpe_decimal_data* ct, const fpe_tweak* tw, const fpe_encrypt_ctx *ctx);

fpe_err_t _FEA_Enc_Bits(fpe_bits_data* ct, const fpe_bits_data* pt, const fpe_tweak* tw, const fpe_encrypt_ctx *ctx);
fpe_err_t _FEA_Dec_Bits(fpe_bits_data* pt, const fpe_bits_data* ct, const fpe_tweak* tw, const fpe_encrypt_ctx *ctx);

fpe_err_t _FEA_Enc_Radix(fpe_radix_data* ct, const fpe_radix_data* pt, const fpe_tweak* tw, const fpe_encrypt_ctx *ctx);
fpe_err_t _FEA_Dec_Radix(fpe_radix_data* pt, const fpe_radix_data* ct, const fpe_tweak* tw, const fpe_encrypt_ctx *ctx);

#ifdef __cplusplus
}
#endif

#endif