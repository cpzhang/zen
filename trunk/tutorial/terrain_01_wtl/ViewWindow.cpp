#include "ViewWindow.h"
#include "render/rendercontext.h"
ViewWindow::ViewWindow()
{

}

ViewWindow::~ViewWindow()
{

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
		getRenderContex()->createDevice(m_hWnd, 0, 0, true, true, Vector2::Zero);
	}
	if (NULL == getRenderContex()->getDxDevice())
	{
		return;
	}

	uint32 clearFlags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
	if ( getRenderContex()->isStencilAvailable() )
		clearFlags |= D3DCLEAR_STENCIL;
	getRenderContex()->getDxDevice()->Clear( 0, NULL, clearFlags, 0, 1, 0 );
	getRenderContex()->beginScene();
	getRenderContex()->endScene();
	getRenderContex()->present();
}
