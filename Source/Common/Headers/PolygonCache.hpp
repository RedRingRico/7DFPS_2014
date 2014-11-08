#ifndef __FPS_POLYGONCACHE_HPP__
#define __FPS_POLYGONCACHE_HPP__

#include <DataTypes.hpp>
#include <GL/glew.h>
#include <GL/gl.h>

namespace FPS
{
	typedef struct _POLYGONCACHE
	{
		FPS_BYTE	*pVertices;
		FPS_UINT16	*pIndices;
		FPS_MEMSIZE	VertexCount;
		FPS_MEMSIZE	IndexCount;
		FPS_MEMSIZE	PolygonCount;
		GLuint		VertexBufferID;
		GLuint		IndexBufferID;
		GLuint		VertexArrayID;
	}POLYGONCACHE,*PPOLYGONCACHE;

	class PolygonCache
	{
	public:
		PolygonCache( );
		~PolygonCache( );

		FPS_UINT32 Create( const FPS_MEMSIZE p_VertexCount,
			const FPS_MEMSIZE p_IndexCount, const FPS_MEMSIZE p_CacheLines,
			const FPS_UINT64 p_VertexAttributes );
		
		FPS_UINT32 Destroy( );

		void FlushLine( const FPS_MEMSIZE p_Index );
		void FlushFullest( );
		void FlushAll( );

	private:
		POLYGONCACHE	*m_pCache;
		FPS_MEMSIZE		m_CacheLines;
		FPS_UINT64		m_VertexAttributes;
		FPS_MEMSIZE		m_VertexAttributeCount;
		FPS_MEMSIZE		m_Stride;
		FPS_MEMSIZE		m_VertexCapacity;
		FPS_MEMSIZE		m_IndexCapacity;
	};
}

#endif // __FPS_POLYGONCACHE_HPP__

