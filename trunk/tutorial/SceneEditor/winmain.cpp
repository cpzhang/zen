#include "FrameWindow.h"
#include "Global.h"

CAppModule _Module;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE , LPTSTR lpstrCmdLine, int nCmdShow)
{
	
	//
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
			static float lastTick = GetTickCount();
			float currentTick = GetTickCount();
			float delta = currentTick - lastTick;
			if (delta >= 33.33f)
			{
				mw.onIdle(delta);
				lastTick = currentTick;
			}
		}
	}
	destroyGlobal();

	return 0;
}