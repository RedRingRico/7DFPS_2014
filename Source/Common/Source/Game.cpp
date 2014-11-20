#include <Game.hpp>
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <MaterialManager.hpp>
#include <PolygonCache.hpp>
#include <MD5.hpp>
#include <Matrix4x4.hpp>
#include <Vector4.hpp>
#include <Maths.hpp>

namespace FPS
{
	Game::Game( )
	{
	}

	Game::~Game( )
	{
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
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK,
			SDL_GL_CONTEXT_PROFILE_CORE );

		SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 32 );
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
		SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
		SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );
		SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
		SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
		SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
		SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );



		m_pWindow = SDL_CreateWindow( "Red Ring Rico's 7DFPS", 0, 0, 800, 600,
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );

		if( !m_pWindow )
		{
			std::cout << "[7DFPS::Game::Initialise] <ERROR> Could not create "
				"an SDL window" << std::endl;

			return FPS_FAIL;
		}

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
			"Content/Raw/Materials/TexturedSimple.material";
		
		if( MatMan.CreateMaterial( MaterialFile, Digest ) != FPS_OK )
		{
			return FPS_FAIL;
		}

		FPS_FLOAT32 Vertices[ ] =
		// Position | Colour
		/*{
			-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f
		};*/
		// Position | ST
		{
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f
		};

		FPS_UINT16 Indices[ ] =
		{
			0, 1, 2
		};

		FPS_UINT32 PolygonID;

		m_Renderer.RegisterPolygons( 3, 3,
			reinterpret_cast< const FPS_BYTE * >( Vertices ), Indices,
			GL_TRIANGLES, /*0x66*/0x56, PolygonID );

		Matrix4x4 View, Projection, World, WorldRotateZ, WorldTranslate;
		Vector4 Translation( 1.0f, 0.0f, 0.0f, 1.0f );

		Projection.CreatePerspectiveFOV( 45.0f, 800.0f / 600.0f, 1.0f,
			100000.0f );

		Vector4 EyePosition, LookPoint, WorldUp;

		WorldUp.Set( 0.0f, 1.0f, 0.0f, 1.0f );
		// Look down the Z axis
		LookPoint.Set( 0.0f, 0.0f, -1.0f, 1.0f );

		EyePosition.Set( 0.0f, 0.0f, 100.0f, 1.0f );

		FPS_FLOAT32 Rotate = 0.0f;

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

			FPS_FLOAT32 ViewRaw[ 16 ], ProjectionRaw[ 16 ], WorldRaw[ 16 ];

			// Recreate the view matrix
			View.CreateViewLookAt( EyePosition, LookPoint, WorldUp );

			Rotate += 0.01f;
			WorldRotateZ.RotateZ( Rotate );

			View.GetAsFloatArray( ViewRaw );
			Projection.GetAsFloatArray( ProjectionRaw );

			m_Renderer.Clear( FPS_TRUE, FPS_TRUE, FPS_TRUE );
			m_Renderer.GBufferBegin( );
			FPS_FLOAT32 YPosition = 10.0f;
			for( FPS_MEMSIZE i = 0; i < 10; ++i )
			{
				FPS_FLOAT32 XPosition = -10.0f;
				for( FPS_MEMSIZE j = 0; j < 10; ++j )
				{
					Translation.SetX( XPosition );
					Translation.SetY( YPosition );
					WorldTranslate.Translate( Translation );

					World = WorldTranslate * WorldRotateZ;

					World.GetAsFloatArray( WorldRaw );

					MatMan.SetShaderParameter( Digest, "u_ViewMatrix",
						ViewRaw );
					MatMan.SetShaderParameter( Digest, "u_ProjectionMatrix",
						ProjectionRaw );
					MatMan.SetShaderParameter( Digest, "u_WorldMatrix",
						WorldRaw );
					MatMan.ApplyMaterial( Digest );
					m_Renderer.RenderPolygons( PolygonID );
					XPosition += 2.0f;
				}
				YPosition -= 2.0f;
			}
			m_Renderer.GBufferEnd( );
			m_Renderer.SwapBuffers( );
		}

		return FPS_OK;
	}
}

