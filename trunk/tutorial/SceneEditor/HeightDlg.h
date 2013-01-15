#pragma once
#include "misc/atlHead.h"
#include "CEditImpl.h"
#include "resource.h"
#include "IdleHandler.h"
class RadiusTrackBarCtrl : public CWindowImpl<RadiusTrackBarCtrl, CTrackBarCtrl>
{
public:
	BEGIN_MSG_MAP(RadiusTrackBarCtrl)
		MESSAGE_HANDLER(WM_HSCROLL + OCM__BASE, OnNMThemeChangedSlider)//父窗口没有处理，reflect给子窗口是消息码加了OCM__BASE
	END_MSG_MAP()
public:
	LRESULT OnNMThemeChangedSlider( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
};
class StrengthTrackBarCtrl : public CWindowImpl<StrengthTrackBarCtrl, CTrackBarCtrl>
{
public:
	BEGIN_MSG_MAP(StrengthTrackBarCtrl)
		MESSAGE_HANDLER(WM_HSCROLL + OCM__BASE, OnNMThemeChangedSlider)//父窗口没有处理，reflect给子窗口是消息码加了OCM__BASE
	END_MSG_MAP()
public:
	LRESULT OnNMThemeChangedSlider( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
};
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
private:
	RadiusTrackBarCtrl radius_;
	StrengthTrackBarCtrl strength_;
	CEditImpl absoluteHeight;
	int aHeight_;
};