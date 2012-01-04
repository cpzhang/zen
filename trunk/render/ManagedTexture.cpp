#include "ManagedTexture.h"
#include "misc/dxHead.h"
#include "rendercontext.h"
#include "misc/FileSystem.h"
ManagedTexture::ManagedTexture()
{
	clear_();
}

ManagedTexture::~ManagedTexture()
{
	clear_();
}

bool ManagedTexture::load( const tstring & resourceID )
{
	if (SUCCEEDED(D3DXCreateTextureFromFileEx(getRenderContex()->getDxDevice(), resourceID.c_str(), 
		D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &dxTexture_)))
	{
		resourceID_ = resourceID;
	}
	else
	{
		tstring ts = FileSystem::getDataDirectory() + resourceID;
		if (SUCCEEDED(D3DXCreateTextureFromFileEx(getRenderContex()->getDxDevice(), ts.c_str(), 
			D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &dxTexture_)))
		{
			resourceID_ = ts;
		}
		else
		{
			return false;
		}
	}
	return true;
}

IDirect3DTexture9* ManagedTexture::getDxTexture()
{
	return dxTexture_;
}

void ManagedTexture::destroy()
{
	if (dxTexture_)
	{
		dxTexture_->Release();
		clear_();
	}
}

void ManagedTexture::clear_()
{
	dxTexture_ = NULL;
	resourceID_.clear();
}

tstring ManagedTexture::getFileName()
{
	return resourceID_;
}
