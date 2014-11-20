#ifndef __FPS_TEXTURE_HPP__
#define __FPS_TEXTURE_HPP__

#include <DataTypes.hpp>
#include <MD5.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <hl_md5.h>
#include <string>

namespace FPS
{
	// Currently, only Targa files are handled at all
#pragma pack( 1 )
	typedef struct _tagTARGA_HEADER
	{
		FPS_BYTE	IDLength;
		FPS_BYTE	ColourmapType;
		FPS_BYTE	ImageType;
		FPS_BYTE	ColourmapSpecification[ 5 ];
		FPS_UINT16	X;
		FPS_UINT16	Y;
		FPS_UINT16	Width;
		FPS_UINT16	Height;
		FPS_BYTE	BitsPerPixel;
		FPS_BYTE	ImageDescription;
	}TARGA_HEADER,*PTARGA_HEADER;
#pragma pack( )

	class Shader;

	class Texture
	{
	public:
		Texture( );
		~Texture( );

		FPS_UINT32 LoadFromFile( const std::string &p_FileName );

		void SetActive( Shader *p_pShader );

		FPS_UINT32 GetDigest( MD5_DIGEST &p_Digest ) const;

	private:
		MD5_DIGEST	m_Digest;
		MD5			m_MD5;
		HL_MD5_CTX	m_MD5Context;
		GLuint		m_TextureID;
		GLuint		m_SamplerID;
	};
}

#endif // __FPS_TEXTURE_HPP__

