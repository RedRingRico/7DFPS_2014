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

	void PolygonCache::FlushLine( const FPS_MEMSIZE p_Index )
	{
		if( p_Index > ( m_CacheLines - 1 ) )
		{
			return;
		}

		// Flush the line (call OpenGL's draw function)
	}

	void PolygonCache::FlushFullest( )
	{
		FPS_MEMSIZE LargestCache = m_pCache[ 0 ].PolygonCount;

		for( FPS_MEMSIZE Index = 1; Index < m_CacheLines; ++Index )
		{
			if( m_pCache[ Index ].PolygonCount > LargestCache )
			{
				LargestCache = m_pCache[ Index ].PolygonCount;
			}
		}

		this->FlushLine( LargestCache );
	}

	void PolygonCache::FlushAll( )
	{
		for( FPS_MEMSIZE Index = 0; Index < m_CacheLines; ++Index )
		{
			this->FlushLine( Index );
		}
	}
}

