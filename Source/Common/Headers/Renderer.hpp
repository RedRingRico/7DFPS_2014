#ifndef __FPS_RENDERER_HPP__
#define __FPS_RENDERER_HPP__

#include <DataTypes.hpp>
#include <SDL2/SDL.h>

namespace FPS
{
	class Renderer
	{
	public:
		Renderer( );
		~Renderer( );

		FPS_UINT32 Initialise( SDL_Window *p_pWindow );

		FPS_UINT32 RenderPolygons( const FPS_BYTE *p_pVertices,
			const FPS_MEMSIZE p_VertexCount, const FPS_UINT16 *p_pIndices,
			const FPS_MEMSIZE p_IndexCount,
			const FPS_UINT64 p_VertexAttributes );

		void SetClearColour( const FPS_FLOAT32 p_Red,
			const FPS_FLOAT32 p_Green, const FPS_FLOAT32 p_Blue );

		void Clear( const FPS_BOOL p_ColourBuffer,
			const FPS_BOOL p_DepthBuffer, const FPS_BOOL p_StencilBuffer );
		void SwapBuffers( );

	private:
		SDL_Window	*m_pWindow;
	};
}

#endif // __FPS_RENDERER_HPP__

