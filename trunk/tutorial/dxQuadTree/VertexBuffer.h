#pragma once
class VertexBuffer
{
public:
	VertexBuffer()
	{
		clear_();
	}
public:
	HRESULT create( uint32 size, DWORD usage, DWORD FVF, D3DPOOL pool)
	{
		release();
		HRESULT hr;
		IDirect3DVertexBuffer9* temp;
		if( SUCCEEDED( getRenderContex()->getDxDevice()->CreateVertexBuffer(size, usage, FVF, pool, &temp, NULL ) ) )
		{
			vertexBuffer_ = temp;
		}
		return hr;
	}
	void release()
	{
		if (vertexBuffer_)
		{
			vertexBuffer_->Release();
			clear_();
		}
	}
private:
	void clear_()
	{
		vertexBuffer_ = NULL;
	}
private:
	IDirect3DVertexBuffer9* vertexBuffer_;
};