#ifndef __FPS_RENDERER_HPP__
#define __FPS_RENDERER_HPP__

#include <DataTypes.hpp>

namespace FPS
{
	class Renderer
	{
	public:
		Renderer( );
		~Renderer( );

		FPS_UINT32 Initialise( );
		FPS_UINT32 RenderPolygons( const FPS_BYTE *p_pVertices,
			const FPS_MEMSIZE p_VertexCount, const FPS_UINT16 *p_pIndices,
			const FPS_MEMSIZE p_IndexCount,
			const FPS_UINT64 p_VertexAttributes );

	private:
	};
}

#endif // __FPS_RENDERER_HPP__

