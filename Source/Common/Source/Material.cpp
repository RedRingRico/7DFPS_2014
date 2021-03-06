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
		std::cout << "Deleting material" << std::endl;
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

		MD5 MaterialMD5;
		HL_MD5_CTX MaterialMD5Context;
		
		MaterialMD5.MD5Init( &MaterialMD5Context );

		MaterialMD5.MD5Update( &MaterialMD5Context,
			reinterpret_cast< unsigned char * >( pSource ),
			strlen( pSource ) );

		SafeDeleteArray< char >( pSource );

		MaterialMD5.MD5Final( m_MD5Digest.Digest, &MaterialMD5Context );

		MATERIAL_SHADER MaterialShader;
		MaterialShader.Types = SHADER_TYPE_NONE;

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
							std::cout << "[FPS::Material::CreateFromFile] "
								"<ERROR> Unrecognised shader type '" <<
								TypeString << "'" << std::endl;

							return FPS_FAIL;
						}
					}
					else
					{
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
						std::cout << "[FPS::Material::CreateFromFile] <ERROR> "
							"Failed to get either the path to a shader file "
							"or the source code directly (neither code nor "
							"path were found)" << std::endl;

						return FPS_FAIL;
					}

					switch( Type )
					{
						case SHADER_TYPE_VERTEX:
						{
							MaterialShader.VertexSource = ShaderSource;
							MaterialShader.Types |= SHADER_TYPE_VERTEX;
							MaterialShader.VertexFile = FromFile;
							break;
						}
						case SHADER_TYPE_FRAGMENT:
						{
							MaterialShader.FragmentSource = ShaderSource;
							MaterialShader.Types |= SHADER_TYPE_FRAGMENT;
							MaterialShader.FragmentFile = FromFile;
							break;
						}
						case SHADER_TYPE_GEOMETRY:
						{
							MaterialShader.GeometrySource = ShaderSource;
							MaterialShader.Types |= SHADER_TYPE_GEOMETRY;
							MaterialShader.GeometryFile = FromFile;
							break;
						}
						default:
						{
							return FPS_FAIL;
						}
					}
				}
			}
			else
			{
				return FPS_FAIL;
			}
		}
		else
		{
			std::cout << "[FPS::Material::CreateFromFile] <ERROR> "
				"Could not find shader in JSON" << std::endl;
			return FPS_FAIL;
		}

		if( MaterialFile.HasMember( "texture" ) )
		{
			rapidjson::Value &TextureRoot = MaterialFile[ "texture" ];

			if( TextureRoot.IsArray( ) )
			{
				for( rapidjson::SizeType i = 0; i < TextureRoot.Size( ); ++i )
				{
					std::string FileName;
					if( TextureRoot[ i ].HasMember( "type" ) )
					{
						std::string TypeString =
							TextureRoot[ i ][ "type" ].GetString( );

						if( TypeString.compare( "albedo" ) == 0 )
						{
						}
						else
						{
							return FPS_FAIL;
						}
					}
					else
					{
						return FPS_FAIL;
					}

					if( TextureRoot[ i ].HasMember( "path" ) )
					{
						FileName = TextureRoot[ i ][ "path" ].GetString( );
					}
					else
					{
						return FPS_FAIL;
					}

					MD5_DIGEST TextureMD5;
					m_pMaterialManager->LoadTexture( FileName, TextureMD5 );
					m_TextureMD5Digest.push_back( TextureMD5 );

					std::cout << "Texture MD5: " << MD5AsString( TextureMD5 ) << std::endl;
				}
			}
			else
			{
				return FPS_FAIL;
			}
		}
		else
		{
			std::cout << "[FPS::Material::CreateFromFile] <INFO> No textures "
				"present" << std::endl;
		}

		m_pMaterialManager->CreateShader( MaterialShader, m_ShaderParameters,
			m_ShaderMD5Digest );

		return FPS_OK;
	}

	FPS_UINT32 Material::GetShader( MD5_DIGEST &p_Digest ) const
	{
		memcpy( &p_Digest, &m_ShaderMD5Digest, sizeof( m_ShaderMD5Digest ) );

		return FPS_OK;
	}
	
	FPS_UINT32 Material::GetTextures(
		std::vector< MD5_DIGEST > &p_Digests ) const
	{
		for( FPS_MEMSIZE TextureIndex = 0;
			TextureIndex < m_TextureMD5Digest.size( ); ++TextureIndex )
		{
			p_Digests.push_back( m_TextureMD5Digest[ TextureIndex ] );
		}

		return FPS_OK;
	}
}

