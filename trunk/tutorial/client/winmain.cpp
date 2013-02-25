#pragma once
#include <windows.h>
#include "resource.h"
#include "misc/FileSystem.h"
#include "render/rendercontext.h"
#include "scene/SceneManager.h"
#include "render/FxManager.h"
#include "render/TextureManager.h"
#include "render/OrbitCamera.h"
const LPCTSTR APP_NAME = TEXT("Game");
const LPCTSTR APP_TITLE = TEXT("Game Client");
HWND			_hwnd;
OrbitCamera camera_;
bool adjustClientArea()
{
	//800
	RECT rect = {0, 0, 600, 800};
	if (!AdjustWindowRect(&rect, 
		GetWindowLong(_hwnd, GWL_STYLE), GetMenu(_hwnd) != NULL))
	{
		return false;
	}

	//
	unsigned int width = GetSystemMetrics(SM_CXFULLSCREEN);
	unsigned int height = GetSystemMetrics(SM_CYFULLSCREEN);

	//
	RECT rectWin;
	GetWindowRect(_hwnd, &rectWin);

	double x = (width - (rectWin.right - rectWin.left)) * 0.5;
	double y = (height - (rectWin.bottom - rectWin.top)) * 0.5;
	//
	if (!MoveWindow(_hwnd, (unsigned int)x, (unsigned int)y,
		rect.right - rect.left,
		rect.bottom - rect.top,
		true))
	{
		return false;
	}

	//
	return true;
}
LRESULT CALLBACK _wndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
		// Send the WM_QUIT message, terminating the message loop
	case WM_DESTROY:
		{
			::PostQuitMessage(0);
		}
		break;
	default:
		{
		}
		break;
	}
	// Forward any other messages we didn't handle above to the default window procedure
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}
bool play()
{
	//
	camera_.update(1, 0);
	getRenderContex()->setViewMatrix(camera_.view_);
	getSceneManager()->update(1);
	//
	u32 clearFlags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
	if ( getRenderContex()->isStencilAvailable() )
		clearFlags |= D3DCLEAR_STENCIL;
	static Vector4 scc(0.0f,0.0f,0.0f, 0.0f);
	getRenderContex()->getDxDevice()->Clear( 0, NULL, clearFlags, scc.getARGB(), 1, 0 );
	getRenderContex()->beginScene();
	//poem();
	if (getSceneManager())
	{
		//»­µØ±í
		getSceneManager()->render();
	}
	getRenderContex()->endScene();
	getRenderContex()->present();

	return true;
}
void postPlay()
{
	destroySceneManager();
	destroyFxManager();
	destroyTextureManager();
	destroyRenderContex();
}
int PASCAL WinMain(	HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	// Fill out the WNDCLASS structure
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = _wndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = ::GetModuleHandle(NULL);
	wc.hIcon = ::LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = 0;
	wc.lpszClassName = APP_NAME;

	// Register the WNDCLASS description 
	if (!::RegisterClass(&wc))
	{
		return false;
	}

	// Create the window
	_hwnd = ::CreateWindow(APP_NAME, APP_TITLE,
		WS_OVERLAPPEDWINDOW,
		0, 0, 600, 800,
		GetDesktopWindow(), 0, ::GetModuleHandle(0), 0);
	if (_hwnd ==0)
	{
		return false;
	}

	// Show the window
	::ShowWindow(_hwnd, SW_NORMAL);
	// Update the window
	::UpdateWindow(_hwnd);

	//
	//adjustClientArea();
	createRenderContex();
	int index = 0;
	bool pf = false;
	for (int i = 0; i != getRenderContex()->getDevicesNumber(); ++i)
	{
		DeviceInfo di = getRenderContex()->getDeviceInfo(i);
		std::string dp(di.identifier_.Description);
		if (dp.find("PerfHUD") != std::string::npos)
		{
			index = i;
			pf = true;
			break;
		}
	}
	getRenderContex()->createDevice(_hwnd, index, 0, true, true, Vector2::Zero);
	createTextureManager();
	createFxManager();
	createSceneManager();
	camera_.setSpeed(5.0f);
	Vector3 minBound = -Vector3( 100.5f, 0.f, 100.5f );
	Vector3 maxBound = Vector3(10000, 5000.0f, 10000.0f);
	camera_.limit_ =  BoundingBox( minBound, maxBound );
	camera_.create(10, MATH_PI, MATH_PI_Half);
	//
	Camera c = getRenderContex()->getCamera();
	c.setFarPlane(10000.0f);
	getRenderContex()->setCamera(c);

	FileSystem::setDataDirectory(FileSystem::guessDataDirectory());
	getSceneManager()->open("scene\\bornland");
	// Message Structure
	MSG msg;
	::ZeroMemory(&msg, sizeof(msg));
	// Loop until getting a WM_QUIT message
	while(true)
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			if (!play())
			{
				break;
			}
		}
	}
	// clean up
	postPlay();
}
