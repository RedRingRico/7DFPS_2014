#ifndef __FPS_TEXTURE_HPP__
#define __FPS_TEXTURE_HPP__

#include <DataTypes.hpp>
#include <GL/gl.h>

namespace FPS
{
	class Texture
	{
	public:
		Texture( );
		~Texture( );

	private:
		FPS_UINT32	m_ID;
		GLuint		m_TextureID;
	};
}

#endif // __FPS_TEXTURE_HPP__

