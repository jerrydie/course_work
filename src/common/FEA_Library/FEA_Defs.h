#pragma once
#ifndef FEA_DEFS_H 
#define FEA_DEFS_H

#include <stddef.h>
#include <limits.h>
#include "FEA_Build_Config.h"

/**
 * @file FEA_Defs.h
 */

#if !defined(_STDINT) && !defined(_GCC_STDINT_H) && !defined(__CLANG_STDINT_H) && !defined(UINT_FAST32_MAX)
#if __STDC_VERSION__ >= 199901L
//GCC, Clang and most C99 compatible compiler.
#include <stdint.h>
#include <inttypes.h>
#elif defined(_MSC_VER) && (_MSC_VER >= 1600)
//MSVC++ 2010+
#include <stdint.h>

#if _MSC_VER > 1600
#include <inttypes.h>
#else
#define PRIx64 "I64x"
#define PRId64 "I64d"
#define PRIu64 "I64u"
#endif
#elif defined(_MSC_VER)
//until MSVC++ 2008. Lightweight stdint declaration.
typedef __int8  int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;

typedef unsigned __int8  uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

#define INT8_MAX   _I8_MAX
#define INT16_MAX  _I16_MAX
#define INT32_MAX  _I32_MAX
#define INT64_MAX  _I64_MAX

#define UINT8_MAX  _UI8_MAX
#define UINT16_MAX _UI16_MAX
#define UINT32_MAX _UI32_MAX
#define UINT64_MAX _UI64_MAX

#define INT32_C(val) (val ## i32)
#define INT64_C(val) (val ## i64)
#define UINT32_C(val) (val ## ui32)
#define UINT64_C(val) (val ## ui64)

#ifndef PRIx64
#define PRIx64 "I64x"
#endif

#ifndef PRId64
#define PRId64 "I64d"
#endif

#ifndef PRIu64
#define PRIu64 "I64u"
#endif

#else
//Unexpected case. long long and 'ull' may not be supported.

#ifndef __int8_t_defined
typedef char               int8_t;	
typedef unsigned char      uint8_t;	
#endif
#ifndef __int16_t_defined
typedef short              int16_t;	
typedef unsigned short     uint16_t;
#endif

#define INT8_MAX   0x7f				
#define INT16_MAX  0x7fff			
#define UINT8_MAX  0xff			
#define UINT16_MAX 0xffff			

#if UINT_MAX > UINT16_MAX
#ifndef __int32_t_defined
typedef int                int32_t;		
typedef unsigned int       uint32_t;
#endif
#define INT32_C(val)  (val)			
#define UINT32_C(val) (val ## u)	

#elif ULONG_MAX > UINT_MAX
#ifndef __int32_t_defined
typedef long			   int32_t;
typedef unsigned long      uint32_t;
#endif
#define INT32_C(val)  (val ## l)
#define UINT32_C(val) (val ## ul)

#else
#error "Unexpected uint32_t type."
#endif

#define INT32_MAX  INT32_C(0x7fffffff)	
#define UINT32_MAX UINT32_C(0xffffffff) 

#if ULONG_MAX > UINT32_MAX
#ifndef __int64_t_defined
typedef long               int64_t;
typedef unsigned long      uint64_t;
#endif
#define INT64_C(val)  (val ## l)
#define UINT64_C(val) (val ## ul)

#ifndef PRIx64
#define PRIx64 "lx"
#endif

#ifndef PRId64
#define PRId64 "ld"
#endif

#ifndef PRIu64
#define PRIu64 "lu"
#endif

#elif defined(ULLONG_MAX) || defined(LONG_LONG_MAX)
#ifndef __int64_t_defined
typedef long long          int64_t;
typedef unsigned long long uint64_t;
#endif
#define INT64_C(val)  (val ## ll)
#define UINT64_C(val) (val ## ull)

#ifndef PRIx64
#define PRIx64 "llx"
#endif

#ifndef PRId64
#define PRId64 "lld"
#endif

#ifndef PRIu64
#define PRIu64 "llu"
#endif


#else
#error "Unexpected uint64_t type."
#endif

#define INT64_MAX  INT64_C(0x7fffffffffffffff) 
#define UINT64_MAX UINT64_C(0xffffffffffffffff) 

#endif
#endif


/*********************************************************************
* FEA Types
*/
#define FEA_ALG_INVALID			UINT32_C(0)
#define FEA_ALG_ID_FEA_A		UINT32_C(0x00040100)
#define FEA_ALG_ID_FEA_B		UINT32_C(0x00040200)
#define FEA_ALG_ID_NAIVE		UINT32_C(0x00030100)

typedef uint32_t fpe_algtype_t;

#define FEA_KEY_SIZE_128	128
#define FEA_KEY_SIZE_192	192
#define FEA_KEY_SIZE_256	256

#define FEA_DATA_BITS		0x0002
#define FEA_DATA_DECIMAL	0x000a
#define FEA_DATA_RADIX		0x0000

typedef uint32_t fpe_objtype_t;

#define FEA_MIN_RADIX		2
#define FEA_MAX_RADIX		65535

/**********************************************************************
* FEA Error Type
*/
#define FEA_SUCCESS					0
#define FEA_NULL_PTR				-1000
#define FEA_INVALID_ARGUMENT		-1001
#define FEA_INVALID_TYPE			-1002
#define FEA_INVALID_KEY_SIZE		-1003
#define FEA_INVALID_OBJ_LENGTH		-1004
#define FEA_INVALID_RADIX			-1006
#define FEA_INVALID_STATE			-1007

typedef int fpe_err_t;





#endif