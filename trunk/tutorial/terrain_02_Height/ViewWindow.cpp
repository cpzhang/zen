#include "ViewWindow.h"
#include "render/rendercontext.h"
#include "scene/SceneManager.h"
#include "render/FxManager.h"
#include "terrain/Terrain.h"
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
	createFxManager();
	createSceneManager();
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
	//
	destroySceneManager();
	//
	destroyFxManager();
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
		Matrix gViewMatrix;
		Vector3 gEye(0,150,-10);
		Vector3 gLookAt(0, 0, 0);
		Vector3 gUp(0, 1, 0);
		D3DXMatrixLookAtLH(&gViewMatrix, &gEye, &gLookAt, &gUp);
		camera_.view(gViewMatrix);
		//
		Camera c = getRenderContex()->getCamera();
		c.setFarPlane(10000.0f);
		getRenderContex()->setCamera(c);
		getRenderContex()->updateProjectionMatrix();
		getSceneManager()->setRunType(eRunType_Editor);
		getSceneManager()->setAllChunksVisible(false);
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
		//       
		const Matrix* pmatProj = &getRenderContex()->getProjectionMatrix();

		POINT ptCursor;
		GetCursorPos( &ptCursor );
		::ScreenToClient( m_hWnd, &ptCursor );

		// Compute the vector of the pick ray in screen space
		D3DXVECTOR3 v;
		v.x = ( ( ( 2.0f * ptCursor.x ) / getRenderContex()->getScreenWidth() ) - 1 ) / pmatProj->_11;
		v.y = -( ( ( 2.0f * ptCursor.y ) / getRenderContex()->getScreenHeight() ) - 1 ) / pmatProj->_22;
		v.z = 1.0f;

		// Get the inverse view matrix
		const Matrix matView = getRenderContex()->getViewMatrix();
		const Matrix matWorld = Matrix::Identity;
		Matrix mWorldView = matWorld;
		mWorldView.postMultiply(matView);
		Matrix m;
		m.invert(mWorldView);

		// Transform the screen space pick ray into 3D space
		Vector3 vPickRayDir, vPickRayOrig;
		vPickRayDir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
		vPickRayDir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
		vPickRayDir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;
		vPickRayOrig.x = m._41;
		vPickRayOrig.y = m._42;
		vPickRayOrig.z = m._43;
		float t = -vPickRayOrig.y / vPickRayDir.y;
		float x = vPickRayOrig.x + t * vPickRayDir.x;
		float z = vPickRayOrig.z + t * vPickRayDir.z;
		if (getSceneManager()->getTerrain())
		{
			getSceneManager()->getTerrain()->updateVisibleChunks(x, z);
		}
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
