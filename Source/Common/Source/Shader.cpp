#include <Shader.hpp>

namespace FPS
{
	Shader::Shader( ) :
		m_ID( 0 ),
		m_VertexID( 0 ),
		m_FragmentID( 0 ),
		m_GeometryID( 0 ),
		m_ProgramID( 0 ),
		m_Linked( FPS_FALSE )
	{
	}

	Shader::~Shader( )
	{
	}

	FPS_UINT32 Shader::LoadShaderFromSource( const std::string &p_Source,
		const SHADER_TYPE p_ShaderType, const FPS_BOOL p_FromFile )
	{
		return FPS_FAIL;
	}

	FPS_UINT32 Shader::Apply( )
	{
		return FPS_FAIL;
	}

	FPS_UINT32 Shader::SetShaderParameter( const std::string &p_Name,
		const SHADER_PARAMETER_TYPE, void *p_pValue )
	{
		return FPS_FAIL;
	}
}

