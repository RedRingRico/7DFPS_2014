#include <MD5.hpp>
#include <cstring>

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
}

