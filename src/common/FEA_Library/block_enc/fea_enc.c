#include <stddef.h>
#include <string.h>
#include "../FEA_Build_Config.h"

#ifdef FEA_MODULE_BLKENC

#include "block_local.h"
#include "block_tables.h"

static u8 mul2(const u8 x) {
	return (x << 1) ^ ((x >> 7)?poly_p:0);
}

// GF(2^8) field multiplication
static u8 field_mult(const u8 a, const u8 b) {
	u8 result = 0;
	
	for (int i = 0; i < 8; i++) {
		u8 mult_val = b;
		for (int j = 0; j < i; j++) {
			mult_val = mul2(mult_val);
		}
		result ^= ((a >> i) & 0x1) * mult_val;
	}
	return result;
}

// Substitution and diffusion layers
static u64 sub_diff_layer(const u64 val) {
	u64 result = 0;
	u8 tmp_arr[8] = { 0 };
	for (int i = 0; i < 8; i++) {
		u8 tmp = (u8)(val >> ((7 - i) * 8));
		tmp_arr[i] = S_box[tmp];
	}

	for (int y = 0; y < 8; y++) {
		u8 tmp = 0;
		for (int x = 0; x < 8; x++) {
			tmp ^= field_mult(diff_mat[y][x], tmp_arr[x]);
		}
		result ^= ((u64)tmp) << ((7 - y) * 8);
	}
	return result;
}

fpe_err_t FEA_Keyschedule(fpe_encrypt_ctx* ctx, const fpe_transform_ctx* t_ctx, const uint32_t enc_type, const uint8_t* key, const uint32_t key_bits)
{

	int i;
	int algtype, keytype;

	u64 t0, t1, t2, t3;
	u64 tt0, tt1, ttt;

	u64 block_bits;
	const u64* key64 = (const u64*)key;
	const u64* rc_now = NULL;

	fpe_enckey* rk;
	if (ctx == NULL || key == NULL || t_ctx == NULL) {
		return FEA_NULL_PTR;
	}

	if (&ctx->t_ctx != t_ctx) {
		ctx->t_ctx = *t_ctx;
	}

	rk = &ctx->e_ctx;
	memset(rk, 0, sizeof(fpe_enckey));
	ctx->alg = enc_type;
	rk->key_bits = key_bits;
	block_bits = ctx->t_ctx.block_bits;

	if (enc_type == FEA_ALG_ID_FEA_A)
	{
		switch (rk->key_bits) {
		case 128: rk->rounds = 12; break;
		case 192: rk->rounds = 14; break;
		case 256: rk->rounds = 16; break;
		default: return FEA_INVALID_KEY_SIZE;
		}
	}
	else if (enc_type == FEA_ALG_ID_FEA_B)
	{
		switch (rk->key_bits) {
		case 128: rk->rounds = 18; break;
		case 192: rk->rounds = 21; break;
		case 256: rk->rounds = 24; break;
		default: return FEA_INVALID_KEY_SIZE;
		}
	}
	else {
		return FEA_INVALID_TYPE;
	}

	t0 = load64BE(key64[0]);
	t1 = load64BE(key64[1]);

	algtype = (enc_type == FEA_ALG_ID_FEA_A) ? 0 : 1;

	if (rk->key_bits == 128)
	{
		t2 = 0;	t3 = 0;
		keytype = 0;
	}
	else if (rk->key_bits == 192)
	{
		t2 = load64BE(key[2]); t3 = 0;
		keytype = 1;
	}
	else if (rk->key_bits == 256)
	{
		t2 = load64BE(key[2]); t3 = load64BE(key[3]);
		keytype = 2;
	}
	else {
		return FEA_INVALID_KEY_SIZE;
	}

	rc_now = &rc[algtype][keytype][0];

	for (i = 0; i < rk->rounds / 2; i++) {
		tt0 = t0^t2;		tt1 = t1^t3;

		tt0 ^= rc_now[i];
		tt0 = sub_diff_layer(tt0);
		tt1 ^= block_bits;
		tt1 ^= tt0;
		tt1 = sub_diff_layer(tt1);
		tt0 ^= tt1;
		t0 ^= tt0;		t1 ^= tt1;		t2 ^= tt0;		t3 ^= tt1;
		ttt = t2;		t2 ^= t3;		t3 = ttt;
		rk->rk[4 * i] = t0;
		rk->rk[4 * i + 1] = t1;
		rk->rk[4 * i + 2] = t2;
		rk->rk[4 * i + 3] = t3;
	}
	if ((rk->rounds % 2) == 1)
	{
		tt0 = t0^t2;		tt1 = t1^t3;

		tt0 ^= rc_now[i];
		tt0 = sub_diff_layer(tt0);
		tt1 ^= block_bits;
		tt1 ^= tt0;
		tt1 = sub_diff_layer(tt1);
		tt0 ^= tt1;
		t0 ^= tt0;		t1 ^= tt1;		t2 ^= tt0;		t3 ^= tt1;
		ttt = t2;		t2 ^= t3;		t3 = ttt;
		rk->rk[4 * i] = t0;
		rk->rk[4 * i + 1] = t1;
	}
	return FEA_SUCCESS;
}



