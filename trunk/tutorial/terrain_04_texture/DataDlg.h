#pragma once
#include "misc/atlHead.h"
#include "CEditImpl.h"
#include "dependence/FileTreeCtrl/WtlFileTreeCtrl.h"
#include "resource.h"
#include "IdleHandler.h"
#include "dependence/PropertyList/PropertyList.h"
class DataDlg :
	public CDialogImpl<DataDlg>
	,public CWinDataExchange<DataDlg>
	,public IdleHandler
{
public:
	enum { IDD = IDD_DIALOG_Data};
	BEGIN_MSG_MAP(DataDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroyDialog)
		MESSAGE_HANDLER( WM_ITEM_SELECTED, OnFileItemSelected )
		COMMAND_HANDLER(IDC_BUTTON_DataRefresh, BN_CLICKED, OnBnClickedButtonDatarefresh)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(DataDlg)
	END_DDX_MAP()

	IdleHandler_Derived
private:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroyDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnFileItemSelected(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	CWtlFileTreeCtrl fileTree_;
	LRESULT OnBnClickedButtonDatarefresh(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

class TerrainTextureDlg :
	public CDialogImpl<TerrainTextureDlg>
{
public:
	enum { IDD = IDD_DIALOG_TerrainTexture};
	BEGIN_MSG_MAP(TerrainTextureDlg)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()
};

class OptionsDlg :
	public CDialogImpl<OptionsDlg>
{
public:
	enum { IDD = IDD_DIALOG_Options};
	BEGIN_MSG_MAP(OptionsDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		NOTIFY_HANDLER(IDC_LIST_Options, PIN_ITEMCHANGED, OnItemChanged)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()
public:
	LRESULT OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
	{
		bHandled = TRUE;
		//
		//
		properties_.SubclassWindow(GetDlgItem(IDC_LIST_Options));
		properties_.SetExtendedListStyle(PLS_EX_CATEGORIZED);
		//
		HPROPERTY hBehaviour = properties_.AddItem( PropCreateCategory(_T("渲染管道")) );
		//类型
		LPCTSTR list[] = { _T("0 - 固定管道"), _T("1 - Shader2.0"), NULL};
		properties_.AddItem( PropCreateList(_T("ShaderModel"), list, 0), hBehaviour);
		return bHandled;
	}
	LRESULT OnItemChanged( int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/ );

private:
	CPropertyListCtrl properties_;
};