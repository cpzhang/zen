#pragma once
#include "common/atlHead.h"
#include "misc/FileSystem.h"
#include "CEditImpl.h"
#include "IdleHandler.h"
#include "resource.h"
#include "misc/EventManager.h"
#include "TrackBarCtrlWrapper.h"
class Entity;
class AnimationDlg :
	public CDialogImpl<AnimationDlg>
{
public:
	enum { IDD = IDD_DIALOG_Animation};
	BEGIN_MSG_MAP(AnimationDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroyDialog)
		COMMAND_HANDLER(IDC_LIST_Animation, LBN_SELCHANGE, OnLbnSelchangeListAnimation)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	//IdleHandler_Derived
public:
	AnimationDlg();
	~AnimationDlg();
private:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroyDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLbnSelchangeListAnimation(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
private:
	bool onModelChanged(const EventArgs& args);
	bool upateRadius( const EventArgs& args);
private:
	TrackBarCtrlWrapper radius_;
	CListBox properties_;
	SubscriberSlot ss_;
};