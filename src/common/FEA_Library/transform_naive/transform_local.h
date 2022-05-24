#pragma once
#ifndef FEA_TRANSFORM_NAIVE_TRANSFORM_LOCAL_H
#define FEA_TRANSFORM_NAIVE_TRANSFORM_LOCAL_H

#include "../FEA.h"
#include "../FEA_Local.h"

/**
* @file transform_naive/transform_local.h
*
*/

#ifdef __cplusplus
extern "C" {
#endif

static INLINE void SplitBits(int* lhs, int* rhs, const int bits) {
	int half = bits / 2;
	*rhs = half;
	*lhs = bits - half;
}

#ifdef __cplusplus
}
#endif

#endif