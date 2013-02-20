#pragma once
#include "misc/stdHead.h"
#include "misc/atlHead.h"
class IRender;
class PreviewWindow : public CWindowImpl<PreviewWindow, CStatic>
{
//public:
	BEGIN_MSG_MAP(PreviewWindow)
		MESSAGE_HANDLER(WM_CREATE, onCreate)
		//MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		//MESSAGE_HANDLER(WM_MOVE, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_PAINT, OnEraseBkgnd)
		//MESSAGE_HANDLER(WM_PAINT, OnEraseBkgnd)
		FORWARD_NOTIFICATIONS()
	END_MSG_MAP()
public:
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT onCreate(UINT, WPARAM, LPARAM, BOOL&) 
	{	//
		return 1;
	}
public:
	void render();
};
