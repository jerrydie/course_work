#include <string.h>
#include <stdlib.h>
#include "../FEA_Build_Config.h"

#ifndef FEA_MODULE_TRANSFORM
#include "transform_local.h"
#include "math_simple.h"

#define MIN(a,b) (((a)<=(b))?(a):(b))


fpe_radix_table* FEA_BuildRadixTable(uint32_t radix) {
	fpe_radix_table* table;
	u32 idx;
	u64 overflow = 0;
	u128 tmp, next;
	u64 tmp64;

	if (radix > FEA_MAX_RADIX || radix < FEA_MIN_RADIX) {
		return NULL;
	}

	table = (fpe_radix_table*)malloc(sizeof(fpe_radix_table));
	if (table == NULL) {
		return NULL;
	}
	table->Radix = radix;

	//MinLen
	tmp64 = radix;
	{
		idx = 1;
		while (tmp64 <= UINT8_MAX) {
			tmp64 *= radix;
			idx++;
		}
		table->MinLen = idx;
	}

	//MaxLen64
	{
		tmp = Mul64(radix, radix);
		next = Mul128(tmp, radix, NULL);
		idx = 2;
		while (next.high == 0) {
			idx++;
			tmp = next;
			next = Mul128(next, radix, NULL);
		}
		table->MaxLen64 = idx;
	}

	//MaxLen128
	{
		idx = table->MaxLen64;
		overflow = 0;
		while (overflow == 0) {
			idx++;
			tmp = next;
			next = Mul128(next, radix, &overflow);
		}
		table->MaxLen128 = idx;
		
	}
	
	return table;
}


void FEA_DestroyRadixTable(fpe_radix_table* table) {
	if (table == NULL) {
		return;
	}
	free(table);
}



fpe_err_t FEA_SetupLength(fpe_transform_ctx* ctx, const fpe_objtype_t obj_type, const uint32_t obj_length, const fpe_radix_table * table) {
	u128 value = Make128(1);
	u32 radix;
	u32 i;
	if (ctx == NULL) {
		return FEA_NULL_PTR;
	}

	memset(ctx, 0, sizeof(fpe_transform_ctx));
	ctx->type = obj_type;

	if (obj_type == FEA_DATA_BITS) {

		if (obj_length < 8 || obj_length > 128) {
			return FEA_INVALID_OBJ_LENGTH;
		}

		ctx->obj_length = obj_length;
		ctx->block_bits = obj_length;

	}
	else if (obj_type == FEA_DATA_DECIMAL) {
		
		if (obj_length < 3 || obj_length > 38) {
			return FEA_INVALID_OBJ_LENGTH;
		}

		ctx->obj_length = obj_length;

		for (i = 0; i < obj_length; i++) {
			value = Mul128(value, 10, NULL);
		}

		if (value.high != 0) {
			ctx->block_bits = 128 - my_clz64(value.high);
		}
		else {
			ctx->block_bits = 64 - my_clz64(value.low);
		}
		ctx->domain_size_Q = value.high;
		ctx->domain_size_R = value.low;

	}
	else {
		if (table == NULL) {
			return FEA_NULL_PTR;
		}

		if (obj_length < table->MinLen || obj_length > table->MaxLen128) {
			return FEA_INVALID_OBJ_LENGTH;
		}

		ctx->radix_table = table;
		ctx->obj_length = obj_length;

		radix = table->Radix;
		for (i = 0; i < obj_length; i++) {
			value = Mul128(value, radix, NULL);
		}

		if (value.high != 0) {
			ctx->block_bits = 128 - my_clz64(value.high);
		}
		else {
			ctx->block_bits = 64 - my_clz64(value.low);
		}
		ctx->domain_size_Q = value.high;
		ctx->domain_size_R = value.low;
	}

	return FEA_SUCCESS;
}

void Gen_NumFromArray(u64 *QR, const u32 *ar, const fpe_transform_ctx* table) {
	u64 value;
	u32 i, till;
	u32 radix;
	if (QR == NULL || ar == NULL || table == NULL) {
		return;
	}
	value = ar[0];
	radix = table->radix_table->Radix;
	till = MIN(table->obj_length, table->radix_table->MaxLen64);
	for (i = 1; i < till; i++) {
		value *= radix;
		value += ar[i];
	}
	QR[1] = value;
}

void Gen_TwoNumFromArray(u64 *QR, const u32 *ar, const fpe_transform_ctx * table) {
	u128 value;
	u32 i, till;
	u32 radix;
	if (QR == NULL || ar == NULL || table == NULL) {
		return;
	}
	radix = table->radix_table->Radix;
	till = MIN(table->obj_length, table->radix_table->MaxLen128);
	
	value = Make128(ar[0]);
	for (i = 1; i < till; i++) {
		value = Mul128(value, radix, NULL);
		value = Add128(value, ar[i]);
	}
	
	QR[0] = value.high;
	QR[1] = value.low;
}

