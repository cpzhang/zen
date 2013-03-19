#pragma once
#include "common/atlHead.h"
#include "misc/FileSystem.h"
#include "CEditImpl.h"
#include "IdleHandler.h"
#include "resource.h"
#include "misc/EventManager.h"
#include "TrackBarCtrlWrapper.h"
class Entity;
class BoneDlg :
	public CDialogImpl<BoneDlg>
{
public:
	enum { IDD = IDD_DIALOG_Bones};
	BEGIN_MSG_MAP(BoneDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroyDialog)
		COMMAND_HANDLER(IDC_LIST_Bones, LBN_SELCHANGE, OnLbnSelchangeListAnimation)
		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenuListBones)
		COMMAND_ID_HANDLER(ID_AddChildNode, OnReferencemodelBind)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	//IdleHandler_Derived
public:
	BoneDlg();
	~BoneDlg();
private:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroyDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLbnSelchangeListAnimation(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnContextMenuListBones(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnReferencemodelBind(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
private:
	bool onModelChanged(const EventArgs& args);
private:
	CListBox bonesList_;
	CListBox nodesList_;
	SubscriberSlot ss_;
};