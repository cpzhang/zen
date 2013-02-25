#include "Fx.h"
#include "rendercontext.h"
#include "misc/FileSystem.h"
Fx::Fx()
{
	clear_();
}

void Fx::clear_()
{
	pEffect_ = NULL;
	hCurrentTechnique_ = 0;
}

ID3DXEffect* Fx::getDxEffect()
{
	return pEffect_;
}

D3DXHANDLE Fx::getCurrentTechnique() const
{
	return hCurrentTechnique_;
}

bool Fx::create( const tstring& effectResource )
{
	if (SUCCEEDED(D3DXCreateEffectFromFile(getRenderContex()->getDxDevice(), effectResource.c_str(), NULL, NULL, D3DXSHADER_DEBUG|D3DXSHADER_SKIPOPTIMIZATION, NULL, &pEffect_, NULL)))
	{
		resourceID_ = effectResource;
	}
	else
	{
		tstring ts = FileSystem::getDataDirectory() + "\\" + effectResource;
		if (SUCCEEDED(D3DXCreateEffectFromFile(getRenderContex()->getDxDevice(), ts.c_str(), NULL, NULL, D3DXSHADER_DEBUG|D3DXSHADER_SKIPOPTIMIZATION, NULL, &pEffect_, NULL)))
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

void Fx::destroy()
{
	if (pEffect_)
	{
		pEffect_->Release();
	}
	clear_();
}

const char* Fx::getWorldMatrixName()
{
	return WorldMatrixName.c_str();
}

const char* Fx::getViewMatrixName()
{
	return ViewMatrixName.c_str();
}

const char* Fx::getProjectionMatrixName()
{
	return ProjectionMatrixName.c_str();
}

const char* Fx::getViewProjectionMatrixName()
{
	return ViewProjectionMatrixName.c_str();
}

const char* Fx::getWorldViewProjectionMatrixName()
{
	return WorldViewProjectionMatrixName.c_str();
}

tstring Fx::getFilePath()
{
	return resourceID_;
}

const std::string Fx::WorldViewProjectionMatrixName("gWorldViewProjection");

const std::string Fx::ViewProjectionMatrixName("gViewProjection");

const std::string Fx::WorldMatrixName("gWorld");
const std::string Fx::ViewMatrixName("gView");
 const std::string Fx::ProjectionMatrixName("gProjection");
