#include "FrameWindow.h"
#include "Global.h"
CAppModule _Module;
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE , LPTSTR lpstrCmdLine, int nCmdShow)
{
	createGlobal();
	FrameWindow mw;
	mw.CreateEx();
	mw.ShowWindow(SW_SHOWMAXIMIZED);
	mw.UpdateWindow();
	MSG m;
	::ZeroMemory(&m, sizeof(m));
	float life = 0.0f;
	float frame = 0.0f;
	float fps = 0.0f;
	const size_t tDuration = 1000;
	const size_t tInterval = 1000/65;
	float inter = tInterval;
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
			// GetTickCount only has a guaranteed resolution of 1/18th of a second, which is pretty horrible for game timing
			//Retrieves the number of milliseconds that have elapsed since the system was started, up to 49.7 days.
			float beforeRun = ::timeGetTime();
			mw.onIdle(inter);
			float afterRun = ::timeGetTime();
			float timeRun = afterRun - beforeRun;
			//一次赶上来，会造成动画不连续，可考虑均匀分布，平滑
			if (timeRun > tInterval)
			{
				inter = timeRun;
			}
			//else
			{
				inter = tInterval;
			}
			++frame;
			float timeSleep = tInterval - timeRun;
			//计算FPS
 			life += inter;
 			if (life >= tDuration)
 			{
 				fps = 1000.0f * frame / life;
//				mw.setFPS(fps);
 				life = 0.0f;
 				frame = 0.0f;
 			}
			//限帧
			while(timeSleep >= 1.0f)
			{
				Sleep(1);
				--timeSleep;
			}
		}
	}
	destroyGlobal();

	return 0;
}