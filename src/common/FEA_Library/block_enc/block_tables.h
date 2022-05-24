#pragma once
#ifndef BLOCK_ENC_BLOCK_TABLES_H
#define BLOCK_ENC_BLOCK_TABLES_H

#include "../FEA_Local.h"


// S-box
static const u8 S_box[256] = {
 0x62, 0x31, 0x70, 0x8e, 0xbc, 0x30, 0x9c, 0x78, 0xe0, 0x5c, 0xce, 0xbb, 0x42, 0xac, 0xb8, 0xdf,
 0x29, 0xe7, 0x86, 0x5f, 0xee, 0xba, 0x3f, 0x87, 0xc0, 0x36, 0xc3, 0x14, 0x7c, 0xec, 0x73, 0xda,
 0x57, 0x72, 0xf6, 0x77, 0x98, 0x3b, 0xc5, 0xc4, 0x4c, 0x52, 0x81, 0x20, 0x15, 0x97, 0x26, 0xfc,
 0x8b, 0x3c, 0xaf, 0x6e, 0xc8, 0x7e, 0xf0, 0x40, 0x24, 0xa1, 0xb1, 0x54, 0xff, 0xad, 0x51, 0xbd,
 0xc1, 0x13, 0x41, 0xb5, 0x6b, 0x94, 0x63, 0xd6, 0xde, 0x6f, 0x89, 0xd2, 0xa9, 0xd4, 0x17, 0x38,
 0xa5, 0xf2, 0xe3, 0xdb, 0x47, 0x66, 0xed, 0xcb, 0x4e, 0xd5, 0x05, 0x60, 0x8c, 0x06, 0x92, 0xa3,
 0xbe, 0x68, 0x56, 0xa7, 0x80, 0x32, 0xfa, 0x6c, 0x8f, 0x88, 0xd9, 0x50, 0x0a, 0x21, 0x3d, 0x75,
 0x71, 0x01, 0xe5, 0x7a, 0xc6, 0xb9, 0x82, 0x64, 0xd1, 0x00, 0x7d, 0x2b, 0xa0, 0x1a, 0x5e, 0xf5,
 0x35, 0x90, 0x2f, 0x2a, 0x83, 0x49, 0x5a, 0xa8, 0xd8, 0x8d, 0x46, 0x96, 0xdc, 0xb0, 0xc9, 0xdd,
 0xcd, 0x65, 0x44, 0xc7, 0x43, 0x67, 0x55, 0xeb, 0xe1, 0x9d, 0x34, 0x74, 0xb3, 0x4a, 0xca, 0xd7,
 0x79, 0xbf, 0xf7, 0x99, 0x6a, 0x2d, 0xef, 0x85, 0xe2, 0x5d, 0xfe, 0x11, 0x0f, 0x19, 0xcc, 0xe4,
 0x58, 0x09, 0x8a, 0x1b, 0x6d, 0x91, 0x9f, 0x4b, 0x61, 0x2c, 0x2e, 0xcf, 0x27, 0x10, 0x18, 0xb7,
 0x1d, 0x0c, 0x9b, 0x39, 0x7f, 0xd3, 0x84, 0xa4, 0xf9, 0x76, 0x33, 0xf4, 0xf3, 0xd0, 0x07, 0x0e,
 0x22, 0x1f, 0xfd, 0x25, 0x12, 0x08, 0x1e, 0x4d, 0xb6, 0xb4, 0x53, 0x37, 0xe8, 0xb2, 0x9e, 0x93,
 0x02, 0xe9, 0xf1, 0x3a, 0x0b, 0xfb, 0x45, 0x69, 0xea, 0xf8, 0xc2, 0x1c, 0x04, 0x59, 0x03, 0x48,
 0x16, 0xa2, 0x4f, 0x3e, 0x9a, 0x23, 0xaa, 0xae, 0x5b, 0xe6, 0x95, 0xab, 0x7b, 0x0d, 0x28, 0xa6
};


