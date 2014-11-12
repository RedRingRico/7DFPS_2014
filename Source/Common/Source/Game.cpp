#include <Game.hpp>
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <MaterialManager.hpp>
#include <PolygonCache.hpp>
#include <MD5.hpp>

namespace FPS
{
	Game::Game( )
	{
	}

	Game::~Game( )
	{
		SDL_GL_DeleteContext( m_GLContext );
		SDL_DestroyWindow( m_pWindow );
		SDL_Quit( );
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

		GLenum GLEWError = glewInit( );

		if( GLEWError != GLEW_OK )
		{
			std::cout << "[7DFPS::Game::Initialise] <ERROR> Failed to "
				"initialise GLEW" << std::endl;

			return FPS_FAIL;
		}

		std::cout << "[7DFPS::Game::Initialise] <INFO> GLEW Version: " <<
			glewGetString( GLEW_VERSION ) << std::endl;

		if( m_Renderer.Initialise( m_pWindow ) != FPS_OK )
		{
			return FPS_FAIL;
		}

		m_Renderer.SetClearColour( 118.0f / 255.0f, 185.0f / 255.0f, 0.0f );

		return FPS_OK;
	}

	FPS_UINT32 Game::Execute( )
	{
		FPS_BOOL Run = FPS_TRUE;
		SDL_Event Event;

		MaterialManager MatMan;
		MD5_DIGEST Digest;

		std::string MaterialFile =
			"Content/Raw/Materials/SolidColour.material";
		
		if( MatMan.CreateMaterial(
			"Content/Raw/Materials/SolidColour.material", Digest ) != FPS_OK )
		{
			return FPS_FAIL;
		}

		FPS_FLOAT32 Vertices[ ] =
		// Position | Colour
		{
			-10.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			10.0f, 10.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			10.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
		};

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

			m_Renderer.Clear( FPS_TRUE, FPS_TRUE, FPS_TRUE );
			m_Renderer.SwapBuffers( );
		}

		return FPS_OK;
	}
}

