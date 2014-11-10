#ifndef __FPS_MATERIAL_HPP__
#define __FPS_MATERIAL_HPP__

#include <DataTypes.hpp>

namespace FPS
{
	class Material
	{
	public:
		Material( );
		~Material( );

		FPS_UINT32 SetTexture( const FPS_UINT32 p_TextureID );
		FPS_UINT32 SetShader( const FPS_UINT32 p_ShaderID );

	private:
		FPS_UINT32	m_ID;
		FPS_UINT32	m_TextureID;
		FPS_UINT32	m_ShaderID;
	};
}

#endif // __FPS_MATERIAL_HPP__

