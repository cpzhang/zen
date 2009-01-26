#pragma once
#include <windows.h>
#include "resource.h"
#include "misc/windowsHead.h"
#include "misc/Logger.h"
#include "misc/FileSystem.h"
//
#include "render/rendercontext.h"
#include "render/FxManager.h"
#include "render/TextureManager.h"
#include "render/OrbitCamera.h"
//
#include "model/EntityInstance.h"
//
#include "scene/Node.h"
#include "scene/Terrain.h"
#include "scene/SceneManager.h"
const LPCTSTR APP_NAME = TEXT("Game");
const LPCTSTR APP_TITLE = TEXT("Game Client");
HWND			_hwnd;
HeroCamera camera_;
Node* Hero_ = NULL;
EntityInstance* HeroInstance_ = NULL;
void setHero( const char* resID )
{
	HeroInstance_ = getSceneManager()->createEntityInstance(resID);
	Hero_ = NodeManager::getInstancePtr()->createNode("Hero");
	Hero_->attach(HeroInstance_);
	HeroInstance_->setAnimation("Run");
}
bool adjustClientArea()
{
	//800
	RECT rect = {0, 0, 600, 600};
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

	double x = (width - (rect.right - rect.left)) * 0.5;
	double y = (height - (rect.bottom - rect.top)) * 0.5;
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
	case WM_MOUSEMOVE:
		{
			//camera_.onMouseMove();
		}
		break;
	case WM_MOUSEWHEEL:
		{
			float delta = ( short )HIWORD( wParam );
			delta /= 120.0f;
			camera_.onMouseWheel(delta);
		}
	case WM_SIZE:
		{
			return 1;
		}break;
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
				// Destroy the window and Send the WM_DESTROY message
			case VK_ESCAPE:
				{
					::DestroyWindow((HWND)hWnd);
				}
				break;
			case 'W':
			case 'S':
			case 'A':
			case 'D':
				{
					camera_.onKeyDown(wParam);
				}break;
			}
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
bool play(const float delta)
{
	//
	camera_.update(delta, 0);
	getRenderContex()->setViewMatrix(camera_.getViewMatrix());
	getSceneManager()->update(delta);
	Hero_->update(delta);
	//拾取高度
	Vector3 position_ = camera_.getCenter();
	position_.y = getSceneManager()->getTerrain()->getHeightFromeWorldSpacePosition(position_.x, position_.z);
	HeroInstance_->setPosition(position_);
	HeroInstance_->rotateY(camera_.getAngleY() + MATH_PI);
	//
	u32 clearFlags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
	if ( getRenderContex()->isStencilAvailable() )
		clearFlags |= D3DCLEAR_STENCIL;
	static Vector4 scc(0.0f,0.0f,0.0f, 0.0f);
	getRenderContex()->getDxDevice()->Clear( 0, NULL, clearFlags, scc.getARGB(), 1, 0 );
	getRenderContex()->beginScene();
	//
	if (getSceneManager())
	{
		//画场景
		getSceneManager()->render();
	}
	Hero_->render();
	getRenderContex()->endScene();
	getRenderContex()->present();

	return true;
}
void postPlay()
{
	HeroInstance_->release();
	Hero_->release();
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

	MessageBox(NULL, "远程连接后点击确定", "测试", MB_OK);
	//
	adjustClientArea();
	//
	Error("游戏客户端log");
	//
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
	//
	Camera c = getRenderContex()->getCamera();
	c.setFarPlane(10000.0f);
	getRenderContex()->setCamera(c);
	getRenderContex()->updateProjectionMatrix();
	FileSystem::setDataDirectory(FileSystem::guessDataDirectory());
	getSceneManager()->open("\\scene\\bornland");
	setHero(TEXT("\\model\\Character_1015\\Character_1015.entity"));
	//
	camera_.setCenter(Vector3(30, 0.0, 35));
	// Message Structure
	MSG msg;
	::ZeroMemory(&msg, sizeof(msg));
	// Loop until getting a WM_QUIT message
	float lastTick = GetTickCount();
	float currentTick = 0.0f;
	float delta = 0.0f;
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
			currentTick = GetTickCount();
			delta = currentTick - lastTick;
			if (delta >= 30.0f)
			{
				if (!play(delta))
				{
					break;
				}
				lastTick = currentTick;
			}
		}
	}
	// clean up
	postPlay();
	Error("==end");
}
