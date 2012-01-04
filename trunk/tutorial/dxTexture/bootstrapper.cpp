#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>
//
IDirect3DDevice9* gDevice = NULL;
LPD3DXEFFECT gEffect = NULL;
LPDIRECT3DTEXTURE9 gTexture = NULL;
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
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			{
				PostQuitMessage(0);
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
			if (gDevice)
			{
				//
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
struct sVertex
{
	FLOAT    x, y, z;
	D3DCOLOR diffuse;
	FLOAT	u,v;
};
const DWORD gVertexFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
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
	D3DXCreateEffectFromFile(gDevice, TEXT("e:/ZenBin/data/shader/Position.fx"), NULL, NULL, D3DXSHADER_DEBUG, NULL, &gEffect, NULL);	
	//
	D3DXCreateTextureFromFileEx(gDevice, TEXT("e:/ZenBin/data/image/AID_hportal.bmp"), D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &gTexture);
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
			gDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x000020, 1, 0);
			gDevice->BeginScene();
			gDevice->SetFVF(gVertexFVF);
			sVertex v[4];
			size_t intdex = 0;
			v[intdex].x = -0.5f;
			v[intdex].y = 0.5f;
			v[intdex].z = 0.0f;
			v[intdex].diffuse = D3DCOLOR_RGBA(0, 100, 0, 0);
			v[intdex].u = 0;
			v[intdex].v = 0;

			intdex = 1;
			v[intdex].x = 0.5f;
			v[intdex].y = 0.5f;
			v[intdex].z = 0.0f;
			v[intdex].diffuse = D3DCOLOR_RGBA(0, 100, 0, 0);
			v[intdex].u = 1;
			v[intdex].v = 0;

			intdex = 2;
			v[intdex].x = -0.5f;
			v[intdex].y = -0.5f;
			v[intdex].z = 0.0f;
			v[intdex].diffuse = D3DCOLOR_RGBA(0, 100, 0, 0);
			v[intdex].u = 0;
			v[intdex].v = 1;

			intdex = 3;
			v[intdex].x = 0.5f;
			v[intdex].y = -0.5f;
			v[intdex].z = 0.0f;
			v[intdex].diffuse = D3DCOLOR_RGBA(0, 100, 0, 0);
			v[intdex].u = 1;
			v[intdex].v = 1;
			gEffect->SetTexture("lay0", gTexture);
			UINT pPasses = 0;
			gEffect->SetTechnique("PP0");
			gEffect->Begin(&pPasses, D3DXFX_DONOTSAVESTATE);
			for (int i = 0; i != pPasses; ++i)
			{
				gEffect->BeginPass(i);
				gDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(sVertex));
				gEffect->EndPass();
			}
			gEffect->End();
			gDevice->EndScene();
			gDevice->Present(NULL, NULL, 0, NULL);		
		}
	}
	//
	if (gTexture)
	{
		gTexture->Release();
		gTexture = NULL;
	}
	//
	if (gEffect)
	{
		gEffect->Release();
		gEffect = NULL;
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
