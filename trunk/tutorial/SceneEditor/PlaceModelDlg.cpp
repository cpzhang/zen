#include "placemodeldlg.h"
#include "Global.h"
#include "render/Decal.h"
#include "StateManager.h"
#include "misc/FileSystem.h"
#include "Misc.h"
#include "dependence/FileTreeCtrl/FileFind.h"
#include "render/math.h"
#include "EventArgs.h"
LRESULT PlaceModelDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	bHandled = false;
	//
	PositionXSpin_.Attach(GetDlgItem(IDC_SPIN_PositionX));
	PositionXEdit_.Attach(GetDlgItem(IDC_EDIT_PositionX));
	PositionXSpin_.SetBuddy(PositionXEdit_);
	PositionXSpin_.SetRange(-100000, 100000);//单位，cm
	PositionXSpin_.SetPos(0);
	//
	PositionYSpin_.Attach(GetDlgItem(IDC_SPIN_PositionY));
	PositionYEdit_.Attach(GetDlgItem(IDC_EDIT_PositionY));
	PositionYSpin_.SetBuddy(PositionYEdit_);
	PositionYSpin_.SetRange(-100000, 100000);//单位，cm
	PositionYSpin_.SetPos(0);
	//
	//
	PositionZSpin_.Attach(GetDlgItem(IDC_SPIN_PositionZ));
	PositionZEdit_.Attach(GetDlgItem(IDC_EDIT_PositionZ));
	PositionZSpin_.SetBuddy(PositionZEdit_);
	PositionZSpin_.SetRange(-100000, 100000);//单位，cm
	PositionZSpin_.SetPos(0);
	//
	//
	ScaleX_.Attach(GetDlgItem(IDC_SPIN_ScaleX));
	ScaleXEdit_.Attach(GetDlgItem(IDC_EDIT_ScaleX));
	ScaleX_.SetBuddy(ScaleXEdit_);
	ScaleX_.SetRange(1, 10000);//单位，%
	ScaleX_.SetPos(100);
	//
	ScaleY_.Attach(GetDlgItem(IDC_SPIN_ScaleY));
	ScaleYEdit_.Attach(GetDlgItem(IDC_EDIT_ScaleY));
	ScaleY_.SetBuddy(ScaleYEdit_);
	ScaleY_.SetRange(1, 10000);//单位，%
	ScaleY_.SetPos(100);
	//
	ScaleZ_.Attach(GetDlgItem(IDC_SPIN_ScaleZ));
	ScaleZEdit_.Attach(GetDlgItem(IDC_EDIT_ScaleZ));
	ScaleZ_.SetBuddy(ScaleZEdit_);
	ScaleZ_.SetRange(1, 10000);//单位，%
	ScaleZ_.SetPos(100);
	//
	RotationX_.Attach(GetDlgItem(IDC_SPIN_RotationX));
	RotationXEdit_.Attach(GetDlgItem(IDC_EDIT_RotationX));
	RotationX_.SetBuddy(RotationXEdit_);
	RotationX_.SetRange(0, 360);//单位，degree
	RotationX_.SetPos(0);
	//
	RotationY_.Attach(GetDlgItem(IDC_SPIN_RotationY));
	RotationYEdit_.Attach(GetDlgItem(IDC_EDIT_RotationY));
	RotationY_.SetBuddy(RotationYEdit_);
	RotationY_.SetRange(0, 360);//单位，degree
	RotationY_.SetPos(0);
	//
	RotationZ_.Attach(GetDlgItem(IDC_SPIN_RotationZ));
	RotationZEdit_.Attach(GetDlgItem(IDC_EDIT_RotationZ));
	RotationZ_.SetBuddy(RotationZEdit_);
	RotationZ_.SetRange(0, 360);//单位，degree
	RotationZ_.SetPos(0);
	//
	ImageList_.Create(32, 32, ILC_COLOR24, 0, 1);
	//icon之间有空隙，此空隙如何设置？
	Icons_.Attach(GetDlgItem(IDC_LIST_Model));
	Icons_.SetExtendedListViewStyle(LVS_EX_BORDERSELECT | LVS_EX_FLATSB | LVS_EX_GRIDLINES);
	Icons_.SetImageList(ImageList_.m_hImageList, LVSIL_NORMAL);
	//
	getGlobal()->addHandler("PlaceModelDlg", this);
	//
	Position_ = Vector3::Zero;
	Scale_ = Vector3::One;
	Rotation_ = Vector3::Zero;
	//
	EventManager::GetInstance().subscribeEvent(PlaceModelDlgPositionChangedEventArgs::tEventName, SubscriberSlot(&PlaceModelDlg::upatePosition, this));
	EventManager::GetInstance().subscribeEvent(PlaceModelDlgFileChangedEventArgs::tEventName, SubscriberSlot(&PlaceModelDlg::upateFile, this));
	//
	CenterWindow();
	return TRUE;
}

