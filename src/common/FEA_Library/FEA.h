#pragma once
#ifndef FEA_H
#define FEA_H

#include "FEA_Defs.h"

#ifdef FEA_MODULE_BLKENC
#include "block_enc/block_defs.h"
#else
#include "block_naive/block_defs.h"
#endif

#ifdef FEA_MODULE_TRANSFORM
#include "transform_fast/transform_defs.h"
#else
#include "transform_naive/transform_defs.h"
#endif




/**
 * @mainpage Format-preserving encryption algorithm FEA
 *
 * * Radix : 2 ~ 65535
 * * Key legnths : 128/192/256 bits
 * * Length of plaintext : 8 bits ~ 128 bits
 *
 */

typedef struct _fpe_transform {
	
	fpe_objtype_t type;

	uint64_t domain_size_Q;
	uint64_t domain_size_R;

	uint32_t obj_length;
	uint32_t block_bits;

	const fpe_radix_table* radix_table;

} fpe_transform_ctx;

typedef struct _fpe_encrypt {
	fpe_algtype_t alg;
	fpe_enckey e_ctx;
	fpe_transform_ctx t_ctx;

} fpe_encrypt_ctx;

typedef struct _fpe_tweak {
	uint64_t tweak[2];
} fpe_tweak;

typedef struct _fpe_decimal_data {
	uint8_t value[40];
} fpe_decimal_data;

typedef struct _fpe_radix_data {
	uint32_t value[128];
} fpe_radix_data;

typedef struct _fpe_bits_data
{
	uint8_t value[16];
} fpe_bits_data;

typedef struct _fea_encrypt_order {
    uint32_t obj_length[2];
    uint32_t count[2];
} fea_encrypt_order;

#ifdef __cplusplus
extern "C" {
#endif

fpe_radix_table* FEA_BuildRadixTable(uint32_t radix);

fpe_err_t FEA_SetupLength(fpe_transform_ctx* ctx, const fpe_objtype_t obj_type, const uint32_t obj_length, const fpe_radix_table * table);

fpe_err_t FEA_Keyschedule(fpe_encrypt_ctx* ctx, const fpe_transform_ctx* t_ctx, const uint32_t enc_type, const uint8_t* key, const uint32_t key_bits);

fpe_err_t FEA_Encrypt(void* ct, const void* pt, const fpe_tweak* tweak, const fpe_encrypt_ctx* ctx);

fpe_err_t FEA_Decrypt(void* pt, const void* ct, const fpe_tweak* tweak, const fpe_encrypt_ctx* ctx);

fpe_err_t FEA_Calc_EncOrder(fea_encrypt_order* order, const uint32_t length, const uint32_t min_len, const uint32_t max_len);

fpe_err_t FEA_CBC_Encrypt(void* ct, const void* pt, const void* iv, size_t pt_len, const fpe_tweak* tweak, const fpe_encrypt_ctx* ctx_head, const fpe_encrypt_ctx* ctx_tail);
fpe_err_t FEA_CBC_Decrypt(void* pt, const void* ct, const void* iv, size_t pt_len, const fpe_tweak* tweak, const fpe_encrypt_ctx* ctx_head, const fpe_encrypt_ctx* ctx_tail);

fpe_err_t FEA_CBC2_Encrypt(void* ct, const void* pt, size_t pt_len, const fpe_tweak* tweak, const fpe_encrypt_ctx* ctx_head, const fpe_encrypt_ctx* ctx_tail);
fpe_err_t FEA_CBC2_Encrypt(void* pt, const void* ct, size_t pt_len, const fpe_tweak* tweak, const fpe_encrypt_ctx* ctx_head, const fpe_encrypt_ctx* ctx_tail);

fpe_err_t FEA_CTR_Encrypt(void* ct, const void* pt, const void* ctr, size_t pt_len, const fpe_tweak* tweak, const fpe_encrypt_ctx* ctx);
fpe_err_t FEA_CTR_Decrypt(void* ct, const void* pt, const void* ctr, size_t pt_len, const fpe_tweak* tweak, const fpe_encrypt_ctx* ctx);

fpe_err_t FEA_CTR2_Encrypt(void* ct, const void* pt, size_t pt_len, const fpe_tweak* tweak, const fpe_encrypt_ctx* ctx);
fpe_err_t FEA_CTR2_Decrypt(void* ct, const void* pt, size_t pt_len, const fpe_tweak* tweak, const fpe_encrypt_ctx* ctx);

void FEA_DestroyRadixTable(fpe_radix_table* table);

#ifdef __cplusplus
}
#endif

#endif