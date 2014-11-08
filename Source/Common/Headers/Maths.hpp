#ifndef __FPS_MATHS_HPP__
#define __FPS_MATHS_HPP__

#include <DataTypes.hpp>

namespace FPS
{
	const FPS_FLOAT32 FPS_EPSILON = 1.0e-10;
	const FPS_FLOAT32 FPS_HALF_EPSILON = 1.0e-5;

	FPS_BOOL IsZero( const FPS_FLOAT32 p_Value );
	FPS_BOOL IsEqual( const FPS_FLOAT32 p_Left, const FPS_FLOAT32 p_Right );

	FPS_FLOAT32 SquareRoot( const FPS_FLOAT32 p_Value );
	FPS_FLOAT32 InvSquareRoot( const FPS_FLOAT32 p_Value );

	void SineCosine( const FPS_FLOAT32 p_Angle, FPS_FLOAT32 &p_Sine,
		FPS_FLOAT32 &p_Cosine );
}

#endif // __FPS_MATHS_HPP__

