#include "NavDlg.h"
#include "Global.h"
#include "render/Decal.h"
#include "StateManager.h"
#include "misc/FileSystem.h"
#include "Misc.h"
#include "dependence/FileTreeCtrl/FileFind.h"
#include "render/math.h"
#include "EventArgs.h"
LRESULT NavDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	bHandled = false;
	//
	CellRadiusSpin_.Attach(GetDlgItem(IDC_SPIN_CellRadius));
	CellRadiusSpin_.SetBuddy(GetDlgItem(IDC_EDIT_CellRadius));
	CellRadiusSpin_.SetRange(-100000, 100000);//µ¥Î»£¬cm
	CellRadiusSpin_.SetPos(0);
	//
	CellRadius_ = 1;
	CellHeight_ = 1;
	AgentRadius_ = 1;
	AgentHeight_ = 1;
	AgentMaxClimb_ = 1;
	AgentMaxSlope_ = 1;
	// First DDX call, hooks up variables to controls.
	DoDataExchange(false); 
	//
	CenterWindow();
	return TRUE;
}

void NavDlg::onIdle(const float delta)
{
	
}

void NavDlg::onRefreshLuaScript()
{
}

LRESULT NavDlg::OnDestroyDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	
	return TRUE;
}
