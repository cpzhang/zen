#pragma once
template<typename BufferType, typename VertexType>
class BufferLocker
{
protected:
	void* vertices_;
	BufferType& vb_;
public:
	BufferLocker( BufferType& vb )
		: vertices_( 0 ), vb_( vb )
	{
		if( FAILED( vb.lock( 0, 0, &vertices_, 0 ) ) )
			vertices_ = NULL;// shouldn't it?
	}
	~BufferLocker()
	{
		if(vertices_)
			vb_.unlock();
	}
	BufferLocker( BufferType& vb, u32 offset, u32 size, DWORD flags )
		: vertices_( 0 ), vb_( vb )
	{
		if( FAILED( vb.lock( offset, size, &vertices_, flags ) ) )
			vertices_ = NULL;// shouldn't it?
	}
	operator void*() const
	{
		return vertices_;
	}
	void fill( const void* buffer, u32 size )
	{
		memcpy( vertices_, buffer, size );
	}
	void pull( void* buffer, u32 size ) const
	{
		memcpy( buffer, vertices_, size );
	}
	VertexType& operator[]( int index )
	{
		return ((VertexType*)vertices_)[ index ];
	}
	VertexType& operator[]( int index ) const
	{
		return ((VertexType*)vertices_)[ index ];
	}
};
