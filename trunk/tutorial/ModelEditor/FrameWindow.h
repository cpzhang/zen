#pragma once
#include "misc/atlHead.h"
#include "ViewWindow.h"
#include "IdleHandler.h"
#include "HeightDlg.h"
#include "DataDlg.h"
#include "resource.h"
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
		COMMAND_ID_HANDLER(ID_BUTTON_ChangeHeight, OnToobarChangeHeight)
		COMMAND_ID_HANDLER(ID_BUTTON_PaintTerrain, OnToobarChangeTexture)
		COMMAND_ID_HANDLER(ID_BUTTON_Data, OnToobarData)
		COMMAND_ID_HANDLER(ID_BUTTON_Options, OnToobarOptions)
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
private:
	LRESULT onCreate(UINT, WPARAM, LPARAM, BOOL&);
private:
	//
	LRESULT OnNewscene(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnToobarChangeHeight(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnToobarData(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnToobarChangeTexture(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnToobarOptions(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
private:
	CSplitterWindow verticalSplitter_;
	ViewWindow canvas_;
	CTabView	tabs_;
	HeightDlg	dlgChangeHeight_;
	DataDlg		dlgData_;
	TerrainTextureDlg dlgTerrainTexture_;
	OptionsDlg dlgOptions_;
};