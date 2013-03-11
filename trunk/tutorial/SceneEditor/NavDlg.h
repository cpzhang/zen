#pragma once
#include "common/atlHead.h"
#include "CEditImpl.h"
#include "resource.h"
#include "IdleHandler.h"
#include "TrackBarCtrlWrapper.h"
#include "render/vector3.h"
class NavDlg:
	public CDialogImpl<NavDlg>
	,public CWinDataExchange<NavDlg>
	,public IdleHandler
{
public:
	enum { IDD = IDD_DIALOG_Nav};
	BEGIN_MSG_MAP(NavDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroyDialog)
		COMMAND_HANDLER(IDC_BUTTON_ConstructNavMesh, BN_CLICKED, OnBnClickedButtonConstructnavmesh)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

 	BEGIN_DDX_MAP(NavDlg)
		//
 		DDX_CONTROL(IDC_EDIT_CellRadius, CellRadiusEdit_)//功能等价于SubclassWindow，接管消息回调
		DDX_INT_RANGE(IDC_EDIT_CellRadius, CellRadius_, 1, 200)//
		//
		DDX_CONTROL(IDC_EDIT_CellHeight, CellHeightEdit_)
		DDX_INT_RANGE(IDC_EDIT_CellHeight, CellHeight_, 1, 200)//
		//
		DDX_CONTROL(IDC_EDIT_AgentRadius, AgentRadiusEdit_)
		DDX_INT_RANGE(IDC_EDIT_AgentRadius, AgentRadius_, 1, 200)//
		//
		DDX_CONTROL(IDC_EDIT_AgentHeight, AgentHeightEdit_)
		DDX_INT_RANGE(IDC_EDIT_AgentHeight, AgentHeight_, 1, 200)//
		//
		DDX_CONTROL(IDC_EDIT_AgentMaxClimb, AgentMaxClimbEdit_)
		DDX_INT_RANGE(IDC_EDIT_AgentMaxClimb, AgentMaxClimb_, 1, 200)//
		//
		DDX_CONTROL(IDC_EDIT_AgentMaxSlope, AgentMaxSlopeEdit_)
		DDX_INT_RANGE(IDC_EDIT_AgentMaxSlope, AgentMaxSlope_, 1, 200)//
 	END_DDX_MAP()

	IdleHandler_Derived
private:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroyDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonConstructnavmesh(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
private:
	int CellRadius_;
	int CellHeight_;
	int AgentRadius_;
	int AgentHeight_;
	int AgentMaxClimb_;
	int AgentMaxSlope_;
	//
	CUpDownCtrl CellRadiusSpin_;
	CUpDownCtrl CellHeightSpin_;
	CUpDownCtrl AgentRadiusSpin_;
	CUpDownCtrl AgentHeightSpin_;
	CUpDownCtrl AgentMaxClimbSpin_;
	CUpDownCtrl AgentMaxSlopeSpin_;
	//
	CEditImpl CellRadiusEdit_;
	CEditImpl CellHeightEdit_;
	CEditImpl AgentRadiusEdit_;
	CEditImpl AgentHeightEdit_;
	CEditImpl AgentMaxClimbEdit_;
	CEditImpl AgentMaxSlopeEdit_;
};