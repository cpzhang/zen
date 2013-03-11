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
	CellRadiusSpin_.SetRange(-100000, 100000);//单位，cm
	CellRadiusSpin_.SetPos(0);
	//
	CellRadius_ = 30;
	CellHeight_ = 20;
	AgentRadius_ = 40;
	AgentHeight_ = 200;
	AgentMaxClimb_ = 90;
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
