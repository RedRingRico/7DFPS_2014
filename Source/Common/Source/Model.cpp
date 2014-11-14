#include <Model.hpp>
#include <Renderer.hpp>
#include <Memory.hpp>
#include <iostream>
#include <cstdio>

namespace FPS
{
	Model::Model( Renderer *p_pRenderer ) :
		m_pRenderer( p_pRenderer ),
		m_pMeshData( FPS_NULL ),
		m_MeshCount( 0U )
	{
	}

	Model::~Model( )
	{
		SafeDeleteArray< MESH_DATA >( m_pMeshData );
	}

	FPS_UINT32 Model::LoadFromFile( const std::string &p_FileName )
	{
		FILE *pModelFile = fopen( p_FileName.c_str( ), "rb" );

		if( pModelFile == FPS_NULL )
		{
			std::cout << "[FPS::Model::LoadFromFile] <ERROR> Failed to open "
				"file " << p_FileName << " for reading" << std::endl;

			return FPS_FAIL;
		}

		fseek( pModelFile, 0, SEEK_END );
		FPS_UINT32 BytesToGo = ftell( pModelFile );
		rewind( pModelFile );

		// TODO
		// Get the header
		// Then get the meta data
		// Then the meshes
		// Finally, the joints
		MODEL_HEADER ModelHeader;
		fread( &ModelHeader, sizeof( ModelHeader ), 1, pModelFile );

		BytesToGo -= sizeof( ModelHeader );

		m_MeshCount = ModelHeader.MeshCount;
		m_pMeshData = new MESH_DATA[ m_MeshCount ];

		FILE_CHUNK Chunk;
		fread( &Chunk, sizeof( FILE_CHUNK ), 1, pModelFile );

		FPS_UINT32 ChunkSkip = sizeof( FILE_CHUNK );

		// Get chunks
		while( BytesToGo )
		{
			BytesToGo -= ChunkSkip;
			if( ReadChunk( Chunk ) != FPS_OK )
			{
				return FPS_FAIL;
			}
			BytesToGo -= Chunk.Size;
		}

		if( pModelFile )
		{
			fclose( pModelFile );
			pModelFile = FPS_NULL;
		}

		return FPS_FAIL;
	}

	FPS_UINT32 Model::Render( )
	{
		std::cout << "[FPS::Model::Render] <ERROR> NOT IMPLEMENTED" <<
			std::endl;

		return FPS_FAIL;
	}

	FPS_UINT32 Model::ReadChunk( FILE_CHUNK &p_Chunk )
	{
		std::cout << "[FPS::Model::ReadChunk] <ERROR> NOT IMPLEMENTED" <<
			std::endl;

		return FPS_FAIL;
	}
}

