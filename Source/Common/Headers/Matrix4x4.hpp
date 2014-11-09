#ifndef __FPS_MATRIX4X4_HPP__
#define __FPS_MATRIX4X4_HPP__

#include <DataTypes.hpp>

namespace FPS
{
	class Vector4;

	class Matrix4x4
	{
	public:
		Matrix4x4( );
		~Matrix4x4( );

		void Identity( );

		Matrix4x4 &Rotate( const FPS_FLOAT32 p_Angle, const Vector4 &p_Axis );
		Matrix4x4 &Rotate( const FPS_FLOAT32 p_Roll, const FPS_FLOAT32 p_Pitch,
			const FPS_FLOAT32 p_Yaw );

		Matrix4x4 &RotateX( const FPS_FLOAT32 p_Angle );
		Matrix4x4 &RotateY( const FPS_FLOAT32 p_Angle );
		Matrix4x4 &RotateZ( const FPS_FLOAT32 p_Angle );

		Matrix4x4 &Scale( const FPS_FLOAT32 p_Scale );
		Matrix4x4 &Scale( const Vector4 &p_Scale );

		Matrix4x4 &ScaleX( const FPS_FLOAT32 p_Scale );
		Matrix4x4 &ScaleY( const FPS_FLOAT32 p_Scale );
		Matrix4x4 &ScaleZ( const FPS_FLOAT32 p_Scale );

		void SetRows( const Vector4 &p_Row1, const Vector4 &p_Row2,
			const Vector4 &p_Row3, const Vector4 &p_Row4 );
		void SetColumns( const Vector4 &p_Column1, const Vector4 &p_Column2,
			const Vector4 &p_Column3, const Vector4 &p_Colum4 );

		void Clean( );

		Matrix4x4 &Transpose( );
		Matrix4x4 &TransposeOf( const Matrix4x4 &p_Matrix );
		void Transpose( Matrix4x4 &p_Matrix ) const;

		Matrix4x4 &AffineInverse( );
		Matrix4x4 &AffineInverseOf( const Matrix4x4 &p_Matrix );
		void AffineInverse( Matrix4x4 &p_Matrix ) const;

		Matrix4x4 &Translate( const Vector4 &p_Tranlation );
		Vector4 Transform( const Vector4 &p_Point ) const;

		FPS_FLOAT32 Trace( ) const;

		FPS_BOOL operator==( const Matrix4x4 &p_Right ) const;
		FPS_BOOL operator!=( const Matrix4x4 &p_Right ) const;

		Matrix4x4 operator+( const Matrix4x4 &p_Right ) const;
		Matrix4x4 operator-( const Matrix4x4 &p_Right ) const;
		Matrix4x4 operator*( const Matrix4x4 &p_Right ) const;
		Matrix4x4 operator*( const FPS_FLOAT32 p_Scalar ) const;

		Matrix4x4 &operator-( );

		Vector4 operator*( const Vector4 &p_Vector ) const;

		Matrix4x4 &operator+=( const Matrix4x4 &p_Right );
		Matrix4x4 &operator-=( const Matrix4x4 &p_Right );
		Matrix4x4 &operator*=( const Matrix4x4 &p_Right );
		Matrix4x4 &operator*=( const FPS_FLOAT32 p_Scalar );

		FPS_FLOAT32 &operator[ ]( const FPS_MEMSIZE p_Index );
		FPS_FLOAT32 operator[ ]( const FPS_MEMSIZE p_Index ) const;

	private:
		// The matrix is stored as follows:
		// 0 4 8  12
		// 1 5 9  13
		// 2 6 10 14
		// 3 7 11 15
		FPS_FLOAT32 m_M[ 16 ];
	};

	Vector4 operator*( const Vector4 &p_Vector, const Matrix4x4 &p_Matrix );
}

#endif // __FPS_MATRIX4X4_HPP__

