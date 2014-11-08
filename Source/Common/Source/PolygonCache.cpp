#include <PolygonCache.hpp>
#include <Memory.hpp>

#define BUFFER_OFFSET( Offset ) ( ( char * )FPS_NULL + ( Offset ) )

namespace FPS
{
	FPS_MEMSIZE GetAttributeCount( const FPS_UINT64 p_VertexAttributes );
	FPS_MEMSIZE AttributeToSize( const FPS_BYTE p_Attribute );

	PolygonCache::PolygonCache( ) :
		m_pCache( FPS_NULL ),
		m_CacheLines( 0 ),
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

		m_VertexCapacity = p_VertexCount;
		m_IndexCapacity = p_IndexCount;

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

		m_pCache = new POLYGONCACHE[ p_CacheLines ];

		for( FPS_MEMSIZE Index = 0; Index < p_CacheLines; ++Index )
		{
			m_pCache[ Index ].pVertices = new FPS_BYTE[ p_VertexCount ];
			m_pCache[ Index ].pIndices = new FPS_UINT16[ p_IndexCount ];
			m_pCache[ Index ].VertexCount = p_VertexCount;
			m_pCache[ Index ].IndexCount = p_IndexCount;
			m_pCache[ Index ].IndexBufferID = 0;
			glGenBuffers( 1, &m_pCache[ Index ].VertexBufferID );
			glGenBuffers( 1, &m_pCache[ Index ].IndexBufferID );
			glGenVertexArrays( 1, &m_pCache[ Index ].VertexArrayID );

			glBindVertexArray( m_pCache[ Index ].VertexArrayID );
			glBindBuffer( GL_ARRAY_BUFFER, m_pCache[ Index ].VertexBufferID );
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,
				m_pCache[ Index ].IndexBufferID );

			FPS_MEMSIZE AccumulatedDimension = 0;

			for( FPS_MEMSIZE VertexAttributeIndex = 0;
				VertexAttributeIndex < m_VertexAttributeCount;
				++VertexAttributeIndex )
			{
				FPS_MEMSIZE Dimension = 0;
				FPS_MEMSIZE TypeSize = 0;
				GLenum Type = GL_INVALID_ENUM;
				FPS_BYTE VertexAttribute = 0x0F & ( m_VertexAttributes >> (
					VertexAttributeIndex * 4 ) );

				Dimension = 0x03 & ( ( VertexAttribute ) + 1 );

				switch( ( 0x0C & ( VertexAttribute ) ) >> 2 )
				{
					case 0:
					{
						Type = GL_INT;
						TypeSize = sizeof( FPS_SINT32 );
						break;
					}
					case 1:
					{
						Type = GL_FLOAT;
						TypeSize = sizeof( FPS_FLOAT32 );
						break;
					}
					case 2:
					{
						Type = GL_DOUBLE;
						TypeSize = sizeof( FPS_FLOAT64 );
						break;
					}
					case 3:
					{
						Type = GL_FLOAT;
						TypeSize = sizeof( FPS_FLOAT32 );
						Dimension *= Dimension;
						break;
					}
				}

				glVertexAttribPointer( VertexAttributeIndex, Dimension, Type,
					GL_FALSE, m_Stride,
					BUFFER_OFFSET( TypeSize * AccumulatedDimension ) );

				AccumulatedDimension += Dimension;
				glEnableVertexAttribArray( VertexAttributeIndex );
			}

			glBufferData( GL_ARRAY_BUFFER, m_VertexCapacity * m_Stride,
				FPS_NULL, GL_STREAM_DRAW );
			glBufferData( GL_ELEMENT_ARRAY_BUFFER,
				m_IndexCapacity * sizeof( FPS_UINT16 ), FPS_NULL,
				GL_STREAM_DRAW );

			glBindVertexArray( 0 );
		}

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

