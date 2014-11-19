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

		GLenum Error = GL_NO_ERROR;
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

		glGenFramebuffers( 1, &m_Framebuffer );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_Framebuffer );

		glGenTextures( sizeof( m_GBuffer ) / sizeof( m_GBuffer[ 0 ] ),
			m_GBuffer );
		glGenTextures( 1, &m_DepthTexture );

		for( FPS_MEMSIZE i = 0; i < GBUFFER_TOTAL; ++i )
		{
			glBindTexture( GL_TEXTURE_2D, m_GBuffer[ i ] );
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, 800, 600, 0, GL_RGBA,
				GL_FLOAT, FPS_NULL );
			glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_GBuffer[ i ], 0 );
		}

		glBindTexture( GL_TEXTURE_2D, m_DepthTexture );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, 800, 600, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, FPS_NULL );
		glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_TEXTURE_2D, m_DepthTexture, 0 );

		GLenum DrawBuffers[ ] =
		{
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2
		};

		glDrawBuffers( sizeof( DrawBuffers ) / sizeof( DrawBuffers[ 0 ] ),
			DrawBuffers );

		Error = glCheckFramebufferStatus( GL_FRAMEBUFFER );

		if( Error != GL_FRAMEBUFFER_COMPLETE )
		{
			std::cout << "[FPS::Renderer::Initialise] <ERROR> Framebuffer "
				"creation error: 0x" << std::hex <<
				static_cast< int >( Error ) << std::dec << std::endl;
			return FPS_FAIL;
		}

		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );

		glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_GEQUAL );

		m_pPolygonCache = new PolygonCache( );

		return FPS_OK;
	}

	FPS_UINT32 Renderer::GBufferBegin( )
	{
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_Framebuffer );

		glClearColor( 1.0f, 0.0f, 0.0f, 1.0f );

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		return FPS_OK;
	}

	void Renderer::GBufferEnd( )
	{
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );

		glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glBindFramebuffer( GL_READ_FRAMEBUFFER, m_Framebuffer );

		GLsizei HalfWidth = 400;
		GLsizei HalfHeight = 300;

		glReadBuffer( GL_COLOR_ATTACHMENT0 );
		glBlitFramebuffer( 0, 0, 800, 600, 0, 0, HalfWidth, HalfHeight,
			GL_COLOR_BUFFER_BIT, GL_LINEAR );
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

