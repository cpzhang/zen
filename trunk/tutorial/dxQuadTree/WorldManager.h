#pragma once
#include "Singleton.h"
class WorldManager : public Singleton<WorldManager>
{
public:
public:
	void render(float delta)
	{
		bool b = false;
		if (!getRenderContex()->checkDevice(b))
		{
			Sleep(100);
			return;
		}
		beginRender_();
		getRenderContex()->endScene();
		getRenderContex()->present();
	}
private:
	void beginRender_()
	{
		if (getRenderContex()->isStencilAvailable())
		{
			getRenderContex()->getDxDevice()->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 0x000020, 1, 0);
		}
		else
		{
			getRenderContex()->getDxDevice()->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x000020, 1, 0);
		}
		getRenderContex()->resetWorldMatricesStack();
		getRenderContex()->updateViewProjectionMatrix();

		getRenderContex()->beginScene();
	}
private:
};