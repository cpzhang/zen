#include "IndexBuffer.h"
#include "rendercontext.h"

IndexBuffer::IndexBuffer()
{
	clear_();
}

HRESULT IndexBuffer::create( u32 size, DWORD usage, D3DFORMAT FVF, D3DPOOL pool )
{
	destroy();
	HRESULT hr;
	IDirect3DIndexBuffer9* temp;
	if( SUCCEEDED( getRenderContex()->getDxDevice()->CreateIndexBuffer(size, usage, FVF, pool, &temp, NULL ) ) )
	{
		indexBuffer_ = temp;
	}
	return hr;
}

void IndexBuffer::destroy()
{
	if (indexBuffer_)
	{
		indexBuffer_->Release();
		clear_();
	}
}

void IndexBuffer::clear_()
{
	indexBuffer_ = NULL;
}

IndexBuffer::~IndexBuffer()
{
	clear_();
}

HRESULT IndexBuffer::apply() const
{
	return getRenderContex()->getDxDevice()->SetIndices( indexBuffer_);
}

HRESULT IndexBuffer::lock( UINT offset, UINT size, VOID** data, DWORD flags )
{
	HRESULT hr = 0;
	if (indexBuffer_)
	{
		hr = indexBuffer_->Lock(offset, size, data, flags);
	}
	return hr;
}

void IndexBuffer::unlock()
{
	if (indexBuffer_)
	{
		indexBuffer_->Unlock();
	}
}
