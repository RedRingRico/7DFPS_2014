#include <Shader.hpp>
#include <Memory.hpp>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

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
		void *p_pValue )
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

			// Step 1: Get the type
			FPS_MEMSIZE TypeStart = SourceCopy.find_first_of( " \t",
				UniformPosition );
			// Get rid of any leading whitespace
			TypeStart = SourceCopy.find_first_not_of( " \t", TypeStart );
			FPS_MEMSIZE TypeEnd = SourceCopy.find_first_of( " \t", TypeStart );

			std::string Type = SourceCopy.substr( TypeStart,
				TypeEnd - TypeStart );

			SHADER_PARAMETER ShaderParameter =
				{ SHADER_PARAMETER_TYPE_UNKNOWN, 1, 0 };

			if( Type.compare( "float" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_FLOAT1;
			}
			else if( Type.compare( "vec2" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_FLOAT2;
			}
			else if( Type.compare( "vec3" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_FLOAT3;
			}
			else if( Type.compare( "vec4" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_FLOAT4;
			}
			else if( Type.compare( "bool" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_BOOL1;
			}
			else if( Type.compare( "bvec2" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_BOOL2;
			}
			else if( Type.compare( "bvec3" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_BOOL3;
			}
			else if( Type.compare( "bvec4" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_BOOL4;
			}
			else if( Type.compare( "int" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_INT1;
			}
			else if( Type.compare( "ivec2" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_INT2;
			}
			else if( Type.compare( "ivec3" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_INT3;
			}
			else if( Type.compare( "ivec4" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_INT4;
			}
			else if( Type.compare( "uint" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_UINT1;
			}
			else if( Type.compare( "uvec2" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_UINT2;
			}
			else if( Type.compare( "uvec3" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_UINT3;
			}
			else if( Type.compare( "uvec4" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_UINT4;
			}
			else if( Type.compare( "double" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_DOUBLE1;
			}
			else if( Type.compare( "dvec2" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_DOUBLE2;
			}
			else if( Type.compare( "dvec3" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_DOUBLE3;
			}
			else if( Type.compare( "dvec4" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_DOUBLE4;
			}
			else if(	( Type.compare( "mat2" ) == 0 ) ||
						( Type.compare( "mat2x2" ) == 0 ) )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_MATRIX2X2;
			}
			else if(	( Type.compare( "mat3" ) == 0 ) ||
						( Type.compare( "mat3x3" ) == 0 ) )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_MATRIX3X3;
			}
			else if(	( Type.compare( "mat4" ) == 0 ) ||
						( Type.compare( "mat4x4" ) == 0 ) )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_MATRIX4X4;
			}
			else
			{
				std::cout << "[FPS::Shader::ExtractUniformNames] <ERROR> "
					"Unknown parameter type: '" << Type << "'" << std::endl;

				return FPS_FAIL;
			}

			// Step 2: Find the name
			FPS_MEMSIZE UniformNameStart = SourceCopy.find_first_not_of( " \t",
				TypeEnd );
			UniformNameStart = SourceCopy.find_first_not_of( " \t",
				UniformNameStart );
			FPS_MEMSIZE UniformNameEnd = SourceCopy.find_first_of( " [;\t",
				UniformNameStart );

			// Step 3: If we're not finished, check for an array and extract
			// the dimensions
			if( UniformNameEnd != SemiColonPosition )
			{
				FPS_MEMSIZE ArrayStart = SourceCopy.find_first_of( '[',
					UniformPosition );
				ArrayStart = SourceCopy.find_first_not_of( " \t", ArrayStart );
				FPS_MEMSIZE ArrayEnd = SourceCopy.find_first_of( ']',
					ArrayStart );

				if( ArrayStart < SemiColonPosition )
				{
					if( ( ArrayEnd != std::string::npos ) &&
						( ArrayEnd < SemiColonPosition ) )
					{
						FPS_MEMSIZE ArrayEndTmp = SourceCopy.find_first_of(
							" \t", ArrayStart, ArrayEnd - ArrayStart );

						if( ArrayEndTmp != std::string::npos )
						{
							ArrayEnd = ArrayEndTmp;
						}
					}
				}
				else
				{
					// There shouldn't be a start of an array without the end
					// before the semi-colon
					std::cout << "[FPS::Shader::ExtractUniformNames] <ERROR> "
						"Incorrect array format.  End bracket was not found "
						"before the line end" << std::endl;
					return FPS_FAIL;
				}

				std::string ArrayString = SourceCopy.substr( ArrayStart,
					ArrayEnd - ArrayStart );
				ShaderParameter.ArraySize = atoi( ArrayString.c_str( ) );
			}

			std::pair< std::map< std::string,
				SHADER_PARAMETER >::iterator, bool > InsertResult;

			InsertResult = m_UniformLocationMap.insert(
				std::pair< std::string, SHADER_PARAMETER >(
					SourceCopy.substr( UniformNameStart,
					( UniformNameEnd - UniformNameStart ) + 1 ),
					ShaderParameter ) );

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