/// Macro for FPE-A round function
#define FEA_A_ENC_ROUND_O(rr) {\
	T0 = (X1&masktab[nBlkBits][1])^ Tw[0] ^ rk[(rr)<<1];\
	T1 = sub_diff_layer(T0);\
	T1 ^= rk[((rr)<<1)+1];\
	T0 = sub_diff_layer(T1);\
	X0 ^= T0;\
	}
#define FEA_A_ENC_ROUND_E(rr) {\
	T0 = (X0&masktab[nBlkBits][0])^ Tw[1]  ^ rk[(rr)<<1];\
	T1 = sub_diff_layer(T0);\
	T1 ^= rk[((rr)<<1)+1];\
	T0 = sub_diff_layer(T1);\
	X1 ^= T0;\
	}

/// Macro for FPE-B round function
#define FEA_B_ENC_CORE(r) {\
	T1 = sub_diff_layer(T0);\
	T1 ^= rk[((r)<<1)+1];\
	T0 = sub_diff_layer(T1);\
}
#define FEA_B_ENC_ROUND_0(rr) {T0 = (X1&masktab[nBlkBits][1])^ rk[(rr)<<1];FEA_B_ENC_CORE(rr); X0^=T0;}
#define FEA_B_ENC_ROUND_1(rr) {T0 = (X0&masktab[nBlkBits][0])^ Tw[0] ^ rk[(rr)<<1];FEA_B_ENC_CORE(rr); X1^=T0;}
#define FEA_B_ENC_ROUND_2(rr) {T0 = (X1&masktab[nBlkBits][1])^ Tw[1] ^ rk[(rr)<<1];FEA_B_ENC_CORE(rr); X0^=T0;}
#define FEA_B_ENC_ROUND_3(rr) {T0 = (X0&masktab[nBlkBits][0])^ rk[(rr)<<1];FEA_B_ENC_CORE(rr);	X1^=T0;}
#define FEA_B_ENC_ROUND_4(rr) {T0 = (X1&masktab[nBlkBits][1])^ Tw[0] ^ rk[(rr)<<1];FEA_B_ENC_CORE(rr); X0^=T0;}
#define FEA_B_ENC_ROUND_5(rr) {T0 = (X0&masktab[nBlkBits][0])^ Tw[1] ^ rk[(rr)<<1];FEA_B_ENC_CORE(rr); X1^=T0;}




// Encryption and decryption functions

