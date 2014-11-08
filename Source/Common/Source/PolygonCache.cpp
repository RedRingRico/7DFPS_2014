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
			m_pCache[ Index ].PolygonCount = 0;
			m_pCache[ Index ].MaterialID = 0;
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

	FPS_UINT32 PolygonCache::AddPolygons( const FPS_MEMSIZE p_VertexCount,
		const FPS_MEMSIZE p_IndexCount, const FPS_BYTE *p_pVertices,
		const FPS_UINT16 *p_pIndices, const FPS_UINT32 p_MaterialID,
		GLenum p_PrimitiveType )
	{
		FPS_MEMSIZE CacheLine = 0;
		FPS_BOOL MaterialMatch = FPS_FALSE;
		// Find a cache with the same material ID, if there isn't one, flush
		// the fullest line and add it
		for( FPS_MEMSIZE Index = 0; Index < m_CacheLines; ++Index )
		{
			if( ( m_pCache[ Index ].MaterialID == p_MaterialID ) &&
				( m_pCache[ Index ].PrimitiveType == p_PrimitiveType ) )
			{
				CacheLine = Index;
				MaterialMatch = FPS_TRUE;
				break;
			}

			if( m_pCache[ Index ].MaterialID == 0 )
			{
				CacheLine = Index;
				MaterialMatch = FPS_TRUE;
				break;
			}
		}

		if( MaterialMatch == FPS_FALSE )
		{
			CacheLine = this->FlushFullestLine( );
		}

		if( p_VertexCount > m_VertexCapacity )
		{
			return FPS_FAIL;
		}

		if( p_IndexCount > m_IndexCapacity )
		{
			return FPS_FAIL;
		}

		if( ( p_VertexCount + m_pCache[ CacheLine ].VertexCount ) >
			m_VertexCapacity )
		{
			this->FlushLine( CacheLine );
		}

		if( ( p_IndexCount + m_pCache[ CacheLine ].IndexCount ) >
			m_IndexCapacity )
		{
			this->FlushLine( CacheLine );
		}

		glBindVertexArray( m_pCache[ CacheLine ].VertexArrayID );

		glBufferSubData( GL_ARRAY_BUFFER,
			m_pCache[ CacheLine ].VertexCount * m_Stride,
			p_VertexCount * m_Stride, p_pVertices );

		// Offset the indices to start at the VertexCount
		FPS_UINT16 *pIndices = new FPS_UINT16[ p_IndexCount ];
		for( FPS_MEMSIZE Index = 0; Index < p_IndexCount; ++Index )
		{
			pIndices[ Index ] = p_pIndices[ Index ] +
				m_pCache[ CacheLine ].VertexCount;
		}

		glBufferSubData( GL_ELEMENT_ARRAY_BUFFER,
			m_pCache[ CacheLine ].IndexCount * sizeof( FPS_UINT16 ),
			p_IndexCount * sizeof( FPS_UINT16 ), pIndices );

		SafeDeleteArray< FPS_UINT16 >( pIndices );

		m_pCache[ CacheLine ].VertexCount += p_VertexCount;
		m_pCache[ CacheLine ].IndexCount += p_IndexCount;
		FPS_MEMSIZE PolygonCount = 0;
		switch( p_PrimitiveType )
		{
			case GL_TRIANGLES:
			{
				PolygonCount = p_IndexCount / 3;
				break;
			}
			case GL_TRIANGLE_STRIP:
			case GL_TRIANGLE_FAN:
			{
				PolygonCount = p_IndexCount - 2;
				break;
			}
			case GL_LINES:
			{
				PolygonCount = p_IndexCount / 2;
				break;
			}
			case GL_LINE_STRIP:
			{
				PolygonCount = p_IndexCount - 1;
				break;
			}
			case GL_LINE_LOOP:
			{
				PolygonCount = p_IndexCount;
				break;
			}
		}
		m_pCache[ CacheLine ].PolygonCount += PolygonCount;
		m_pCache[ CacheLine ].PrimitiveType = p_PrimitiveType;

		glBindVertexArray( 0 );

		return FPS_OK;
	}

	void PolygonCache::FlushLine( const FPS_MEMSIZE p_Index )
	{
		if( p_Index > ( m_CacheLines - 1 ) )
		{
			return;
		}

		glBindVertexArray( m_pCache[ p_Index ].VertexArrayID );

		glDrawElements( m_pCache[ p_Index ].PrimitiveType,
			m_pCache[ p_Index ].IndexCount, GL_UNSIGNED_SHORT,
			( GLubyte * )FPS_NULL + 0 );

		glBindVertexArray( 0 );

		m_pCache[ p_Index ].VertexCount = 0;
		m_pCache[ p_Index ].IndexCount = 0;
		m_pCache[ p_Index ].PolygonCount = 0;
		m_pCache[ p_Index ].MaterialID = 0;
	}

	FPS_MEMSIZE PolygonCache::FlushFullestLine( )
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

		return LargestCache;
	}

	void PolygonCache::FlushAllLines( )
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

