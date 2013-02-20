#include "FrameWindow.h"
#include "Global.h"
CAppModule _Module;
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE , LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	// Ensures that the common control DLL (Comctl32.dll) is loaded, and registers specific common control classes from the DLL.
	// An application must call this function before creating a common control.
	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);
	hRes = _Module.Init(NULL, hInstance);

	createGlobal();
	FrameWindow mw;
	mw.CreateEx();
	mw.ShowWindow(SW_SHOWMAXIMIZED);
	mw.UpdateWindow();
	MSG m;
	::ZeroMemory(&m, sizeof(m));
	for (;;)
	{
		::ZeroMemory(&m, sizeof(m));
		if (::PeekMessage(&m, NULL, 0, 0, PM_NOREMOVE))
		{
			::GetMessage(&m, NULL, 0, 0);
			if (m.message == WM_QUIT)
			{
				break;
			}
			::TranslateMessage(&m);
			::DispatchMessage(&m);
		}
		else
		{
			//Retrieves the number of milliseconds that have elapsed since the system was started, up to 49.7 days.
			static float lastTick = GetTickCount();
			float currentTick = GetTickCount();
			float delta = currentTick - lastTick;
			if (delta >= 30.9f)
			{
				mw.onIdle(delta);
				lastTick = currentTick;
			}
		}
	}
	destroyGlobal();

	_Module.Term();
	//Closes the COM library on the current thread, unloads all DLLs loaded by the thread, 
	//frees any other resources that the thread maintains, and forces all RPC connections on the thread to close.
	::CoUninitialize();

	return 0;
}