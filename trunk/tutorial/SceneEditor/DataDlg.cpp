#include "DataDlg.h"
#include "Global.h"
#include "luaScript/LuaScript.h"
#include "render/FxManager.h"
#include "model/Part.h"
#include "model/Mesh.h"
#include "model/Material.h"
#include "model/Entity.h"
#include "model/Skeleton.h"
#include "model/Part.h"
#include "EventManager.h"
#include "PreviewWindow.h"
#include "scene/SceneManager.h"
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
	//
	properties_.SubclassWindow(GetDlgItem(IDC_LIST_DataProperty));
	properties_.SetExtendedListStyle(PLS_EX_CATEGORIZED);
	//
	pw_.SubclassWindow( GetDlgItem( IDC_BUTTON_Previewer ) );   
	getGlobal()->setPreviewWindow(&pw_);
	//
	return bHandled;
}

LRESULT DataDlg::OnFileItemSelected( UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	LPCSTR FileName = (LPCSTR)wParam;
	SelectFile(FileName);
	
	return TRUE;
}
LRESULT DataDlg::OnFileItemRightButtonUp( UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	LPCSTR FileName = (LPCSTR)wParam;
	getGlobal()->addEntityInstance(FileName);
	return TRUE;
}

void DataDlg::SelectFile( const tstring& s )
{
	//getLuaScript()->doFile(FileName);
	getGlobal()->onSelectFile(s);
	//
	IFileManager::getFile(s)->update(s, &properties_);
}

void DataDlg::onIdle(const float delta)
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

LRESULT DataDlg::onMouseMove( UINT, WPARAM, LPARAM lParam, BOOL& b )
{
	b = FALSE;
	//获得焦点
	SetFocus(); 
	//SetCapture();
	return 1;
}
LRESULT DataDlg::onMouseLeftButtonDown( UINT, WPARAM, LPARAM lParam, BOOL& b )
{
	SetCapture();
	return 1;
}
LRESULT DataDlg::onMouseLeftButtonUp( UINT, WPARAM, LPARAM lParam, BOOL& b )
{
	ReleaseCapture();
	return 1;
}

void FileNull::update( const std::string& fn, CPropertyListCtrl* pl , bool reset /*= true*/, CCategoryProperty* ct /*= NULL*/)
{
	if (reset)
	{
		pl->ResetContent();
	}
}

void FilePart::update( const std::string& fn, CPropertyListCtrl* pl , bool reset /*= true*/, CCategoryProperty* ct /*= NULL*/)
{
	
}
void FileEntity::update( const std::string& fn, CPropertyListCtrl* pl , bool reset /*= true*/, CCategoryProperty* ct /*= NULL*/)
{
	
}
IFile* IFileManager::getFile( const std::string& fn )
{
	eFile f = eFile_Null;
	tstring ext = FileSystem::getFileExtension(fn);
	SelectModelEventArgs args;
	if (ext == "part")
	{
		f = eFile_Part;
	}
	else if (ext == "entity")
	{
		f = eFile_Entity;
		args.FilePath_ = fn;
	}
	//
	EventManager::GetInstance().fireEvent(eEvent_SelectModel, args);
	return getFile(f);
}

IFile* IFileManager::getFile( eFile f )
{
	static FilePart fp;
	static FileNull fn;
	static FileEntity fe;
	IFile* i = &fn;
	switch(f)
	{
	case eFile_Part:
		{
			i = &fp;
		}
		break;
	case eFile_Entity:
		{
			i = &fe;
		}
		break;
	}
	return i;
}
