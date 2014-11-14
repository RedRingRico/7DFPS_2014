#ifndef __FPS_POLYGONCACHE_HPP__
#define __FPS_POLYGONCACHE_HPP__

#include <DataTypes.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>

namespace FPS
{
	typedef struct _POLYGONCACHE
	{
		FPS_UINT32	ID;
		FPS_MEMSIZE	Stride;
		FPS_MEMSIZE	VertexCount;
		FPS_MEMSIZE	IndexCount;
		FPS_MEMSIZE	PolygonCount;
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

		FPS_UINT32 AddPolygons( const FPS_MEMSIZE p_VertexCount,
			const FPS_MEMSIZE p_IndexCount, const FPS_BYTE *p_pVertices,
			const FPS_UINT16 *p_pIndices, GLenum p_PrimitiveType,
			const FPS_UINT64 p_VertexAttributes, FPS_UINT32 &p_CacheID );
		
		FPS_UINT32 Render( const FPS_UINT32 p_CacheID );

	private:
		std::vector< POLYGONCACHE >	m_Cache;

		FPS_UINT32 m_CacheID;
	};
}

#endif // __FPS_POLYGONCACHE_HPP__

