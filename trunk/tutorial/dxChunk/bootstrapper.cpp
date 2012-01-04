#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>
//
#include <vector>
#include <cmath>
//
class Vector3 : public D3DXVECTOR3
{
public:
	Vector3()
	{

	}
	Vector3( float a, float b, float c )
		:D3DXVECTOR3( a, b, c)
	{

	}
};
//
class Vector4 : public D3DXVECTOR4
{
public:
	Vector4()
	{

	}
	Vector4( float a, float b, float c, float d )
		:D3DXVECTOR4( a, b, c, d )
	{
	}
};
class Matrix : public D3DXMATRIX
{
public:
	Matrix()
	{
		setZero();
	}

	Matrix( const Vector4& v0,
		const Vector4& v1,
		const Vector4& v2,
		const Vector4& v3 )
	{
		row( 0, v0);
		row( 1, v1);
		row( 2, v2);
		row( 3, v3);					
	}
public:
	void setZero()
	{
		memset( this, 0, sizeof( *this ) );
	}
	void row( int i, const Vector4& value )
	{
		m[i][0] = value.x;
		m[i][1] = value.y;
		m[i][2] = value.z;
		m[i][3] = value.w;
	}
	void column( int i, const Vector4& value )
	{
		m[0][i] = value.x;
		m[1][i] = value.y;
		m[2][i] = value.z;
		m[3][i] = value.w;
	}
	void lookAt( const Vector3& eye, const Vector3& at, const Vector3& up )
	{
		D3DXMatrixLookAtLH(this, &eye, &at, &up);
	}
	bool invert()
	{
		return (D3DXMatrixInverse( this, NULL, this ) != NULL);
	}
public:
	static const Matrix Identity;
};

const Matrix Matrix::Identity(	Vector4( 1.0f, 0.0f, 0.0f, 0.0f ),
							  Vector4( 0.0f, 1.0f, 0.0f, 0.0f ),
							  Vector4( 0.0f, 0.0f, 1.0f, 0.0f ),
							  Vector4( 0.0f, 0.0f, 0.0f, 1.0f ) );

//
IDirect3DDevice9* gDevice = NULL;
LPD3DXEFFECT gEffect = NULL;
LPDIRECT3DTEXTURE9 gTexture = NULL;
const size_t gMaxLOD = 5;
IDirect3DVertexBuffer9 *gVB = NULL;
IDirect3DIndexBuffer9* gIB = NULL;
Matrix gViewMatrix = Matrix::Identity;
Matrix gProjectionMatrix = Matrix::Identity;
std::vector<short> gIndices;
void create()
{

}
void generateLOD(int index)
{
	gIndices.clear();
	int lod = pow((double)2, (long)index);
	int verticesNum = pow((double)2, (double)gMaxLOD) + 1;
	for (int j = 0; j < verticesNum - 1;j += lod)
	for (int i = 0; i < verticesNum - 1;i += lod)
	{
		short baseIndex = i + j * verticesNum;
		gIndices.push_back(baseIndex);
		gIndices.push_back(baseIndex + verticesNum*lod);
		gIndices.push_back(baseIndex + lod);

		gIndices.push_back(baseIndex + verticesNum*lod);
		gIndices.push_back(baseIndex + lod + verticesNum*lod);
		gIndices.push_back(baseIndex + lod);
	}

	void* data;
	gIB->Lock(0, 0, &data, 0);
	memcpy(data, &gIndices[0], gIndices.size()*sizeof(short));
	gIB->Unlock();
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
		case VK_ADD:
			{
				//
				static int index = -1;
				index++;
				if (index >= gMaxLOD+1)
				{
					index = 0;
				}
				generateLOD(index);
			}break;
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
	//D3DCOLOR diffuse;
	//FLOAT	u,v;
};
const DWORD gVertexFVF = D3DFVF_XYZ;// | D3DFVF_DIFFUSE | D3DFVF_TEX1;
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
	int verticesNum = pow((double)2, (double)gMaxLOD) + 1;
	//
	gDevice->CreateVertexBuffer(verticesNum*verticesNum*sizeof(sVertex), 0, gVertexFVF, D3DPOOL_MANAGED, &gVB, 0);
	//
	gDevice->CreateIndexBuffer((verticesNum - 1)*(verticesNum - 1)*2*3*2, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &gIB, 0);
	//
	std::vector<sVertex> vertices;
	for (size_t z = 0; z != verticesNum; z++)
	for (size_t x = 0; x != verticesNum; x++)
	{
		sVertex v;
		v.x = x*3;
		v.z = z*3;
		v.y = 0;
		//v.u = v.x*0.01f;
		//v.v = v.y*0.01f;
		vertices.push_back(v);
	}
	{
		void* data;
		gVB->Lock(0, 0, &data, 0);
		memcpy(data, &vertices[0], vertices.size()*sizeof(sVertex));
		gVB->Unlock();
	}
	generateLOD(0);
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
			gDevice->SetStreamSource(0, gVB, 0, sizeof(sVertex));
			gDevice->SetIndices(gIB);
			gEffect->SetTexture("lay0", gTexture);
			UINT pPasses = 0;
			gEffect->SetTechnique("PP0");
			gViewMatrix.column(0, Vector4(1, 0, 0, 0));
			gViewMatrix.column(1, Vector4(0, 0, 1, 0));
			gViewMatrix.column(2, Vector4(0, -1, 0, 150));
			gViewMatrix.column(3, Vector4(0, 0, 0, 1));
			gEffect->SetMatrix("gView", &gViewMatrix);
			D3DXMatrixPerspectiveFovLH(&gProjectionMatrix, 1.5, 1, 0.01, 1000);
			gEffect->SetMatrix("gProjection", &gProjectionMatrix);
			gEffect->Begin(&pPasses, D3DXFX_DONOTSAVESTATE);
			for (int i = 0; i != pPasses; ++i)
			{
				gEffect->BeginPass(i);
				gDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, gMaxLOD*gMaxLOD, 0, gIndices.size()/3);
				gEffect->EndPass();
			}
			gEffect->End();
			gDevice->EndScene();
			gDevice->Present(NULL, NULL, 0, NULL);		
		}
	}
	//
	if (gIB)
	{
		gIB->Release();
		gIB = NULL;
	}
	//
	if (gVB)
	{
		gVB->Release();
		gVB = NULL;
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
