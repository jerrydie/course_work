#pragma once
#ifndef FEA_TRANSFORM_H
#define FEA_TRANSFORM_H

#include "FEA.h"
#include "FEA_Local.h"

/**
 * @file FEA_Transform.h
 * 
 */

#ifdef __cplusplus
extern "C" {
#endif

void Gen_NumFromArray(u64 *QR, const u32 *ar, const fpe_transform_ctx* table);
void Gen_TwoNumFromArray(u64 *QR, const u32 *ar, const fpe_transform_ctx * table);
void Gen_ArrayFromNum(u32 *ar, const u64 *QR, const fpe_transform_ctx * table);
void Gen_ArrayFromTwoNum(u32 *ar, const u64 *QR, const fpe_transform_ctx * table);

void NumFromArray(u64 *QR, const u8 *ar, const u32 obj_length);
void TwoNumFromArray(u64 *QR, const u8 *ar, const u32 obj_length);
void ArrayFromNum(u8 *ar, const u64 *QR, const u32 obj_length);
void ArrayFromTwoNum(u8 *ar, const u64 *QR, const u32 obj_length);

void BytesToU64LR(const u8 *bytes_in, u64 *bytes_out, const int block_bits, const int is_even_round);
void U64LRToBytes(const u64 *bytes_in, u8 *bytes_out, const int block_bits, const int is_even_round);
void SplitToU64LR(const u64 *bytes_in, u64 *bytes_out, const int block_bits, const int is_even_round);
void MergeU64LR(const u64 *bytes_in, u64 *bytes_out, const int block_bits, const int is_even_round);


u32 GetRadixMaxObjLen(const fpe_radix_table* table);
u32 GetRadixMinObjLen(const fpe_radix_table* table);
u32 GetRadixFromTable(const fpe_radix_table* table);

#ifdef __cplusplus
}
#endif

#endif