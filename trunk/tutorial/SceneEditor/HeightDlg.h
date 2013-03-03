#pragma once
#include "common/atlHead.h"
#include "CEditImpl.h"
#include "resource.h"
#include "IdleHandler.h"
#include "TrackBarCtrlWrapper.h"
class HeightDlg :
	public CDialogImpl<HeightDlg>
	,public CWinDataExchange<HeightDlg>
	,public IdleHandler
{
public:
	enum { IDD = IDD_DIALOG_ChangeHeight};
	BEGIN_MSG_MAP(HeightDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_CHECK_AbsoluteHeight, BN_CLICKED, OnBnClickedCheckAbsoluteheight)
		COMMAND_HANDLER(IDC_CHECK_SmoothAverage, BN_CLICKED, OnBnClickedCheckSmoothAverage)
		COMMAND_HANDLER(IDC_EDIT_AbsoluteHeight, EN_CHANGE, OnEnChangeEditAbsoluteheight)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(HeightDlg)
		DDX_CONTROL(IDC_EDIT_AbsoluteHeight, absoluteHeight)//功能等价于SubclassWindow，接管消息回调
		DDX_INT_RANGE(IDC_EDIT_AbsoluteHeight, aHeight_, 0, 255)//做校验
	END_DDX_MAP()

	IdleHandler_Derived
private:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedCheckAbsoluteheight(WORD wNotifyCode, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedCheckSmoothAverage(WORD wNotifyCode, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEnChangeEditAbsoluteheight(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	bool upateRadius(const EventArgs&);
	bool upateStrength(const EventArgs&);
private:
	TrackBarCtrlWrapper radius_;
	TrackBarCtrlWrapper strength_;
	CEditImpl absoluteHeight;
	int aHeight_;
};