void PlaceModelDlg::refreshModelIcons()
{
	FilePaths_.clear();
	ImageList_.RemoveAll();
	Icons_.DeleteAllItems();
	//
	PathSelected_ = FileSystem::addDataDir("model");
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
				//sNewPath = find.GetFilePath();
				//FilePaths.push_back( sNewPath );
			}  
		}
	}
	findEntityFiles(PathSelected_, FilePaths_, "");
	//
	for (size_t i = 0; i != DirectoryPaths.size(); ++i)
	{
		findEntityFiles(PathSelected_ + "\\" + DirectoryPaths[i], FilePaths_, DirectoryPaths[i]);
	}
}

void PlaceModelDlg::onIdle(const float delta)
{
	if (PathSelected_.empty())
	{
		refreshModelIcons();
	}
	//每次只处理一个
	if (!PathSelected_.empty() && !FilePaths_.empty())
	{
		//
		tstring iconFn = FileSystem::removeFileExtension(PathSelected_ + '\\' + FilePaths_[FilePaths_.size() - 1]) + ".bmp";
		if (!FileSystem::isFileExist(iconFn))
		{
			iconFn = PathSelected_ + "\\notfound.dds";
		}
		HBITMAP h = generateHBitMap(iconFn, 32, 32, true);
		ImageList_.Add(h);
		DeleteObject(h);
		Icons_.InsertItem(Icons_.GetItemCount(), FilePaths_[FilePaths_.size() - 1].c_str(), Icons_.GetItemCount());
		FilePaths_.pop_back();
	}
}

void PlaceModelDlg::onRefreshLuaScript()
{
}

LRESULT PlaceModelDlg::OnDestroyDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	PositionXSpin_.Detach();
	return TRUE;
}

LRESULT PlaceModelDlg::OnNMDblclkListIcons(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	// single-selection only
	CString n;
	Icons_.GetItemText(Icons_.GetSelectedIndex(), 0, n);
	getStateManager()->getCurrentState()->setModelFile(PathSelected_ + "\\" + n.GetBuffer(0));
	return 0;
}

LRESULT PlaceModelDlg::OnEnChangeEditPositionx(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (NULL == getStateManager() || NULL == getStateManager()->getCurrentState())
	{
		return 0;
	}
	//
	TCHAR b[64]={0};
	PositionXEdit_.GetLine(0, b, 64);
	int n = atoi(b);
	Position_.x = n;
	Position_.x *= 0.01f;
	getStateManager()->getCurrentState()->setPosition(Position_);
	return 0;
}

LRESULT PlaceModelDlg::OnEnChangeEditPositiony(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (NULL == getStateManager() || NULL == getStateManager()->getCurrentState())
	{
		return 0;
	}
	TCHAR b[64]={0};
	PositionYEdit_.GetLine(0, b, 64);
	int n = atoi(b);
	Position_.y = n;
	Position_.y *= 0.01f;
	getStateManager()->getCurrentState()->setPosition(Position_);
	return 0;
}

