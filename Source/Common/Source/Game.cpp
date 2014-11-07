#include <Game.hpp>
#include <iostream>

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

		return FPS_OK;
	}

	FPS_UINT32 Game::Execute( )
	{
		return FPS_OK;
	}
}

