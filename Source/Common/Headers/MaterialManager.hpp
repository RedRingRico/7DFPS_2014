#ifndef __FPS_MATERIALMANAGER_HPP__
#define __FPS_MATERIALMANAGER_HPP__

#include <Material.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <MD5.hpp>
#include <string>
#include <map>

namespace FPS
{
    class MaterialManager
    {
    public:
		MaterialManager( );
		~MaterialManager( );

		FPS_UINT32 CreateMaterial( const std::string &p_FileName,
			MD5_DIGEST &p_Digest );

		FPS_UINT32 ApplyMaterial( const MD5_DIGEST &p_Digest );

		FPS_UINT32 SetShaderParameter( const MD5_DIGEST &p_Digest,
			const std::string &p_Name, void *p_pData );

		FPS_UINT32 AddShader( const Shader &p_Shader );

    private:
		std::map< MD5_DIGEST, Material >	m_Materials;
		std::map< MD5_DIGEST, Shader >		m_Shaders;
		std::map< MD5_DIGEST, Texture >		m_Textures;
    };
}

#endif // __FPS_MATERIALMANAGER_HPP__

