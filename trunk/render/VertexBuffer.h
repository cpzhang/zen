#pragma once
#include "common/dxHead.h"
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
	HRESULT apply( u32 streamNumber = 0, u32 offsetInBytes = 0, u32 stride = 0 ) const;
	HRESULT lock(u32 offset, u32 size, VOID** data, DWORD flags );
	void unlock();
private:
	void clear_();
private:
	IDirect3DVertexBuffer9* vertexBuffer_;
};