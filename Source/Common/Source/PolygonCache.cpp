#include <PolygonCache.hpp>
#include <Memory.hpp>

namespace FPS
{
	FPS_MEMSIZE GetAttributeCount( const FPS_UINT64 p_VertexAttributes );
	FPS_MEMSIZE AttributeToSize( const FPS_BYTE p_Attribute );

	PolygonCache::PolygonCache( ) :
		m_pCache( FPS_NULL ),
		m_CacheLines( 0 ),
		m_VertexArrayID( 0 ),
		m_VertexAttributes( 0ULL ),
		m_VertexAttributeCount( 0 ),
		m_Stride( 0 )
	{
	}

	PolygonCache::~PolygonCache( )
	{
		this->Destroy( );
	}

	FPS_UINT32 PolygonCache::Create( const FPS_MEMSIZE p_VertexCount,
		const FPS_MEMSIZE p_IndexCount, const FPS_MEMSIZE p_CacheLines,
		const FPS_UINT64 p_VertexAttributes )
	{
		this->Destroy( );

		m_pCache = new POLYGONCACHE[ p_CacheLines ];

		for( FPS_MEMSIZE Index = 0; Index < p_CacheLines; ++Index )
		{
			m_pCache[ Index ].pVertices = new FPS_BYTE[ p_VertexCount ];
			m_pCache[ Index ].pIndices = new FPS_UINT16[ p_IndexCount ];
			m_pCache[ Index ].VertexCount = p_VertexCount;
			m_pCache[ Index ].IndexCount = p_IndexCount;
			m_pCache[ Index ].PolygonCount = 0;
			m_pCache[ Index ].VertexBufferID = 0;
			m_pCache[ Index ].IndexBufferID = 0;
		}

		m_CacheLines = p_CacheLines;

		m_VertexAttributes = p_VertexAttributes;
		m_VertexAttributeCount = GetAttributeCount( m_VertexAttributes );

		m_Stride = 0;

		for( FPS_MEMSIZE Index = 0; Index < m_VertexAttributeCount; ++Index )
		{
			FPS_BYTE Attribute =
				( m_VertexAttributes >> ( Index * 4 ) ) & 0x0F;

			m_Stride += AttributeToSize( Attribute );
		}

		if( m_VertexArrayID )
		{
			glDeleteVertexArrays( 1, &m_VertexArrayID );
		}

		glGenVertexArrays( 1, &m_VertexArrayID );

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

		if( m_VertexArrayID )
		{
			glDeleteVertexArrays( 1, &m_VertexArrayID );
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

	FPS_MEMSIZE GetAttributeCount( const FPS_UINT64 p_VertexAttributes )
	{
		FPS_MEMSIZE Count = 0;

		for( FPS_MEMSIZE Index = 0; Index < 16; ++Index )
		{
			// Get the nybble going from the lowest to the highest
			FPS_BYTE Nybble = ( p_VertexAttributes >> ( Index * 4 ) ) & 0x0F;

			if( Nybble )
			{
				++Count;
			}
			else
			{
				break;
			}
		}

		return Count;
	}

	FPS_MEMSIZE AttributeToSize( const FPS_BYTE p_Attribute )
	{
		FPS_BYTE Type = p_Attribute >> 2;
		FPS_MEMSIZE Size = ( 0x03 & p_Attribute ) + 1;

		switch( Type )
		{
			case 0:
			{
				Size *= sizeof( FPS_SINT32 );
				break;
			}
			case 1:
			{
				Size *= sizeof( FPS_FLOAT32 );
				break;
			}
			case 2:
			{
				Size *= sizeof( FPS_FLOAT64 );
				break;
			}
			case 3:
			{
				Size *= Size;
				Size *= sizeof( FPS_FLOAT32 );
				break;
			}
		}

		return Size;
	}
}

