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
		tstring ts = FileSystem::addDataDir(resourceID);
		if (SUCCEEDED(D3DXCreateTextureFromFileEx(getRenderContex()->getDxDevice(), ts.c_str(), 
			D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &dxTexture_)))
		{
			resourceID_ = resourceID;
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

bool ManagedTexture::createTexture( u32 Width, u32 Height, u32 MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool )
{
	if (SUCCEEDED(D3DXCreateTexture (getRenderContex()->getDxDevice(), 
		Width, 
		Height,
		MipLevels,
		Usage, 
		Format,
		Pool,
		&dxTexture_)))
	{
		return true;
	}
	return false;
}
//Pitch of source image, in bytes. For DXT formats, this number should represent the width of one row of cells, in bytes
bool ManagedTexture::setSubData(int left, int top, int width, int height, void* pData, D3DFORMAT pf, int pitch)
{
	if(!dxTexture_)return false;

	IDirect3DSurface9 *pSurface = 0;
	dxTexture_->GetSurfaceLevel(0,&pSurface);
	if(NULL == pSurface)
	{
		return false;
	}
	RECT rc = {0,0,width,height};
	RECT drc = {left,top,width + left,height + top};
	HRESULT hr = D3DXLoadSurfaceFromMemory(pSurface,
		NULL,
		&drc,
		pData,
		pf,
		pitch,
		NULL,
		&rc,
		D3DX_DEFAULT,
		0);
	pSurface->Release();
	return SUCCEEDED(hr);
}
