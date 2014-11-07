#ifndef __FPS_GAME_HPP__
#define __FPS_GAME_HPP__

#include <DataTypes.hpp>
#include <SDL2/SDL.h>

namespace FPS
{
	class Game
	{
	public:
		Game( );
		~Game( );

		FPS_UINT32 Initialise( );

		FPS_UINT32 Execute( );

	private:
		FPS_UINT32 PlatformInitialise( );

		SDL_Window		*m_pWindow;
		SDL_GLContext	m_GLContext;
	};
}

#endif // __FPS_GAME_HPP__

