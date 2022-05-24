#pragma once
#ifndef FEA_TRANSFORM_NAIVE_TRANSFORM_DEFS_H
#define FEA_TRANSFORM_NAIVE_TRANSFORM_DEFS_H

/**
 * @file transform_naive/transform_defs.h
 */

#include "../FEA_Defs.h"


typedef struct _fpe_radix_table
{
	uint32_t Radix;
	uint32_t MinLen;
	uint32_t MaxLen64;
	uint32_t MaxLen128;
} fpe_radix_table;
#endif