// Diffusion layer matrix M
static const u8 diff_mat[8][8] = {
 0x28, 0x1a, 0x7b, 0x78, 0xc3, 0xd0, 0x42, 0x40,
 0x1a, 0x7b, 0x78, 0xc3, 0xd0, 0x42, 0x40, 0x28,
 0x7b, 0x78, 0xc3, 0xd0, 0x42, 0x40, 0x28, 0x1a,
 0x78, 0xc3, 0xd0, 0x42, 0x40, 0x28, 0x1a, 0x7b,
 0xc3, 0xd0, 0x42, 0x40, 0x28, 0x1a, 0x7b, 0x78,
 0xd0, 0x42, 0x40, 0x28, 0x1a, 0x7b, 0x78, 0xc3,
 0x42, 0x40, 0x28, 0x1a, 0x7b, 0x78, 0xc3, 0xd0,
 0x40, 0x28, 0x1a, 0x7b, 0x78, 0xc3, 0xd0, 0x42
};


// Irreducible polynomial p
static const u8 poly_p = 0x71;



/**
 * @brief round constants rc[Type][KeyLength][Round]
 *
 * rc[Type][KeyLength][Round]
 *
 * Type			: 0 ==> FEA-A
 *
 *				  1 ==> FEA-B
 *
 * KeyLength	: 0 ==> 128 bit
 *
 *				  1 ==> 192 bit
 *
 *				  3 ==> 256 bit
 *
 * Round		: i ==> 2*i, 2*i+1 round
 */
static const u64 rc[2][3][12] = {
	U64C(0x71366FBD8EEF2E7D),U64C(0x9063FF208A85D13F),U64C(0xFDB54B3C9A86CB08),U64C(0xF2EA772BE55E4DE0),
	U64C(0x7C8814F95B9F8D0B),U64C(0xEB21FBFFCCBB8DF5),0,0,0,0,0,0,

	U64C(0xD2F928B5C6C08B51),U64C(0x4CBE190CDCC2962C),U64C(0xD0A2A85F772C8A07),U64C(0xE3FB1D49F5932802),
	U64C(0x047117EEE8007DFE),U64C(0x4390E40073A64C7D),U64C(0xEE9FAB45168DDADC),0,0,0,0,0,

	U64C(0x8F1C67DA8E609269),U64C(0x9B705F1835E0CDDC),U64C(0x6BF524A08A50A621),U64C(0x6B3C821900ADAB39),
	U64C(0x1F0EB84F4DE6881C),U64C(0x887FBA6319CBF504),U64C(0x51547790DD0B8145),U64C(0xAD7C1F118CA88090),
	0,0,0,0,


	U64C(0xC9E3B39803F2F6AF),U64C(0x40F343267298B62D),U64C(0x8A0D175B8BAAFA2B),U64C(0xE7B876206DEBAC98),
	U64C(0x559552FB4AFA1B10),U64C(0xED2EAE35C1382144),U64C(0x27573B291169B825),U64C(0x3E96CA16224AE8C5),
	U64C(0x1ACBDA11317C387E),0,0,0,

	U64C(0xA4198D55053B7CB5),U64C(0xBE1442D9B7E08DF0),U64C(0x3D97EEEA5149358C),U64C(0xAA9782D20CC69850),
	U64C(0x5071F733039A8ED5),U64C(0x625C15071EA7BCA1),U64C(0xCF37D8F11024C664),U64C(0x86D094E21E74D0A5),
	U64C(0x47DF6E91FC91754B),U64C(0x1F0B2F23B88200E7),U64C(0x29816E82B43E6464),0,

	U64C(0x93C7673007E5ED5E),U64C(0x81E6864CE5316C5B),U64C(0x141A2EB71755F457),U64C(0xCF70EC40DBD75930),
	U64C(0xAB2AA5F695F43621),U64C(0xDA5D5C6B82704288),U64C(0x4EAE765222D3704A),U64C(0x7D2D942C4495D18A),
	U64C(0x3597B42262F870FD),U64C(0x73D53787626CC076),U64C(0x4ADF41D8ECAFEE96),U64C(0xE59D0F633ACA9195)
};

/**
* @brief Tables for plaintexts and ciphertexts maskings
 *
 * masktab[bit_length][pos] - Maskings for plaintexts and ciphertexts
 */
