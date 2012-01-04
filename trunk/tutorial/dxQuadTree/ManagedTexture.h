#pragma once
#include "Texture.h"
class ManagedTexture: public Texture
{
public:
public:
	bool load(const std::string & resourceID)
	{
		resourceID_ = resourceID;
		//
		D3DXCreateTextureFromFileEx(getRenderContex().getDxDevice(), TEXT("e:/ZenBin/data/image/AID_hportal.bmp"), 
			D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &dxTexture_);
		return true;
	}
	virtual IDirect3DTexture9*	getDxTexture( )
	{
		return dxTexture_;
	}
private:
	IDirect3DTexture9* dxTexture_;
	std::string			resourceID_;
};

