#pragma once
#include "Texture.h"
#include "misc/stdHead.h"
class ApiRender_ ManagedTexture: public Texture
{
public:
	ManagedTexture();
	~ManagedTexture();
public:
	bool load(const tstring & resourceID);
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

