#ifndef __FPS_SHADER_HPP__
#define __FPS_SHADER_HPP__

#include <DataTypes.hpp>
#include <map>
#include <string>
#include <GL/glew.h>
#include <GL/gl.h>

namespace FPS
{
	typedef enum _tagSHADER_TYPE
	{
		SHADER_TYPE_VERTEX		= 1,
		SHADER_TYPE_FRAGMENT	= 2,
		SHADER_TYPE_GEOMETRY	= 3,
		SHADER_TYPE_UNKNOWN		= 0x7FFFFFFF
	}SHADER_TYPE;

	typedef enum _tagSHADER_PARAMETER_TYPE
	{
		SHADER_PARAMETER_TYPE_FLOAT1	= 1,
		SHADER_PARAMETER_TYPE_FLOAT2	= 2,
		SHADER_PARAMETER_TYPE_FLOAT3	= 3,
		SHADER_PARAMETER_TYPE_FLOAT4	= 4,

		SHADER_PARAMETER_TYPE_MATRIX2X2	= 10,
		SHADER_PARAMETER_TYPE_MATRIX3X3	= 11,
		SHADER_PARAMETER_TYPE_MATRIX4X4	= 12,

		SHADER_PARAMETER_TYPE_BOOL1		= 20,
		SHADER_PARAMETER_TYPE_BOOL2		= 21,
		SHADER_PARAMETER_TYPE_BOOL3		= 22,
		SHADER_PARAMETER_TYPE_BOOL4		= 23,

		SHADER_PARAMETER_TYPE_INT1		= 30,
		SHADER_PARAMETER_TYPE_INT2		= 31,
		SHADER_PARAMETER_TYPE_INT3		= 32,
		SHADER_PARAMETER_TYPE_INT4		= 33,

		SHADER_PARAMETER_TYPE_UINT1		= 40,
		SHADER_PARAMETER_TYPE_UINT2		= 41,
		SHADER_PARAMETER_TYPE_UINT3		= 42,
		SHADER_PARAMETER_TYPE_UINT4		= 43,

		SHADER_PARAMETER_TYPE_UNKNOWN	= 0x7FFFFFFF
	}SHADER_PARAMETER_TYPE;

	typedef struct _tagSHADER_PARAMETER
	{
		SHADER_PARAMETER_TYPE	Type;
		GLint					Location;
		GLsizei					ArraySize;
	}SHADER_PARAMETER,*PSHADER_PARAMETER;

	class Shader
	{
	public:
		Shader( );
		~Shader( );

		FPS_UINT32 CreateShaderFromSource( const std::string &p_Source,
			const SHADER_TYPE p_ShaderType, const FPS_BOOL p_FromFile );

		FPS_UINT32 Apply( );

		FPS_UINT32 SetShaderParameter( const std::string &p_Name,
			void *p_pValue );

	private:
		FPS_UINT32 ExtractUniformNames( const GLchar *p_Source );
		FPS_UINT32 Link( );
		FPS_UINT32 BindUniformNamesToLocations( );

		FPS_UINT32									m_ID;
		GLuint										m_VertexID;
		GLuint										m_FragmentID;
		GLuint										m_GeometryID;
		GLuint										m_ProgramID;
		FPS_BOOL									m_Linked;
		std::map< std::string, SHADER_PARAMETER >	m_UniformLocationMap;
	};
}

#endif // __FPS_SHADER_HPP__

