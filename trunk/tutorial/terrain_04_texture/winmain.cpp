#include "FrameWindow.h"
#include "Global.h"
CAppModule _Module;
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE , LPTSTR lpstrCmdLine, int nCmdShow)
{
	createGlobal();
	FrameWindow mw;
	mw.CreateEx();
	mw.ShowWindow(SW_MAXIMIZE);
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
			mw.onIdle();
		}
	}
	destroyGlobal();
	return 0;
}