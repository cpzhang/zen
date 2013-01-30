#pragma once
#include "misc/helper.h"
#include "misc/stdHead.h"
#include "misc/dxHead.h"
#include "ConfigRender.h"

class ApiRender_ Texture
{
public:
	Texture()
	{
	}
	virtual ~Texture()
	{
	}
#define InterfaceTexture(terminal)\
public:\
	virtual void destroy() ##terminal\

#define Texture_Base InterfaceTexture(=0;)
#define Texture_Null InterfaceTexture({};)
#define Texture_Derived InterfaceTexture(;)
	virtual IDirect3DTexture9*	getDxTexture()
	{
		return 0;
	}
	virtual bool load(const tstring & resourceID)
	{
		return false;
	}
	virtual bool setSubData(int left, int top, int width, int height, void* pData, D3DFORMAT pf, int pitch)
	{
		return false;
	}
	virtual tstring getFileName()
	{
		return "";
	}
	Texture_Null

	static Texture* getNullObject()
	{
		static Texture s;
		return &s;
	}
};

