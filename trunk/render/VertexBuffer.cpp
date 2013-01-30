#include "VertexBuffer.h"
#include "rendercontext.h"
VertexBuffer::VertexBuffer()
{
	clear_();
}

HRESULT VertexBuffer::create( u32 size, DWORD usage, DWORD FVF, D3DPOOL pool )
{
	destroy();
	HRESULT hr;
	IDirect3DVertexBuffer9* temp;
	if( SUCCEEDED( getRenderContex()->getDxDevice()->CreateVertexBuffer(size, usage, FVF, pool, &temp, NULL ) ) )
	{
		vertexBuffer_ = temp;
	}
	return hr;
}

void VertexBuffer::destroy()
{
	if (vertexBuffer_)
	{
		vertexBuffer_->Release();
		clear_();
	}
}

void VertexBuffer::clear_()
{
	vertexBuffer_ = NULL;
}

VertexBuffer::~VertexBuffer()
{
	clear_();
}

HRESULT VertexBuffer::apply( u32 streamNumber /*= 0*/, u32 offsetInBytes /*= 0*/, u32 stride /*= 0 */ ) const
{
	return getRenderContex()->getDxDevice()->SetStreamSource( streamNumber, vertexBuffer_, offsetInBytes, stride );
}

HRESULT VertexBuffer::lock( u32 offset, u32 size, VOID** data, DWORD flags )
{
	HRESULT hr = 0;
	if (vertexBuffer_)
	{
		hr = vertexBuffer_->Lock(offset, size, data, flags);
	}
	return hr;
}

void VertexBuffer::unlock()
{
	if (vertexBuffer_)
	{
		vertexBuffer_->Unlock();
	}
}
