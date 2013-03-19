#include "TerrainTextureDlg.h"
#include "StateManager.h"
#include "render/Decal.h"
#include "Global.h"
#include "misc/FileSystem.h"
#include "dependence/FileTreeCtrl/FileFind.h"
#include "Misc.h"
LRESULT TerrainTextureDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	//
	BrushStrength_.SubclassWindow(GetDlgItem(IDC_SLIDER_TerrainTexture_BrushStrength));
	BrushStrength_.SetRange(1, 255);
	BrushStrength_.SetPos(10);
	BrushStrength_.registerCallback(SubscriberSlot(&TerrainTextureDlg::upateStrength, this));
	//
	//
	BrushRadiusInner_.SubclassWindow(GetDlgItem(IDC_SLIDER_TerrainTexture_RadiusInner));
	BrushRadiusInner_.SetRange(2, 10);
	BrushRadiusInner_.SetPos(3);
	BrushRadiusInner_.registerCallback(SubscriberSlot(&TerrainTextureDlg::upateRadiusInner, this));
	//
	BrushRadiusOuter_.SubclassWindow(GetDlgItem(IDC_SLIDER_TerrainTexture_RadiusOuter));
	BrushRadiusOuter_.SetRange(3, 20);
	BrushRadiusOuter_.SetPos(6);
	BrushRadiusOuter_.registerCallback(SubscriberSlot(&TerrainTextureDlg::upateRadiusOuter, this));
	//
	ImageList_.Create(32, 32, ILC_COLOR24, 0, 1);
	//icon之间有空隙，此空隙如何设置？
	Icons_.Attach(GetDlgItem(IDC_LIST_TerrainTexture));
	Icons_.SetExtendedListViewStyle(LVS_EX_BORDERSELECT | LVS_EX_FLATSB | LVS_EX_GRIDLINES);
	Icons_.SetImageList(ImageList_.m_hImageList, LVSIL_NORMAL);
	return 1;
}
void TerrainTextureDlg::refreshModelIcons()
{
	FilePaths_.clear();
	ImageList_.RemoveAll();
	Icons_.DeleteAllItems();
	//
	PathSelected_ = FileSystem::addDataDir("brush");
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
void TerrainTextureDlg::onIdle(const float delta)
{
	if (PathSelected_.empty())
	{
		refreshModelIcons();
	}
	//每次只处理一个
	if (!PathSelected_.empty() && !FilePaths_.empty())
	{
		//
		tstring iconFn =PathSelected_ + '\\' + FilePaths_[FilePaths_.size() - 1];
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
LRESULT TerrainTextureDlg::OnNMDblclkListIcons(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	// single-selection only
	CString n;
	Icons_.GetItemText(Icons_.GetSelectedIndex(), 0, n);
	getStateManager()->getCurrentState()->setModelFile(PathSelected_ + "\\" + n.GetBuffer(0));
	return 0;
}

bool TerrainTextureDlg::upateStrength( const EventArgs& args)
{
	const TrackBarCtrlWrapperEventArgs* m = (TrackBarCtrlWrapperEventArgs*)&args;
	getStateManager()->getCurrentState()->setBrushStrength(m->nPercent);
	return true;
}

bool TerrainTextureDlg::upateRadiusInner( const EventArgs& args)
{
	const TrackBarCtrlWrapperEventArgs* m = (TrackBarCtrlWrapperEventArgs*)&args;
	getStateManager()->getCurrentState()->setBrushRadiusInner(m->nPercent);
	return true;
}

bool TerrainTextureDlg::upateRadiusOuter( const EventArgs& args)
{
	const TrackBarCtrlWrapperEventArgs* m = (TrackBarCtrlWrapperEventArgs*)&args;
	getStateManager()->getCurrentState()->setBrushRadiusOuter(m->nPercent);
	return true;
}
void TerrainTextureDlg::onRefreshLuaScript()
{
}
