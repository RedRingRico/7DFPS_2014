#ifndef __FPS_DATATYPES_HPP__
#define __FPS_DATATYPES_HPP__

#include <cstdint>
#include <cstddef>

typedef unsigned char	FPS_BYTE;
typedef unsigned char	FPS_UCHAR;
typedef signed char		FPS_SCHAR;
typedef int8_t			FPS_SINT8;
typedef uint8_t			FPS_UINT8;
typedef int16_t			FPS_SINT16;
typedef uint16_t		FPS_UINT16;
typedef int32_t			FPS_SINT32;
typedef uint32_t		FPS_UINT32;
typedef int64_t			FPS_SINT64;
typedef uint64_t		FPS_UINT64;

typedef FPS_UINT32	FPS_BOOL;
typedef size_t		FPS_MEMSIZE;
typedef float		FPS_FLOAT32;
typedef double		FPS_FLOAT64;

#define FPS_NULL nullptr

#define FPS_EXPLICIT explicit

const FPS_UINT32 FPS_OK		= 0x00000000;
const FPS_UINT32 FPS_FAIL	= 0xFFFFFFFF;

const FPS_BOOL FPS_TRUE		= 1;
const FPS_BOOL FPS_FALSE	= 0;

#endif // __FPS_DATATYPES_HPP__

