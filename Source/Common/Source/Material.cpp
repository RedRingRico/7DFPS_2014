#include <Material.hpp>
#include <Shader.hpp>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

namespace FPS
{
	Material::Material( )
	{
		MD5Zero( m_MD5Digest );
		MD5Zero( m_ShaderMD5Digest );
		MD5Zero( m_TextureMD5Digest );
	}

	Material::~Material( )
	{
	}

	FPS_UINT32 Material::CreateFromFile( const std::string &p_FileName )
	{
		if( p_FileName.size( ) == 0 )
		{
			return FPS_FAIL;
		}

		FILE *pMaterialFile = fopen( p_FileName.c_str( ), "rb" );

		if( pMaterialFile == FPS_NULL )
		{
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
			rapidjson::Value &ShaderRoot = MaterialFile[ "shader" ];

			if( ShaderRoot.HasMember( "source" ) )
			{
				rapidjson::Value &ShaderSourceRoot = ShaderRoot[ "source" ];

				for( rapidjson::SizeType i = 0; i < ShaderRoot.Size( ); ++i )
				{
					SHADER_TYPE Type = SHADER_TYPE_UNKNOWN;
					std::string ShaderSource;
					FPS_BOOL FromFile = FPS_TRUE;

					if( ShaderRoot[ i ].HasMember( "type" ) )
					{
						std::string TypeString =
							ShaderRoot[ i ][ "type" ].GetString( );

						if( TypeString.compare( "vertex" ) )
						{
							Type = SHADER_TYPE_VERTEX;
						}
						else if( TypeString.compare( "fragment" ) )
						{
							Type = SHADER_TYPE_FRAGMENT;
						}
						else if( TypeString.compare( "geometry" ) )
						{
							Type = SHADER_TYPE_GEOMETRY;
						}
					}
					else
					{
						return FPS_FAIL;
					}

					if( ShaderRoot[ i ].HasMember( "path" ) )
					{
						ShaderSource = ShaderRoot[ i ][ "path" ].GetString( );
						FromFile = FPS_TRUE;
					}
					else if( ShaderRoot[ i ].HasMember( "code" ) )
					{
						ShaderSource = ShaderRoot[ i ][ "code" ].GetString( );
						FromFile = FPS_FALSE;
					}
					else
					{
						return FPS_FAIL;
					}

					MaterialShader.CreateShaderFromSource( ShaderSource, Type,
						FromFile );
				}
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

		MaterialShader.GetShaderParameters( m_ShaderParameters );

		return FPS_OK;
	}
}

