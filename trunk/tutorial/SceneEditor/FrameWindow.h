#pragma once
#include "common/atlHead.h"
#include "resource.h"
#include "ViewWindow.h"
#include "IdleHandler.h"
#include "HeightDlg.h"
#include "DataDlg.h"
#include "AnimationDlg.h"
#include "SceneNewDlg.h"
#include "TerrainTextureDlg.h"
#include "PlaceModelDlg.h"
#include "NavDlg.h"
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
		COMMAND_ID_HANDLER(ID_BUTTON_Test, OnTest)
		COMMAND_ID_HANDLER(ID_BUTTON_Hero, OnHero)
		COMMAND_ID_HANDLER(ID_BUTTON_PlaceModel, OnPlaceModel)
		COMMAND_ID_HANDLER(ID_BUTTON_NavigationMesh, OnNavigationMesh)
		COMMAND_ID_HANDLER(ID_SaveScene, OnSavescene)
		COMMAND_ID_HANDLER(ID_OpenScene, OnOpenscene)
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
	LRESULT OnTest(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnHero(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnPlaceModel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNavigationMesh(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	//
	void tabSwitch(WORD id);
private:
	CSplitterWindow verticalSplitter_;
	ViewWindow canvas_;
	CTabView	tabs_;
	HeightDlg	dlgChangeHeight_;
	DataDlg		dlgData_;
	TerrainTextureDlg dlgTerrainTexture_;
	OptionsDlg dlgOptions_;
	AnimationDlg dlgAnimation_;
	SceneNewDlg dlgSceneNew;
	PlaceModelDlg dlgPlaceModel;
	NavDlg dlgNav;
	tstring scenePath_;
	LRESULT OnSavescene(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOpenscene(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};