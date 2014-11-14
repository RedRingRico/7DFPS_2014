#ifndef __FPS_MODEL_HPP__
#define __FPS_MODEL_HPP__

#include <DataTypes.hpp>
#include <MD5.hpp>
#include <string>

namespace FPS
{
	class Renderer;
	class Matrix4x4;

	const FPS_UINT32 MESH_CHUNK		= 0x00000001;
	const FPS_UINT32 JOINT_CHUNK	= 0x00000002;

	typedef struct _tagMODEL_HEADER
	{
		char		ID[ 4 ];
		FPS_UINT32	MeshCount;
		FPS_UINT32	MaterialCount;
		FPS_UINT32	JointCount;
	}MODEL_HEADER,*PMODEL_HEADER;

	typedef struct _tagMESHL_DATA
	{
		FPS_UINT32	ID;
		MD5_DIGEST	MaterialMD5;
	}MESH_DATA,*PMESH_DATA;

	class Model
	{
	public:
		Model( Renderer *p_pRenderer );
		~Model( );

		FPS_UINT32 LoadFromFile( const std::string &p_FileName );

		FPS_UINT32 Render( );

	private:
		FPS_UINT32 ReadChunk( FILE_CHUNK &p_Chunk );

		Renderer	*m_pRenderer;
		MESH_DATA	*m_pMeshData;
		FPS_UINT32	m_MeshCount;
	};
}

#endif // __FPS_MODEL_HPP__

