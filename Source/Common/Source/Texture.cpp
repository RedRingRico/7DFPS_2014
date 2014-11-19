#include <Texture.hpp>
#include <Shader.hpp>
#include <Memory.hpp>
#include <iostream>
#include <cstring>

namespace FPS
{
	Texture::Texture( ) :
		m_TextureID( 0 )
	{
		MD5Zero( m_Digest );
	}

	Texture::~Texture( )
	{
	}

	FPS_UINT32 Texture::LoadFromFile( const std::string &p_FileName )
	{
		if( m_TextureID )
		{
			glDeleteTextures( 1, &m_TextureID );
		}

		FILE *pFile = FPS_NULL;
		pFile = fopen( p_FileName.c_str() , "rb" );

		if( pFile == FPS_NULL )
		{
			std::cout << "[FPS::Texture::LoadFromFile] <ERROR> Could not load "
				"file \"" << p_FileName << "\"" << std::endl;

			return FPS_FAIL;
		}

		TARGA_HEADER TargaHeader;

		FPS_MEMSIZE TotalFileSize = 0;

		fseek( pFile, 0L, SEEK_END );
		TotalFileSize = ftell( pFile );
		rewind( pFile );

		fread( &TargaHeader, sizeof( TargaHeader ), 1, pFile );

		fseek( pFile, TargaHeader.IDLength, SEEK_SET );

		FPS_MEMSIZE ExpectedImageSize = TargaHeader.Width *
			TargaHeader.Height * ( TargaHeader.BitsPerPixel / 8 );

		FPS_BYTE *pImageData = new FPS_BYTE[ ExpectedImageSize ];

		fread( pImageData, sizeof( FPS_BYTE ), ExpectedImageSize, pFile );

		if( pFile )
		{
			fclose( pFile );
			pFile = FPS_NULL;
		}

		m_MD5.MD5Init( &m_MD5Context );

		m_MD5.MD5Update( &m_MD5Context,
			reinterpret_cast< unsigned char * >( &TargaHeader ),
			sizeof( TargaHeader ) );

		m_MD5.MD5Update( &m_MD5Context, pImageData, ExpectedImageSize );

		m_MD5.MD5Final( m_Digest.Digest, &m_MD5Context );

		GLint InternalFormat;
		GLenum Format, Type;

		switch( TargaHeader.BitsPerPixel )
		{
			case 16:
			{
				InternalFormat = GL_RGB5_A1;
				Format = GL_RGBA;
				Type = GL_UNSIGNED_SHORT_5_5_5_1;
				break;
			}
			case 24:
			{
				InternalFormat = GL_RGB8;
				Format = GL_BGR;
				Type = GL_UNSIGNED_BYTE;
				break;
			}
			case 32:
			{
				InternalFormat = GL_RGBA8;
				Format = GL_BGRA;
				Type = GL_UNSIGNED_BYTE;
				for( FPS_MEMSIZE i = 0;
					i < ( TargaHeader.Width * TargaHeader.Height ); ++i )
				{
					// I'm not sure why, but the Targa exported from GIMP
					// has the format of RABG instead of BGRA
					// It looks like two 16-bit values in a different endian
					char ColourBuffer[ 4 ];
					memcpy( ColourBuffer, &pImageData[ i * 4 ], 4 );
					// R
					pImageData[ i * 4 ] = ColourBuffer[ 2 ];
					// G
					pImageData[ ( i * 4 ) + 1 ] = ColourBuffer[ 3 ];
					// B
					pImageData[ ( i * 4 ) + 2 ] = ColourBuffer[ 0 ];
					// A
					pImageData[ ( i * 4 ) + 3 ] = ColourBuffer[ 1 ];
				}
				break;
			}
			default:
			{
				std::cout << "[FPS::Texture::LoadFromFile] <ERROR> Unknown "
					"format" << std::endl;
				return FPS_FAIL;
			}
		}

		glGenTextures( 1, &m_TextureID );
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, m_TextureID );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0 );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0 );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		glTexStorage2D( GL_TEXTURE_2D, 1, InternalFormat, TargaHeader.Width,
			TargaHeader.Height );
		glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, TargaHeader.Width,
			TargaHeader.Height, Format, Type, pImageData );

		SafeDeleteArray< FPS_BYTE >( pImageData );

		return FPS_OK;
	}

	void Texture::SetActive( Shader *p_pShader )
	{
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, m_TextureID );

		FPS_SINT32 Zero = 0;
		p_pShader->SetShaderParameter( "u_Texture", &Zero );
	}

	FPS_UINT32 Texture::GetDigest( MD5_DIGEST &p_Digest ) const
	{
		memcpy( &p_Digest, &m_Digest, sizeof( m_Digest ) );

		return FPS_OK;
	}
}

