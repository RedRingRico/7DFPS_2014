#include <PolygonCache.hpp>
#include <Memory.hpp>

namespace FPS
{
	PolygonCache::PolygonCache( ) :
		m_pCache( FPS_NULL ),
		m_CacheLines( 0 )
	{
	}

	PolygonCache::~PolygonCache( )
	{
		this->Destroy( );
	}

	FPS_UINT32 PolygonCache::Create( const FPS_MEMSIZE p_VertexCount,
		const FPS_MEMSIZE p_IndexCount, const FPS_MEMSIZE p_CacheLines )
	{
		this->Destroy( );

		m_pCache = new POLYGONCACHE[ p_CacheLines ];

		for( FPS_MEMSIZE Index = 0; Index < p_CacheLines; ++Index )
		{
			m_pCache[ Index ].pVertices = new FPS_BYTE[ p_VertexCount ];
			m_pCache[ Index ].pIndices = new FPS_UINT16[ p_IndexCount ];
			m_pCache[ Index ].VertexCount = p_VertexCount;
			m_pCache[ Index ].IndexCount = p_IndexCount;
		}

		m_CacheLines = p_CacheLines;

		return FPS_OK;
	}

	FPS_UINT32 PolygonCache::Destroy( )
	{
		if( m_CacheLines )
		{
			for( FPS_MEMSIZE Index = 0; Index < m_CacheLines; ++Index )
			{
				SafeDeleteArray< FPS_BYTE >( m_pCache[ Index ].pVertices );
				SafeDeleteArray< FPS_UINT16 >( m_pCache[ Index ].pIndices  );
			}

			SafeDeleteArray< POLYGONCACHE >( m_pCache );
		}

		return FPS_OK;
	}
}

