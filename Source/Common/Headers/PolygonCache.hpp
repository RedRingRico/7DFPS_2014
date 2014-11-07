#ifndef __FPS_POLYGONCACHE_HPP__
#define __FPS_POLYGONCACHE_HPP__

#include <DataTypes.hpp>

namespace FPS
{
	typedef struct _POLYGONCACHE
	{
		FPS_BYTE	*pVertices;
		FPS_UINT16	*pIndices;
		FPS_MEMSIZE	VertexCount;
		FPS_MEMSIZE	IndexCount;
		FPS_MEMSIZE	PolygonCount;
	}POLYGONCACHE,*PPOLYGONCACHE;

	class PolygonCache
	{
	public:
		PolygonCache( );
		~PolygonCache( );

		FPS_UINT32 Create( const FPS_MEMSIZE p_VertexCount,
			const FPS_MEMSIZE p_IndexCount, const FPS_MEMSIZE p_CacheLines );
		
		FPS_UINT32 Destroy( );

		void FlushLine( const FPS_MEMSIZE p_Index );
		void FlushFullest( );
		void FlushAll( );

	private:
		POLYGONCACHE	*m_pCache;
		FPS_MEMSIZE		m_CacheLines;
	};
}

#endif // __FPS_POLYGONCACHE_HPP__

