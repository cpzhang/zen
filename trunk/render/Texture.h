#pragma once
#include "misc/helper.h"
#include "misc/stdHead.h"
#include "ConfigRender.h"
struct IDirect3DTexture9;
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

