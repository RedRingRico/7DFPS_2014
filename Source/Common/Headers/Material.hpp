#ifndef __FPS_MATERIAL_HPP__
#define __FPS_MATERIAL_HPP__

#include <DataTypes.hpp>
#include <MD5.hpp>
#include <list>
#include <string>

namespace FPS
{
	class Material
	{
	public:
		Material( );
		~Material( );

		FPS_UINT32 CreateFromFile( const std::string &p_FileName );

		FPS_UINT32 Apply( );

	private:
		MD5_DIGEST	m_MD5Digest;
		MD5_DIGEST	m_ShaderMD5Digest;
		MD5_DIGEST	m_TextureMD5Digest;

		std::list< std::string >	m_ShaderParameters;
	};
}

#endif // __FPS_MATERIAL_HPP__

