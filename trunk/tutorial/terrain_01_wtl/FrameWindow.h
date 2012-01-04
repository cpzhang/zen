#pragma once
#include "atlHeaders.h"
#include "ViewWindow.h"
#include "IdleHandler.h"
class FrameWindow : 
	public CFrameWindowImpl<FrameWindow>
	,public IdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(_T("SceneEditor"), IDR_MAINFRAME);

	BEGIN_MSG_MAP(FrameWindow)
		MESSAGE_HANDLER(WM_CREATE, onCreate)
		COMMAND_ID_HANDLER(ID_NewScene, OnNewscene)
		CHAIN_MSG_MAP(CFrameWindowImpl<FrameWindow>)
		REFLECT_NOTIFICATIONS()// to child
	END_MSG_MAP()

	IdleHandler_Derived
public:
	FrameWindow();
private:
	LRESULT onCreate(UINT, WPARAM, LPARAM, BOOL&);
private:
	//
	LRESULT OnNewscene(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
private:
	CSplitterWindow verticalSplitter_;
	ViewWindow canvas_;
};

