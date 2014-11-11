#ifndef __FPS_MATERIAL_HPP__
#define __FPS_MATERIAL_HPP__

#include <DataTypes.hpp>
#include <MD5.hpp>
#include <list>
#include <string>

namespace FPS
{
	class MaterialManager;

	class Material
	{
	public:
		Material( MaterialManager *p_pMaterialManager );
		~Material( );

		FPS_UINT32 CreateFromFile( const std::string &p_FileName );

		FPS_UINT32 SetShaderParameter( const std::string &p_Name,
			void *p_pData );

		FPS_UINT32 GetShader( MD5_DIGEST &p_Digest ) const;
		FPS_UINT32 GetTextures( std::list< MD5_DIGEST > &p_Digests ) const;

	private:
		MD5_DIGEST					m_MD5Digest;
		MD5_DIGEST					m_ShaderMD5Digest;
		std::list< MD5_DIGEST >		m_TextureMD5Digest;
		std::list< std::string >	m_ShaderParameters;
		MaterialManager				*m_pMaterialManager;
	};
}

#endif // __FPS_MATERIAL_HPP__

