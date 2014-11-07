#ifndef __FPS_GAME_HPP__
#define __FPS_GAME_HPP__

#include <DataTypes.hpp>

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
	};
}

#endif // __FPS_GAME_HPP__

