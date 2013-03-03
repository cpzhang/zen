#pragma once
#include "Texture.h"
#include "misc/stdHead.h"
#include "common/dxHead.h"
class ApiRender_ ManagedTexture: public Texture
{
public:
	ManagedTexture();
	~ManagedTexture();
public:
	bool load(const tstring & resourceID);
	bool createTexture(u32 Width,
		u32 Height,
		u32 MipLevels,
		DWORD Usage,
		D3DFORMAT Format,
		D3DPOOL Pool);
	bool setSubData(int left, int top, int width, int height, void* pData, D3DFORMAT pf, int pitch);
public:
	virtual IDirect3DTexture9*	getDxTexture();
	virtual tstring getFileName();
	Texture_Derived
private:
	void clear_();
private:
	IDirect3DTexture9* dxTexture_;
	tstring			resourceID_;
};