static const u64 masktab[129][2] = {
	{ U64C(0x0000000000000000), U64C(0x0000000000000000) },{ U64C(0x8000000000000000), U64C(0x0000000000000000) },
	{ U64C(0x8000000000000000), U64C(0x8000000000000000) },{ U64C(0xc000000000000000), U64C(0x8000000000000000) },
	{ U64C(0xc000000000000000), U64C(0xc000000000000000) },{ U64C(0xe000000000000000), U64C(0xc000000000000000) },
	{ U64C(0xe000000000000000), U64C(0xe000000000000000) },{ U64C(0xf000000000000000), U64C(0xe000000000000000) },
	{ U64C(0xf000000000000000), U64C(0xf000000000000000) },{ U64C(0xf800000000000000), U64C(0xf000000000000000) },
	{ U64C(0xf800000000000000), U64C(0xf800000000000000) },{ U64C(0xfc00000000000000), U64C(0xf800000000000000) },
	{ U64C(0xfc00000000000000), U64C(0xfc00000000000000) },{ U64C(0xfe00000000000000), U64C(0xfc00000000000000) },
	{ U64C(0xfe00000000000000), U64C(0xfe00000000000000) },{ U64C(0xff00000000000000), U64C(0xfe00000000000000) },
	{ U64C(0xff00000000000000), U64C(0xff00000000000000) },{ U64C(0xff80000000000000), U64C(0xff00000000000000) },
	{ U64C(0xff80000000000000), U64C(0xff80000000000000) },{ U64C(0xffc0000000000000), U64C(0xff80000000000000) },
	{ U64C(0xffc0000000000000), U64C(0xffc0000000000000) },{ U64C(0xffe0000000000000), U64C(0xffc0000000000000) },
	{ U64C(0xffe0000000000000), U64C(0xffe0000000000000) },{ U64C(0xfff0000000000000), U64C(0xffe0000000000000) },
	{ U64C(0xfff0000000000000), U64C(0xfff0000000000000) },{ U64C(0xfff8000000000000), U64C(0xfff0000000000000) },
	{ U64C(0xfff8000000000000), U64C(0xfff8000000000000) },{ U64C(0xfffc000000000000), U64C(0xfff8000000000000) },
	{ U64C(0xfffc000000000000), U64C(0xfffc000000000000) },{ U64C(0xfffe000000000000), U64C(0xfffc000000000000) },
	{ U64C(0xfffe000000000000), U64C(0xfffe000000000000) },{ U64C(0xffff000000000000), U64C(0xfffe000000000000) },
	{ U64C(0xffff000000000000), U64C(0xffff000000000000) },{ U64C(0xffff800000000000), U64C(0xffff000000000000) },
	{ U64C(0xffff800000000000), U64C(0xffff800000000000) },{ U64C(0xffffc00000000000), U64C(0xffff800000000000) },
	{ U64C(0xffffc00000000000), U64C(0xffffc00000000000) },{ U64C(0xffffe00000000000), U64C(0xffffc00000000000) },
	{ U64C(0xffffe00000000000), U64C(0xffffe00000000000) },{ U64C(0xfffff00000000000), U64C(0xffffe00000000000) },
	{ U64C(0xfffff00000000000), U64C(0xfffff00000000000) },{ U64C(0xfffff80000000000), U64C(0xfffff00000000000) },
	{ U64C(0xfffff80000000000), U64C(0xfffff80000000000) },{ U64C(0xfffffc0000000000), U64C(0xfffff80000000000) },
	{ U64C(0xfffffc0000000000), U64C(0xfffffc0000000000) },{ U64C(0xfffffe0000000000), U64C(0xfffffc0000000000) },
	{ U64C(0xfffffe0000000000), U64C(0xfffffe0000000000) },{ U64C(0xffffff0000000000), U64C(0xfffffe0000000000) },
	{ U64C(0xffffff0000000000), U64C(0xffffff0000000000) },{ U64C(0xffffff8000000000), U64C(0xffffff0000000000) },
	{ U64C(0xffffff8000000000), U64C(0xffffff8000000000) },{ U64C(0xffffffc000000000), U64C(0xffffff8000000000) },
	{ U64C(0xffffffc000000000), U64C(0xffffffc000000000) },{ U64C(0xffffffe000000000), U64C(0xffffffc000000000) },
	{ U64C(0xffffffe000000000), U64C(0xffffffe000000000) },{ U64C(0xfffffff000000000), U64C(0xffffffe000000000) },
	{ U64C(0xfffffff000000000), U64C(0xfffffff000000000) },{ U64C(0xfffffff800000000), U64C(0xfffffff000000000) },
	{ U64C(0xfffffff800000000), U64C(0xfffffff800000000) },{ U64C(0xfffffffc00000000), U64C(0xfffffff800000000) },
	{ U64C(0xfffffffc00000000), U64C(0xfffffffc00000000) },{ U64C(0xfffffffe00000000), U64C(0xfffffffc00000000) },
	{ U64C(0xfffffffe00000000), U64C(0xfffffffe00000000) },{ U64C(0xffffffff00000000), U64C(0xfffffffe00000000) },
	{ U64C(0xffffffff00000000), U64C(0xffffffff00000000) },{ U64C(0xffffffff80000000), U64C(0xffffffff00000000) },
	{ U64C(0xffffffff80000000), U64C(0xffffffff80000000) },{ U64C(0xffffffffc0000000), U64C(0xffffffff80000000) },
	{ U64C(0xffffffffc0000000), U64C(0xffffffffc0000000) },{ U64C(0xffffffffe0000000), U64C(0xffffffffc0000000) },
	{ U64C(0xffffffffe0000000), U64C(0xffffffffe0000000) },{ U64C(0xfffffffff0000000), U64C(0xffffffffe0000000) },
	{ U64C(0xfffffffff0000000), U64C(0xfffffffff0000000) },{ U64C(0xfffffffff8000000), U64C(0xfffffffff0000000) },
	{ U64C(0xfffffffff8000000), U64C(0xfffffffff8000000) },{ U64C(0xfffffffffc000000), U64C(0xfffffffff8000000) },
	{ U64C(0xfffffffffc000000), U64C(0xfffffffffc000000) },{ U64C(0xfffffffffe000000), U64C(0xfffffffffc000000) },
	{ U64C(0xfffffffffe000000), U64C(0xfffffffffe000000) },{ U64C(0xffffffffff000000), U64C(0xfffffffffe000000) },
	{ U64C(0xffffffffff000000), U64C(0xffffffffff000000) },{ U64C(0xffffffffff800000), U64C(0xffffffffff000000) },
	{ U64C(0xffffffffff800000), U64C(0xffffffffff800000) },{ U64C(0xffffffffffc00000), U64C(0xffffffffff800000) },
	{ U64C(0xffffffffffc00000), U64C(0xffffffffffc00000) },{ U64C(0xffffffffffe00000), U64C(0xffffffffffc00000) },
	{ U64C(0xffffffffffe00000), U64C(0xffffffffffe00000) },{ U64C(0xfffffffffff00000), U64C(0xffffffffffe00000) },
	{ U64C(0xfffffffffff00000), U64C(0xfffffffffff00000) },{ U64C(0xfffffffffff80000), U64C(0xfffffffffff00000) },
	{ U64C(0xfffffffffff80000), U64C(0xfffffffffff80000) },{ U64C(0xfffffffffffc0000), U64C(0xfffffffffff80000) },
	{ U64C(0xfffffffffffc0000), U64C(0xfffffffffffc0000) },{ U64C(0xfffffffffffe0000), U64C(0xfffffffffffc0000) },
	{ U64C(0xfffffffffffe0000), U64C(0xfffffffffffe0000) },{ U64C(0xffffffffffff0000), U64C(0xfffffffffffe0000) },
	{ U64C(0xffffffffffff0000), U64C(0xffffffffffff0000) },{ U64C(0xffffffffffff8000), U64C(0xffffffffffff0000) },
	{ U64C(0xffffffffffff8000), U64C(0xffffffffffff8000) },{ U64C(0xffffffffffffc000), U64C(0xffffffffffff8000) },
	{ U64C(0xffffffffffffc000), U64C(0xffffffffffffc000) },{ U64C(0xffffffffffffe000), U64C(0xffffffffffffc000) },
	{ U64C(0xffffffffffffe000), U64C(0xffffffffffffe000) },{ U64C(0xfffffffffffff000), U64C(0xffffffffffffe000) },
	{ U64C(0xfffffffffffff000), U64C(0xfffffffffffff000) },{ U64C(0xfffffffffffff800), U64C(0xfffffffffffff000) },
	{ U64C(0xfffffffffffff800), U64C(0xfffffffffffff800) },{ U64C(0xfffffffffffffc00), U64C(0xfffffffffffff800) },
	{ U64C(0xfffffffffffffc00), U64C(0xfffffffffffffc00) },{ U64C(0xfffffffffffffe00), U64C(0xfffffffffffffc00) },
	{ U64C(0xfffffffffffffe00), U64C(0xfffffffffffffe00) },{ U64C(0xffffffffffffff00), U64C(0xfffffffffffffe00) },
	{ U64C(0xffffffffffffff00), U64C(0xffffffffffffff00) },{ U64C(0xffffffffffffff80), U64C(0xffffffffffffff00) },
	{ U64C(0xffffffffffffff80), U64C(0xffffffffffffff80) },{ U64C(0xffffffffffffffc0), U64C(0xffffffffffffff80) },
	{ U64C(0xffffffffffffffc0), U64C(0xffffffffffffffc0) },{ U64C(0xffffffffffffffe0), U64C(0xffffffffffffffc0) },
	{ U64C(0xffffffffffffffe0), U64C(0xffffffffffffffe0) },{ U64C(0xfffffffffffffff0), U64C(0xffffffffffffffe0) },
	{ U64C(0xfffffffffffffff0), U64C(0xfffffffffffffff0) },{ U64C(0xfffffffffffffff8), U64C(0xfffffffffffffff0) },
	{ U64C(0xfffffffffffffff8), U64C(0xfffffffffffffff8) },{ U64C(0xfffffffffffffffc), U64C(0xfffffffffffffff8) },
	{ U64C(0xfffffffffffffffc), U64C(0xfffffffffffffffc) },{ U64C(0xfffffffffffffffe), U64C(0xfffffffffffffffc) },
	{ U64C(0xfffffffffffffffe), U64C(0xfffffffffffffffe) },{ U64C(0xffffffffffffffff), U64C(0xfffffffffffffffe) },
	{ U64C(0xffffffffffffffff), U64C(0xffffffffffffffff) }
};

