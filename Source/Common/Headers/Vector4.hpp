#ifndef __FPS_VECTOR4_HPP__
#define __FPS_VECTOR4_HPP__

#include <DataTypes.hpp>
#include <string>

namespace FPS
{
	class Vector4
	{
	public:
		Vector4( );
		Vector4( const FPS_FLOAT32 p_X, const FPS_FLOAT32 p_Y,
			const FPS_FLOAT32 p_Z, const FPS_FLOAT32 p_W );
		~Vector4( );

		void Set( const FPS_FLOAT32 p_X, const FPS_FLOAT32 p_Y,
			const FPS_FLOAT32 p_Z, const FPS_FLOAT32 p_W );
		void SetX( const FPS_FLOAT32 p_X );
		void SetY( const FPS_FLOAT32 p_Y );
		void SetZ( const FPS_FLOAT32 p_Z );
		void SetW( const FPS_FLOAT32 p_W );

		FPS_FLOAT32 GetX( ) const;
		FPS_FLOAT32 GetY( ) const;
		FPS_FLOAT32 GetZ( ) const;
		FPS_FLOAT32 GetW( ) const;

		FPS_FLOAT32 GetMagnitude( ) const;
		FPS_FLOAT32 GetMagnitudeSq( ) const;

		FPS_FLOAT32 Distance( const Vector4 &p_Right ) const;
		FPS_FLOAT32 DistanceSq( const Vector4 &p_Right ) const;

		void Normalise( );

		void Zero( );
		void Clean( );

		// While the cross product is only valid for 3-element vectors, this
		// will only operate on the first 3 components
		Vector4 CrossProduct( const Vector4 &p_Right ) const;
		FPS_FLOAT32 DotProduct( const Vector4 &p_Right ) const;

		FPS_BOOL operator==( const Vector4 &p_Right ) const;
		FPS_BOOL operator!=( const Vector4 &p_Right ) const;

		Vector4 &operator-( );

		Vector4 operator+( const Vector4 &p_Right ) const;
		Vector4 operator-( const Vector4 &p_Right ) const;
		Vector4 operator*( const Vector4 &p_Right ) const;
		Vector4 operator*( const FPS_FLOAT32 p_Scalar ) const;

		Vector4 &operator+=( const Vector4 &p_Right );
		Vector4 &operator-=( const Vector4 &p_Right );
		Vector4 &operator*=( const Vector4 &p_Right );
		Vector4 &operator*=( const FPS_FLOAT32 p_Scalar );

		void Print( const std::string &p_Name ) const;
		
	private:
		FPS_FLOAT32 m_X, m_Y, m_Z, m_W;
	};

}

#endif // __FPS_VECTOR4_HPP__

