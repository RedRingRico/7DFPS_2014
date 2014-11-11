#include <MaterialManager.hpp>

namespace FPS
{
    MaterialManager::MaterialManager( )
    {
    }

    MaterialManager::~MaterialManager( )
    {
    }

    MD5_DIGEST MaterialManager::CreateMaterial( const std::string &p_FileName )
    {
	MD5_DIGEST Dummy;
	MD5Zero( Dummy );
	return Dummy;
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
	return FPS_FAIL;
    }
}

