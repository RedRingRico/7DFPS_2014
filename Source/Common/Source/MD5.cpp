#include <MD5.hpp>
#include <cstring>
#include <sstream>

namespace FPS
{
	FPS_BOOL MD5Equal( const MD5_DIGEST &p_Left, const MD5_DIGEST &p_Right )
	{
		for( FPS_MEMSIZE Index = 0; Index < 16; ++Index )
		{
			if( p_Left.Digest[ Index ] != p_Right.Digest[ Index ] )
			{
				return FPS_FALSE;
			}
		}

		return FPS_TRUE;
	}

	void MD5Zero( MD5_DIGEST &p_Digest )
	{
		memset( &p_Digest, 0, sizeof( p_Digest ) );
	}

	std::string MD5AsString( const MD5_DIGEST &p_Digest )
	{
		std::stringstream Stream;
		for( FPS_MEMSIZE i = 0; i < 16; ++i )
		{
			Stream << std::hex << static_cast< int >( p_Digest.Digest[ i ] );
		}

		return Stream.str( );
	}

	bool operator<( const MD5_DIGEST &p_Left, const MD5_DIGEST &p_Right )
	{
		// Keep comparing bytes until one of them is smaller than the other
		for( FPS_MEMSIZE i = 0; i < 16; ++i )
		{
			if( p_Left.Digest[ i ] == p_Right.Digest[ i ] )
			{
				continue;
			}

			if( p_Left.Digest[ i ] < p_Right.Digest[ i ] )
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		return true;
	}
}