LRESULT PlaceModelDlg::OnEnChangeEditPositionz(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (NULL == getStateManager() || NULL == getStateManager()->getCurrentState())
	{
		return 0;
	}
	TCHAR b[64]={0};
	PositionZEdit_.GetLine(0, b, 64);
	int n = atoi(b);
	Position_.z = n;
	Position_.z *= 0.01f;
	getStateManager()->getCurrentState()->setPosition(Position_);
	return 0;
}

LRESULT PlaceModelDlg::OnEnChangeEditRotationx(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (NULL == getStateManager() || NULL == getStateManager()->getCurrentState())
	{
		return 0;
	}
	TCHAR b[64]={0};
	RotationXEdit_.GetLine(0, b, 64);
	int n = atoi(b);
	Rotation_.x = DEG_TO_RAD(n);
	getStateManager()->getCurrentState()->setRotation(Rotation_);
	return 0;
}

LRESULT PlaceModelDlg::OnEnChangeEditRotationy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (NULL == getStateManager() || NULL == getStateManager()->getCurrentState())
	{
		return 0;
	}
	TCHAR b[64]={0};
	RotationYEdit_.GetLine(0, b, 64);
	int n = atoi(b);
	Rotation_.y = DEG_TO_RAD(n);
	getStateManager()->getCurrentState()->setRotation(Rotation_);
	return 0;
}

LRESULT PlaceModelDlg::OnEnChangeEditRotationz(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (NULL == getStateManager() || NULL == getStateManager()->getCurrentState())
	{
		return 0;
	}
	TCHAR b[64]={0};
	RotationZEdit_.GetLine(0, b, 64);
	int n = atoi(b);
	Rotation_.z = DEG_TO_RAD(n);
	getStateManager()->getCurrentState()->setRotation(Rotation_);
	return 0;
}

LRESULT PlaceModelDlg::OnEnChangeEditScalex(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (NULL == getStateManager() || NULL == getStateManager()->getCurrentState())
	{
		return 0;
	}
	TCHAR b[64]={0};
	ScaleXEdit_.GetLine(0, b, 64);
	int n = atoi(b);
	Scale_.x = n;
	Scale_.x *= 0.01;
	getStateManager()->getCurrentState()->setScale(Scale_);
	return 0;
}

LRESULT PlaceModelDlg::OnEnChangeEditScaley(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (NULL == getStateManager() || NULL == getStateManager()->getCurrentState())
	{
		return 0;
	}
	TCHAR b[64]={0};
	ScaleYEdit_.GetLine(0, b, 64);
	int n = atoi(b);
	Scale_.y = n;
	Scale_.y *= 0.01;
	getStateManager()->getCurrentState()->setScale(Scale_);
	return 0;
}

LRESULT PlaceModelDlg::OnEnChangeEditScalez(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (NULL == getStateManager() || NULL == getStateManager()->getCurrentState())
	{
		return 0;
	}
	TCHAR b[64]={0};
	ScaleZEdit_.GetLine(0, b, 64);
	int n = atoi(b);
	Scale_.z = n;
	Scale_.z *= 0.01;
	getStateManager()->getCurrentState()->setScale(Scale_);
	return 0;
}

bool PlaceModelDlg::upatePosition( const EventArgs& a)
{
	PlaceModelDlgPositionChangedEventArgs* args = (PlaceModelDlgPositionChangedEventArgs*)&a;
	PositionXSpin_.SetPos(args->Postion_.x * 100);
	PositionYSpin_.SetPos(args->Postion_.y * 100);
	PositionZSpin_.SetPos(args->Postion_.z * 100);
	return 1;
}

bool PlaceModelDlg::upateFile( const EventArgs& )
{
	PositionXSpin_.SetPos(0);
	PositionYSpin_.SetPos(0);
	PositionZSpin_.SetPos(0);
	//
	ScaleX_.SetPos(100);
	ScaleY_.SetPos(100);
	ScaleZ_.SetPos(100);
	//
	RotationX_.SetPos(0);
	RotationY_.SetPos(0);
	RotationZ_.SetPos(0);
	//
	return 1;
}
