#ifndef __FPS_MEMORY_HPP__
#define __FPS_MEMORY_HPP__

#include <DataTypes.hpp>

namespace FPS
{
	template< typename T >
	void SafeDelete( T *&p_pPointer )
	{
		if( p_pPointer )
		{
			delete p_pPointer;
			p_pPointer = FPS_NULL;
		}
	}

	template< typename T >
	void SafeDeleteArray( T *&p_pPointer )
	{
		if( p_pPointer )
		{
			delete [ ] p_pPointer;
			p_pPointer = FPS_NULL;
		}
	}
}

#endif // __FPS_MEMORY_HPP__

