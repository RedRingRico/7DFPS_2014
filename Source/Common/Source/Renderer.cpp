#include <Renderer.hpp>
#include <PolygonCache.hpp>
#include <Memory.hpp>
#include <iostream>

namespace FPS
{
	Renderer::Renderer( ) :
		m_pWindow( FPS_NULL ),
		m_GLContext( FPS_NULL ),
		m_pPolygonCache( FPS_NULL )
	{
	}

	Renderer::~Renderer( )
	{
		SDL_GL_DeleteContext( m_GLContext );
		SafeDelete< PolygonCache >( m_pPolygonCache );
	}

	FPS_UINT32 Renderer::Initialise( SDL_Window *p_pWindow )
	{
		if( p_pWindow == FPS_NULL )
		{
			return FPS_FAIL;
		}

		m_pWindow = p_pWindow;

		m_GLContext = SDL_GL_CreateContext( m_pWindow );

		glewExperimental = GL_TRUE;
		GLenum GLEWError = glewInit( );

		if( GLEWError != GLEW_OK )
		{
			std::cout << "[7DFPS::Renderer::Initialise] <ERROR> Failed to "
				"initialise GLEW" << std::endl;

			return FPS_FAIL;
		}

		std::cout << "[7DFPS::Renderer::Initialise] <INFO> GLEW Version: " <<
			glewGetString( GLEW_VERSION ) << std::endl;

		std::cout << "[7DFPS::Renderer::Initialise] <INFO> Checking for "
			"required OpenGL extensions" << std::endl;

		std::cout << "\tARB_texture_storage... ";
		if( GLEW_ARB_texture_storage == GL_TRUE )
		{
			std::cout << "[OK]" << std::endl;
		}
		else
		{
			std::cout << "[!!]" << std::endl;
			return FPS_FAIL;
		}

		glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_GEQUAL );

		m_pPolygonCache = new PolygonCache( );

		return FPS_OK;
	}

	FPS_UINT32 Renderer::RegisterPolygons( const FPS_MEMSIZE p_VertexCount,
		const FPS_MEMSIZE p_IndexCount, const FPS_BYTE *p_pVertices,
		const FPS_UINT16 *p_pIndices, const GLenum p_PrimitiveType,
		const FPS_UINT64 p_VertexAttributes, FPS_UINT32 &p_CacheID )
	{
		return m_pPolygonCache->AddPolygons( p_VertexCount, p_IndexCount,
			p_pVertices, p_pIndices, p_PrimitiveType, p_VertexAttributes,
			p_CacheID );
	}

	FPS_UINT32 Renderer::RenderPolygons( const FPS_UINT32 p_CacheID )
	{
		return m_pPolygonCache->Render( p_CacheID );
	}

	void Renderer::SetClearColour( const FPS_FLOAT32 p_Red,
		const FPS_FLOAT32 p_Green, const FPS_FLOAT32 p_Blue )
	{
		glClearColor( p_Red, p_Green, p_Blue, 1.0f );
	}

	void Renderer::Clear( const FPS_BOOL p_ColourBuffer,
		const FPS_BOOL p_DepthBuffer, const FPS_BOOL p_StencilBuffer )
	{
		GLbitfield Flags = 0;

		if( p_ColourBuffer )
		{
			Flags |= GL_COLOR_BUFFER_BIT;
		}

		if( p_DepthBuffer )
		{
			Flags |= GL_STENCIL_BUFFER_BIT;
		}

		if( p_StencilBuffer )
		{
			Flags |= GL_STENCIL_BUFFER_BIT;
		}

		glClear( Flags );
	}

	void Renderer::SwapBuffers( )
	{
		SDL_GL_SwapWindow( m_pWindow );		
	}
}

