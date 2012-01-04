#include "DataDlg.h"
#include "Global.h"
#include "luaScript/LuaScript.h"
#include "render/FxManager.h"
LRESULT DataDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	bHandled = TRUE;
	//
	fileTree_.SubclassWindow( ::GetDlgItem( m_hWnd, IDC_TREE_Data ) );
	//fileTree_.SetRootFolder(getGlobal()->getDataRootDirectory());

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false); 
	//
	CenterWindow();
	//
	getGlobal()->addHandler("DataDlg", this);
	return bHandled;
}

LRESULT DataDlg::OnFileItemSelected( UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	LPCSTR FileName = (LPCSTR)wParam;
	//getLuaScript()->doFile(FileName);
	getGlobal()->setCurrentLayer(FileName);
	return TRUE;
}


void DataDlg::onIdle()
{
}

void DataDlg::onRefreshLuaScript()
{
	fileTree_.SetRootFolder(getGlobal()->getDataRootDirectory());
}

LRESULT DataDlg::OnDestroyDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	getGlobal()->removeHandler("DataDlg");
	return 1;
}

LRESULT OptionsDlg::OnItemChanged( int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/ )
{
	LPNMPROPERTYITEM nmp = (LPNMPROPERTYITEM)pnmh;
	bool pathParasChanged =false;
	if (nmp->prop == properties_.FindProperty(_T("ShaderModel")))
	{
		CComVariant v;
		properties_.GetItemValue(nmp->prop, &v);
		getFxManager()->setShaderType((eFx)v.iVal);
		//
	}
	return 0;
}

LRESULT DataDlg::OnBnClickedButtonDatarefresh(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	//std::string d = fileTree_.ItemToPath(0);
	//fileTree_.SetSelectedPath(d);
	//fileTree_.OnViewRefresh();
	fileTree_.ReLoad();
	//getGlobal()->refreshDataRoot();
	return 0;
}