void Gen_ArrayFromNum(u32 *ar, const u64 *QR, const fpe_transform_ctx * table) {
	u64 value;
	u32 radix;
	u32 i, till;
	if (ar == NULL || QR == NULL || table == NULL) {
		return;
	}
	value = QR[1];
	radix = table->radix_table->Radix;
	till = MIN(table->obj_length, table->radix_table->MaxLen64);
	for (i = 0; i < till; i++) {
		ar[table->obj_length - i - 1] = (u32)(value % radix);
		value /= radix;
	}
	for (i = 0; i < table->obj_length - till; i++) {
		ar[i] = 0;
	}
}

void Gen_ArrayFromTwoNum(u32 *ar, const u64 *QR, const fpe_transform_ctx * table) {
	u128 value;
	u64 mod;
	u32 radix;
	u32 i, till;
	if (ar == NULL || QR == NULL || table == NULL) {
		return;
	}
	value.high = QR[0];
	value.low = QR[1];
	radix = table->radix_table->Radix;
	till = MIN(table->obj_length, table->radix_table->MaxLen128);
	for (i = 0; i < till; i++) {
		value = Div128(value, radix, &mod);
		ar[table->obj_length - i - 1] = (u32)mod;
	}
	for (i = 0; i < table->obj_length - till; i++) {
		ar[i] = 0;
	}
}

void NumFromArray(u64 *QR, const u8 *ar, const u32 obj_length) {
	u64 value;
	u32 i;
	if (QR == NULL || ar == NULL) {
		return;
	}
	if (obj_length == 0) {
		return;
	}
	value = ar[0];
	for (i = 1; i < obj_length; i++) {
		value *= 10;
		value += ar[i];
	}
	QR[1] = value;
}

void TwoNumFromArray(u64 *QR, const u8 *ar, const u32 obj_length) {
	u128 value;
	u32 i;
	if (QR == NULL || ar == NULL) {
		return;
	}
	if (obj_length == 0) {
		return;
	}
	value = Make128(ar[0]);
	for (i = 1; i < obj_length; i++) {
		value = Mul128(value, 10, NULL);
		value = Add128(value, ar[i]);
	}
	QR[0] = value.high;
	QR[1] = value.low;
}

void ArrayFromNum(u8 *ar, const u64 *QR, const u32 obj_length) {
	u64 value;
	u32 i;
	if (ar == NULL || QR == NULL) {
		return;
	}
	value = QR[1];
	for (i = 0; i < obj_length; i++) {
		ar[obj_length - i - 1] = (u8)(value % 10);
		value /= 10;
	}
}

void ArrayFromTwoNum(u8 *ar, const u64 *QR, const u32 obj_length) {
	u128 value;
	u32 i;
	u64 mod;
	if (ar == NULL || QR == NULL) {
		return;
	}
	value.high = QR[0];
	value.low = QR[1];
	for (i = 0; i < obj_length; i++) {
		value = Div128(value, 10, &mod);
		ar[obj_length - i - 1] = (u8)mod;
	}
}

void BytesToU64LR(const u8 *bytes_in, u64 *bytes_out, const int block_bits, const int is_even_round)
{
	int n, nByte;
	u64 X0, X1;
	u64 Y0, Y1;
	u8 pt[16] = { 0, };
	u64* pt64 = (u64*)pt;
	int nb, nbe, nbo;

	nb = block_bits;
	if (is_even_round == 0) {
		SplitBits(&nbe, &nbo, nb);
	}
	else
	{
		SplitBits(&nbo, &nbe, nb);
	}

	n = nb % 8;

	if (n == 0)
	{
		nByte = nb / 8;
		memcpy(pt, bytes_in, nByte);
		Y0 = load64LE(pt64[1]);
		Y1 = load64LE(pt64[0]);
	}
	else
	{
		nByte = nb / 8 + 1;
		memcpy(pt, bytes_in, nByte);
		Y0 = load64BE(pt64[0]);
		Y1 = load64BE(pt64[1]);
		if (nb <= 64) {
			Y1 = Y0 >> (64 - nb);
			Y0 = 0;
		}
		else {
			Y1 = (Y0 << (nb - 64)) ^ (Y1 >> (128 - nb));
			Y0 = Y0 >> (128 - nb);
		}
	}

	if (nb <= 64) {
		Y1 &= (UINT64_MAX >> (64 - nb));
		X0 = (Y1 >> nbo) << (64 - nbe);
		X1 = Y1 << (64 - nbo);
	}
	else if (nb<127) {
		Y0 &= (UINT64_MAX >> (128 - nb));
		X0 = (Y0 << (128 - nb)) ^ ((Y1 >> nbo) << (64 - nbe));
		X1 = (Y1 << (64 - nbo));
	}
	else if (nb == 127) {
		X0 = (Y0 << 1) ^ (Y1 >> 63);
		X1 = (Y1 << (64 - nbo));
	}
	else {
		X0 = Y0;
		X1 = Y1;
	}

	if (is_even_round == 0)
	{
		bytes_out[0] = X0;
		bytes_out[1] = X1;
	}
	else
	{
		bytes_out[0] = X1;
		bytes_out[1] = X0;
	}

}

