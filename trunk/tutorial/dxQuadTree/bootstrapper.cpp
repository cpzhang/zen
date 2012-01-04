#pragma once
#include "windowsHead.h"
#include "dxHead.h"
#include "matrix.h"
#include "quadtree.h"
#include "rendercontext.h"
#include "MouseCamera.h"
#include "WorldManager.h"
#include "ChunkManager.h"
ID3DXEffect * gEffect = NULL;
IDirect3DTexture9 *gTexture = NULL;

Matrix gViewMatrix = Matrix::Identity;
Matrix gProjectionMatrix = Matrix::Identity;
QuadTree* gRoot = NULL;
Vector3 gEye(0,150,-10);
Vector3 gLookAt(0, 0, 0);
Vector3 gUp(0, 1, 0);
MouseCamera gCamera;
void create()
{
	createRenderContex();
	new ChunkManager;
	new WorldManager;
}
void destroy()
{
	destroyRenderContex();
	delete ChunkManager::getInstancePtr();
	delete WorldManager::getInstancePtr();
}
LRESULT CALLBACK windowMessageCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		// Send the WM_QUIT message, terminating the message loop
	case WM_CREATE:
		{
		}break;
	case WM_DESTROY:
		{
			::PostQuitMessage(0);
		}
		break;
	case WM_MOUSEWHEEL:
		{
			short delta = HIWORD(wParam);
			Vector3 n = gEye - gLookAt;
			D3DXVec3Normalize(&n, &n);
			gEye += n*delta*0.01;
		}
		break;
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			{
				PostQuitMessage(0);
			}
			break;
		case VK_ADD:
			{
				//
				static int index = -1;
				index++;
				if (index >= gMaxLOD+1)
				{
					index = 0;
				}
				//generateLOD(index);
			}break;
		case 'W':
			{
			}
			break;
		}
		break;
	case WM_SIZE:
		{
			//
			if (gEffect)
			{
				gEffect->OnLostDevice();
			}
			if (getRenderContex() && getRenderContex()->getDxDevice())
			{
				//
				//D3DVIEWPORT9 vp;
				//vp.X = 0;
				//vp.Y = 0;
				//vp.Width = LOWORD(lParam);
				//vp.Height = HIWORD(lParam);
				//vp.MinZ = 0;
				//vp.MaxZ = 1;
				////gDevice->SetViewport(&vp);
				////
				//LPDIRECT3DSWAPCHAIN9 sc;
				//getRenderContex()->getDxDevice()->GetSwapChain(0, &sc);
				//sc->Release();
				////
				//D3DPRESENT_PARAMETERS pp;
				//sc->GetPresentParameters(&pp);

				//pp.BackBufferWidth = vp.Width;
				//pp.BackBufferHeight = vp.Height;
				//getRenderContex()->getDxDevice()->Reset(&pp);//会重设viewport，仅设vp而不reset是错误的
			}
		}break;
	default:
		{
		}
		break;
	}
	// Forward any other messages we didn't handle above to the default window procedure
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}
TCHAR gClassName[]= TEXT("demo");
//The user-provided entry point for a graphical Windows-based application.
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// Fill out the WNDCLASS structure
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = windowMessageCallback;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = ::GetModuleHandle(NULL);
	wc.hIcon = ::LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = 0;
	wc.lpszClassName = gClassName;

	// Register the WNDCLASS description 
	if (!::RegisterClass(&wc))
	{
		return false;
	}

	// Create the window
	HWND hwnd = ::CreateWindow(gClassName, TEXT("client"),
		WS_OVERLAPPEDWINDOW,
		0, 0, 512, 512,
		GetDesktopWindow(), 0, ::GetModuleHandle(0), 0);
	if (hwnd ==0)
	{
		return 0;
	}

	// Show the window
	::ShowWindow(hwnd, SW_NORMAL);
	// Update the window
	::UpdateWindow(hwnd);

	create();
	getRenderContex()->createDevice(hwnd, 0, 0, false, false, Vector2(0,0), false);

	//
	gCamera.speed(30.0f);
	gCamera.turboSpeed(60.0f);
	Vector3 minBound = Vector3( .5f, 0.f, .5f );
	Vector3 maxBound = Vector3(100, 100.0f, 100.0f) - Vector3( .5f, 0.f, .5f );
	gCamera.limit_ =  BoundingBox( minBound, maxBound );
	D3DXMatrixLookAtLH(&gViewMatrix, &gEye, &gLookAt, &gUp);
	gCamera.view(gViewMatrix);
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
			static float lastTick = GetTickCount();
			float currentTick = GetTickCount();
			float delta = currentTick - lastTick;
			WorldManager::getInstancePtr()->render(delta);
		}
	}
	
	//
	destroy();
	return 0;
}
