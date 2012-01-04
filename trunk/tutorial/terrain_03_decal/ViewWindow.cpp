#include "ViewWindow.h"
#include "render/rendercontext.h"
#include "render/TextureManager.h"
#include "scene/SceneManager.h"
#include "render/FxManager.h"
#include "scene/Terrain.h"
#include "Global.h"
ViewWindow::ViewWindow()
{

}

ViewWindow::~ViewWindow()
{

}
void createBeforeD3DDevice()
{

}
void createAfterD3DDevice()
{
	createGlobal();
}
LRESULT ViewWindow::onKeyDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

LRESULT ViewWindow::onCreate( UINT, WPARAM, LPARAM, BOOL& )
{
	//
	createRenderContex();
	//
	return 1;
}

LRESULT ViewWindow::onDestroy( UINT, WPARAM, LPARAM, BOOL& )
{
	destroyGlobal();
	//
	if (getRenderContex())
	{
		destroyRenderContex();
	}
	
	return 1;
}

// LRESULT ViewWindow::onPaint( UINT, WPARAM, LPARAM, BOOL& b )
// {
// 	b = FALSE;//不加这句，无法进入onIdle()
// 	return 1;
// }

LRESULT ViewWindow::onMouseWheel( UINT, WPARAM wParam, LPARAM, BOOL& b )
{
	return 1;
}

LRESULT ViewWindow::onMouseMove( UINT, WPARAM, LPARAM lParam, BOOL& b )
{
	return 1;
}

LRESULT ViewWindow::onMouseLeftButtonUp( UINT, WPARAM, LPARAM lParam, BOOL& b )
{
	return 1;
}

LRESULT ViewWindow::onSize( UINT, WPARAM, LPARAM lParam, BOOL& b )
{
	if (getRenderContex())
	{
		getRenderContex()->resetDevice();
	}
	return 1;
}

void ViewWindow::run()
{
}

void ViewWindow::onIdle()
{
	if (NULL == getRenderContex())
	{
		return;
	}
	//
	if (!getRenderContex()->isInitialized())
	{
		getRenderContex()->setWaitForVBL(false);
		getRenderContex()->createDevice(m_hWnd, 0, 0, true, true, Vector2::Zero);
		createAfterD3DDevice();
		camera_.speed(1.0f);
		camera_.turboSpeed(2.0f);
		Vector3 minBound = -Vector3( 100.5f, 0.f, 100.5f );
		Vector3 maxBound = Vector3(10000, 5000.0f, 10000.0f);
		camera_.limit_ =  BoundingBox( minBound, maxBound );
		Vector3 gEye(0,150,-10);
		Vector3 gLookAt(0, 0, 0);
		Vector3 gUp(0, 1, 0);
		camera_.view(gEye, gLookAt, gUp);
		//
		Camera c = getRenderContex()->getCamera();
		c.setFarPlane(10000.0f);
		getRenderContex()->setCamera(c);
		getRenderContex()->updateProjectionMatrix();
		getSceneManager()->setRunType(eRunType_Editor);
		getSceneManager()->setAllChunksVisible(true);
	}
	if (NULL == getRenderContex()->getDxDevice())
	{
		return;
	}
	//
	static float lastTick = GetTickCount();
	float currentTick = GetTickCount();
	float delta = currentTick - lastTick;
	camera_.update(delta);
	getSceneManager()->update();
	{
		getGlobal()->update();
	}
	//
	getRenderContex()->setViewMatrix(camera_.view_);
	//
	uint32 clearFlags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
	if ( getRenderContex()->isStencilAvailable() )
		clearFlags |= D3DCLEAR_STENCIL;
	getRenderContex()->getDxDevice()->Clear( 0, NULL, clearFlags, 0, 1, 0 );
	getRenderContex()->beginScene();
	if (getSceneManager())
	{
		getSceneManager()->render();
	}
	getRenderContex()->endScene();
	getRenderContex()->present();
}