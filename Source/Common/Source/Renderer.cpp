#include <Renderer.hpp>
#include <GL/gl.h>

namespace FPS
{
	Renderer::Renderer( )
	{
	}

	Renderer::~Renderer( )
	{
	}

	FPS_UINT32 Renderer::Initialise( SDL_Window *p_pWindow )
	{
		if( p_pWindow == FPS_NULL )
		{
			return FPS_FAIL;
		}

		m_pWindow = p_pWindow;

		return FPS_OK;
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