void U64LRToBytes(const u64 *bytes_in, u8 *bytes_out, const int block_bits, const int is_even_round)
{
	int n, nByte;
	u64 Y0, Y1;
	u64 X0, X1;
	u8 ct[16] = { 0, };
	u64* ct64 = (u64*)ct;

	int nb, nbe, nbo;

	nb = block_bits;
	if (is_even_round == 0)
	{
		SplitBits(&nbe, &nbo, nb);

		X0 = bytes_in[0];
		X1 = bytes_in[1];
	}
	else
	{
		SplitBits(&nbo, &nbe, nb);

		X0 = bytes_in[1];
		X1 = bytes_in[0];
	}

	n = nb % 8;

	if (nb <= 64) { Y0 = 0;	Y1 = (X0 >> (64 - nb)) ^ (X1 >> (64 - nbo)); }
	else if (nb<128) { Y0 = (X0 >> (128 - nb));	Y1 = (X0 << (nb - 64)) ^ (X1 >> (64 - nbo)); }
	else { Y0 = X0;	Y1 = X1; }

	if (n == 0)
	{
		nByte = nb / 8;
		ct64[0] = load64LE(Y1);
		ct64[1] = load64LE(Y0);
		memcpy(bytes_out, ct, nByte);
	}
	else
	{
		nByte = nb / 8 + 1;
		if (nb <= 64) {
			Y0 = Y1 << (64 - nb);
			Y1 = 0;
		}
		else {
			Y0 = (Y0 << (128 - nb)) ^ (Y1 >> (nb - 64));
			Y1 = Y1 << (128 - nb);
		}
		ct64[0] = load64BE(Y0);
		ct64[1] = load64BE(Y1);
		memcpy(bytes_out, ct, nByte);
	}
}

void SplitToU64LR(const u64 *bytes_in, u64 *bytes_out, const int block_bits, const int is_even_round)
{
	u64 IN0, IN1, OUT0, OUT1;
	int nb, nbe, nbo;

	IN0 = bytes_in[0];
	IN1 = bytes_in[1];

	nb = block_bits;

	if (is_even_round == 0)
	{
		SplitBits(&nbe, &nbo, nb);
	}
	else
	{
		SplitBits(&nbo, &nbe, nb);
	}

	if (nb <= 64) {
		IN1 &= (UINT64_MAX >> (64 - nb));
		OUT0 = (IN1 >> nbo) << (64 - nbe);
		OUT1 = IN1 << (64 - nbo);
	}
	else if (nb<127) {
		IN0 &= (UINT64_MAX >> (128 - nb));
		OUT0 = (IN0 << (128 - nb)) ^ ((IN1 >> nbo) << (64 - nbe));
		OUT1 = (IN1 << (64 - nbo));
	}
	else if (nb == 127) {
		OUT0 = (IN0 << 1) ^ (IN1 >> 63);
		OUT1 = (IN1 << (64 - nbo));
	}
	else {
		OUT0 = IN0;
		OUT1 = IN1;
	}
	if (is_even_round == 0)
	{
		bytes_out[0] = OUT0;
		bytes_out[1] = OUT1;
	}
	else
	{
		bytes_out[0] = OUT1;
		bytes_out[1] = OUT0;
	}

}

void MergeU64LR(const u64 *bytes_in, u64 *bytes_out, const int block_bits, const int is_even_round)
{

	u64 IN0, IN1;
	int nb, nbe, nbo;
	nb = block_bits;
	if (is_even_round == 0)
	{
		SplitBits(&nbe, &nbo, nb);

		IN0 = bytes_in[0];
		IN1 = bytes_in[1];

	}
	else
	{
		SplitBits(&nbo, &nbe, nb);

		IN0 = bytes_in[1];
		IN1 = bytes_in[0];
	}
	if (nb <= 64) {
		bytes_out[0] = 0;
		bytes_out[1] = (IN0 >> (64 - nb)) ^ (IN1 >> (64 - nbo));
	}
	else if (nb<128) {
		bytes_out[0] = (IN0 >> (128 - nb));
		bytes_out[1] = (IN0 << (nb - 64)) ^ (IN1 >> (64 - nbo));
	}
	else {
		bytes_out[0] = IN0;
		bytes_out[1] = IN1;
	}
}

u32 GetRadixMaxObjLen(const fpe_radix_table* table) {
	if (table == NULL) {
		return 0;
	}
	return table->MaxLen128;
}
u32 GetRadixMinObjLen(const fpe_radix_table* table) {
	if (table == NULL) {
		return 0;
	}
	return table->MinLen;
}
u32 GetRadixFromTable(const fpe_radix_table* table) {
	if (table == NULL) {
		return 0;
	}
	return table->Radix;
}

#endif