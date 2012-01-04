#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>
//
IDirect3DDevice9* gDevice = NULL;
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
	case WM_SIZE:
		{
			D3DVIEWPORT9 vp;
			vp.X = 0;
			vp.Y = 0;
			vp.Width = LOWORD(lParam);
			vp.Height = HIWORD(lParam);
			vp.MinZ = 0;
			vp.MaxZ = 1;
			//gDevice->SetViewport(&vp);
			//
			LPDIRECT3DSWAPCHAIN9 sc;
			gDevice->GetSwapChain(0, &sc);
			sc->Release();
			//
			D3DPRESENT_PARAMETERS pp;
			sc->GetPresentParameters(&pp);

			pp.BackBufferWidth = vp.Width;
			pp.BackBufferHeight = vp.Height;
			gDevice->Reset(&pp);//会重设viewport，仅设vp而不reset是错误的
		}break;
	default:
		{
		}
		break;
	}
	// Forward any other messages we didn't handle above to the default window procedure
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}
struct sVertex
{
	FLOAT    x, y, z;
	D3DCOLOR diffuse;
};
const DWORD gVertexFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
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
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pp.hDeviceWindow = hwnd;
	pp.Windowed = TRUE;
	pp.EnableAutoDepthStencil = TRUE;
	pp.AutoDepthStencilFormat = D3DFMT_D24S8;
	pp.Flags = 0;
	pp.FullScreen_RefreshRateInHz = 0;
	pp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &gDevice);
	if (gDevice == NULL)
	{
		return 0;
	}
	LPD3DXEFFECT ppEffect;
	D3DXCreateEffectFromFile(gDevice, TEXT("e:/ZenBin/data/shader/Position.fx"), NULL, NULL, D3DXSHADER_DEBUG, NULL, &ppEffect, NULL);	
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
			gDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(100, 0, 0, 0), 0.0f, 0);
			gDevice->BeginScene();
			gDevice->SetFVF(gVertexFVF);
			sVertex v[3];
			v[0].x = 0.5f;
			v[0].y = 0.85f;
			v[0].z = 0.0f;
			v[0].diffuse = D3DCOLOR_RGBA(0, 100, 0, 0);

			v[1].x = 0.15f;
			v[1].y = 0.0f;
			v[1].z = 0.0f;
			v[1].diffuse = D3DCOLOR_RGBA(0, 100, 0, 0);

			v[2].x = -0.15f;
			v[2].y = 0.0f;
			v[2].z = 0.0f;
			v[2].diffuse = D3DCOLOR_RGBA(0, 100, 0, 0);
			gDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, v, sizeof(sVertex));
			gDevice->EndScene();
			gDevice->Present(NULL, NULL, 0, NULL);		
		}
	}
	//
	if (ppEffect)
	{
		ppEffect->Release();
		ppEffect = NULL;
	}
	//
	if (gDevice)
	{
		gDevice->Release();
		gDevice = NULL;
	}
	//
	d3d9->Release();
	d3d9 = NULL;
	return 0;
}
