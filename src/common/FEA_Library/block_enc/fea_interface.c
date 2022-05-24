#include <stddef.h>
#include <string.h>
#include "../FEA_Build_Config.h"

#ifdef FEA_MODULE_BLKENC
#include "../FEA_Local.h"
#include "block_local.h"
#include "../FEA_Transform.h"

/**
* Encryption function for decimal strings
*
* @param [in] pt	Plaintext (decimal strings)
* @param [in] tw	Tweak
* @param [out] ct	Ciphertext (decimal strings)
*
* @return ::FEA_SUCCESS (if successful)
*/
fpe_err_t _FEA_Enc_Decimal(fpe_decimal_data* ct, const fpe_decimal_data* pt, const fpe_tweak* tw, const fpe_encrypt_ctx *ctx)
{
	u64 t64[2] = { 0,0 };
	u64 x64[2] = { 0,0 };

	if (ct == NULL || pt == NULL || tw == NULL || ctx == NULL) {
		return FEA_NULL_PTR;
	}

	if (ctx->t_ctx.domain_size_Q == 0)
	{

		NumFromArray(t64, pt->value, ctx->t_ctx.obj_length);
		do {
			SplitToU64LR(t64, x64, ctx->t_ctx.block_bits, 0);
			if (ctx->alg == FEA_ALG_ID_FEA_A) { FEA_A_Enc_U64ToU64(ctx->e_ctx.rk, tw->tweak, x64, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
			else if (ctx->alg == FEA_ALG_ID_FEA_B) { FEA_B_Enc_U64ToU64(ctx->e_ctx.rk, tw->tweak, x64, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
			if ((ctx->e_ctx.rounds % 2) == 1) { MergeU64LR(x64, t64, ctx->t_ctx.block_bits, 0); }
			else { MergeU64LR(x64, t64, ctx->t_ctx.block_bits, 1); }
		} while (t64[1] >= ctx->t_ctx.domain_size_R);

		ArrayFromNum(ct->value, t64, ctx->t_ctx.obj_length);
		return FEA_SUCCESS;
	}
	else
	{
		TwoNumFromArray(t64, pt->value, ctx->t_ctx.obj_length);
		do {
			SplitToU64LR(t64, x64, ctx->t_ctx.block_bits, 0);
			if (ctx->alg == FEA_ALG_ID_FEA_A) { FEA_A_Enc_U64ToU64(ctx->e_ctx.rk, tw->tweak, x64, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
			else if (ctx->alg == FEA_ALG_ID_FEA_B) { FEA_B_Enc_U64ToU64(ctx->e_ctx.rk, tw->tweak, x64, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
			if ((ctx->e_ctx.rounds % 2) == 1) { MergeU64LR(x64, t64, ctx->t_ctx.block_bits, 0); }
			else { MergeU64LR(x64, t64, ctx->t_ctx.block_bits, 1); }
		} while ((t64[0] > ctx->t_ctx.domain_size_Q) | ((t64[0] == ctx->t_ctx.domain_size_Q)&(t64[1] >= ctx->t_ctx.domain_size_R)));
		ArrayFromTwoNum(ct->value, t64, ctx->t_ctx.obj_length);
		return FEA_SUCCESS;
	}
}
/**
* Decryption function for decimal strings
*
* @param [in] ct	Ciphertext (decimal strings)
* @param [in] tw	Tweak
* @param [out] pt	Plaintext (decimal strings)
*
* @return ::FEA_SUCCESS (if successful)
*/
fpe_err_t _FEA_Dec_Decimal(fpe_decimal_data* pt, const fpe_decimal_data* ct, const fpe_tweak* tw, const fpe_encrypt_ctx *ctx)
{
	u64 t64[2] = { 0,0 };
	u64 x64[2] = { 0,0 };

	if (ct == NULL || pt == NULL || tw == NULL || ctx == NULL) {
		return FEA_NULL_PTR;
	}

	if (ctx->t_ctx.domain_size_Q == 0)
	{
		NumFromArray(t64, ct->value, ctx->t_ctx.obj_length);
		do {
			if ((ctx->e_ctx.rounds % 2) == 1)	SplitToU64LR(t64, x64, ctx->t_ctx.block_bits, 0);
			else { SplitToU64LR(t64, x64, ctx->t_ctx.block_bits, 1); }
			if (ctx->alg == FEA_ALG_ID_FEA_A) { FEA_A_Dec_U64ToU64(ctx->e_ctx.rk, tw->tweak, x64, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
			else if (ctx->alg == FEA_ALG_ID_FEA_B) { FEA_B_Dec_U64ToU64(ctx->e_ctx.rk, tw->tweak, x64, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
			MergeU64LR(x64, t64, ctx->t_ctx.block_bits, 0);
		} while (t64[1] >= ctx->t_ctx.domain_size_R);

		ArrayFromNum(pt->value, t64, ctx->t_ctx.obj_length);
		return FEA_SUCCESS;
	}
	else
	{
		TwoNumFromArray(t64, ct->value, ctx->t_ctx.obj_length);
		do {
			if ((ctx->e_ctx.rounds % 2) == 1)	SplitToU64LR(t64, x64, ctx->t_ctx.block_bits, 0);
			else { SplitToU64LR(t64, x64, ctx->t_ctx.block_bits, 1); }
			if (ctx->alg == FEA_ALG_ID_FEA_A) { FEA_A_Dec_U64ToU64(ctx->e_ctx.rk, tw->tweak, x64, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
			else if (ctx->alg == FEA_ALG_ID_FEA_B) { FEA_B_Dec_U64ToU64(ctx->e_ctx.rk, tw->tweak, x64, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
			MergeU64LR(x64, t64, ctx->t_ctx.block_bits, 0);
		} while ((t64[0] > ctx->t_ctx.domain_size_Q) || ((t64[0] == ctx->t_ctx.domain_size_Q) && (t64[1] >= ctx->t_ctx.domain_size_R)));

		ArrayFromTwoNum(pt->value, t64, ctx->t_ctx.obj_length);
		return FEA_SUCCESS;
	}
}
/**
* Encryption function for binary strings
*
* @param [in] pt	Plaintext (binary strings)
* @param [in] tw	Tweak
* @param [out] ct	Ciphertext (binary strings)
*
* @return ::FEA_SUCCESS (if successful)
*/
fpe_err_t _FEA_Enc_Bits(fpe_bits_data* ct, const fpe_bits_data* pt, const fpe_tweak* tw, const fpe_encrypt_ctx *ctx)
{
	u64 XX[2] = { 0,0 };

	if (ct == NULL || pt == NULL || tw == NULL || ctx == NULL) {
		return FEA_NULL_PTR;
	}

	BytesToU64LR(pt->value, XX, ctx->t_ctx.block_bits, 0);
	if (ctx->alg == FEA_ALG_ID_FEA_A) { FEA_A_Enc_U64ToU64(ctx->e_ctx.rk, tw->tweak, XX, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
	else if (ctx->alg == FEA_ALG_ID_FEA_B) { FEA_B_Enc_U64ToU64(ctx->e_ctx.rk, tw->tweak, XX, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
	if ((ctx->e_ctx.rounds % 2) == 1) U64LRToBytes(XX, ct->value, ctx->t_ctx.block_bits, 0);
	else                         U64LRToBytes(XX, ct->value, ctx->t_ctx.block_bits, 1);

	return FEA_SUCCESS;
}

/**
* Decryption function for binary strings
*
* @param [in] ct	Ciphertext (binary strings)
* @param [in] tw	Tweak
* @param [out] pt	Plaintext (binary strings)
*
* @return ::FEA_SUCCESS (if successful)
*/
fpe_err_t _FEA_Dec_Bits(fpe_bits_data* pt, const fpe_bits_data* ct, const fpe_tweak* tw, const fpe_encrypt_ctx *ctx)
{
	u64 XX[2] = { 0,0 };
	if (ct == NULL || pt == NULL || tw == NULL || ctx == NULL) {
		return FEA_NULL_PTR;
	}

	if ((ctx->e_ctx.rounds % 2) == 1) BytesToU64LR(ct->value, XX, ctx->t_ctx.block_bits, 0);
	else                       BytesToU64LR(ct->value, XX, ctx->t_ctx.block_bits, 1);
	if (ctx->alg == FEA_ALG_ID_FEA_A) { FEA_A_Dec_U64ToU64(ctx->e_ctx.rk, tw->tweak, XX, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
	else if (ctx->alg == FEA_ALG_ID_FEA_B) { FEA_B_Dec_U64ToU64(ctx->e_ctx.rk, tw->tweak, XX, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
	U64LRToBytes(XX, pt->value, ctx->t_ctx.block_bits, 0);

	return FEA_SUCCESS;
}
/**
* Encryption function for various radix strings
*
* @param [in] pt	Plaintext (in various radix)
* @param [in] tw	Tweak
* @param [out] ct	Ciphertext (in various radix)
*
* @return ::FEA_SUCCESS (if successful)
*/
fpe_err_t _FEA_Enc_Radix(fpe_radix_data* ct, const fpe_radix_data* pt, const fpe_tweak* tw, const fpe_encrypt_ctx *ctx)
{
	u64 t64[2] = { 0,0 };
	u64 x64[2] = { 0,0 };

	if (ct == NULL || pt == NULL || tw == NULL || ctx == NULL) {
		return FEA_NULL_PTR;
	}

	if ((ctx->t_ctx.domain_size_Q == 0) && (ctx->t_ctx.domain_size_R != 0))
	{


		Gen_NumFromArray(t64, pt->value, &ctx->t_ctx);

		do {
			SplitToU64LR(t64, x64, ctx->t_ctx.block_bits, 0);
			if (ctx->alg == FEA_ALG_ID_FEA_A) { FEA_A_Enc_U64ToU64(ctx->e_ctx.rk, tw->tweak, x64, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
			else if (ctx->alg == FEA_ALG_ID_FEA_B) { FEA_B_Enc_U64ToU64(ctx->e_ctx.rk, tw->tweak, x64, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
			if ((ctx->e_ctx.rounds % 2) == 1) { MergeU64LR(x64, t64, ctx->t_ctx.block_bits, 0); }
			else { MergeU64LR(x64, t64, ctx->t_ctx.block_bits, 1); }

		} while (t64[1] >= ctx->t_ctx.domain_size_R);

		Gen_ArrayFromNum(ct->value, t64, &ctx->t_ctx);
		return FEA_SUCCESS;
	}
	else if ((ctx->t_ctx.domain_size_Q == 0) && (ctx->t_ctx.domain_size_R == 0))
	{

		Gen_TwoNumFromArray(t64, pt->value, &ctx->t_ctx);
		SplitToU64LR(t64, x64, ctx->t_ctx.block_bits, 0);
		if (ctx->alg == FEA_ALG_ID_FEA_A) { FEA_A_Enc_U64ToU64(ctx->e_ctx.rk, tw->tweak, x64, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
		else if (ctx->alg == FEA_ALG_ID_FEA_B) { FEA_B_Enc_U64ToU64(ctx->e_ctx.rk, tw->tweak, x64, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }

		if ((ctx->e_ctx.rounds % 2) == 1) { MergeU64LR(x64, t64, ctx->t_ctx.block_bits, 0); }
		else { MergeU64LR(x64, t64, ctx->t_ctx.block_bits, 1); }
		Gen_ArrayFromTwoNum(ct->value, t64, &ctx->t_ctx);
		return FEA_SUCCESS;
	}
	else
	{
		Gen_TwoNumFromArray(t64, pt->value, &ctx->t_ctx);

		do {
			SplitToU64LR(t64, x64, ctx->t_ctx.block_bits, 0);
			if (ctx->alg == FEA_ALG_ID_FEA_A) { FEA_A_Enc_U64ToU64(ctx->e_ctx.rk, tw->tweak, x64, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
			else if (ctx->alg == FEA_ALG_ID_FEA_B) { FEA_B_Enc_U64ToU64(ctx->e_ctx.rk, tw->tweak, x64, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
			if ((ctx->e_ctx.rounds % 2) == 1) { MergeU64LR(x64, t64, ctx->t_ctx.block_bits, 0); }
			else { MergeU64LR(x64, t64, ctx->t_ctx.block_bits, 1); }
		} while ((t64[0] > ctx->t_ctx.domain_size_Q) || ((t64[0] == ctx->t_ctx.domain_size_Q)&(t64[1] >= ctx->t_ctx.domain_size_R)));

		Gen_ArrayFromTwoNum(ct->value, t64, &ctx->t_ctx);

		return FEA_SUCCESS;
	}
}
/**
* Decryption function for various radix strings
*
* @param [in] ct	Ciphertext (in various strings)
* @param [in] tw	Tweak
* @param [out] pt	Plaintext (in various strings)
*
* @return ::FEA_SUCCESS (if successful)
*/
fpe_err_t _FEA_Dec_Radix(fpe_radix_data* pt, const fpe_radix_data* ct, const fpe_tweak* tw, const fpe_encrypt_ctx *ctx)
{
	u64 t64[2] = { 0,0 };
	u64 x64[2] = { 0,0 };

	if (ct == NULL || pt == NULL || tw == NULL || ctx == NULL) {
		return FEA_NULL_PTR;
	}

	if ((ctx->t_ctx.domain_size_Q == 0) && (ctx->t_ctx.domain_size_R != 0))
	{
		Gen_NumFromArray(t64, ct->value, &ctx->t_ctx);
		do {
			if ((ctx->e_ctx.rounds % 2) == 1)	SplitToU64LR(t64, x64, ctx->t_ctx.block_bits, 0);
			else { SplitToU64LR(t64, x64, ctx->t_ctx.block_bits, 1); }
			if (ctx->alg == FEA_ALG_ID_FEA_A) { FEA_A_Dec_U64ToU64(ctx->e_ctx.rk, tw->tweak, x64, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
			else if (ctx->alg == FEA_ALG_ID_FEA_B) { FEA_B_Dec_U64ToU64(ctx->e_ctx.rk, tw->tweak, x64, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
			MergeU64LR(x64, t64, ctx->t_ctx.block_bits, 0);
		} while (t64[1] >= ctx->t_ctx.domain_size_R);

		Gen_ArrayFromNum(pt->value, t64, &ctx->t_ctx);
		return FEA_SUCCESS;
	}
	else if ((ctx->t_ctx.domain_size_Q == 0) && (ctx->t_ctx.domain_size_R == 0))
	{
		Gen_TwoNumFromArray(t64, ct->value, &ctx->t_ctx);
		if ((ctx->e_ctx.rounds % 2) == 1)	SplitToU64LR(t64, x64, ctx->t_ctx.block_bits, 0);
		else { SplitToU64LR(t64, x64, ctx->t_ctx.block_bits, 1); }
		if (ctx->alg == FEA_ALG_ID_FEA_A) { FEA_A_Dec_U64ToU64(ctx->e_ctx.rk, tw->tweak, x64, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
		else if (ctx->alg == FEA_ALG_ID_FEA_B) { FEA_B_Dec_U64ToU64(ctx->e_ctx.rk, tw->tweak, x64, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
		MergeU64LR(x64, t64, ctx->t_ctx.block_bits, 0);
		Gen_ArrayFromTwoNum(pt->value, t64, &ctx->t_ctx);
		return FEA_SUCCESS;
	}
	else
	{
		Gen_TwoNumFromArray(t64, ct->value, &ctx->t_ctx);
		do {
			if ((ctx->e_ctx.rounds % 2) == 1)	SplitToU64LR(t64, x64, ctx->t_ctx.block_bits, 0);
			else { SplitToU64LR(t64, x64, ctx->t_ctx.block_bits, 1); }
			if (ctx->alg == FEA_ALG_ID_FEA_A) { FEA_A_Dec_U64ToU64(ctx->e_ctx.rk, tw->tweak, x64, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
			else if (ctx->alg == FEA_ALG_ID_FEA_B) { FEA_B_Dec_U64ToU64(ctx->e_ctx.rk, tw->tweak, x64, ctx->t_ctx.block_bits, ctx->e_ctx.rounds); }
			MergeU64LR(x64, t64, ctx->t_ctx.block_bits, 0);
		} while ((t64[0] > ctx->t_ctx.domain_size_Q) || ((t64[0] == ctx->t_ctx.domain_size_Q)&(t64[1] >= ctx->t_ctx.domain_size_R)));

		Gen_ArrayFromTwoNum(pt->value, t64, &ctx->t_ctx);
		return FEA_SUCCESS;
	}
}


fpe_err_t FEA_Encrypt(void* ct, const void* pt, const fpe_tweak* tweak, const fpe_encrypt_ctx* ctx) {
	u64 t64[2] = { 0,0 };
	u64 x64[2] = { 0,0 };

	if (ct == NULL || pt == NULL || tweak == NULL || ctx == NULL) {
		return FEA_NULL_PTR;
	}

	if (ctx->t_ctx.type == FEA_DATA_BITS) {
		return _FEA_Enc_Bits((fpe_bits_data*)ct, (const fpe_bits_data*)pt, tweak, ctx);
	}
	else if (ctx->t_ctx.type == FEA_DATA_DECIMAL) {
		return _FEA_Enc_Decimal((fpe_decimal_data*)ct, (const fpe_decimal_data*)pt, tweak, ctx);
	}
	else {
		return _FEA_Enc_Radix((fpe_radix_data*)ct, (const fpe_radix_data*)pt, tweak, ctx);
	}
}


fpe_err_t FEA_Decrypt(void* pt, const void* ct, const fpe_tweak* tweak, const fpe_encrypt_ctx* ctx) {
	u64 t64[2] = { 0,0 };
	u64 x64[2] = { 0,0 };

	if (ct == NULL || pt == NULL || tweak == NULL || ctx == NULL) {
		return FEA_NULL_PTR;
	}

	if (ctx->t_ctx.type == FEA_DATA_BITS) {
		return _FEA_Dec_Bits((fpe_bits_data*)pt, (const fpe_bits_data*)ct, tweak, ctx);
	}
	else if (ctx->t_ctx.type == FEA_DATA_DECIMAL) {
		return _FEA_Dec_Decimal((fpe_decimal_data*)pt, (const fpe_decimal_data*)ct, tweak, ctx);
	}
	else {
		return _FEA_Dec_Radix((fpe_radix_data*)pt, (const fpe_radix_data*)ct, tweak, ctx);
	}
}

#endif