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
#include "EventArgs.h"
#include "PreviewWindow.h"
#include "scene/SceneManager.h"
#include "Misc.h"
#include "dependence/FileTreeCtrl/FileFind.h"
#include "StateManager.h"
LRESULT DataDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	bHandled = TRUE;
	//
	FileTree_.SubclassWindow( ::GetDlgItem( m_hWnd, IDC_TREE_Data ) );
	//FileTree_.SetRootFolder(getGlobal()->getDataRootDirectory());

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false); 
	//
	CenterWindow();
	//
	getGlobal()->addHandler("DataDlg", this);
	//
	//properties_.SubclassWindow(GetDlgItem(IDC_LIST_DataProperty));
	//properties_.SetExtendedListStyle(PLS_EX_CATEGORIZED);
	//
	pw_.SubclassWindow( GetDlgItem( IDC_BUTTON_Previewer ) );   
	getGlobal()->setPreviewWindow(&pw_);
	//
	ImageList_.Create(64, 64, ILC_COLOR24, 0, 1);
	//icon之间有空隙，此空隙如何设置？
	Icons_.Attach(GetDlgItem(IDC_LIST_Icons));
	Icons_.SetExtendedListViewStyle(LVS_EX_BORDERSELECT | LVS_EX_FLATSB | LVS_EX_GRIDLINES);
	Icons_.SetImageList(ImageList_.m_hImageList, LVSIL_NORMAL);
	//
	return bHandled;
}

LRESULT DataDlg::OnFileItemSelected( UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	LPCSTR FileName = (LPCSTR)wParam;
	SelectFile(FileName);
	
	return TRUE;
}

void DataDlg::SelectFile( const tstring& s )
{

	//getLuaScript()->doFile(FileName);
	getGlobal()->onSelectFile(s);
	//
	//IFileManager::getFile(s)->update(s, &properties_);
}
bool DataDlg::isBrushDirSelected()
{
	tstring bd = FileSystem::addDataDir("brush");
	if (PathSelected_ == bd)
	{
		return true;
	}
	return false;
}
bool DataDlg::isModelDirSelected()
{
	tstring bd = FileSystem::addDataDir("model");
	if (PathSelected_.compare(0, bd.size(), bd) == 0)
	{
		return true;
	}
	return false;
}
void DataDlg::refreshBrushIcons()
{
	FilePaths_.clear();
	ImageList_.RemoveAll();
	Icons_.DeleteAllItems();
	// Find all the directories and files underneath sPath
	std::vector< std::string > DirectoryPaths;
	CFileFind find;
	std::string sFile(PathSelected_ + _T("\\*.*"));
	std::string sNewPath;

	BOOL bFind = find.FindFile( sFile.c_str() );  
	while( bFind )
	{
		bFind = find.FindNextFile();
		if( find.IsDirectory() )
		{
			if( !find.IsDots() )
			{
				sNewPath = find.GetFilePath();
				DirectoryPaths.push_back( sNewPath );
			}
		}
		else 
		{
			if( !find.IsHidden())
			{
				sNewPath = find.GetFilePath();
				FilePaths_.push_back( sNewPath );
			}  
		}
	}
}
HBITMAP generateHBitMapFromModel(const std::string& fileName, int width, int height)
{
	getGlobal()->getPreviewWindow()->setModel(fileName);
	tstring fn = getGlobal()->saveBackBuffer(FileSystem::getParent(fileName));
	return generateHBitMap(fn, width, height, false);
}
void DataDlg::refreshModelIcons()
{
	// Find all the directories and files underneath sPath
	std::vector< std::string > DirectoryPaths;
	std::vector< std::string > FilePaths;

	CFileFind find;
	std::string sFile(PathSelected_ + _T("\\*.*"));
	std::string sNewPath;

	BOOL bFind = find.FindFile( sFile.c_str() );  
	while( bFind )
	{
		bFind = find.FindNextFile();
		if( find.IsDirectory() )
		{
			if( !find.IsDots() )
			{
				sNewPath = find.GetFilePath();
				DirectoryPaths.push_back( sNewPath );
			}
		}
		else 
		{
			if( !find.IsHidden())
			{
				//sNewPath = find.GetFilePath();
				//FilePaths.push_back( sNewPath );
			}  
		}
	}
	findEntityFiles(PathSelected_, FilePaths, "");
	//
	for (size_t i = 0; i != DirectoryPaths.size(); ++i)
	{
		findEntityFiles(PathSelected_ + "\\" + DirectoryPaths[i], FilePaths, DirectoryPaths[i]);
	}
	//
	if (!FilePaths.empty())
	{
		ImageList_.RemoveAll();
		for (size_t i = 0; i != FilePaths.size(); ++i)
		{
			HBITMAP h = generateHBitMapFromModel(PathSelected_ + '\\' + FilePaths[i], 64, 64);
			ImageList_.Add(h);
			DeleteObject(h);
		}
		//
		Icons_.DeleteAllItems();
		for (size_t i = 0; i != FilePaths.size(); ++i)
		{
			tstring fn(PathSelected_ + '\\' + FilePaths[i]);
			fn = FileSystem::cutDataPath(fn);
			Icons_.InsertItem(i, fn.c_str(), i);
		}
	}
}

void DataDlg::onIdle(const float delta)
{
	std::string sn = FileTree_.GetSelectedPath();
	if (sn != PathSelected_)
	{
		PathSelected_ = sn;
		//耗时，可以考虑的方案：1.异步 2.预生成 3.每次只处理一个
		if (isBrushDirSelected())
		{
			refreshBrushIcons();
		}
		else if (isModelDirSelected())
		{
			refreshModelIcons();
		}
	}
	else
	{
		//3.每次只处理一个
		if (!PathSelected_.empty() && !FilePaths_.empty())
		{
			//
			HBITMAP h = generateHBitMap(PathSelected_ + '\\' + FilePaths_[FilePaths_.size() - 1], 64, 64, true);
			ImageList_.Add(h);
			DeleteObject(h);
			Icons_.InsertItem(Icons_.GetItemCount(), FilePaths_[FilePaths_.size() - 1].c_str(), Icons_.GetItemCount());
			FilePaths_.pop_back();
		}
	}
}

void DataDlg::onRefreshLuaScript()
{
	tstring s = FileSystem::toWindowsFilePath(getGlobal()->getDataRootDirectory());
	FileTree_.SetRootFolder(s);
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
	//std::string d = FileTree_.ItemToPath(0);
	//FileTree_.SetSelectedPath(d);
	//FileTree_.OnViewRefresh();
	FileTree_.ReLoad();
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

LRESULT DataDlg::OnSelectIcon( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
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
	EventManager::GetInstance().fireEvent(SelectModelEventArgs::tEventName, args);
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

LRESULT DataDlg::OnNMDblclkListIcons(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	// single-selection only
	CString n;
	Icons_.GetItemText(Icons_.GetSelectedIndex(), 0, n);
	getStateManager()->getCurrentState()->setModelFile(n.GetBuffer(0));
	return 0;
}
