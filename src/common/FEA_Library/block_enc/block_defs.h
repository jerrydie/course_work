#pragma once
#ifndef FEA_BLOCK_ENC_BLOCK_DEFS_H
#define FEA_BLOCK_ENC_BLOCK_DEFS_H

#include "../FEA_Defs.h"

/**
 * @file block_enc/block_defs.h
 * @brief round keys of FEA
 */

/// @brief round keys of FEA
typedef struct _fpe_enckey
{
	int key_bits; /// Key length (128, 192, or 256)
	int rounds; /// number of rounds
	uint64_t rk[100]; /// round keys
} fpe_enckey;

#endif