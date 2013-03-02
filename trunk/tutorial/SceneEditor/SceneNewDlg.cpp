#include "SceneNewDlg.h"
#include "scene/SceneManager.h"
#include "scene/Terrain.h"
#include "misc/FileSystem.h"
void SceneNewDlg::clear()
{
	mWidth = 1;
	mHeight = 1;
	mN = 5;
	mPath.clear();
	tPath[0] = NULL;
	mChunkSize = 30;
}

SceneNewDlg::SceneNewDlg()
{
}

SceneNewDlg::~SceneNewDlg()
{
	clear();
}

LRESULT SceneNewDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	//
	clear();
	// First DDX call, hooks up variables to controls.
	DoDataExchange(false); 
	//
	CenterWindow();
	ShowWindow(SW_NORMAL);
	return TRUE;
}

LRESULT SceneNewDlg::OnBnClickedOk( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	// TODO: 在此添加控件通知处理程序代码
	// Transfer data from the controls to member variables.
	if (DoDataExchange(true))
	{
		//创建场景
		getSceneManager()->createTerrain(mWidth, mHeight, mN, mChunkSize);
		getSceneManager()->constructTerrainQuadTree(mWidth, mHeight);
		//保存路径
		if (mPath.empty())
		{
			mPath = FileSystem::getDataDirectory() + "\\scene";
		}
		if (mcstrName.IsEmpty())
		{
			mcstrName = "BornLand";
		}
		Terrain* tn = SceneManager::getInstancePtr()->getTerrain();
		if (tn)
		{
			tn->setFileName(mPath + TEXT("\\") + getSceneName());
			tstring fxName(TEXT("shader\\Terrain.fx"));
			tn->setFX(fxName);
		}
		//1，否则domodel处条件判断失败，估计enddialog的参数即domodel的返回值
		EndDialog(1);
		//
	}
	return 0;
}

LRESULT SceneNewDlg::OnBnClickedCancel( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(0);
	return 0;
}

void SceneNewDlg::OnDataValidateError( UINT nCtrlID, BOOL bSave, _XData& data )
{
	MessageBox(TEXT("width and height range [1, 200], lod range [0, 6]"));
	return;
}
int CALLBACK BrowseCallbackProc(          HWND hwnd,
								UINT uMsg,
								LPARAM lParam,
								LPARAM lpData
								)
{
	switch(uMsg)
	{
	case BFFM_INITIALIZED:
		SendMessage(hwnd,BFFM_SETSELECTIONA,1,(LPARAM)lpData);
		break;
	}
	return 0;
}
tstring selectDirectory(HWND hWnd, tstring caption, tstring initPath)
{
	char pszDisplayName[MAX_PATH];
	tstring dir;
	BROWSEINFO BrowseInfo;
	LPITEMIDLIST pidlBrowse;
	LPMALLOC pMalloc;

	if( !SUCCEEDED(SHGetMalloc(&pMalloc)) ) 
		return FALSE; 
	BrowseInfo.hwndOwner = hWnd; 
	BrowseInfo.pidlRoot = NULL; 
	BrowseInfo.pszDisplayName = (LPSTR)initPath.c_str(); 
	BrowseInfo.lpszTitle = caption.c_str();
	BrowseInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_USENEWUI; 
	BrowseInfo.lpfn = 0; 
	BrowseInfo.lParam = 0; 
	if(!initPath.empty())
	{
		BrowseInfo.lpfn = BrowseCallbackProc; 
		BrowseInfo.lParam = (LPARAM)initPath.c_str(); 
	}
	BrowseInfo.iImage = 0; 
	pidlBrowse = SHBrowseForFolder(&BrowseInfo); 
	if( pidlBrowse!=NULL ) 
	{ 
		if( SHGetPathFromIDList(pidlBrowse,pszDisplayName) ) 
		{ 
			dir = pszDisplayName;
			pMalloc->Free(pidlBrowse); 
			pMalloc->Release(); 
		} 
	}
	else
		pMalloc->Release(); 
	return dir;
}
LRESULT SceneNewDlg::OnBnClickedButtonScenepath(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//mPath = selectDirectory(m_hWnd, TEXT("选择场景保存路径"), FileSystem::getDataDirectory());
	mPath = selectDirectory(m_hWnd, TEXT("选择场景保存路径"), TEXT("d:\\work\\zen\\data\\scene"));

	mPathEdit.InsertText(0, mPath.c_str());
	//IFileDialog d;
	return 0;
}
