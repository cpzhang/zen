#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <d3d9.h>
void create()
{

}
LRESULT CALLBACK windowMessageCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		// Send the WM_QUIT message, terminating the message loop
	case WM_CREATE:
		{
			create();
		}break;
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

	//
	IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (NULL == d3d9)
	{
		return 0;
	}
	D3DPRESENT_PARAMETERS pp;
	memset(&pp, 0, sizeof(D3DPRESENT_PARAMETERS ));
	pp.BackBufferWidth = 0;
	pp.BackBufferHeight = 0;
	pp.BackBufferFormat = D3DFMT_UNKNOWN;
	pp.BackBufferCount = 1;
	pp.MultiSampleType = D3DMULTISAMPLE_NONE;
	pp.MultiSampleQuality = 0;
	pp.SwapEffect = D3DSWAPEFFECT_COPY;
	pp.hDeviceWindow = hwnd;
	pp.Windowed = TRUE;
	pp.EnableAutoDepthStencil = TRUE;
	pp.AutoDepthStencilFormat = D3DFMT_D24S8;
	pp.Flags = 0;
	pp.FullScreen_RefreshRateInHz = 0;
	pp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	IDirect3DDevice9* device;
	d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &device);
	if (device == NULL)
	{
		return 0;
	}
	
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
			device->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(100, 0, 0, 0), 0.0f, 0);
			device->BeginScene();
			device->EndScene();
			device->Present(NULL, NULL, 0, NULL);		
		}
	}
	//
	if (device)
	{
		device->Release();
		device = NULL;
	}
	//
	d3d9->Release();
	d3d9 = NULL;
	return 0;
}
