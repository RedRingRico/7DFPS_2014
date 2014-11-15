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

#pragma pack( 1 )
	typedef struct _tagMODEL_VERTEX
	{
		FPS_FLOAT32	Position[ 3 ];
		FPS_FLOAT32	Normal[ 3 ];
		FPS_FLOAT32	ST[ 2 ];
		FPS_UINT32	JointIndices[ 4 ];
		FPS_FLOAT32	JointWeights[ 4 ];
	}MODEL_VERTEX,*PMODEL_VERTEX;

	typedef struct _tagMODEL_HEADER
	{
		char		ID[ 4 ];
		FPS_UINT32	MeshCount;
		FPS_UINT32	MaterialCount;
		FPS_UINT32	JointCount;
	}MODEL_HEADER,*PMODEL_HEADER;

	typedef struct _tagMESH_INFORMATION
	{
		FPS_UINT32	VertexCount;
		FPS_UINT32	IndexCount;
		MD5_DIGEST	MaterialMD5;
	}MESH_INFORMATION,*PMESH_INFORMATION;

	typedef struct _tagMESHL_DATA
	{
		FPS_UINT32	ID;
		MD5_DIGEST	MaterialMD5;
	}MESH_DATA,*PMESH_DATA;
#pragma pack( )

	class Model
	{
	public:
		Model( Renderer *p_pRenderer );
		~Model( );

		FPS_UINT32 LoadFromFile( const std::string &p_FileName );

		FPS_UINT32 Render( );

	private:
		FPS_UINT32 ReadChunk( FILE_CHUNK &p_Chunk, FILE *p_pFile );
		FPS_UINT32 ReadMeshData( FILE *p_pFile, const FPS_UINT32 p_MeshIndex,
			FPS_UINT32 &p_BytesRead );

		Renderer	*m_pRenderer;
		MESH_DATA	*m_pMeshData;
		FPS_UINT32	m_MeshCount;
	};
}

#endif // __FPS_MODEL_HPP__

