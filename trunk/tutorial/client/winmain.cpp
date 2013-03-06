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
//
#include "font/FlowText.h"
#include "font/FontManager.h"
//
#include "CrashRpt/CrashRpt.h"
const LPCTSTR APP_NAME = TEXT("Game");
const LPCTSTR APP_TITLE = TEXT("Game Client");
bool g_bActive = false;
bool g_bAppQuit = false;
bool g_bAppStarted = false;
HWND			_hwnd;
HeroCamera camera_;
Node* Hero_ = NULL;
EntityInstance* HeroInstance_ = NULL;
void setHero( const char* resID )
{
	HeroInstance_ = getSceneManager()->createEntityInstance(resID);
	Hero_ = NodeManager::getInstancePtr()->createNode("Hero");
	Hero_->attach(HeroInstance_);
	HeroInstance_->setAnimation("Run", true);
	HeroInstance_->scale(Vector3(0.02f, 0.02f, 0.02f));
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
			if (g_bActive)
			{
				camera_.onMouseMove();
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if (g_bActive)
			{
				std::vector<Vector3> ps;
				Vector2 pp = getSceneManager()->getPickingPoint();
				if (camera_.getCenter().x >= 0.0f && camera_.getCenter().z >= 0.0f )
				{
					getSceneManager()->getPath(camera_.getCenter(), Vector3(pp.x, 0.0f, pp.y), ps);
					if (!ps.empty())
					{
						camera_.resetPath();
						for (int i = 0; i != ps.size(); ++i)
						{
							camera_.addPath(ps[i]);
						}
						camera_.beginPath();
					}
				}
			}
		}break;
	case WM_MOUSEWHEEL:
		{
			if (g_bActive)
			{
				float delta = ( short )HIWORD( wParam );
				delta /= 120.0f;
				camera_.onMouseWheel(delta);
			}
		}
		break;
	case WM_SIZE:
		{
			g_bActive = !( (SIZE_MAXHIDE == wParam) || (SIZE_MINIMIZED == wParam) || ( 0 == lParam ));
			if (g_bActive && g_bAppStarted)
			{
				getRenderContex()->resetDevice();
			}
			return 1;
		}break;
	case WM_ACTIVATE:
		{
			/*
			http://msdn.microsoft.com/en-us/library/windows/desktop/ms646274(v=vs.85).aspx
			*/
			short d = ( short )LOWORD( wParam );
			if (d == WA_ACTIVE)
			{
				g_bActive = true;
			}
			else if (d == WA_CLICKACTIVE)
			{
				g_bActive = true;
			}
			else if (d == WA_INACTIVE)
			{
				g_bActive = false;
			}
			return 0;
		}
		break;
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
			case VK_ADD:
				{
					if (g_bActive)
					{
						camera_.setSpeed(camera_.getSpeed() * 1.2f);
					}
				}break;
			case VK_SUBTRACT:
				{
					if (g_bActive)
					{
						camera_.setSpeed(camera_.getSpeed() * 0.8f);
					}
				}break;
			case VK_LBUTTON:
				{
					
				}
				break;
			case 'W':
			case 'S':
			case 'A':
			case 'D':
				{
					//camera_.onKeyDown(wParam);
				}
				break;
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
	Vector3 dst;
	if (camera_.getDst(delta, dst))
	{
		std::vector<Vector3> ps;
		if (camera_.getCenter().x >= 0.0f && camera_.getCenter().z >= 0.0f )
		{
			getSceneManager()->getPath(camera_.getCenter(), Vector3(dst.x, 0.0f, dst.z), ps);
			if (!ps.empty())
			{
				camera_.resetPath();
				for (int i = 0; i != ps.size(); ++i)
				{
					camera_.addPath(ps[i]);
				}
				camera_.beginPath();
			}
		}
	}
	//拾取高度
	Vector3 position_ = camera_.getCenter();
	if(0){
		static Vector3 vp = position_;
		Vector3 dp = vp - position_;
		if (dp.lengthSquared() > 0.0001f)
		{
			std::ostringstream ss;
			ss<<"delta == "<<dp.lengthSquared();
			FlowText::getSingletonP()->add(ss.str(), Vector4::One);
			vp = position_;
		}
	}
	float h = getSceneManager()->getTerrain()->getHeightFromeWorldSpacePosition(position_.x, position_.z);
	camera_.setTerrainHeight(h);
	position_.y += h;
	HeroInstance_->setPosition(position_);
	HeroInstance_->rotateY(camera_.getAngleY() + MATH_PI);
	//
	if (g_bActive)
	{
		camera_.update(delta, 0);
		getRenderContex()->setViewMatrix(camera_.getViewMatrix());
	}
	FlowText::getSingletonP()->update(delta);
	getSceneManager()->update(delta);
	Hero_->update(delta);
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
	FlowText::getSingletonP()->render();
	FontManager::getPointer()->getFont()->render();
	getRenderContex()->endScene();
	getRenderContex()->present();

	return true;
}
void postPlay()
{
	HeroInstance_->release();
	Hero_->release();
	getRenderContex()->releaseRenderTarget(getRenderContex()->getCurrentRenderTarget());
	//delete FontManager::getPointer();
	FontManager::getPointer()->destroy();
	destroySceneManager();
	destroyFxManager();
	destroyTextureManager();
	destroyRenderContex();
}
// Define the callback function that will be called on crash
BOOL WINAPI CrashCallback(LPVOID /*lpvState*/)
{  
	// The application has crashed!

	// Close the log file here
	// to ensure CrashRpt is able to include it into error report

	// Return TRUE to generate error report
	return TRUE;
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

	//MessageBox(NULL, "远程连接后点击确定", "测试", MB_OK);
	//
	adjustClientArea();
	//
	g_bAppStarted = true;
	Error("游戏客户端log");
	//
	//
	{
		// Define CrashRpt configuration parameters
		CR_INSTALL_INFO info;  
		memset(&info, 0, sizeof(CR_INSTALL_INFO));  
		info.cb = sizeof(CR_INSTALL_INFO);    
		info.pszAppName = _T("MyApp");  
		info.pszAppVersion = _T("1.0.0");  
		info.pszEmailSubject = _T("MyApp 1.0.0 Error Report");  
		//info.pszEmailTo = _T("297191409@qq.com");    
		info.pszEmailText = _T("text");
		//info.pszSmtpProxy =_T("smtp.21cn.com:25");
		//int i = rand() % 10 + 1;
		//char mailinfo[256];
		//sprintf_s(mailinfo, "bccr%03d", i);
		//info.pszSmtpLogin =_T("bccr001");
		//info.pszSmtpPassword =_T("107628");
		//info.pszUrl = _T("http://myapp.com/tools/crashrpt.php");  
		info.pfnCrashCallback = CrashCallback;   
		info.uPriorities[CR_HTTP] = 1;  // First try send report over HTTP 
		info.uPriorities[CR_SMTP] = 3;  // Second try send report over SMTP  
		info.uPriorities[CR_SMAPI] = 2; // Third try send report over Simple MAPI    
		// Install all available exception handlers, use HTTP binary transfer encoding (recommended).
		info.dwFlags |= CR_INST_ALL_POSSIBLE_HANDLERS;
		info.dwFlags |= CR_INST_HTTP_BINARY_ENCODING; 
		info.dwFlags |= CR_INST_APP_RESTART; 
		//info.dwFlags |= CR_INST_SEND_QUEUED_REPORTS; 
		info.dwFlags |= CR_INST_DONT_SEND_REPORT;
		info.dwFlags |= CR_INST_NO_GUI;
		//info.dwFlags |= CR_INST_STORE_ZIP_ARCHIVES;
		//info.pszRestartCmdLine = _T("/restart");
		//
		char binDir[256];
		tstring path = FileSystem::getBinDirectory();
		strncpy(binDir, path.c_str(), 256);
		info.pszErrorReportSaveDir = binDir;
		//info.pszErrorReportSaveDir 
		// Define the Privacy Policy URL 
		//info.pszPrivacyPolicyURL = _T("http://myapp.com/privacypolicy.html"); 

		// Install exception handlers
		int nResult = crInstall(&info);    
		if(nResult!=0)  
		{    
			// Something goes wrong. Get error message.
			TCHAR szErrorMsg[512] = _T("");        
			crGetLastErrorMsg(szErrorMsg, 512);    
			_tprintf_s(_T("%s\n"), szErrorMsg);    
			return 1;
		} 

		// Add our log file to the error report
		crAddFile2(_T("log.txt"), NULL, _T("Log File"), CR_AF_MAKE_FILE_COPY);    

		// We want the screenshot of the entire desktop is to be added on crash
		crAddScreenshot(CR_AS_VIRTUAL_SCREEN);   

		// Add a named property that means what graphics adapter is
		// installed on user's machine
		crAddProperty(_T("VideoCard"), _T("nVidia GeForce 8600 GTS"));
	}
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
	getSceneManager()->open("\\scene\\1");
	setHero(TEXT("\\model\\Character_1015\\Character_1015.entity"));
	//
	FontManager::getPointer()->createFont(std::string("freetype\\simkai.ttf"), 16, eFontProperty_Normal, "freeNormal");
	//
	camera_.setCenter(Vector3(0, 0.0, 0));
	camera_.setSpeed(0.2f);
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
			if (delta >= 1.0f)
			{
				if (!play(delta))
				{
					break;
				}
				lastTick = currentTick;
			}
			else
			{
				Sleep(1);
			}
		}
	}
	// clean up
	postPlay();
	//
	//
	{
		// Unset exception handlers before exiting the main function
		crUninstall();
	}
	Error("==end");
}
