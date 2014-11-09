#include <Matrix4x4.hpp>
#include <Vector4.hpp>
#include <Maths.hpp>

namespace FPS
{
	Matrix4x4::Matrix4x4( )
	{
		this->Identity( );
	}

	Matrix4x4::~Matrix4x4( )
	{
	}

	void Matrix4x4::Identity( )
	{
		m_M[ 0 ] = m_M[ 5 ] = m_M[ 10 ] = m_M[ 15 ] = 1.0f;
		m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] = m_M[ 6 ] = m_M[ 7 ] =
			m_M[ 8 ] = m_M[ 9 ] = m_M[ 11 ] = m_M[ 12 ] = m_M[ 14 ] = 0.0f;
	}

	Matrix4x4 &Matrix4x4::Rotate( const FPS_FLOAT32 p_Angle,
		const Vector4 &p_Axis )
	{
		FPS_FLOAT32 Sin = 0.0f, Cos = 0.0f, Tan = 0.0f;
		FPS::SineCosine( p_Angle, Sin, Cos );

		Tan = 1.0f - Cos;

		m_M[ 0 ] = ( Tan * p_Axis.GetX( ) * p_Axis.GetX( ) ) + Cos;
		m_M[ 1 ] = ( Tan * p_Axis.GetX( ) * p_Axis.GetY( ) ) +
			( Sin + p_Axis.GetZ( ) );
		m_M[ 2 ] = ( Tan * p_Axis.GetX( ) * p_Axis.GetZ( ) ) -
			( Sin * p_Axis.GetY( ) );

		m_M[ 4 ] = ( Tan * p_Axis.GetX( ) * p_Axis.GetY( ) ) -
			( Sin * p_Axis.GetZ( ) );
		m_M[ 5 ] = ( Tan * p_Axis.GetY( ) * p_Axis.GetY( ) ) + Cos;
		m_M[ 6 ] = ( Tan * p_Axis.GetY( ) * p_Axis.GetZ( ) ) +
			( Sin * p_Axis.GetX( ) );

		m_M[ 8 ] = ( Tan * p_Axis.GetX( ) * p_Axis.GetZ( ) ) +
			( Sin * p_Axis.GetY( ) );
		m_M[ 9 ] = ( Tan * p_Axis.GetY( ) * p_Axis.GetZ( ) ) -
			( Sin * p_Axis.GetX( ) );
		m_M[ 10 ] = ( Tan * p_Axis.GetZ( ) * p_Axis.GetZ( ) ) + Cos;

		m_M[ 3 ] = m_M[ 7 ] = m_M[ 11 ] = m_M[ 12 ] = m_M[ 13 ] = m_M[ 14 ] =
			0.0f;
		m_M[ 15 ] = 1.0f;

		return *this;
	}

	Matrix4x4 &Matrix4x4::RotateX( const FPS_FLOAT32 p_Angle )
	{
		FPS_FLOAT32 Sin = 0.0f, Cos = 0.0f;
		FPS::SineCosine( p_Angle, Sin, Cos );

		m_M[ 0 ] = 1.0f;
		m_M[ 1 ] = 0.0f;
		m_M[ 2 ] = 0.0f;
		m_M[ 3 ] = 0.0f;

		m_M[ 4 ] = 0.0f;
		m_M[ 5 ] = Cos;
		m_M[ 6 ] = -Sin;
		m_M[ 7 ] = 0.0f;

		m_M[ 8 ] = 0.0f;
		m_M[ 9 ] = Sin;
		m_M[ 10 ] = Cos;
		m_M[ 11 ] = 0.0f;

		m_M[ 12 ] = 0.0f;
		m_M[ 13 ] = 0.0f;
		m_M[ 14 ] = 0.0f;
		m_M[ 15 ] = 1.0f;

		return *this;
	}

	Matrix4x4 &Matrix4x4::RotateY( const FPS_FLOAT32 p_Angle )
	{
		FPS_FLOAT32 Sin = 0.0f, Cos = 0.0f;
		FPS::SineCosine( p_Angle, Sin, Cos );

		m_M[ 0 ] = Cos;
		m_M[ 1 ] = 0.0f;
		m_M[ 2 ] = Sin;
		m_M[ 3 ] = 0.0f;

		m_M[ 4 ] = 0.0f;
		m_M[ 5 ] = 1.0f;
		m_M[ 6 ] = 0.0f;
		m_M[ 7 ] = 0.0f;

		m_M[ 8 ] = -Sin;
		m_M[ 9 ] = 0.0f;
		m_M[ 10 ] = Cos;
		m_M[ 11 ] = 0.0f;

		m_M[ 12 ] = 0.0f;
		m_M[ 13 ] = 0.0f;
		m_M[ 14 ] = 0.0f;
		m_M[ 15 ] = 1.0f;

		return *this;
	}

	Matrix4x4 &Matrix4x4::RotateZ( const FPS_FLOAT32 p_Angle )
	{
		FPS_FLOAT32 Sin = 0.0f, Cos = 0.0f;
		FPS::SineCosine( p_Angle, Sin, Cos );

		m_M[ 0 ] = Cos;
		m_M[ 1 ] = -Sin;
		m_M[ 2 ] = 0.0f;
		m_M[ 3 ] = 0.0f;

		m_M[ 4 ] = Sin;
		m_M[ 5 ] = Cos;
		m_M[ 6 ] = 0.0f;
		m_M[ 7 ] = 0.0f;

		m_M[ 8 ] = 0.0f;
		m_M[ 9 ] = 0.0f;
		m_M[ 10 ] = 1.0f;
		m_M[ 11 ] = 0.0f;

		m_M[ 12 ] = 0.0f;
		m_M[ 13 ] = 0.0f;
		m_M[ 14 ] = 0.0f;
		m_M[ 15 ] = 1.0f;

		return *this;
	}

	Matrix4x4 &Matrix4x4::Scale( const FPS_FLOAT32 p_Scale )
	{
		m_M[ 0 ] = m_M[ 5 ] = m_M[ 10 ] = p_Scale;

		m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] = m_M[ 6 ] = m_M[ 7 ] =
			m_M[ 8 ] = m_M[ 9 ] = m_M[ 11 ] = m_M[ 12 ] = m_M[ 13 ] =
			m_M[ 14 ] = 0.0f;
		
		m_M[ 15 ] = 1.0f;

		return *this;
	}

	Matrix4x4 &Matrix4x4::Scale( const Vector4 &p_Scale )
	{
		m_M[ 0 ] = p_Scale.GetX( );
		m_M[ 5 ] = p_Scale.GetY( );
		m_M[ 10 ] = p_Scale.GetZ( );

		m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] = m_M[ 6 ] = m_M[ 7 ] =
			m_M[ 8 ] = m_M[ 9 ] = m_M[ 11 ] = m_M[ 12 ] = m_M[ 13 ] =
			m_M[ 14 ] = 0.0f;

		m_M[ 15 ] = 1.0f;

		return *this;
	}

	Matrix4x4 &Matrix4x4::ScaleX( const FPS_FLOAT32 p_Scale )
	{
		this->Identity( );
		m_M[ 0 ] = p_Scale;
		
		return *this;
	}

	Matrix4x4 &Matrix4x4::ScaleY( const FPS_FLOAT32 p_Scale )
	{
		this->Identity( );
		m_M[ 5 ] = p_Scale;
		
		return *this;
	}

	Matrix4x4 &Matrix4x4::ScaleZ( const FPS_FLOAT32 p_Scale )
	{
		this->Identity( );
		m_M[ 10 ] = p_Scale;

		return *this;
	}

	FPS_FLOAT32 &Matrix4x4::operator[ ]( const FPS_MEMSIZE p_Index )
	{
		return m_M[ p_Index ];
	}

	FPS_FLOAT32 Matrix4x4::operator[ ]( const FPS_MEMSIZE p_Index ) const
	{
		return m_M[ p_Index ];
	}

	Vector4 operator*( const Vector4 &p_Vector, const Matrix4x4 &p_Matrix )
	{
		Vector4 Multiply;

		Multiply.SetX(	( p_Vector.GetX( ) * p_Matrix[ 0 ] ) +
						( p_Vector.GetY( ) * p_Matrix[ 1 ] ) +
						( p_Vector.GetZ( ) * p_Matrix[ 2 ] ) +
						( p_Vector.GetW( ) * p_Matrix[ 3 ] ) );

		Multiply.SetY(	( p_Vector.GetX( ) * p_Matrix[ 4 ] ) +
						( p_Vector.GetY( ) * p_Matrix[ 5 ] ) +
						( p_Vector.GetZ( ) * p_Matrix[ 6 ] ) +
						( p_Vector.GetW( ) * p_Matrix[ 7 ] ) );

		Multiply.SetZ(	( p_Vector.GetX( ) * p_Matrix[ 8 ] ) +
						( p_Vector.GetY( ) * p_Matrix[ 9 ] ) +
						( p_Vector.GetZ( ) * p_Matrix[ 10 ] ) +
						( p_Vector.GetW( ) * p_Matrix[ 11 ] ) );

		Multiply.SetW(	( p_Vector.GetX( ) * p_Matrix[ 12 ] ) +
						( p_Vector.GetY( ) * p_Matrix[ 13 ] ) +
						( p_Vector.GetZ( ) * p_Matrix[ 14 ] ) +
						( p_Vector.GetW( ) * p_Matrix[ 15 ] ) );

		return Multiply;
	}
}

