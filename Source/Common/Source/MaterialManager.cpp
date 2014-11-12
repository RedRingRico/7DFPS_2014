#include <MaterialManager.hpp>
#include <iostream>

namespace FPS
{
    MaterialManager::MaterialManager( )
    {
    }

    MaterialManager::~MaterialManager( )
    {
    }

    FPS_UINT32 MaterialManager::CreateMaterial( const std::string &p_FileName,
		MD5_DIGEST &p_Digest )
    {
		MD5_DIGEST Digest;
		MD5Zero( Digest );
		Material NewMaterial( this );

		if( NewMaterial.CreateFromFile( p_FileName ) != FPS_OK )
		{
			return FPS_FAIL;
		}

		if( NewMaterial.GetDigest( Digest ) != FPS_OK )
		{
			return FPS_FAIL;
		}

		return FPS_OK;
    }

    FPS_UINT32 MaterialManager::ApplyMaterial( const MD5_DIGEST &p_Digest )
    {
		std::map< MD5_DIGEST, Material >::const_iterator MaterialItr =
			m_Materials.begin( );

		MD5_DIGEST MaterialDigest;

		while( MaterialItr != m_Materials.end( ) )
		{
			if( MD5Equal( MaterialItr->first, p_Digest ) )
			{
				break;
			}

			++MaterialItr;
		}

		if( MaterialItr == m_Materials.end( ) )
		{
			return FPS_FAIL;
		}

		MD5_DIGEST ShaderDigest;

		MaterialItr->second.GetShader( ShaderDigest );

		std::map< MD5_DIGEST, Shader >::iterator ShaderItr =
			m_Shaders.begin( );

		while( ShaderItr != m_Shaders.end( ) )
		{
			if( MD5Equal( ShaderItr->first, ShaderDigest ) )
			{
				break;
			}
			++ShaderItr;
		}

		if( ShaderItr == m_Shaders.end( ) )
		{
			return FPS_FAIL;
		}

		return ShaderItr->second.Apply( );
    }

    FPS_UINT32 MaterialManager::SetShaderParameter( const MD5_DIGEST &p_Digest,
		const std::string &p_Name, void *p_pData )
    {
		std::map< MD5_DIGEST, Material >::const_iterator MaterialItr =
			m_Materials.begin( );

		MD5_DIGEST MaterialDigest;

		while( MaterialItr != m_Materials.end( ) )
		{
			if( MD5Equal( MaterialItr->first, p_Digest ) )
			{
				break;
			}

			++MaterialItr;
		}

		if( MaterialItr == m_Materials.end( ) )
		{
			return FPS_FAIL;
		}

		MD5_DIGEST ShaderDigest;

		MaterialItr->second.GetShader( ShaderDigest );

		std::map< MD5_DIGEST, Shader >::iterator ShaderItr =
			m_Shaders.begin( );

		while( ShaderItr != m_Shaders.end( ) )
		{
			if( MD5Equal( ShaderItr->first, ShaderDigest ) )
			{
				break;
			}
			++ShaderItr;
		}

		if( ShaderItr == m_Shaders.end( ) )
		{
			return FPS_FAIL;
		}

		return ShaderItr->second.SetShaderParameter( p_Name, p_pData );
    }

    FPS_UINT32 MaterialManager::AddShader( const Shader &p_Shader )
    {
		std::pair< std::map< MD5_DIGEST, Shader >::iterator, bool >
			InsertResult;
		MD5_DIGEST Digest;

		p_Shader.GetDigest( Digest );

		InsertResult = m_Shaders.insert( std::pair< MD5_DIGEST, Shader >(
			Digest, p_Shader ) );

		if( InsertResult.second == false )
		{
			return FPS_FAIL;
		}

		return FPS_OK;
    }
}

