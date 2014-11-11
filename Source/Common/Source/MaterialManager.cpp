#include <MaterialManager.hpp>
#include <hl_md5.h>
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
		return FPS_FAIL;
    }

    FPS_UINT32 MaterialManager::SetShaderParameter( const MD5_DIGEST &p_Digest,
	const std::string &p_name, void *p_pData )
    {
		return FPS_FAIL;
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

