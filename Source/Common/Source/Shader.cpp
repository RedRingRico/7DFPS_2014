#include <Shader.hpp>
#include <Memory.hpp>
#include <iostream>
#include <cstdio>
#include <cstring>

namespace FPS
{
	Shader::Shader( ) :
		m_ID( 0 ),
		m_VertexID( 0 ),
		m_FragmentID( 0 ),
		m_GeometryID( 0 ),
		m_ProgramID( 0 ),
		m_Linked( FPS_FALSE )
	{
	}

	Shader::~Shader( )
	{
	}

	FPS_UINT32 Shader::CreateShaderFromSource( const std::string &p_Source,
		const SHADER_TYPE p_ShaderType, const FPS_BOOL p_FromFile )
	{
		if( p_Source.size( ) == 0 )
		{
			std::cout << "[FPS::Shader::CreateShaderFromSource] <ERROR> "
				"Source passed in was of zero length" << std::endl;

			return FPS_FAIL;
		}

		GLuint ShaderID = 0;
		std::string ShaderTypeName;

		switch( p_ShaderType )
		{
			case SHADER_TYPE_VERTEX:
			{
				if( m_VertexID == 0 )
				{
					m_VertexID = glCreateShader( GL_VERTEX_SHADER );
					if( glIsShader( m_VertexID ) != GL_TRUE )
					{
						return FPS_FAIL;
					}

					ShaderID = m_VertexID;
					ShaderTypeName = "Vertex";
				}
				break;
			}
			case SHADER_TYPE_FRAGMENT:
			{
				if( m_FragmentID == 0 )
				{
					m_FragmentID = glCreateShader( GL_FRAGMENT_SHADER );
					if( glIsShader( m_FragmentID ) != GL_TRUE )
					{
						return FPS_FAIL;
					}

					ShaderID = m_FragmentID;
					ShaderTypeName = "Fragment";
				}
				break;
			}
			case SHADER_TYPE_GEOMETRY:
			{
				if( m_GeometryID == 0 )
				{
					m_GeometryID = glCreateShader( GL_GEOMETRY_SHADER );
					if( glIsShader( m_GeometryID ) != GL_TRUE )
					{
						return FPS_FAIL;
					}

					ShaderID = m_GeometryID;
					ShaderTypeName = "Geometry";
				}
				break;
			}
			default:
			{
				std::cout << "[FPS::Shader::CreateShaderFromSource] <ERROR> "
					"Unknown shader type" << std::endl;

				return FPS_FAIL;
			}
		}

		if( glIsShader( ShaderID ) != GL_TRUE )
		{
			return FPS_FAIL;
		}

		GLchar *pSource = FPS_NULL;
		GLint SourceLength = 0;

		if( p_FromFile == FPS_FALSE )
		{
			SourceLength = p_Source.size( );
			pSource = new GLchar[ SourceLength ];
			memcpy( pSource, p_Source.c_str( ), SourceLength );
		}
		else
		{
			FILE *pFile = fopen( p_Source.c_str( ), "rb" );

			fseek( pFile, 0, SEEK_END );
			SourceLength = ftell( pFile );
			rewind( pFile );

			pSource = new GLchar[ SourceLength ];

			fread( pSource, sizeof( GLchar ), SourceLength, pFile );

			if( pFile )
			{
				fclose( pFile );
				pFile = FPS_NULL;
			}
		}

		glShaderSource( ShaderID, 1,
			const_cast< const GLchar ** >( &pSource ), &SourceLength );

		glCompileShader( ShaderID );

		GLint CompileStatus = 0;

		glGetShaderiv( ShaderID, GL_COMPILE_STATUS, &CompileStatus );

		if( !CompileStatus )
		{
			GLint LogLength = 0;
			GLchar *pLog = FPS_NULL;

			glGetShaderiv( ShaderID, GL_INFO_LOG_LENGTH, &LogLength );
			std::cout << "[FPS::Shader::CreateShaderFromSource] <ERROR> " <<
				ShaderTypeName << " Shader Log:" << std::endl;
			std::cout << "\t" << pLog << std::endl;

			SafeDeleteArray< GLchar >( pLog );

			SafeDeleteArray< GLchar >( pSource );

			return FPS_FAIL;
		}

		if( this->ExtractUniformNames( pSource ) != FPS_OK )
		{
			std::cout << "[FPS::Shader::CreateShaderFromSource] <ERROR> "
				"Unable to extract uniform names correctly" << std::endl;

			SafeDeleteArray< GLchar >( pSource );

			return FPS_FAIL;
		}

		SafeDeleteArray< GLchar >( pSource );

		return FPS_OK;
	}

	FPS_UINT32 Shader::Apply( )
	{
		return FPS_FAIL;
	}

	FPS_UINT32 Shader::SetShaderParameter( const std::string &p_Name,
		const SHADER_PARAMETER_TYPE, void *p_pValue )
	{
		return FPS_FAIL;
	}

	FPS_UINT32 Shader::ExtractUniformNames( const GLchar *p_pSource )
	{
		// Look for the keyword "uniform" which should be followed by
		// whitespace until a built-in keyword for a vector, matrix or scalar
		// type is found.  The next part of the line should be whitespace,
		// followed by the uniform's name, possibly more whitespace, then a
		// semi-colon indicating the end of the line
		std::string SourceCopy( p_pSource );

		FPS_MEMSIZE UniformPosition = SourceCopy.find_first_of( "uniform" );

		while( UniformPosition == std::string::npos )
		{
			// Get the first set of non-whitespace characters before the semi-
			// colon
			FPS_MEMSIZE SemiColonPosition = SourceCopy.find_first_of( ';',
				UniformPosition );

			if( SemiColonPosition == std::string::npos )
			{
				// This can't be correct
				return FPS_FAIL;
			}

			// Find any arrays and stop progression; making it easier to
			// separate the uniform name instead of working backward from the
			// semi-colon and handling spaces, numbers inside the square
			// brackets and the square brackets
			FPS_MEMSIZE UniformNameEnd = SourceCopy.find_first_of( '[',
				UniformPosition );

			--UniformNameEnd;

			if( UniformNameEnd > SemiColonPosition )
			{
				UniformNameEnd = SourceCopy.find_last_not_of( " \t",
					UniformNameEnd );
			}

			FPS_MEMSIZE UniformNameStart = SourceCopy.find_last_of( " \t",
				UniformNameEnd );

			std::pair< std::map< std::string, GLuint >::iterator, bool >
				InsertResult;

			InsertResult = m_UniformLocationMap.insert(
				std::pair< std::string, GLuint >(
					SourceCopy.substr( UniformNameStart,
					( UniformNameEnd - UniformNameStart ) + 1 ), 0 ) );

			if( InsertResult.second == false )
			{
				std::cout << "[FPS::Shader::ExtractUniformNames] <WARN> "
					"Duplicate value for " <<
					SourceCopy.substr( UniformNameStart,
						( UniformNameEnd - UniformNameStart ) + 1 ) <<
					" found" << std::endl;
			}

			UniformPosition = SourceCopy.find_first_of( "uniform",
				SemiColonPosition );
		}
		
		return FPS_OK;
	}
}

