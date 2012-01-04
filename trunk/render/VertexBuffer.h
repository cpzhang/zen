#pragma once
#include "misc/dxHead.h"
#include "misc/helper.h"
#include "ConfigRender.h"
class ApiRender_ VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();
public:
	HRESULT create( u32 size, DWORD usage, DWORD FVF, D3DPOOL pool);
	void destroy();
	HRESULT apply( UINT streamNumber = 0, UINT offsetInBytes = 0, UINT stride = 0 ) const;
	HRESULT lock(UINT offset, UINT size, VOID** data, DWORD flags );
	void unlock();
private:
	void clear_();
private:
	IDirect3DVertexBuffer9* vertexBuffer_;
};