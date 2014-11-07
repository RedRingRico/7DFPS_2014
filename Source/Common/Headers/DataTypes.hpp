#ifndef __FPS_DATATYPES_HPP__
#define __FPS_DATATYPES_HPP__

#include <cstdint>

typedef int32_t		FPS_SINT32;
typedef uint32_t	FPS_UINT32;

typedef FPS_UINT32	FPS_BOOL;

#define FPS_NULL nullptr

const FPS_UINT32 FPS_OK		= 0x00000000;
const FPS_UINT32 FPS_FAIL	= 0xFFFFFFFF;

const FPS_BOOL FPS_TRUE		= 1;
const FPS_BOOL FPS_FALSE	= 0;

#endif // __FPS_DATATYPES_HPP__

