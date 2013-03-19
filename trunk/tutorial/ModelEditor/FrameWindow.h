#pragma once
#include "common/atlHead.h"
#include "resource.h"
#include "ViewWindow.h"
#include "IdleHandler.h"
#include "DataDlg.h"
#include "AnimationDlg.h"
#include "BoneDlg.h"
class FrameWindow : 
	public CFrameWindowImpl<FrameWindow>
	,public CUpdateUI<FrameWindow>
	,public IdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(_T("SceneEditor"), IDR_MAINFRAME);

	BEGIN_MSG_MAP(FrameWindow)
		MESSAGE_HANDLER(WM_CREATE, onCreate)
		COMMAND_ID_HANDLER(ID_NewScene, OnNewscene)
		COMMAND_ID_HANDLER(ID_BUTTON_Data, OnToobarData)
		COMMAND_ID_HANDLER(ID_BUTTON_Options, OnToobarOptions)
		COMMAND_ID_HANDLER(ID_BUTTON_Animation, OnToobarAnimation)
		COMMAND_ID_HANDLER(ID_BUTTON_Go, OnGo)
		CHAIN_MSG_MAP(CFrameWindowImpl<FrameWindow>)
		CHAIN_MSG_MAP(CUpdateUI<FrameWindow>)
		REFLECT_NOTIFICATIONS()// to child
	END_MSG_MAP()

	BEGIN_UPDATE_UI_MAP(FrameWindow)
		UPDATE_ELEMENT(ID_BUTTON_ChangeHeight, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_BUTTON_Data, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_BUTTON_PaintTerrain, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_BUTTON_Options, UPDUI_TOOLBAR)
	END_UPDATE_UI_MAP()


	IdleHandler_Derived
public:
	FrameWindow();
	void setFPS(float fps);
private:
	LRESULT onCreate(UINT, WPARAM, LPARAM, BOOL&);
private:
	//
	LRESULT OnNewscene(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnToobarData(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnToobarOptions(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnToobarAnimation(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnGo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	//
	void tabSwitch(WORD id);
private:
	CSplitterWindow verticalSplitter_;
	ViewWindow canvas_;
	CTabView	tabs_;
	DataDlg		dlgData_;
	OptionsDlg dlgOptions_;
	AnimationDlg dlgAnimation_;
	BoneDlg dlgBones_;
};