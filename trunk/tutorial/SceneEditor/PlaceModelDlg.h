#pragma once
#include "common/atlHead.h"
#include "CEditImpl.h"
#include "resource.h"
#include "IdleHandler.h"
#include "TrackBarCtrlWrapper.h"
#include "render/vector3.h"
class PlaceModelDlg:
	public CDialogImpl<PlaceModelDlg>
	//,public CWinDataExchange<PlaceModelDlg>
	,public IdleHandler
{
public:
	enum { IDD = IDD_DIALOG_PlaceModel};
	BEGIN_MSG_MAP(PlaceModelDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroyDialog)
		NOTIFY_HANDLER(IDC_LIST_Model, NM_DBLCLK, OnNMDblclkListIcons)
		COMMAND_HANDLER(IDC_EDIT_PositionX, EN_CHANGE, OnEnChangeEditPositionx)
		COMMAND_HANDLER(IDC_EDIT_PositionY, EN_CHANGE, OnEnChangeEditPositiony)
		COMMAND_HANDLER(IDC_EDIT_PositionZ, EN_CHANGE, OnEnChangeEditPositionz)
		COMMAND_HANDLER(IDC_EDIT_RotationX, EN_CHANGE, OnEnChangeEditRotationx)
		COMMAND_HANDLER(IDC_EDIT_RotationY, EN_CHANGE, OnEnChangeEditRotationy)
		COMMAND_HANDLER(IDC_EDIT_RotationZ, EN_CHANGE, OnEnChangeEditRotationz)
		COMMAND_HANDLER(IDC_EDIT_ScaleX, EN_CHANGE, OnEnChangeEditScalex)
		COMMAND_HANDLER(IDC_EDIT_ScaleY, EN_CHANGE, OnEnChangeEditScaley)
		COMMAND_HANDLER(IDC_EDIT_ScaleZ, EN_CHANGE, OnEnChangeEditScalez)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

// 	BEGIN_DDX_MAP(PlaceModelDlg)
// 		DDX_CONTROL(IDC_EDIT_AbsoluteHeight, absoluteHeight)//功能等价于SubclassWindow，接管消息回调
// 		DDX_INT_RANGE(IDC_EDIT_AbsoluteHeight, aHeight_, 0, 255)//做校验
// 	END_DDX_MAP()

	IdleHandler_Derived
private:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroyDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNMDblclkListIcons(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
	LRESULT OnDeltaposSpinPositionx(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
	LRESULT OnEnChangeEditPositionx(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEnChangeEditPositiony(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEnChangeEditPositionz(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEnChangeEditRotationx(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEnChangeEditRotationy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEnChangeEditRotationz(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEnChangeEditScalex(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEnChangeEditScaley(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEnChangeEditScalez(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void refreshModelIcons();
	bool upatePosition(const EventArgs&);
	bool upateFile(const EventArgs&);
private:
	CUpDownCtrl PositionXSpin_;
	CUpDownCtrl PositionYSpin_;
	CUpDownCtrl PositionZSpin_;
	CUpDownCtrl ScaleX_;
	CUpDownCtrl ScaleY_;
	CUpDownCtrl ScaleZ_;
	CUpDownCtrl RotationX_;
	CUpDownCtrl RotationY_;
	CUpDownCtrl RotationZ_;
	//
	CEdit PositionXEdit_;
	CEdit PositionYEdit_;
	CEdit PositionZEdit_;
	CEdit ScaleXEdit_;
	CEdit ScaleYEdit_;
	CEdit ScaleZEdit_;
	CEdit RotationXEdit_;
	CEdit RotationYEdit_;
	CEdit RotationZEdit_;
	//
	tstring PathSelected_;
	std::vector< std::string > FilePaths_;
	CImageList ImageList_;
	CListViewCtrl Icons_;
	//
	Vector3 Position_;
	Vector3 Scale_;
	Vector3 Rotation_;
};