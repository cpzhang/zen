#pragma once
#include "misc/atlHead.h"
#include "ViewWindow.h"
#include "IdleHandler.h"
#include "HeightDlg.h"
class FrameWindow : 
	public CFrameWindowImpl<FrameWindow>
	,public IdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(_T("SceneEditor"), IDR_MAINFRAME);

	BEGIN_MSG_MAP(FrameWindow)
		MESSAGE_HANDLER(WM_CREATE, onCreate)
		COMMAND_ID_HANDLER(ID_NewScene, OnNewscene)
		COMMAND_ID_HANDLER(ID_BUTTON_ChangeHeight, OnToobarChangeHeight)
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
	LRESULT OnToobarChangeHeight(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
private:
	CSplitterWindow verticalSplitter_;
	ViewWindow canvas_;
	CTabView	tabs_;
	HeightDlg	dlgChangeHeight_;
};