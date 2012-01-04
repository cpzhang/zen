#include "FrameWindow.h"
#include "SceneNewDlg.h"
#include "StateManager.h"
FrameWindow::FrameWindow()
{

}

LRESULT FrameWindow::onCreate( UINT, WPARAM, LPARAM, BOOL& )
{
	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
	UIAddToolBar(hWndToolBar);
	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

	//
	verticalSplitter_.Create ( *this, rcDefault );
	m_hWndClient = verticalSplitter_;	
	verticalSplitter_.SetSplitterExtendedStyle(SPLIT_FIXEDBARSIZE);

	//
	canvas_.Create(verticalSplitter_, rcDefault);
	verticalSplitter_.SetSplitterPane(SPLIT_PANE_LEFT, canvas_);

	//
	tabs_.Create(verticalSplitter_, rcDefault);
	verticalSplitter_.SetSplitterPane(SPLIT_PANE_RIGHT, tabs_);
	//
	dlgData_.Create(tabs_);
	tabs_.AddPage(dlgData_, TEXT("美术资源"));
	//
	dlgTerrainTexture_.Create(tabs_);
	tabs_.AddPage(dlgTerrainTexture_, TEXT("地表纹理"));
	//
	dlgChangeHeight_.Create(tabs_);
	tabs_.AddPage(dlgChangeHeight_, TEXT("地形高度"));
	//
	dlgOptions_.Create(tabs_);
	tabs_.AddPage(dlgOptions_, TEXT("选项"));
	//
	UpdateLayout();

	/*
    Note that you need to set m_hWndClient and call CFrameWindowImpl::UpdateLayout() before setting the splitter position. 
    UpdateLayout() resizes the splitter window to its initial size. 
    If you skip that step, the splitter's size isn't under your control and it might be smaller than 200 pixels wide. 
    The end result would be that SetSplitterPos() wouldn't have the effect you wanted.
  */
	verticalSplitter_.SetSplitterPos(1024);
 
	return 0;
}


LRESULT FrameWindow::OnNewscene(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加命令处理程序代码
	SceneNewDlg dlg;
	dlg.DoModal();

	return 0;
}

void FrameWindow::onIdle()
{
	UIUpdateToolBar(FALSE);
	canvas_.onIdle();
}

void FrameWindow::onRefreshLuaScript()
{
	canvas_.onRefreshLuaScript();
}

LRESULT FrameWindow::OnToobarChangeHeight( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	getStateManager()->gotoState(eState_TerrainHeight);
	UISetCheck(ID_BUTTON_Data, false);
	UISetCheck(ID_BUTTON_ChangeHeight, true);
	UISetCheck(ID_BUTTON_PaintTerrain, false);
	UISetCheck(ID_BUTTON_Options, false);
	tabs_.SetActivePage(2);
	return 0;
}

LRESULT FrameWindow::OnToobarChangeTexture( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	getStateManager()->gotoState(eState_TerrainTexture);
	UISetCheck(ID_BUTTON_Data, false);
	UISetCheck(ID_BUTTON_PaintTerrain, true);
	UISetCheck(ID_BUTTON_ChangeHeight, false);
	UISetCheck(ID_BUTTON_Options, false);
	tabs_.SetActivePage(1);
	return 0;
}

LRESULT FrameWindow::OnToobarData( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	//getStateManager()->gotoState(eState_Data);
	UISetCheck(ID_BUTTON_Data, true);
	UISetCheck(ID_BUTTON_PaintTerrain, false);
	UISetCheck(ID_BUTTON_ChangeHeight, false);
	UISetCheck(ID_BUTTON_Options, false);
	tabs_.SetActivePage(0);
	return 0;
}

LRESULT FrameWindow::OnToobarOptions( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	//getStateManager()->gotoState(eState_Data);
	UISetCheck(ID_BUTTON_Options, true);
	UISetCheck(ID_BUTTON_Data, false);
	UISetCheck(ID_BUTTON_ChangeHeight, false);
	UISetCheck(ID_BUTTON_PaintTerrain, false);
	tabs_.SetActivePage(3);
	return 0;
}
