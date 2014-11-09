#include <Maths.hpp>
#include <cmath>

namespace FPS
{
	FPS_BOOL IsZero( const FPS_FLOAT32 p_Value )
	{
		return ( fabs( p_Value ) < FPS_EPSILON );
	}

	FPS_BOOL IsEqual( FPS_FLOAT32 p_Left, FPS_FLOAT32 p_Right )
	{
		return ( IsZero( p_Left - p_Right ) );
	}

	FPS_FLOAT32 SquareRoot( const FPS_FLOAT32 p_Value )
	{
		return sqrtf( p_Value );
	}

	FPS_FLOAT32 InvSquareRoot( const FPS_FLOAT32 p_Value )
	{
		return ( 1.0f / FPS::SquareRoot( p_Value ) );
	}

	FPS_FLOAT32 Absolute( const FPS_FLOAT32 p_Value )
	{
		return fabs( p_Value );
	}

	void SineCosine( const FPS_FLOAT32 p_Angle, FPS_FLOAT32 &p_Sine,
		FPS_FLOAT32 &p_Cosine )
	{
		p_Sine = sinf( p_Angle );
		p_Cosine = sinf( p_Angle );
	}
}

