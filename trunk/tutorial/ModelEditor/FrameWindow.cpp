#include "FrameWindow.h"
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
	tabs_.AddPage(dlgData_, TEXT("资源"));
	//
	dlgOptions_.Create(tabs_);
	tabs_.AddPage(dlgOptions_, TEXT("选项"));
	//
	dlgAnimation_.Create(tabs_);
	tabs_.AddPage(dlgAnimation_, TEXT("动画"));
	//
	dlgBones_.Create(tabs_);
	tabs_.AddPage(dlgBones_, TEXT("骨骼"));
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
	
	return 0;
}

void FrameWindow::onIdle(const float delta)
{
	UIUpdateToolBar(FALSE);
	canvas_.onIdle(delta);
}

void FrameWindow::onRefreshLuaScript()
{
	canvas_.onRefreshLuaScript();
}

LRESULT FrameWindow::OnToobarData( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	//getStateManager()->gotoState(eState_Data);
	tabSwitch(ID_BUTTON_Data);
	tabs_.SetActivePage(0);
	return 0;
}

LRESULT FrameWindow::OnToobarOptions( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	//getStateManager()->gotoState(eState_Data);
	tabSwitch(ID_BUTTON_Options);
	tabs_.SetActivePage(1);
	return 0;
}

void FrameWindow::tabSwitch(WORD id)
{
	UISetCheck(ID_BUTTON_Options, false);
	UISetCheck(ID_BUTTON_Data, false);
	//
	UISetCheck(id, true);
}
LRESULT FrameWindow::OnGo( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	dlgData_.SelectFile(TEXT("f:\\zen\\data\\model\\Character_1015\\Character_1015.entity"));
	return 0;
}

LRESULT FrameWindow::OnToobarAnimation( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	tabSwitch(ID_BUTTON_Animation);
	tabs_.SetActivePage(2);
	return 0;
}

void FrameWindow::setFPS( float fps )
{
	canvas_.setFPS(fps);
}
