#pragma once
#ifndef FEA_TRANSFORM_NAIVE_MATH_SIMPLE_H
#define FEA_TRANSFORM_NAIVE_MATH_SIMPLE_H

#include "../FEA_Local.h"

/**
 * @file transform_naive/math_simple.h
 */

typedef struct _my_u128 {
	u64 high;
	u64 low;
} u128;

#ifdef __cplusplus
extern "C" {
#endif

uint32_t my_clz32(uint32_t value);
uint32_t my_clz64(uint64_t value);

int Cmp128(const u128 lhs, const u128 rhs);
u128 Add128E(const u128 lhs, const u128 rhs);
u128 Add128(const u128 lhs, const u64 rhs);
u128 Sub128(const u128 lhs, const u64 rhs);
u128 Mul64(const u64 lhs, const u64 rhs);
u128 Mul128(const u128 lhs, const u64 rhs, u64* overflow);
u128 Div128(const u128 lhs, const u64 rhs, u64* mod);
u64 Mod128(const u128 lhs, const u64 rhs);

u128 Make128(const u64 val);

#ifdef __cplusplus
}
#endif

#endif