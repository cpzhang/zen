#include "NavDlg.h"
#include "Global.h"
#include "render/Decal.h"
#include "StateManager.h"
#include "misc/FileSystem.h"
#include "Misc.h"
#include "dependence/FileTreeCtrl/FileFind.h"
#include "render/math.h"
#include "EventArgs.h"
#include "scene/SceneManager.h"
#include "scene/navigation.h"
#include "font/FlowText.h"
LRESULT NavDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	bHandled = false;
	//
	CellRadiusSpin_.Attach(GetDlgItem(IDC_SPIN_CellRadius));
	CellRadiusSpin_.SetBuddy(GetDlgItem(IDC_EDIT_CellRadius));
	CellRadiusSpin_.SetRange(0, 1000);//单位，cm
	CellRadiusSpin_.SetPos(30);
	//
	CellRadius_ = 30;
	//
	CellHeightSpin_.Attach(GetDlgItem(IDC_SPIN_CellHeight));
	CellHeightSpin_.SetBuddy(GetDlgItem(IDC_EDIT_CellHeight));
	CellHeightSpin_.SetRange(0, 1000);//单位，cm
	CellHeightSpin_.SetPos(20);
	CellHeight_ = 20;
	//
	AgentRadiusSpin_.Attach(GetDlgItem(IDC_SPIN_AgentRadius));
	AgentRadiusSpin_.SetBuddy(GetDlgItem(IDC_EDIT_AgentRadius));
	AgentRadiusSpin_.SetRange(0, 1000);//单位，cm
	AgentRadiusSpin_.SetPos(40);
	AgentRadius_ = 40;
	//
	//
	AgentHeightSpin_.Attach(GetDlgItem(IDC_SPIN_AgentHeight));
	AgentHeightSpin_.SetBuddy(GetDlgItem(IDC_EDIT_AgentHeight));
	AgentHeightSpin_.SetRange(0, 1000);//单位，cm
	AgentHeightSpin_.SetPos(200);
	AgentHeight_ = 200;
	//
	//
	AgentMaxClimbSpin_.Attach(GetDlgItem(IDC_SPIN_AgentMaxClimb));
	AgentMaxClimbSpin_.SetBuddy(GetDlgItem(IDC_EDIT_AgentMaxClimb));
	AgentMaxClimbSpin_.SetRange(0, 1000);//单位，cm
	AgentMaxClimbSpin_.SetPos(90);
	AgentMaxClimb_ = 90;
	//
	//
	AgentMaxSlopeSpin_.Attach(GetDlgItem(IDC_SPIN_AgentMaxSlope));
	AgentMaxSlopeSpin_.SetBuddy(GetDlgItem(IDC_EDIT_AgentMaxSlope));
	AgentMaxSlopeSpin_.SetRange(0, 1000);//单位，cm
	AgentMaxSlopeSpin_.SetPos(45);
	AgentMaxSlope_ = 45;
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

LRESULT NavDlg::OnBnClickedButtonConstructnavmesh(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	if (DoDataExchange(true))
	{
		NAVIGATIONCONFIGURATION* c = getSceneManager()->getNavConfig();
		if (c)
		{
			c->cell_size = 0.01f * (float)CellRadius_;
			c->cell_height = 0.01f * (float)CellHeight_;
			c->agent_radius = 0.01f * (float)AgentRadius_;
			c->agent_height = 0.01f * (float)AgentHeight_;
			c->agent_max_climb = 0.01f * (float)AgentMaxClimb_;
			c->agent_max_slope = (float)AgentMaxSlope_;
			//
			if (getSceneManager()->buildNav())
			{
				FlowText::getSingletonP()->add("导航网格生成成功", Vector4(1, 1, 1, 1));
			}
			else
			{
				FlowText::getSingletonP()->add("导航网格生成失败", Vector4(1, 1, 1, 1));
			}
		}
	}
	return 0;
}
