#include <Game.hpp>
#include <iostream>
#include <GL/gl.h>

namespace FPS
{
	Game::Game( )
	{
	}

	Game::~Game( )
	{
	}

	FPS_UINT32 Game::Initialise( )
	{
		if( this->PlatformInitialise( ) != FPS_OK )
		{
			std::cout << "[7DFPS::Game::Initialise] <ERROR> Something went "
				"wrong initialising the platform-specific function" <<
				std::endl;

			return FPS_OK;
		}

		if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
		{
			std::cout << "[7DFPS::Game::Initialise] <ERROR> Failed to "
				"initialise SDL" << std::endl;

			return FPS_FAIL;
		}

		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );

		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
		SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );

		m_pWindow = SDL_CreateWindow( "Red Ring Rico's 7DFPS", 0, 0, 800, 600,
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );

		if( !m_pWindow )
		{
			std::cout << "[7DFPS::Game::Initialise] <ERROR> Could not create "
				"an SDL window" << std::endl;
			return FPS_FAIL;
		}

		m_GLContext = SDL_GL_CreateContext( m_pWindow );

		return FPS_OK;
	}

	FPS_UINT32 Game::Execute( )
	{
		FPS_BOOL Run = FPS_TRUE;
		SDL_Event Event;

		while( Run )
		{
			while( SDL_PollEvent( &Event ) != 0 )
			{
				if( Event.type == SDL_QUIT )
				{
					Run = FPS_FALSE;
				}
			}

			const Uint8 *pKeyState = SDL_GetKeyboardState( NULL );

			if( pKeyState[ SDL_SCANCODE_ESCAPE ] )
			{
				Run = FPS_FALSE;
			}

			glClearColor( 1.0f, 0.0f, 0.0f, 1.0f );
			glClear( GL_COLOR_BUFFER_BIT );
			SDL_GL_SwapWindow( m_pWindow );
		}

		SDL_GL_DeleteContext( m_GLContext );
		SDL_DestroyWindow( m_pWindow );
		SDL_Quit( );

		return FPS_OK;
	}
}

