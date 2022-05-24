#pragma once
#ifndef FEA_LOCAL_H
#define FEA_LOCAL_H

/**
 * @file FEA_Local.h
 * @brief FEA 내부 함수 헤더
 *
 * 
 */

#include "FEA_Defs.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define U32C(val) UINT32_C(val)
#define U64C(val) UINT64_C(val)


#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#ifndef __has_attribute
#define __has_attribute(x) 0
#endif

#ifndef __has_declspec_attribute
#define __has_declspec_attribute(x) 0
#endif

/* Inlining Macro */
#ifdef INLINE
/* do nothing */
#elif defined(_MSC_VER)
#define INLINE __inline
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#define INLINE inline
#elif defined(__cplusplus)
#define INLINE inline
#else
#define INLINE 
#endif

#if defined(FEA_LITTLE_ENDIAN) && defined(FEA_BIG_ENDIAN)
#undef FEA_LITTLE_ENDIAN
#undef FEA_BIG_ENDIAN
#endif

/* Check Endian */
#if defined(FEA_LITTLE_ENDIAN) || defined(FEA_BIG_ENDIAN)
/* do Nothing */
#elif defined(__i386) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_X64)
#	define FEA_LITTLE_ENDIAN
/* Intel Architecture */
#elif defined(_MSC_VER)
#	define FEA_LITTLE_ENDIAN
/* All available "Windows" are Little-Endian except XBOX360. */
#elif defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && defined(__ORDER_LITTLE_ENDIAN__)
/* GCC style */
/* use __BYTE_ORDER__ */
#	if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#		define FEA_LITTLE_ENDIAN
#	else
#		define FEA_BIG_ENDIAN
#	endif
#elif defined(__BIG_ENDIAN__) || defined(__LITTLE_ENDIAN__)
/* use __BIG_ENDIAN__ and __LITTLE_ENDIAN__ */
#	if defined(__LITTLE_ENDIAN__)
#		if __LITTLE_ENDIAN__
#			define FEA_LITTLE_ENDIAN
#		else
#			define FEA_BIG_ENDIAN
#		endif
#	elif defined(__BIG_ENDIAN__)
#		if __BIG_ENDIAN__
#			define FEA_BIG_ENDIAN
#		else
#			define FEA_LITTLE_ENDIAN
#		endif
#	endif
#else
/* use <endian.h> */
#	ifdef BSD
#		include <sys/endian.h>
#	else
#		include <endian.h>
#	endif
#	if __BYTE_ORDER__ == __LITTLE_ENDIAN
#		define FEA_LITTLE_ENDIAN
#	else
#		define FEA_BIG_ENDIAN
#	endif
#endif

#if __has_builtin(__builtin_bswap32) && __has_builtin(__builtin_bswap64) 
#define bswap32(v)	__builtin_bswap32(v)
#define bswap64(v)	__builtin_bswap64(v)

#elif defined(__GNUC__) && ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3))

#define bswap32(v)	__builtin_bswap32(v)
#define bswap64(v)	__builtin_bswap64(v)

#elif defined(_MSC_VER)
#include <stdlib.h>
#define bswap32(v)  _byteswap_ulong(v)  ///< Compiler 내장 32비트 byteswap 명령
#define bswap64(v)  _byteswap_uint64(v) ///< Compiler 내장 64비트 byteswap 명령

#else

/* If you want, you can use htole32 and htole64 instead of below. */
static INLINE u32 bswap32(u32 v) {
	u8* p = (u8*)&v;
	return p[3] << 24 | p[2] << 16 | p[1] << 8 | p[0];
}
static INLINE u64 bswap64(u64 v) {
	u32* p = (u32*)&v;
	return (u64)bswap32(p[0]) << 32 | bswap32(p[1]);
}
#endif

#if defined(load32BE) && defined(load64BE) && defined(load32LE) && defined(load64LE)
/* do Nothing */
#elif defined(FEA_BIG_ENDIAN)
//	big endian
#define load32BE(v)	((u32)(v))
#define load64BE(v)	((u64)(v))
#define load32LE(v) bswap32((u32)(v))
#define load64LE(v) bswap64((u64)(v))

#else
//	little endian
#define load32BE(v)	bswap32((u32)(v))
#define load64BE(v)	bswap64((u64)(v))
#define load32LE(v) ((u32)(v))
#define load64LE(v) ((u64)(v))
#endif

// BigEndian Based
#define load32(v) load32BE(v)
#define load64(v) load64BE(v)

#endif
