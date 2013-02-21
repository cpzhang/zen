#pragma once
#include "misc/stdHead.h"
#include "misc/atlHead.h"
class IRender;
class PreviewWindow : public CWindowImpl<PreviewWindow, CStatic>
{
public:
	BEGIN_MSG_MAP(PreviewWindow)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()
public:
	PreviewWindow()
	{
		model_ = NULL;
	}
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void setModel(IRender* m)
	{
		model_ = m;
	}
public:
	void render();
	IRender* model_;
};
