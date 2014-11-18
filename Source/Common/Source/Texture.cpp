#include <Texture.hpp>
#include <Memory.hpp>
#include <iostream>

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
				InternalFormat = GL_RGBA;
				Format = GL_RGBA;
				Type = GL_UNSIGNED_SHORT_5_5_5_1;
				break;
			}
			case 24:
			{
				InternalFormat = GL_RGB;
				Format = GL_RGB;
				Type = GL_UNSIGNED_BYTE;
				break;
			}
			case 32:
			{
				InternalFormat = GL_RGBA;
				Format = GL_RGBA;
				Type = GL_UNSIGNED_BYTE;
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
		glTexImage2D( GL_TEXTURE_2D, 0, InternalFormat, TargaHeader.Width,
			TargaHeader.Height, 0, Format, Type, pImageData );

		SafeDeleteArray< FPS_BYTE >( pImageData );

		return FPS_OK;
	}

	void Texture::SetActive( )
	{
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, m_TextureID );
	}
}

