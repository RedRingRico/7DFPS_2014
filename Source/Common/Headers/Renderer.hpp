#ifndef __FPS_RENDERER_HPP__
#define __FPS_RENDERER_HPP__

#include <DataTypes.hpp>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>

namespace FPS
{
	class PolygonCache;

	enum GBUFFER_TYPE
	{
		GBUFFER_TYPE_POSITION = 0,
		GBUFFER_TYPE_ALBEDO,
		GBUFFER_TYPE_TEXCOORD,
		GBUFFER_TOTAL
	};

	class Renderer
	{
	public:
		Renderer( );
		~Renderer( );

		FPS_UINT32 Initialise( SDL_Window *p_pWindow );

		FPS_UINT32 RegisterPolygons( const FPS_MEMSIZE p_VertexCount,
			const FPS_MEMSIZE p_IndexCount, const FPS_BYTE *p_pVertices,
			const FPS_UINT16 *p_pIndices, const GLenum p_PrimitiveType,
			const FPS_UINT64 p_VertexAttributes, FPS_UINT32 &p_CacheID );

		FPS_UINT32 GBufferBegin( );
		void GBufferEnd( );

		FPS_UINT32 RenderPolygons( const FPS_UINT32 p_CacheID );

		void SetClearColour( const FPS_FLOAT32 p_Red,
			const FPS_FLOAT32 p_Green, const FPS_FLOAT32 p_Blue );

		void Clear( const FPS_BOOL p_ColourBuffer,
			const FPS_BOOL p_DepthBuffer, const FPS_BOOL p_StencilBuffer );
		void SwapBuffers( );

	private:
		SDL_Window		*m_pWindow;
		SDL_GLContext	m_GLContext;
		PolygonCache	*m_pPolygonCache;

		GLuint			m_GBuffer[ GBUFFER_TOTAL ];
		GLuint			m_DepthTexture;
		GLuint			m_Framebuffer;
	};
}

#endif // __FPS_RENDERER_HPP__

