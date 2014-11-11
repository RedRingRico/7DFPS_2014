#include <Material.hpp>
#include <Shader.hpp>

namespace FPS
{
	Material::Material( )
	{
		MD5Zero( m_MD5Digest );
		MD5Zero( m_ShaderMD5Digest );
		MD5Zero( m_TextureMD5Digest );
	}

	Material::~Material( )
	{
	}

	FPS_UINT32 Material::CreateFromFile( const std::string &p_FileName )
	{
		Shader MaterialShader;
		return FPS_FAIL;
	}
}

