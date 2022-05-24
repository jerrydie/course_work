#include <string.h>
#include "../FEA_Build_Config.h"

#ifndef FEA_MODULE_TRANSFORM
#include "math_simple.h"

uint32_t my_clz32(uint32_t value) {
	uint32_t result = 0;
	while (value != 0) {
		result++;
		value >>= 1;
	}
	return 32 - result;
}

uint32_t my_clz64(uint64_t value) {
	uint32_t result = 0;
	while (value != 0) {
		result++;
		value >>= 1;
	}

	return 64 - result;
}

int Cmp128(const u128 lhs, const u128 rhs) {
	if (lhs.high < rhs.high) {
		return -1;
	}
	else if (lhs.high > rhs.high) {
		return 1;
	}
	else if (lhs.low < rhs.low) {
		return -1;
	}
	else if (lhs.low > rhs.low){
		return 1;
	}
	return 0;
}

u128 Add128E(const u128 lhs, const u128 rhs) {
	u128 result;
	result.high = lhs.high + rhs.high;
	result.low = lhs.low + rhs.low;

	if (result.low < lhs.low) {
		result.high++;
	}
	return result;
}

u128 Add128(const u128 lhs, const u64 rhs) {
	u128 result;
	if (rhs == 0) {
		return lhs;
	}
	result.low = lhs.low + rhs;
	result.high = lhs.high;
	if (result.low < rhs) {
		result.high++;
	}
	return result;
}

u128 Sub128(const u128 lhs, const u64 rhs) {
	u128 result;
	if (rhs == 0) {
		return lhs;
	}
	result.low = lhs.low - rhs;
	result.high = lhs.high;
	if (result.low > rhs) {
		result.high--;
	}
	return result;
}
u128 Mul64(const u64 lhs, const u64 rhs) {
	u128 result;
	u64 lhs32[2] = { lhs >> 32, lhs & UINT32_MAX };
	u64 rhs32[2] = { rhs >> 32, rhs & UINT32_MAX };
	u64 res_tmp[3] = { 0, 0, 0};

	res_tmp[2] = lhs32[1] * rhs32[1];
	res_tmp[1] = lhs32[0] * rhs32[1];
	res_tmp[1] += lhs32[1] * rhs32[0];
	res_tmp[0] = lhs32[0] * rhs32[0];

	res_tmp[1] += res_tmp[2] >> 32;
	res_tmp[2] &= UINT32_MAX;

	res_tmp[0] += res_tmp[1] >> 32;
	res_tmp[1] &= UINT32_MAX;

	result.low = (res_tmp[1] << 32) | res_tmp[2];
	result.high = res_tmp[0];

	return result;
}

u128 Mul128(const u128 lhs, const u64 rhs, u64* overflow) {
	u128 result;
	u64 lhs32[4] = { lhs.high >> 32, lhs.high & UINT32_MAX, lhs.low >> 32, lhs.low & UINT32_MAX };
	u64 rhs32[2] = { rhs >> 32, rhs & UINT32_MAX };
	u64 res_tmp[5] = { 0, 0, 0, 0 };

	if (rhs == 0) {
		return Make128(0);
	}
	else if (rhs == 1) {
		return lhs;
	}
	else if (lhs.high == 0) {
		if (overflow != NULL) {
			overflow = 0;
		}
		return Mul64(lhs.low, rhs);
	}

	res_tmp[4] = lhs32[3] * rhs32[1];
	res_tmp[3] = lhs32[2] * rhs32[1];
	res_tmp[2] = lhs32[1] * rhs32[1];
	res_tmp[1] = lhs32[0] * rhs32[1];
	res_tmp[3] += lhs32[3] * rhs32[0];
	res_tmp[2] += lhs32[2] * rhs32[0];
	res_tmp[1] += lhs32[1] * rhs32[0];
	res_tmp[0] += lhs32[0] * rhs32[0];

	res_tmp[3] += res_tmp[4] >> 32;
	res_tmp[4] &= UINT32_MAX;

	res_tmp[2] += res_tmp[3] >> 32;
	res_tmp[3] &= UINT32_MAX;

	res_tmp[1] += res_tmp[2] >> 32;
	res_tmp[2] &= UINT32_MAX;

	res_tmp[0] += res_tmp[1] >> 32;
	res_tmp[1] &= UINT32_MAX;

	result.low = (res_tmp[3] << 32) | res_tmp[4];
	result.high = (res_tmp[1] << 32) | res_tmp[2];

	if (overflow != NULL) {
		*overflow = res_tmp[0];
	}

	return result;
}

u128 Div128(const u128 lhs, const u64 rhs, u64* mod) {
	u128 result;
	u64 lhs32[3] = { lhs.high, lhs.low >> 32, lhs.low & UINT32_MAX };
	u64 res_tmp[3] = { 0, 0, 0 };
	u64 _mod;

	if (lhs.high == 0) {
		result.high = 0;
		result.low = lhs.low / rhs;
		_mod = lhs.low % rhs;
		if (mod != NULL) {
			*mod = _mod;
		}
		return result;
	}

	res_tmp[0] = lhs32[0] / rhs;
	lhs32[1] |= (lhs32[0] % rhs) << 32;

	res_tmp[1] = lhs32[1] / rhs;
	lhs32[2] |= (lhs32[1] % rhs) << 32;

	res_tmp[2] = lhs32[2] / rhs;
	_mod = lhs32[2] % rhs;

	res_tmp[1] += res_tmp[2] >> 32;
	res_tmp[2] &= UINT32_MAX;

	res_tmp[0] += res_tmp[1] >> 32;
	res_tmp[1] &= UINT32_MAX;

	result.low = (res_tmp[1] << 32) | res_tmp[2];
	result.high = res_tmp[0];

	if (mod != NULL) {
		*mod = _mod;
	}
	return result;
}

u64 Mod128(const u128 lhs, const u64 rhs) {
	u64 lhs32[3] = { lhs.high, lhs.low >> 32, lhs.low & UINT32_MAX };

	if (lhs.high == 0) {
		return lhs.low % rhs;
	}

	lhs32[1] |= (lhs32[0] % rhs) << 32;
	lhs32[2] |= (lhs32[1] % rhs) << 32;
	return lhs32[2] % rhs;
}

u128 Make128(const u64 val) {
	u128 result = { 0, val };
	return result;
}
#endif