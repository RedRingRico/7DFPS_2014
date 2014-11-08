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
		FPS_UINT32	MaterialID;
		GLuint		VertexBufferID;
		GLuint		IndexBufferID;
		GLuint		VertexArrayID;
		GLenum		PrimitiveType;
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

		FPS_UINT32 AddPolygons( const FPS_MEMSIZE p_VertexCount,
			const FPS_MEMSIZE p_IndexCount, const FPS_BYTE *p_pVertices,
			const FPS_UINT16 *p_pIndices, const FPS_UINT32 p_MaterialID,
			GLenum p_PrimitiveType );

		void FlushLine( const FPS_MEMSIZE p_Index );
		FPS_MEMSIZE FlushFullestLine( );
		void FlushAllLines( );

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