/**
* Encryption function for FEA-A
*
* @param [in] rk	Round keys
* @param [in] TW	Tweak
* @param [in] XX	Plaintext
* @param [in] nBlkBits	Bit length
* @param [in] nRounds	Number of rounds
*
* @param [out] XX	Ciphertest
*
* @return
*/
void FEA_A_Enc_U64ToU64(const u64 *rk, const u64 *TW, u64 *XX, int nBlkBits, int nRounds)
{
	u64 T0, T1;
	u64 X0, X1;
	u64 Tw[2] = { TW[0], TW[1] };

	X0 = XX[0];
	X1 = XX[1];
	//트윅 가공
	Tw[0] &= masktabtw[128 - nBlkBits][1];
	Tw[1] &= masktabtw[128 - nBlkBits][0];
    
    //rewrote the round encryptions to trade speed for flexiblity
    int i = 0;
    for (i = 0; i < nRounds/2; ++i) {
        FEA_A_ENC_ROUND_O(2 * i + 0);
        FEA_A_ENC_ROUND_E(2 * i + 1);
    }
    
    /*
	FEA_A_ENC_ROUND_O(0);	FEA_A_ENC_ROUND_E(1);
	FEA_A_ENC_ROUND_O(2);	FEA_A_ENC_ROUND_E(3);
	FEA_A_ENC_ROUND_O(4);	FEA_A_ENC_ROUND_E(5);
	FEA_A_ENC_ROUND_O(6);	FEA_A_ENC_ROUND_E(7);
	FEA_A_ENC_ROUND_O(8);	FEA_A_ENC_ROUND_E(9);
	FEA_A_ENC_ROUND_O(10);	FEA_A_ENC_ROUND_E(11);
	if (nRounds == 14) {
		FEA_A_ENC_ROUND_O(12);	FEA_A_ENC_ROUND_E(13);
	}
	else if (nRounds == 16) {
		FEA_A_ENC_ROUND_O(12);	FEA_A_ENC_ROUND_E(13);
		FEA_A_ENC_ROUND_O(14);	FEA_A_ENC_ROUND_E(15);
	}
    }
    */
        
	XX[0] = X0 & masktab[nBlkBits][0];
	XX[1] = X1 & masktab[nBlkBits][1];
}
/**
* Decryption function for FEA-A
*
* @param [in] rk	Round keys
* @param [in] TW	Tweak
* @param [in] XX	Ciphertext
* @param [in] nBlkBits	Bit length
* @param [in] nRounds	Number of rounds
* @param [out] XX	Plaintext
*
* @return
*/
void FEA_A_Dec_U64ToU64(const u64 *rk, const u64 *TW, u64 *XX, int nBlkBits, int nRounds)
{
	u64 T0, T1;
	u64 X0, X1;
	u64 Tw[2] = { TW[0], TW[1] };

	X0 = XX[0];
	X1 = XX[1];

	Tw[0] &= masktabtw[128 - nBlkBits][1];
	Tw[1] &= masktabtw[128 - nBlkBits][0];

	if (nRounds == 14) {
		FEA_A_ENC_ROUND_E(13);	FEA_A_ENC_ROUND_O(12);
	}
	else if (nRounds == 16) {
		FEA_A_ENC_ROUND_E(15);	FEA_A_ENC_ROUND_O(14);
		FEA_A_ENC_ROUND_E(13);	FEA_A_ENC_ROUND_O(12);
	}
	FEA_A_ENC_ROUND_E(11);	FEA_A_ENC_ROUND_O(10);
	FEA_A_ENC_ROUND_E(9);	FEA_A_ENC_ROUND_O(8);
	FEA_A_ENC_ROUND_E(7);	FEA_A_ENC_ROUND_O(6);
	FEA_A_ENC_ROUND_E(5);	FEA_A_ENC_ROUND_O(4);
	FEA_A_ENC_ROUND_E(3);	FEA_A_ENC_ROUND_O(2);
	FEA_A_ENC_ROUND_E(1);	FEA_A_ENC_ROUND_O(0);

	XX[0] = X0 & masktab[nBlkBits][0];
	XX[1] = X1 & masktab[nBlkBits][1];
}
/**
* Encryption function for FEA-B
*
* @param [in] rk		Round keys
* @param [in] TW		Tweak
* @param [in] XX		Plaintext
* @param [in] nBlkBits	Bit length
* @param [in] nRounds	Number of rounds
* @param [out] XX		Ciphertext
*
* @return
*/
void FEA_B_Enc_U64ToU64(const u64 *rk, const u64 *TW, u64 *XX, int nBlkBits, int nRounds)
{
	u64 T0, T1;
	u64 X0, X1;
	u64 Tw[2] = { TW[0], TW[1] };

	X0 = XX[0];
	X1 = XX[1];
    
    //rewrote the round encryptions to trade speed for flexiblity
    int i = 0;
    if (nRounds % 6 == 0) {
        for (i = 0; i < nRounds/6; ++i) {
            FEA_B_ENC_ROUND_0(6 * i + 0);
            FEA_B_ENC_ROUND_1(6 * i + 1);
            FEA_B_ENC_ROUND_2(6 * i + 2);
            FEA_B_ENC_ROUND_3(6 * i + 3);
            FEA_B_ENC_ROUND_4(6 * i + 4);
            FEA_B_ENC_ROUND_5(6 * i + 5);
        }
    }
    else {
        for (i = 0; i < nRounds/6; ++i) {
            FEA_B_ENC_ROUND_0(6 * i + 0);
            FEA_B_ENC_ROUND_1(6 * i + 1);
            FEA_B_ENC_ROUND_2(6 * i + 2);
            FEA_B_ENC_ROUND_3(6 * i + 3);
            FEA_B_ENC_ROUND_4(6 * i + 4);
            FEA_B_ENC_ROUND_5(6 * i + 5);
        }
        FEA_B_ENC_ROUND_0(6 * i + 0);
        FEA_B_ENC_ROUND_1(6 * i + 1);
        FEA_B_ENC_ROUND_2(6 * i + 2);
    }
    
    
    /*
    FEA_B_ENC_ROUND_0(0);   FEA_B_ENC_ROUND_1(1);   FEA_B_ENC_ROUND_2(2);
    FEA_B_ENC_ROUND_3(3);   FEA_B_ENC_ROUND_4(4);   FEA_B_ENC_ROUND_5(5);
	FEA_B_ENC_ROUND_0(6);	FEA_B_ENC_ROUND_1(7);	FEA_B_ENC_ROUND_2(8);
	FEA_B_ENC_ROUND_3(9);	FEA_B_ENC_ROUND_4(10);	FEA_B_ENC_ROUND_5(11);
	FEA_B_ENC_ROUND_0(12);	FEA_B_ENC_ROUND_1(13);	FEA_B_ENC_ROUND_2(14);
	FEA_B_ENC_ROUND_3(15);	FEA_B_ENC_ROUND_4(16);	FEA_B_ENC_ROUND_5(17);

	if (nRounds == 21) {
		FEA_B_ENC_ROUND_0(18);	FEA_B_ENC_ROUND_1(19);	FEA_B_ENC_ROUND_2(20);
	}
	else if (nRounds == 24) {
		FEA_B_ENC_ROUND_0(18);	FEA_B_ENC_ROUND_1(19);	FEA_B_ENC_ROUND_2(20);
		FEA_B_ENC_ROUND_3(21);	FEA_B_ENC_ROUND_4(22);	FEA_B_ENC_ROUND_5(23);
	}
    */
    

	XX[0] = X0 & masktab[nBlkBits][0];
	XX[1] = X1 & masktab[nBlkBits][1];
}
/**
* Decryption function for FEA-B
*
* @param [in] rk		Round keys
* @param [in] TW		Tweak
* @param [in] XX		Ciphertext
* @param [in] nBlkBits	Bit length
* @param [in] nRounds	Number of rounds
* @param [out] XX		Plaintext
*
* @return
*/
void FEA_B_Dec_U64ToU64(const u64 *rk, const u64 *TW, u64 *XX, int nBlkBits, int nRounds)
{
	u64 T0, T1;
	u64 X0, X1;
	u64 Tw[2] = { TW[0], TW[1] };

	X0 = XX[0];
	X1 = XX[1];

	if (nRounds == 24) {
		FEA_B_ENC_ROUND_5(23);	FEA_B_ENC_ROUND_4(22);	FEA_B_ENC_ROUND_3(21);
		FEA_B_ENC_ROUND_2(20);	FEA_B_ENC_ROUND_1(19);	FEA_B_ENC_ROUND_0(18);
	}
	else if (nRounds == 21) {
		FEA_B_ENC_ROUND_2(20);	FEA_B_ENC_ROUND_1(19);	FEA_B_ENC_ROUND_0(18);
	}
	FEA_B_ENC_ROUND_5(17);	FEA_B_ENC_ROUND_4(16);	FEA_B_ENC_ROUND_3(15);
	FEA_B_ENC_ROUND_2(14);	FEA_B_ENC_ROUND_1(13);	FEA_B_ENC_ROUND_0(12);
	FEA_B_ENC_ROUND_5(11);	FEA_B_ENC_ROUND_4(10);	FEA_B_ENC_ROUND_3(9);
	FEA_B_ENC_ROUND_2(8);	FEA_B_ENC_ROUND_1(7);	FEA_B_ENC_ROUND_0(6);
	FEA_B_ENC_ROUND_5(5);	FEA_B_ENC_ROUND_4(4);	FEA_B_ENC_ROUND_3(3);
	FEA_B_ENC_ROUND_2(2);	FEA_B_ENC_ROUND_1(1);	FEA_B_ENC_ROUND_0(0);

	XX[0] = X0 & masktab[nBlkBits][0];
	XX[1] = X1 & masktab[nBlkBits][1];

}


#endif
