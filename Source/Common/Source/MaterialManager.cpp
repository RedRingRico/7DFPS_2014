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
			std::cout << "[FPS::MaterialManager::CreateMaterial] "
			"<ERROR> Failed to create material from file" << std::endl;

			return FPS_FAIL;
		}

		if( NewMaterial.GetDigest( Digest ) != FPS_OK )
		{
			std::cout << "[FPS::MaterialManager::CreateMaterial] "
			"<ERROR> Failed to obtain the material's MD5 digest" <<
			std::endl;

			return FPS_FAIL;
		}

		std::pair< std::map< MD5_DIGEST, Material >::iterator, bool > Insert;

		Insert = m_Materials.insert( std::pair< MD5_DIGEST, Material >(
			Digest, NewMaterial ) );

		if( Insert.second == false )
		{
			std::cout << "[FPS::MaterialManager::CreateMaterial] <INFO> "
				"Material already exists: " << MD5AsString( Digest ) <<
				std::endl;
		}

		p_Digest = Digest;

		return FPS_OK;
    }

    FPS_UINT32 MaterialManager::ApplyMaterial( const MD5_DIGEST &p_Digest )
    {
		std::map< MD5_DIGEST, Material >::const_iterator MaterialItr =
			m_Materials.begin( );

		MD5_DIGEST MaterialDigest;

		while( MaterialItr != m_Materials.end( ) )
		{
			std::cout << "Current material MD5: " << MD5AsString( MaterialItr->first ) << std::endl;
			if( MD5Equal( MaterialItr->first, p_Digest ) )
			{
				break;
			}

			++MaterialItr;
		}

		if( MaterialItr == m_Materials.end( ) )
		{
			std::cout << "[FPS::MaterialManager::ApplyMaterial] <ERROR> "
				"Failed to find material '" << MD5AsString( p_Digest ) <<
				"'" << std::endl;

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
			std::cout << "[FPS::MaterialManager::ApplyMaterial] <ERROR> "
				"Failed to find shader '" << MD5AsString( ShaderDigest ) <<
				"'for material '" << MD5AsString( p_Digest ) << "'" <<
				std::endl;

			return FPS_FAIL;
		}

		std::cout << "Applying material" << std::endl;

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

