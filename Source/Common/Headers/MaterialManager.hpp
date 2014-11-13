#ifndef __FPS_MATERIALMANAGER_HPP__
#define __FPS_MATERIALMANAGER_HPP__

#include <Shader.hpp>
#include <Material.hpp>
#include <Texture.hpp>
#include <MD5.hpp>
#include <string>
#include <map>

namespace FPS
{
	typedef struct _tagMATERIAL_SHADER
	{
		std::string VertexSource;
		std::string FragmentSource;
		std::string GeometrySource;
		SHADER_TYPE	Types;
		FPS_BOOL	VertexFile;
		FPS_BOOL	FragmentFile;
		FPS_BOOL	GeometryFile;
	}MATERIAL_SHADER,*PMATERIAL_SHADER;

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

		FPS_UINT32 CreateShader( const MATERIAL_SHADER &p_ShaderInfo,
			const MD5_DIGEST &p_MaterialDigest, MD5_DIGEST &p_ShaderDigest );

    private:
		std::map< MD5_DIGEST, Material * >	m_Materials;
		std::map< MD5_DIGEST, Shader * >	m_Shaders;
		std::map< MD5_DIGEST, Texture >		m_Textures;
    };
}

#endif // __FPS_MATERIALMANAGER_HPP__