/**
* @brief Table for tweak maskings
*
* masktabtw[bit_length][pos] - Maskings for tweaks of ALG_ID_FEA_A
*/
static const u64 masktabtw[129][2] = {
	{ U64C(0x0000000000000000), U64C(0x0000000000000000) },{ U64C(0x0000000000000000), U64C(0x0000000000000001) },
	{ U64C(0x0000000000000001), U64C(0x0000000000000001) },{ U64C(0x0000000000000001), U64C(0x0000000000000003) },
	{ U64C(0x0000000000000003), U64C(0x0000000000000003) },{ U64C(0x0000000000000003), U64C(0x0000000000000007) },
	{ U64C(0x0000000000000007), U64C(0x0000000000000007) },{ U64C(0x0000000000000007), U64C(0x000000000000000f) },
	{ U64C(0x000000000000000f), U64C(0x000000000000000f) },{ U64C(0x000000000000000f), U64C(0x000000000000001f) },
	{ U64C(0x000000000000001f), U64C(0x000000000000001f) },{ U64C(0x000000000000001f), U64C(0x000000000000003f) },
	{ U64C(0x000000000000003f), U64C(0x000000000000003f) },{ U64C(0x000000000000003f), U64C(0x000000000000007f) },
	{ U64C(0x000000000000007f), U64C(0x000000000000007f) },{ U64C(0x000000000000007f), U64C(0x00000000000000ff) },
	{ U64C(0x00000000000000ff), U64C(0x00000000000000ff) },{ U64C(0x00000000000000ff), U64C(0x00000000000001ff) },
	{ U64C(0x00000000000001ff), U64C(0x00000000000001ff) },{ U64C(0x00000000000001ff), U64C(0x00000000000003ff) },
	{ U64C(0x00000000000003ff), U64C(0x00000000000003ff) },{ U64C(0x00000000000003ff), U64C(0x00000000000007ff) },
	{ U64C(0x00000000000007ff), U64C(0x00000000000007ff) },{ U64C(0x00000000000007ff), U64C(0x0000000000000fff) },
	{ U64C(0x0000000000000fff), U64C(0x0000000000000fff) },{ U64C(0x0000000000000fff), U64C(0x0000000000001fff) },
	{ U64C(0x0000000000001fff), U64C(0x0000000000001fff) },{ U64C(0x0000000000001fff), U64C(0x0000000000003fff) },
	{ U64C(0x0000000000003fff), U64C(0x0000000000003fff) },{ U64C(0x0000000000003fff), U64C(0x0000000000007fff) },
	{ U64C(0x0000000000007fff), U64C(0x0000000000007fff) },{ U64C(0x0000000000007fff), U64C(0x000000000000ffff) },
	{ U64C(0x000000000000ffff), U64C(0x000000000000ffff) },{ U64C(0x000000000000ffff), U64C(0x000000000001ffff) },
	{ U64C(0x000000000001ffff), U64C(0x000000000001ffff) },{ U64C(0x000000000001ffff), U64C(0x000000000003ffff) },
	{ U64C(0x000000000003ffff), U64C(0x000000000003ffff) },{ U64C(0x000000000003ffff), U64C(0x000000000007ffff) },
	{ U64C(0x000000000007ffff), U64C(0x000000000007ffff) },{ U64C(0x000000000007ffff), U64C(0x00000000000fffff) },
	{ U64C(0x00000000000fffff), U64C(0x00000000000fffff) },{ U64C(0x00000000000fffff), U64C(0x00000000001fffff) },
	{ U64C(0x00000000001fffff), U64C(0x00000000001fffff) },{ U64C(0x00000000001fffff), U64C(0x00000000003fffff) },
	{ U64C(0x00000000003fffff), U64C(0x00000000003fffff) },{ U64C(0x00000000003fffff), U64C(0x00000000007fffff) },
	{ U64C(0x00000000007fffff), U64C(0x00000000007fffff) },{ U64C(0x00000000007fffff), U64C(0x0000000000ffffff) },
	{ U64C(0x0000000000ffffff), U64C(0x0000000000ffffff) },{ U64C(0x0000000000ffffff), U64C(0x0000000001ffffff) },
	{ U64C(0x0000000001ffffff), U64C(0x0000000001ffffff) },{ U64C(0x0000000001ffffff), U64C(0x0000000003ffffff) },
	{ U64C(0x0000000003ffffff), U64C(0x0000000003ffffff) },{ U64C(0x0000000003ffffff), U64C(0x0000000007ffffff) },
	{ U64C(0x0000000007ffffff), U64C(0x0000000007ffffff) },{ U64C(0x0000000007ffffff), U64C(0x000000000fffffff) },
	{ U64C(0x000000000fffffff), U64C(0x000000000fffffff) },{ U64C(0x000000000fffffff), U64C(0x000000001fffffff) },
	{ U64C(0x000000001fffffff), U64C(0x000000001fffffff) },{ U64C(0x000000001fffffff), U64C(0x000000003fffffff) },
	{ U64C(0x000000003fffffff), U64C(0x000000003fffffff) },{ U64C(0x000000003fffffff), U64C(0x000000007fffffff) },
	{ U64C(0x000000007fffffff), U64C(0x000000007fffffff) },{ U64C(0x000000007fffffff), U64C(0x00000000ffffffff) },
	{ U64C(0x00000000ffffffff), U64C(0x00000000ffffffff) },{ U64C(0x00000000ffffffff), U64C(0x00000001ffffffff) },
	{ U64C(0x00000001ffffffff), U64C(0x00000001ffffffff) },{ U64C(0x00000001ffffffff), U64C(0x00000003ffffffff) },
	{ U64C(0x00000003ffffffff), U64C(0x00000003ffffffff) },{ U64C(0x00000003ffffffff), U64C(0x00000007ffffffff) },
	{ U64C(0x00000007ffffffff), U64C(0x00000007ffffffff) },{ U64C(0x00000007ffffffff), U64C(0x0000000fffffffff) },
	{ U64C(0x0000000fffffffff), U64C(0x0000000fffffffff) },{ U64C(0x0000000fffffffff), U64C(0x0000001fffffffff) },
	{ U64C(0x0000001fffffffff), U64C(0x0000001fffffffff) },{ U64C(0x0000001fffffffff), U64C(0x0000003fffffffff) },
	{ U64C(0x0000003fffffffff), U64C(0x0000003fffffffff) },{ U64C(0x0000003fffffffff), U64C(0x0000007fffffffff) },
	{ U64C(0x0000007fffffffff), U64C(0x0000007fffffffff) },{ U64C(0x0000007fffffffff), U64C(0x000000ffffffffff) },
	{ U64C(0x000000ffffffffff), U64C(0x000000ffffffffff) },{ U64C(0x000000ffffffffff), U64C(0x000001ffffffffff) },
	{ U64C(0x000001ffffffffff), U64C(0x000001ffffffffff) },{ U64C(0x000001ffffffffff), U64C(0x000003ffffffffff) },
	{ U64C(0x000003ffffffffff), U64C(0x000003ffffffffff) },{ U64C(0x000003ffffffffff), U64C(0x000007ffffffffff) },
	{ U64C(0x000007ffffffffff), U64C(0x000007ffffffffff) },{ U64C(0x000007ffffffffff), U64C(0x00000fffffffffff) },
	{ U64C(0x00000fffffffffff), U64C(0x00000fffffffffff) },{ U64C(0x00000fffffffffff), U64C(0x00001fffffffffff) },
	{ U64C(0x00001fffffffffff), U64C(0x00001fffffffffff) },{ U64C(0x00001fffffffffff), U64C(0x00003fffffffffff) },
	{ U64C(0x00003fffffffffff), U64C(0x00003fffffffffff) },{ U64C(0x00003fffffffffff), U64C(0x00007fffffffffff) },
	{ U64C(0x00007fffffffffff), U64C(0x00007fffffffffff) },{ U64C(0x00007fffffffffff), U64C(0x0000ffffffffffff) },
	{ U64C(0x0000ffffffffffff), U64C(0x0000ffffffffffff) },{ U64C(0x0000ffffffffffff), U64C(0x0001ffffffffffff) },
	{ U64C(0x0001ffffffffffff), U64C(0x0001ffffffffffff) },{ U64C(0x0001ffffffffffff), U64C(0x0003ffffffffffff) },
	{ U64C(0x0003ffffffffffff), U64C(0x0003ffffffffffff) },{ U64C(0x0003ffffffffffff), U64C(0x0007ffffffffffff) },
	{ U64C(0x0007ffffffffffff), U64C(0x0007ffffffffffff) },{ U64C(0x0007ffffffffffff), U64C(0x000fffffffffffff) },
	{ U64C(0x000fffffffffffff), U64C(0x000fffffffffffff) },{ U64C(0x000fffffffffffff), U64C(0x001fffffffffffff) },
	{ U64C(0x001fffffffffffff), U64C(0x001fffffffffffff) },{ U64C(0x001fffffffffffff), U64C(0x003fffffffffffff) },
	{ U64C(0x003fffffffffffff), U64C(0x003fffffffffffff) },{ U64C(0x003fffffffffffff), U64C(0x007fffffffffffff) },
	{ U64C(0x007fffffffffffff), U64C(0x007fffffffffffff) },{ U64C(0x007fffffffffffff), U64C(0x00ffffffffffffff) },
	{ U64C(0x00ffffffffffffff), U64C(0x00ffffffffffffff) },{ U64C(0x00ffffffffffffff), U64C(0x01ffffffffffffff) },
	{ U64C(0x01ffffffffffffff), U64C(0x01ffffffffffffff) },{ U64C(0x01ffffffffffffff), U64C(0x03ffffffffffffff) },
	{ U64C(0x03ffffffffffffff), U64C(0x03ffffffffffffff) },{ U64C(0x03ffffffffffffff), U64C(0x07ffffffffffffff) },
	{ U64C(0x07ffffffffffffff), U64C(0x07ffffffffffffff) },{ U64C(0x07ffffffffffffff), U64C(0x0fffffffffffffff) },
	{ U64C(0x0fffffffffffffff), U64C(0x0fffffffffffffff) },{ U64C(0x0fffffffffffffff), U64C(0x1fffffffffffffff) },
	{ U64C(0x1fffffffffffffff), U64C(0x1fffffffffffffff) },{ U64C(0x1fffffffffffffff), U64C(0x3fffffffffffffff) },
	{ U64C(0x3fffffffffffffff), U64C(0x3fffffffffffffff) },{ U64C(0x3fffffffffffffff), U64C(0x7fffffffffffffff) },
	{ U64C(0x7fffffffffffffff), U64C(0x7fffffffffffffff) },{ U64C(0x7fffffffffffffff), U64C(0xffffffffffffffff) },
	{ U64C(0xffffffffffffffff), U64C(0xffffffffffffffff) }
};

#endif