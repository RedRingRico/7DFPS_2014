#include <iostream>
#include <Game.hpp>

int main( int p_Argc, char **p_ppArgv )
{
	std::cout << "7DFPS Game by Red Ring Rico" << std::endl;

	FPS::Game TheGame;

	if( TheGame.Initialise( ) != FPS_OK )
	{
		std::cout << "[7DFPS::main] <ERROR> Something went wrong initialising "
			"the game" << std::endl;

		return FPS_FAIL;
	}

	return TheGame.Execute( );
}

