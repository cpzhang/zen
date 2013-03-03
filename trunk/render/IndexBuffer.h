#pragma once
#include "common/dxHead.h"
#include "misc/helper.h"
#include "ConfigRender.h"
class ApiRender_ IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();
public:
	HRESULT create( u32 size, DWORD usage, D3DFORMAT FVF, D3DPOOL pool);
	void destroy();
	HRESULT apply() const;
	HRESULT lock(u32 offset, u32 size, VOID** data, DWORD flags );
	void unlock();
private:
	void clear_();
private:
	IDirect3DIndexBuffer9* indexBuffer_;
};