#include <Material.hpp>
#include <Shader.hpp>
#include <Memory.hpp>
#include <hl_md5.h>
#include <iostream>
#include <cstring>
#include <MaterialManager.hpp>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

namespace FPS
{
	Material::Material( MaterialManager *p_pMaterialManager ) :
		m_pMaterialManager( p_pMaterialManager )
	{
		MD5Zero( m_MD5Digest );
		MD5Zero( m_ShaderMD5Digest );
	}

	Material::~Material( )
	{
	}

	FPS_UINT32 Material::GetDigest( MD5_DIGEST &p_Digest ) const
	{
		memcpy( &p_Digest, &m_MD5Digest, sizeof( m_MD5Digest ) );

		return FPS_OK;
	}

	FPS_UINT32 Material::CreateFromFile( const std::string &p_FileName )
	{
		if( p_FileName.size( ) == 0 )
		{
			std::cout << "[FPS::Material::CreateFromFile] <ERROR> "
				"File path is of zero length" << std::endl;
			return FPS_FAIL;
		}

		FILE *pMaterialFile = fopen( p_FileName.c_str( ), "rb" );

		if( pMaterialFile == FPS_NULL )
		{
			std::cout << "[FPS::Material::CreateFromFile] <ERROR> "
				"Failed to open file '" << p_FileName << "' for reading" <<
				std::endl;

			return FPS_FAIL;
		}

		fseek( pMaterialFile, 0, SEEK_END );
		FPS_MEMSIZE FileSize = ftell( pMaterialFile );
		rewind( pMaterialFile );

		char *pSource = new char[ FileSize ];

		fread( pSource, 1, FileSize, pMaterialFile );

		fclose( pMaterialFile );
		pMaterialFile = FPS_NULL;

		rapidjson::Document MaterialFile;
		MaterialFile.Parse( pSource );

		Shader MaterialShader;

		if( MaterialFile.HasMember( "shader" ) )
		{
			std::cout << "[FPS::Material::CreateFromFile] <INFO> "
				"Found material shader" << std::endl;
			rapidjson::Value &ShaderRoot = MaterialFile[ "shader" ];

			if( ShaderRoot.HasMember( "source" ) )
			{
				std::cout << "[FPS::Material::CreateFromFile] <INFO> "
					"Found shader source in shader" << std::endl;

				if( ShaderRoot[ "source" ].IsArray( ) == false )
				{
					std::cout << "[FPS::Material::CreateFromFile] <ERROR> "
						"Failed to load shader source, it is not recognised "
						"as being an array of values" << std::endl;
					SafeDeleteArray< char >( pSource );

					return FPS_FAIL;
				}

				rapidjson::Value &ShaderSourceRoot = ShaderRoot[ "source" ];

				std::cout << "[FPS::Material::CreateFromFile] <INFO> "
					"Processing " << ShaderSourceRoot.Size( ) << " shaders" <<
					std::endl;

				for( rapidjson::SizeType i = 0; i < ShaderSourceRoot.Size( );
					++i )
				{
					SHADER_TYPE Type = SHADER_TYPE_UNKNOWN;
					std::string ShaderSource;
					FPS_BOOL FromFile = FPS_TRUE;

					if( ShaderSourceRoot[ i ].HasMember( "type" ) )
					{
						std::string TypeString =
							ShaderSourceRoot[ i ][ "type" ].GetString( );

						if( TypeString.compare( "vertex" ) == 0 )
						{
							Type = SHADER_TYPE_VERTEX;
						}
						else if( TypeString.compare( "fragment" ) == 0 )
						{
							Type = SHADER_TYPE_FRAGMENT;
						}
						else if( TypeString.compare( "geometry" ) == 0 )
						{
							Type = SHADER_TYPE_GEOMETRY;
						}
						else
						{
							SafeDeleteArray< char >( pSource );
							std::cout << "[FPS::Material::CreateFromFile] "
								"<ERROR> Unrecognised shader type '" <<
								TypeString << "'" << std::endl;

							return FPS_FAIL;
						}
					}
					else
					{
						SafeDeleteArray< char >( pSource );
						return FPS_FAIL;
					}

					if( ShaderSourceRoot[ i ].HasMember( "path" ) )
					{
						ShaderSource =
							ShaderSourceRoot[ i ][ "path" ].GetString( );
						FromFile = FPS_TRUE;
					}
					else if( ShaderSourceRoot[ i ].HasMember( "code" ) )
					{
						ShaderSource =
							ShaderSourceRoot[ i ][ "code" ].GetString( );
						FromFile = FPS_FALSE;
					}
					else
					{
						SafeDeleteArray< char >( pSource );
						std::cout << "[FPS::Material::CreateFromFile] <ERROR> "
							"Failed to get either the path to a shader file "
							"or the source code directly (neither code nor "
							"path were found)" << std::endl;
						return FPS_FAIL;
					}


					MaterialShader.CreateShaderFromSource( ShaderSource, Type,
						FromFile );
				}
			}
			else
			{
			SafeDeleteArray< char >( pSource );
				return FPS_FAIL;
			}
		}
		else
		{
			SafeDeleteArray< char >( pSource );
			std::cout << "[FPS::Material::CreateFromFile] <ERROR> "
				"Could not find shader in JSON" << std::endl;
			return FPS_FAIL;
		}

		MD5 MaterialMD5;
		HL_MD5_CTX MaterialMD5Context;
		
		MaterialMD5.MD5Init( &MaterialMD5Context );

		MaterialMD5.MD5Update( &MaterialMD5Context,
			reinterpret_cast< unsigned char * >( pSource ),
			strlen( pSource ) );

		SafeDeleteArray< char >( pSource );

		MaterialMD5.MD5Final( m_MD5Digest.Digest, &MaterialMD5Context );

		MaterialShader.GetShaderParameters( m_ShaderParameters );

		MaterialShader.Link( );

		if( m_pMaterialManager->AddShader( MaterialShader ) == FPS_FAIL )
		{
			std::cout << "[FPS::Material::CreateFromFile] <INFO> Shader "
				"already in material manager" << std::endl;
		}

		return FPS_OK;
	}

	FPS_UINT32 Material::GetShader( MD5_DIGEST &p_Digest ) const
	{
		memcpy( &p_Digest, &m_MD5Digest, sizeof( m_MD5Digest ) );

		return FPS_OK;
	}
}

