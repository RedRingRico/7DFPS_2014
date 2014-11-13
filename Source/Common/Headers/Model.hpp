#ifndef __FPS_MODEL_HPP__
#define __FPS_MODEL_HPP__

#include <DataTypes.hpp>
#include <string>

namespace FPS
{
	class Renderer;
	class Matrix4x4;

	class Model
	{
	public:
		Model( Renderer *p_pRenderer );
		~Model( );

		FPS_UINT32 LoadFromFile( const std::string &p_FileName );

		FPS_UINT32 Render( );

	private:
		Renderer	*m_pRenderer;
	};
}

#endif // __FPS_MODEL_HPP__

